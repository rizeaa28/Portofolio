#include <stdio.h>      /* printf, sprintf */
#include <stdlib.h>     /* exit, atoi, malloc, free */
#include <unistd.h>     /* read, write, close */
#include <string.h>     /* memcpy, memset */
#include <sys/socket.h> /* socket, connect */
#include <netinet/in.h> /* struct sockaddr_in, struct sockaddr */
#include <netdb.h>      /* struct hostent, gethostbyname */
#include <arpa/inet.h>
#include "helpers.h"
#include "requests.h"
#include "cJSON.h"

// fgets() reads the characters "\r" and "\n" as well, but those
// are not part of the string, so this function deletes them
char* format_buffer(char* buffer) {
    
    if(strlen(buffer) == 0)
        return buffer;

    if(buffer[strlen(buffer) - 1] == '\n')
        buffer[strlen(buffer) - 1] = '\0';
    if(buffer[strlen(buffer) - 1] == '\r')
        buffer[strlen(buffer) - 1] = '\0';

    return buffer;
}

// a form_data has an even amount of strings because:
// form_data[i] = "type_of_info" and
// form_data[i+1] = "the actual info"
// for easier parsing to json


// reads the username and password from the keyboard
// and returns the form_data with them
char** read_credentials() {
    char **form_data = calloc(4, sizeof(char *));
    for (int i = 0; i < 4; i++) {
        form_data[i] = calloc(LINELEN, sizeof(char));
    }
            
    printf("username=");
    strcpy(form_data[0], "username");
    fgets(form_data[1], LINELEN, stdin);
    form_data[1] = format_buffer(form_data[1]);
            
    printf("password=");
    strcpy(form_data[2], "password");
    fgets(form_data[3], LINELEN, stdin);
    form_data[3] = format_buffer(form_data[3]);

    return form_data;
}

// read the book details into a form_data
char** read_book_details() {
    char **form_data = calloc(10, sizeof(char *));
    for (int i = 0; i < 10; i++) {
        form_data[i] = calloc(LINELEN, sizeof(char));
    }

    printf("title=");
    strcpy(form_data[0], "title");
    fgets(form_data[1], LINELEN, stdin);
    form_data[1] = format_buffer(form_data[1]);

    // verifies if a title was written
    if(strlen(form_data[1]) == 0) {
        printf("Introduceti un titlu valid!\n");
        return NULL;
    }

    printf("author=");
    strcpy(form_data[2], "author");
    fgets(form_data[3], LINELEN, stdin);
    form_data[3] = format_buffer(form_data[3]);

    // verifies if an author was written
    if(strlen(form_data[3]) == 0) {
        printf("Introduceti un autor valid!\n");
        return NULL;
    }

    printf("genre=");
    strcpy(form_data[4], "genre");
    fgets(form_data[5], LINELEN, stdin);
    form_data[5] = format_buffer(form_data[5]);

    // verifies if a genre was written
    if(strlen(form_data[5]) == 0) {
        printf("Introduceti un genre valid!\n");
        return NULL;
    }

    printf("publisher=");
    strcpy(form_data[8], "publisher");
    fgets(form_data[9], LINELEN, stdin);
    form_data[9] = format_buffer(form_data[9]);

    // verifies if a publisher was written
    if(strlen(form_data[9]) == 0) {
        printf("Introduceti un publisher valid!\n");
        return NULL;
    }

    printf("page_count=");
    strcpy(form_data[6], "page_count");
    fgets(form_data[7], LINELEN, stdin);
    form_data[7] = format_buffer(form_data[7]);

    // verifies if a valid page count was written
    if(atoi(form_data[7]) == 0) {
        printf("Introduceti un numar de pagini valid!\n");
        return NULL;
    }

    return form_data;
}

// gets an id
char* get_id() {

    printf("id=");
    char* id = malloc(264);
    scanf("%s", id);

    return id;
}

// frees memory of a form_data
void free_form_data(char** form_data, int n) {
    for (int i = 0; i < n; i++) {
        free(form_data[i]);
    }

    free(form_data);
}

int main(int argc, char *argv[])
{
    char* buffer = malloc(264);
    char *message = NULL;
    char *response = NULL;
    
    // authentification cookie
    char *auth_cookie = NULL;

    // JWT access token
    char *access_token = NULL;
    int sockfd;

    while(1) {

        sockfd = open_connection("34.241.4.235", 8080, AF_INET, SOCK_STREAM, 0); 

        memset(buffer, 0, 264);
		fgets(buffer, 263, stdin);

        buffer = format_buffer(buffer);

        //1
        if(strncmp(buffer, "register", 8) == 0) {
            char **form_data = read_credentials();

            // Checks if an username and password were written
            if(strlen(form_data[1]) == 0) {
                printf("Introduceti un username!\n");
                continue;
            }

            if(strlen(form_data[3]) == 0) {
                printf("Introduceti o parola!\n");
                continue;
            }

            // Checks if theyre valid
            if(strstr(form_data[1], " ") != NULL) {
                printf("Introduceti un username valid!\n");
                continue;
            }

            if(strstr(form_data[3], " ") != NULL) {
                printf("Introduceti o parola valida!\n");
                continue;
            }

            // Sends message to server and gets response
            message = compute_post_request("34.241.4.235", "/api/v1/tema/auth/register", "application/json", form_data, 4, NULL, 0, NULL);

            send_to_server(sockfd, message);
            response = receive_from_server(sockfd);

            // Account was created
            if(strncmp(response, "HTTP/1.1 201 Created", 20) == 0) {
                printf("201 - Created - Utilizator inregistrar cu succes!\n");
            } else {
                // Account couldn't be created
                printf("400 - Bad Request - Username '%s' indisponibil\n", form_data[1]);
            }

            free_form_data(form_data, 4);
        }

        //2
        if(strncmp(buffer, "login", 5) == 0) {

            // Returns this if client is already logged in
            if(auth_cookie != NULL) {
                printf("Esti deja logat!\n");
                continue;
            }

            char **form_data = read_credentials();

            if(strlen(form_data[1]) == 0) {
                printf("Introduceti un username!\n");
                continue;
            }

            if(strlen(form_data[3]) == 0) {
                printf("Introduceti o parola!\n");
                continue;
            }

            if(strstr(form_data[1], " ") != NULL) {
                printf("Introduceti un username valid!\n");
                continue;
            }

            if(strstr(form_data[3], " ") != NULL) {
                printf("Introduceti o parola valida!\n");
                continue;
            }

            message = compute_post_request("34.241.4.235", "/api/v1/tema/auth/login", "application/json", form_data, 4, NULL, 0, NULL);

            send_to_server(sockfd, message);
            response = receive_from_server(sockfd);
            
            // Account logged in
            if(strncmp(response, "HTTP/1.1 200 OK", 15) == 0) {
                
                // Extracts and saves auth_cookie
                char* cookie = strstr(response, "Set-Cookie");
                int i = strcspn(cookie, ";");
                *(cookie + i) = '\0';
                cookie = cookie + 12;
                auth_cookie = malloc(264);
                memcpy(auth_cookie, cookie, strlen(cookie));

                printf("200 - OK - Logare efectuata cu succes\n");
            } else {
                // Wrong password or username
                printf("400 - Bad Request - Username sau parola gresite\n");
            }

            free_form_data(form_data, 4);
        }

        //3
        if(strncmp(buffer, "enter_library", 13) == 0) { 
            
            // if there is a saved auth_cookie (account is logged in), it sends it as parameters in the message
            if(auth_cookie != NULL)
                message = compute_get_request("34.241.4.235", "/api/v1/tema/library/access", NULL, &auth_cookie, 1, NULL);
            else
                message = compute_get_request("34.241.4.235", "/api/v1/tema/library/access", NULL, NULL, 0, NULL);

            send_to_server(sockfd, message);
            response = receive_from_server(sockfd);

            // Account was logged in nd succesfully entered the library
            if(strncmp(response, "HTTP/1.1 200 OK", 15) == 0) {
                printf("200 - OK - Biblioteca este accesata cu succes\n");
                
                // Extracts JWT token
                char* content = basic_extract_json_response(response);
                int length = strlen(content);             
                access_token = malloc(length);
                strncpy(access_token, content + 10, length - 12);
            
            } else {
                //Account isn't logged in
                printf("401 - Unauthorized - Nu sunteti logat!\n");
            }
        }

        //4
        if(strncmp(buffer, "get_books", 9) == 0) {
            
            // If not logged in, don't execute
            if(auth_cookie == NULL) {
                printf("Nu sunteti logat!\n");
                continue;
            }
            
            // If there is a JWT token saved, sends it in the message
            if(access_token != NULL)
                message = compute_get_request("34.241.4.235", "/api/v1/tema/library/books", NULL, NULL, 0, access_token);
            else
                message = compute_get_request("34.241.4.235", "/api/v1/tema/library/books", NULL, NULL, 0, NULL);

            send_to_server(sockfd, message);
            response = receive_from_server(sockfd);

            // Server returned books info
            if(strncmp(response, "HTTP/1.1 200 OK", 15) == 0) {
                printf("200 - OK - Cartile sunt urmatoarele: \n");

                // Extracts and prints the content of the message
                char* content = strstr(response, "[");
                content = format_buffer(content);

                // Formats the content and prints
                cJSON *json = cJSON_Parse(content);
                char *books_string = cJSON_Print(json);
                printf("%s\n", books_string);

                cJSON_Delete(json);
                free(books_string);

            } else {
                // user doesn't have access
                printf("403 - Forbidden - Nu aveti autorizatie\n");
            }
        }

        //5
        if(strcmp(buffer, "get_book") == 0) {

            // reads id
            char *id = get_id();

            // check if id is a number
            if(atoi(id) == 0) {
                printf("Id-ul trebuie sa fie un numar!\n");
                continue;
            }

            // checks if account is logged in
            if(auth_cookie == NULL) {
                printf("Nu sunteti logat!\n");
                continue;
            }

            // builds path with id
            char* path = malloc(264);
            strcpy(path, "/api/v1/tema/library/books/");
            strcat(path, id);
            
            if(access_token != NULL)
                message = compute_get_request("34.241.4.235", path, NULL, NULL, 0, access_token);
            else
                message = compute_get_request("34.241.4.235", path, NULL, NULL, 0, NULL);

            send_to_server(sockfd, message);
            response = receive_from_server(sockfd);

            // server returns book details
            if(strncmp(response, "HTTP/1.1 200 OK", 15) == 0) {
                printf("200 - OK - Detaliile cartii sunt urmatoarele: \n");

                char* content = strstr(response, "[");
                content = format_buffer(content);

                cJSON *json = cJSON_Parse(content);
                char *book_string = cJSON_Print(json);
                printf("%s\n", book_string);

                cJSON_Delete(json);
                free(book_string);

            } else {
                // Wrong id
                if(strncmp(response, "HTTP/1.1 404 Not Found", 13) == 0) {
                    printf("404 - Not Found - Carte indisponibila. \n");
                } else {
                    // User doesn't have access
                    printf("403 - Forbidden - Nu aveti autorizatie\n");
                }
            }

            free(id);
            free(path);
        }
        
        //6
        if(strncmp(buffer, "add_book", 8) == 0) {

            char** form_data = read_book_details();

            // If something was not valid, dont execute 
            if(form_data == NULL)
                continue;

            // If user isn't logged in, dont execute
            if(auth_cookie == NULL) {
                printf("Nu sunteti logat!\n");
                continue;
            }

            if(access_token != NULL)
                message = compute_post_request("34.241.4.235", "/api/v1/tema/library/books", "application/json", form_data, 10, NULL, 0, access_token);
            else
                message = compute_post_request("34.241.4.235", "/api/v1/tema/library/books", "application/json", form_data, 10, NULL, 0, NULL);
            
            
            send_to_server(sockfd, message);
            response = receive_from_server(sockfd);

            
            // Book added
            if(strncmp(response, "HTTP/1.1 200 OK", 15) == 0) {
                printf("200 - OK - Carte adaugata cu succes\n");
            } else {
                // user doesn't have access
                if(strncmp(response, "HTTP/1.1 403 Forbidden", 22) == 0) {
                    printf("403 - Forbidden - Nu aveti autorizatie\n");
                } else {
                    // Format of info wasn't valid
                    // (returns also the reason for it from the server)

                    char* content = strstr(response, "{");
                    content = format_buffer(content);
                    content = content + 10;
                    content[strlen(content) - 2] = '\0';

                    printf("400 - Bad Request - %s\n", content);

                    free(content);
                }
            }

            free_form_data(form_data, 10);
        }

        // 7
        if(strncmp(buffer, "delete_book", 11) == 0) {

            char* id = get_id();
            char* path = malloc(264);
            strcpy(path, "/api/v1/tema/library/books/");
            strcat(path, id);

            if(atoi(id) == 0) {
                printf("Id-ul trebuie sa fie un numar!\n");
                continue;
            }

            if(auth_cookie == NULL) {
                printf("Nu sunteti logat!\n");
                continue;
            }
            
            sockfd = open_connection("34.241.4.235", 8080, AF_INET, SOCK_STREAM, 0); 
            
            if(access_token != NULL)
                message = compute_delete_request("34.241.4.235", path, NULL, NULL, 0, access_token);
            else
                message = compute_delete_request("34.241.4.235", path, NULL, NULL, 0, NULL);

            send_to_server(sockfd, message);
            response = receive_from_server(sockfd);

            // Deteltes book
            if(strncmp(response, "HTTP/1.1 200 OK", 15) == 0) {
                printf("200 - OK - Cartea %s a fost stearsa. \n", id);
            } else {
                // ID doesn't exist
                if(strncmp(response, "HTTP/1.1 404 Not Found", 13) == 0) {
                    printf("404 - Not Found - Carte indisponibila. Nu s-a sters nimic. \n");
                } else {
                    // User doesn't have access
                    printf("403 - Forbidden - Nu aveti autorizatie\n");
                }
            }

            free(id);
            free(path);
        }

        // 8
        if(strncmp(buffer, "logout", 6) == 0) {
            
            if(auth_cookie != NULL)
                message = compute_get_request("34.241.4.235", "/api/v1/tema/auth/logout", NULL, &auth_cookie, 1, NULL);
            else
                message = compute_get_request("34.241.4.235", "/api/v1/tema/auth/logout", NULL, NULL, 0, NULL);

            send_to_server(sockfd, message);
            response = receive_from_server(sockfd);

            // User logged out
            if(strncmp(response, "HTTP/1.1 200 OK", 15) == 0) {
                printf("200 - OK - Logout efectuat cu succes\n");
                
                // deletes authentification info
                access_token = NULL;
                auth_cookie = NULL;
            
            } else {
                // user isn't logged in
                printf("401 - Unauthorized - Nu sunteti logat!\n");
            }
        }

        // 9
        if(strncmp(buffer, "exit", 4) == 0) {
            printf("Client inchis.\n");
            break;
        }
    }

    close(sockfd);
    free(message);
    free(response);
    free(auth_cookie);
    free(access_token);
    free(buffer);
    return 0;
}

