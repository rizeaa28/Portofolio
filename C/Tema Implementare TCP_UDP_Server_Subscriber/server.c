#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "helpers.h"
#include "LinkedList.h"

void usage(char *file) {
    fprintf(stderr, "Usage: %s server_port\n", file);
    exit(0);
}

// Structure for a TCP client
struct TCP_client {
    // client id
    char* ID;
    // the socket which is used by the server to communicate with the client
    int socket;
    // if the server is active (is up)
    int active;
    //number of topics
    int number_of_topics;
    // list of topics
    linkedlist_t *topics;
    // ip
    char client_ip[20];
} TCP_client;

// This server's clients list
int number_of_clients = 0;
linkedlist_t* tcp_clients;

// Structure which is used to know the number of bytes that the
// first 2 values of message_header occupy
struct nr {
    struct sockaddr_in udp_client;
    int length;
} nr;

// Header of a message sent to a client
struct message_header {
    struct sockaddr_in udp_client;
    int length;
    char payload[2000];
} message_header;

// Header of a message received from UDP client
struct udp_msg_hdr {
    char topic[50];
    uint8_t type;
} udp_msg_hdr;

// Checks if a client has a topic
int check_if_has_topic(char *topic, struct TCP_client* client) {
    int i = 0;
    for(i = 0; i < client->number_of_topics; i++) {
        char *curr_topic = (char *)linkedlist_get(client->topics, i);
        if(strcmp(topic, curr_topic) == 0)
            return 1;
    }
    return 0;
}

// Sends a message to every active client that has that topic
void send_topic_message(char *topic, char* buffer, int len) {
    int i = 0;
    int n;
    for(i = 0; i < number_of_clients; i++) {
        struct TCP_client* curr_client = (struct TCP_client*)linkedlist_get(tcp_clients, i);
        
        // Check if client's up
        if(curr_client->active == 0)
            continue;
        
        // Checks if has topic, if it has it it sends the message
        if(check_if_has_topic(topic, curr_client) > 0) {
            n = send(curr_client->socket, buffer, len, 0);
		    DIE(n < 0, "send");
        }
    }
}

// Removes a topic from a client after an unsubscribe command
void remove_topic(char *topic, struct TCP_client *client) {
    int i;
    for(i = 0; i < client->number_of_topics; i++) {
        char *curr_topic = (char *)linkedlist_get(client->topics, i);

        if(strcmp(topic, curr_topic) == 0) {
            linkedlist_remove_index(client->topics, i);
            client->number_of_topics--;
            return;
        }
    }
}

// Returns the client from the list that has that socket
struct TCP_client* get_client_for_socket(int socket) {
    int i = 0;
    struct TCP_client* curr_client = malloc(sizeof(struct TCP_client));
    
    for(i = 0; i < number_of_clients; i++) {
        curr_client = (struct TCP_client*)linkedlist_get(tcp_clients, i);
        if(curr_client->socket == socket)
            return curr_client;
    }
    return NULL;
}

// Removes the client that has that socket
void remove_client_with_socket(int socket) {
    int i;
    for(i = 0; i < number_of_clients; i++) {
        struct TCP_client* curr_client = (struct TCP_client*)linkedlist_get(tcp_clients, i);
        if(curr_client->socket == socket) {
            linkedlist_remove_index(tcp_clients, i);
            number_of_clients--;
        }
    }
}

// Returns the client from the list that has that ID
struct TCP_client* get_client_for_id(char *id) {
    int i = 0;
    
    for(i = 0; i < number_of_clients; i++) {
        struct TCP_client* curr_client = (struct TCP_client*)linkedlist_get(tcp_clients, i);
        if(curr_client->ID == NULL)
            continue;
        
        if(strcmp(curr_client->ID, id) == 0)
            return curr_client;
    }
    return NULL;
}

// Checks if there is a client with a specific ID
int check_if_id_exists(char *id) {
    int i = 0;
    for(i = 0; i < number_of_clients; i++) {
        struct TCP_client* curr_client = (struct TCP_client*)linkedlist_get(tcp_clients, i);
        if(strcmp(curr_client->ID, id) == 0)
            return 1;
    }
    return 0;
}

// Closes all the clients sockets at the end
void close_clients(int fdmax, fd_set read_fds) {
    for (int i = 0; i <= fdmax; i++) {
		if (FD_ISSET(i, &read_fds) && i != STDIN_FILENO) {
            close(i);
            FD_CLR(i, &read_fds);
        }
    }
    linkedlist_free(tcp_clients);
}


// Build the message that has to be sent to client
char* build_message(struct sockaddr_in addr, int n, char *buffer) {
    // Build the header
    struct message_header m;
    m.udp_client = addr;
    m.length = n;

    // Payload includes the UDP message
    memcpy(m.payload, buffer, sizeof(m.payload));

    // Copy struct to buffer
    char* result = malloc(sizeof(struct message_header));
    memcpy(result, &m, sizeof(struct message_header) * sizeof(char));

    // Return buffer
    return result;
}

int main(int argc, char *argv[]) {
    setvbuf(stdout, NULL, _IONBF, BUFSIZ);
    int sockfdTCP, newsockfdTCP, portno;
    int sockfdUDP;
    char buffer[2000];
    struct sockaddr_in serv_addr, cli_addr;
    int n, i, ret, rc;
    socklen_t clilen;

    // Initialise client list
    number_of_clients = 0;
    tcp_clients = linkedlist_init();

    // read descriptors used by select()
    fd_set read_fds;
    // temp descriptor list 
    fd_set tmp_fds;
    // max fd value
    int fdmax;

    // not enough arguments
    if(argc < 2) {
        usage(argv[0]);
    }

    // initialise descriptor lists
    FD_ZERO(&read_fds);
    FD_ZERO(&tmp_fds);

    // socket for starting TCP connections
    sockfdTCP = socket(AF_INET, SOCK_STREAM, 0);
    DIE(sockfdTCP < 0, "TCP socket");

    // socket for UDP messages
    sockfdUDP = socket(AF_INET, SOCK_DGRAM, 0);
    DIE(sockfdUDP < 0, "UDP socket");

    // Initialise server details 
    portno = atoi(argv[1]);
    DIE(portno == 0, "atoi");

    memset((char *) &serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(portno);
    serv_addr.sin_addr.s_addr = INADDR_ANY;

    // Option to reuse socket address (if it raises an error)
    int enable = 1;
    if (setsockopt(sockfdTCP, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0)
        perror("setsockopt(SO_REUSEADDR) failed");

    // TCP bind
    ret = bind(sockfdTCP, (struct sockaddr *) &serv_addr, sizeof(struct sockaddr));
	DIE(ret < 0, "TCP bind");

    // TCP start to listen for connections
	ret = listen(sockfdTCP, MAX_CLIENTS);
	DIE(ret < 0, "listen");

    // Same as before but for UDP socket
    enable = 1;
    if (setsockopt(sockfdUDP, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0)
        perror("setsockopt(SO_REUSEADDR) failed");

    // UDP bind
    rc = bind(sockfdUDP, (struct sockaddr *) &serv_addr, sizeof(struct sockaddr));
    DIE(rc < 0, "UDP bind");

    // add the new file descriptors (TCP and UDP and stdin)
    FD_SET(STDIN_FILENO, &read_fds); //descriptor stdin
	FD_SET(sockfdTCP, &read_fds);
	fdmax = sockfdTCP;

    FD_SET(sockfdUDP, &read_fds);
    if (sockfdUDP > fdmax) { 
		fdmax = sockfdUDP;
	}

    // Type of message received from client
    char *message_type = malloc(sizeof(char) * 3);
    
    while (1) {        
        tmp_fds = read_fds; 
		
        // I/O multiplexing
		ret = select(fdmax + 1, &tmp_fds, NULL, NULL, NULL);
		DIE(ret < 0, "select");

        if(FD_ISSET(STDIN_FILENO, &tmp_fds)) {

			// reads from stdin
			memset(buffer, 0, BUFLEN);
			fgets(buffer, BUFLEN - 1, stdin);

            // Close server on exit command
			if (strncmp(buffer, "exit", 4) == 0) {
				break;
			}
        }

		for (i = 0; i <= fdmax; i++) {
			if (FD_ISSET(i, &tmp_fds)) {
				if (i == sockfdTCP) {
                    // Create new TCP connection
					clilen = sizeof(cli_addr);
					newsockfdTCP = accept(sockfdTCP, (struct sockaddr *) &cli_addr, &clilen);
					DIE(newsockfdTCP < 0, "accept TCP");

					// Add new socket
					FD_SET(newsockfdTCP, &read_fds);
					if (newsockfdTCP > fdmax) { 
						fdmax = newsockfdTCP;
					}

                    // Initialise new TCP Client structure
					struct TCP_client* new_client = malloc(sizeof(struct TCP_client));
                    new_client->ID = NULL;
                    new_client->socket = newsockfdTCP;
                    new_client->number_of_topics = 0;
                    new_client->active = 1;
                    new_client->topics = linkedlist_init();
                    strcpy(new_client->client_ip, inet_ntoa(cli_addr.sin_addr));

                    // Add client in list
                    linkedlist_add(tcp_clients, (void*) new_client);
                    number_of_clients++;
                }

                // received message from UDP client
                if(i == sockfdUDP) {
                    memset(buffer, 0, 2000);
					n = recvfrom(i, buffer, sizeof(buffer), 0, (struct sockaddr *) &cli_addr, &clilen);
					DIE(n < 0, "recvfrom UDP");

                    // Topic from UDP message header (first max 50 bytes)
                    char *topic;
                    topic = malloc(sizeof(char) * 50);
                    strncat(topic, buffer, 50);

                    // Add header to message
                    char* message = build_message(cli_addr, n, buffer);
                    struct message_header msg;
			        memcpy(&msg, message, sizeof(struct message_header));
                    
                    // Send message
                    int len = n + sizeof(struct nr);
                    send_topic_message(topic, message, len);
                }

                // received message from client
                if(i != sockfdTCP && i != sockfdUDP) {
                    
                    memset(buffer, 0, BUFLEN);
					n = recv(i, buffer, sizeof(buffer), 0);
					DIE(n < 0, "recv");

					if (n == 0) {
						// Connection closed
                        struct TCP_client* curr_client = malloc(sizeof(struct TCP_client));
                        curr_client = get_client_for_socket(i);

						printf("Client %s disconnected.\n", curr_client->ID);
						close(i);

                        // udpate TCP client's structure
                        curr_client->socket = -1;
                        curr_client->active = 0;
						
						// remove from descriptor list 
						FD_CLR(i, &read_fds);
					} else {
					
                        // get type of message
                        strncpy(message_type, buffer, 2);
                        message_type[2] = '\0';

                        DIE(sizeof(buffer) <= 3, "short buffer");
                        
                        // If it got the message that announces the client's ID
                        if(strcmp(message_type, "id") == 0) {
                            
                            // Get id from buffer
                            char *length_id = malloc(5*sizeof(char));
                            strncpy(length_id, buffer + 2*sizeof(char), 1);
                            int length = atoi(length_id) + 1;
                            
                            char* new_id = malloc(sizeof(char) * (length + 1));
                            strncpy(new_id, buffer + 3*sizeof(char), length);
                            
                            // get the client that represents this socket
                            struct TCP_client* curr_client = malloc(sizeof(struct TCP_client));
                            curr_client = get_client_for_socket(i);
                            
                            // if we didn't get his ID yet
                            if(curr_client->ID == NULL) {

                                // if there isn't another client with this ID
                                if(get_client_for_id(new_id) == NULL) {
                                    curr_client->ID = malloc(sizeof(new_id));
                                    strcpy(curr_client->ID, new_id);
                                    
                                    //accept connection
                                    printf("New client %s connected from %s:%s.\n", curr_client->ID, curr_client->client_ip, argv[1]);
                                     
                                // If there is another client with that ID
                                } else {
                                    struct TCP_client* initial_client = malloc(sizeof(struct TCP_client));
                                    initial_client = get_client_for_id(new_id);

                                    // if it the client with that id is inactive (has been closed and is reopening now)
                                    if(initial_client->active == 0) {
                                        
                                        // Update TCP client's structure and accept connection
                                        initial_client->active = 1;
                                        printf("New client %s connected from %s:%s.\n", initial_client->ID, initial_client->client_ip, argv[1]);
                                        remove_client_with_socket(i);
                                        initial_client->socket = i;
                                    } else {
                                        //refuse connection
                                        remove_client_with_socket(i);
                                        printf("Client %s already connected.\n", new_id);
                                        close(i);
                                        FD_CLR(i, &read_fds);
                                    }  
                                }
                            }
                        } else {
                            
                            // Got a subscribe or unsubscribe command from client
                            int length;
                            char* message;
                            
                            // Come back here if messages are concatenated
                            concat:

                            // Subscribe command
                            if(strcmp(message_type, "ss") == 0) {
                                
                                message = malloc(BUFLEN*sizeof(char));
                                sscanf(buffer + 2*sizeof(char), "%d!%s", &length, message);
                                
                                // Message from TCp client includes SF bit, but that part
                                // of the homework isn't implemented, but the protocol
                                // includes the possibility of implementation of it in server

                                // Update TCP client's structure with the new topic
                                struct TCP_client* subs_client = get_client_for_socket(i);
                                
                                char* added_topic = malloc(sizeof(char) * length);
                                strncpy(added_topic, message + 1 * sizeof(char), length);
                                
                                linkedlist_add(subs_client->topics, added_topic);
                                subs_client->number_of_topics++;
                            } else {

                                // Unsubscribe command
                                if(strcmp(message_type, "us") == 0) {
                                    
                                    message = malloc(BUFLEN*sizeof(char));
                                    sscanf(buffer + 2*sizeof(char), "%d!%s", &length, message);
                                    
                                    struct TCP_client* subs_client = get_client_for_socket(i);
                                    
                                    char* removed_topic = malloc(length * sizeof(char));
                                    strncpy(removed_topic, message, length);

                                    remove_topic(removed_topic, subs_client);
                                }
                            }
                            
                            // if the messages are concatenated into the received buffer, 
                            // work with the next message in the buffer
                            if(length > strlen(message)) {
                                strncpy(message_type, message + length, 2);
                                memcpy(buffer, message + length + 2, n);

                                goto concat;
                            }
                        }
					}
                }
            }
        }
    } 

    // Closed all open sockets
    close_clients(fdmax, read_fds);
    close(sockfdTCP);
    close(sockfdUDP);
    return 0;   
}