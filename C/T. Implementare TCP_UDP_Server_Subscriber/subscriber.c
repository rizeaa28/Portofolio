#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include "helpers.h"
#include "LinkedList.h"

// Structure for header of message sent to server
struct message_data {
	char message_type[2];
	char length[4];
} message_data;

// Structure for header of message sent from udp client
struct udp_msg_hdr {
	char topic[50];
    uint8_t type;
} udp_msg_hdr;

// Structure for header of message from server
struct message_header {
    struct sockaddr_in udp_client;
    int length;
} message_header;

void usage(char *file)
{
	fprintf(stderr, "Usage: %s server_address server_port\n", file);
	exit(0);
}

// Builds buffer to send this client's ip to server
char* build_id_buffer(char* id) {
	struct message_data m;
	strcpy(m.message_type, "id");
	sprintf(m.length, "%ld", strlen(id));

	char* buffer = malloc(sizeof(id)*sizeof(char) + 10);
	memcpy(buffer, &m, sizeof(struct message_data));
	strcat(buffer, id);
	return buffer;
}

// Builds buffer to send a subscribe command to server
char* build_subscribe_buffer(char* topic, char* sf) {
	struct message_data m;
	strcpy(m.message_type, "ss");
	sprintf(m.length, "%ld", strlen(topic) + 1);

	char* buffer = malloc((sizeof(topic) + 1) * sizeof(char) + 10);
	memcpy(buffer, &m, sizeof(struct message_data));
	strcat(buffer, "!");
	strcat(buffer, sf);
	strcat(buffer, topic);
	return buffer;
}

// Builds buffer to send a unsubscribe command to server
char* build_unsubscribe_buffer(char* topic) {
	struct message_data m;
	strcpy(m.message_type, "us");
	sprintf(m.length, "%ld!", strlen(topic));

	char* buffer = malloc((sizeof(topic)) * sizeof(char) + 10);
	memcpy(buffer, &m, sizeof(struct message_data));
	strcat(buffer, topic);
	return buffer;
}

// calculates pow(a,b)
float my_pow(int base, uint8_t power) {
	float result = base;
	
	while(power != 1) {
		result = result * base;;
		power--;
	}
	return (float) result;
}

// Unifies separated messages just like mentioned in README
char* get_message(char* original_buffer, int sockfd, int *len) {
	int n;
	char buffer[BUFLEN];

	memset(buffer, 0, BUFLEN);
	n = recv(sockfd, buffer, BUFLEN, 0);
	DIE(n < 0, "receive");

	memcpy(original_buffer + (BUFLEN - sizeof(struct message_header)), buffer, BUFLEN);
	(*len) = (*len) + n;
	
	int i = 0;
	while(n == BUFLEN) {
		i++;

		// recv next part of the message
		memset(buffer, 0, BUFLEN);
		n = recv(sockfd, buffer, BUFLEN, 0);
		DIE(n < 0, "receive");

		// unify with the rest of the message
		memcpy(original_buffer + (BUFLEN - sizeof(struct message_header)) + i * BUFLEN, buffer, BUFLEN);
		(*len) = (*len) + n;
	}

	return original_buffer;
}

int main(int argc, char *argv[])
{
	setvbuf(stdout, NULL, _IONBF, BUFSIZ);

	// Similar to lab structure of working with sockets
    int sockfd, n, ret;
	struct sockaddr_in serv_addr;
	char buffer[2000];
	fd_set read_fds, tmp_fds;

	if (argc < 4) {
		usage(argv[0]);
	}

	// set descriptor lists
	FD_ZERO(&read_fds);
    FD_ZERO(&tmp_fds);

	// get id from start command
    char* ID = malloc(sizeof(char)*10);
    strcpy(ID, argv[1]);

	// Start socket
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	DIE(sockfd < 0, "socket");

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(atoi(argv[3]));
	ret = inet_aton(argv[2], &serv_addr.sin_addr);
	DIE(ret == 0, "inet_aton");

	// Start a TCP connection with the server
	ret = connect(sockfd, (struct sockaddr*) &serv_addr, sizeof(serv_addr));
	DIE(ret < 0, "connect");

	// stdin descriptor
	FD_SET(STDIN_FILENO, &read_fds);
	FD_SET(sockfd, &read_fds);

    // Send ID to the server
	char* id_buffer = build_id_buffer(argv[1]);
    n = send(sockfd, id_buffer, strlen(id_buffer), 0);

    while (1) {

		// I/O multiplexing with descriptors
  		tmp_fds = read_fds;
		ret = select(sockfd + 1, &tmp_fds, NULL, NULL, NULL);
		DIE(ret < 0, "select");
		  
		if(FD_ISSET(STDIN_FILENO, &tmp_fds)) {

			// If stdin descriptor is set
			memset(buffer, 0, BUFLEN);
			fgets(buffer, BUFLEN - 1, stdin);

			// Close client on exit command
			if (strncmp(buffer, "exit", 4) == 0) {
				break;
			}

			// Got a subscribe command
			if (strncmp(buffer, "subscribe", 9) == 0) {
				DIE(strlen(buffer) < 10, "short input");
				char* topic = malloc(sizeof(char) * (strlen(buffer) - 10));
				char* sf = malloc(sizeof(char) * 2);
				
				sscanf(buffer, "subscribe %s %s", topic, sf);
				char* subscribe_buffer = build_subscribe_buffer(topic, sf);

				// Sends subscribe command to server
				n = send(sockfd, subscribe_buffer, strlen(subscribe_buffer), 0);
				DIE(n < 0, "send subscribe buffer");

				printf("Subscribed to topic.\n");
			}

			// Got a unsubscribe command
			if (strncmp(buffer, "unsubscribe", 11) == 0) {

				DIE(strlen(buffer) < 12, "short input");
				char* topic = malloc(sizeof(char) * (strlen(buffer) - 12));

				sscanf(buffer, "unsubscribe %s", topic);
				char* unsubscribe_buffer = build_unsubscribe_buffer(topic);
				
				// Sends unsubscribe command to server
				n = send(sockfd, unsubscribe_buffer, strlen(unsubscribe_buffer), 0);
				DIE(n < 0, "send unsubscribe buffer");

				printf("Unsubscribed from topic.\n");
			}
		}

		// Received message from server
		if(FD_ISSET(sockfd, &tmp_fds)) {
			memset(buffer, 0, BUFLEN);
			n = recv(sockfd, buffer, BUFLEN, 0);
			DIE(n < 0, "receive");
			
			// Server stopped, close the client
			if(n == 0)
				break;

			// Message from server
			struct message_header *msg = (struct message_header*)buffer;
			// Original message from UDP client
			char *udp_message;

			// If message has not completely been received, recevie messaged from TCP until the end of the message
			int len = n;
			if(n == 256 || msg->length > (BUFLEN - sizeof(struct message_header))) {
				udp_message = get_message(buffer + sizeof(struct message_header), sockfd, &len);
			}
			
			// If more than one message is concatenated into what was received
			if(len > msg->length + sizeof(struct message_header)) {
				int i = 0;
				
				//Concatenated messages:
				concat:
				msg = (struct message_header*)(buffer + i);
				memcpy(udp_message, buffer + i + sizeof(struct message_header), msg->length);

				// How many bites were already treated like messages from what was received, so we know from what
				// point to read the next message
				i = i + msg->length + sizeof(struct message_header);

				// Length remained
				len = len - msg->length + sizeof(struct message_header);
				
			} else {
				// What we received is only one message
				udp_message = (char*)(buffer + sizeof(struct message_header));
			}
			
			// Header of udp message
			struct udp_msg_hdr *hdr = (struct udp_msg_hdr*)(udp_message);
				
			// Start printing the output for the udp message
			printf("%s:%d - %s - ", inet_ntoa(msg->udp_client.sin_addr), htons(msg->udp_client.sin_port), hdr->topic);

			// If integer
			if(hdr->type == 0) {
				printf("INT - ");
				
				uint8_t sgn;
				memcpy(&sgn, udp_message + sizeof(struct udp_msg_hdr), 1);
				uint32_t number;
				memcpy(&number, udp_message + sizeof(struct udp_msg_hdr) + 1, sizeof(uint32_t));

				if(sgn == 1)
					printf("-");
				
				printf("%u\n", ntohl(number));
			}

			// if short-real
			if(hdr->type == 1) {
				printf("SHORT_REAL - ");
				uint16_t number;
				memcpy(&number, udp_message + sizeof(struct udp_msg_hdr), 2);

				if(ntohs(number) % 100 == 0) {
					float result = ((float) ntohs(number)) / 100;
					printf("%.0f\n", result);
				}
				else {
					float result = ((float) ntohs(number)) / 100;
					printf("%.2f\n", result);
				}
			}

			// if float
			if(hdr->type == 2) {
				printf("FLOAT - ");

				uint8_t sgn;
				memcpy(&sgn, udp_message + sizeof(struct udp_msg_hdr), 1);
				uint32_t number;
				memcpy(&number, udp_message + sizeof(struct udp_msg_hdr) + 1, sizeof(uint32_t));
				uint8_t power;
				memcpy(&power, udp_message + sizeof(struct udp_msg_hdr) + 5, 1);

				float decimals = 1;
				if(power != 0)
					decimals = (float)my_pow(10, power);

				float result = ((float) ntohl(number)) / decimals;
				if(sgn == 1)
					result = result * (-1);
			
				printf("%f\n", result);
			}

			// If string
			if(hdr->type == 3) {
				printf("STRING - ");
				char*s = malloc(1501);
				strncat(s, udp_message + sizeof(struct udp_msg_hdr), 1500);
				printf("%s\n", s);
			}

			// If there is still space for a message in the buffer received, we go back and interpret that message
			if(len > (msg->length + sizeof(struct message_header))) {
				goto concat;
			}
		}
	}

	// Close socket and client
	close(sockfd);
	return 0;
}