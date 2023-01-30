#include <stdlib.h>     /* exit, atoi, malloc, free */
#include <stdio.h>
#include <unistd.h>     /* read, write, close */
#include <string.h>     /* memcpy, memset */
#include <sys/socket.h> /* socket, connect */
#include <netinet/in.h> /* struct sockaddr_in, struct sockaddr */
#include <netdb.h>      /* struct hostent, gethostbyname */
#include <arpa/inet.h>
#include "helpers.h"
#include "requests.h"
#include "cJSON.h"

char *compute_get_request(char *host, char *url, char *query_params,
                            char **cookies, int cookies_count, char* header)
{
    char *message = calloc(BUFLEN, sizeof(char));
    char *line = calloc(LINELEN, sizeof(char));

    // write the method name, URL, request params (if any) and protocol type
    if (query_params != NULL) {
        sprintf(line, "GET %s?%s HTTP/1.1", url, query_params);
    } else {
        sprintf(line, "GET %s HTTP/1.1", url);
    }

    compute_message(message, line);

    // add the host
    memset(line, 0, LINELEN);
    sprintf(line, "Host: %s", host);
    compute_message(message, line);

    // add authorization header
    if(header != NULL) {
        memset(line, 0, LINELEN);
        sprintf(line, "Authorization: Bearer %s", header);
        compute_message(message, line); 
    }

    // add cookies if any
    if (cookies != NULL) {
        memset(line, 0, LINELEN);
        strcat(line, "Cookie: ");

        for (int i = 0; i < cookies_count - 1; i++) {
            strcat(line, cookies[i]);
            strcat(line, ";");
        }

        strcat(line, cookies[cookies_count - 1]);
        compute_message(message, line);
    }

    compute_message(message, "");
    return message;
}

char *compute_delete_request(char *host, char *url, char *query_params,
                            char **cookies, int cookies_count, char* header)
{
    char *message = calloc(BUFLEN, sizeof(char));
    char *line = calloc(LINELEN, sizeof(char));

    // write the method name, URL, request params (if any) and protocol type
    if (query_params != NULL) {
        sprintf(line, "DELETE %s?%s HTTP/1.1", url, query_params);
    } else {
        sprintf(line, "DELETE %s HTTP/1.1", url);
    }

    compute_message(message, line);

    // add the host
    memset(line, 0, LINELEN);
    sprintf(line, "Host: %s", host);
    compute_message(message, line);

    // add authorization header
    if(header != NULL) {
        memset(line, 0, LINELEN);
        sprintf(line, "Authorization: Bearer %s", header);
        compute_message(message, line); 
    }

    // add cookies if any
    if (cookies != NULL) {
        memset(line, 0, LINELEN);
        strcat(line, "Cookie: ");

        for (int i = 0; i < cookies_count - 1; i++) {
            strcat(line, cookies[i]);
            strcat(line, ";");
        }

        strcat(line, cookies[cookies_count - 1]);
        compute_message(message, line);
    }

    compute_message(message, "");
    return message;
}

char *compute_post_request(char *host, char *url, char* content_type, char **body_data,
                            int body_data_fields_count, char **cookies, int cookies_count, char* header)
{
    char *message = calloc(1000000, sizeof(char));
    char *line = calloc(1000000, sizeof(char));

    // write the method name, URL and protocol type
    sprintf(line, "POST %s HTTP/1.1", url);
    compute_message(message, line);
    
    // add the host
    memset(line, 0, 1000000);
    sprintf(line, "Host: %s", host);
    compute_message(message, line);

    // compute message for json type content
    if(strcmp(content_type, "application/json") == 0) {
        
        // start to build content for a form_data type of input
        int body_data_size = 0;

        cJSON *body_message = NULL;
        body_message = cJSON_CreateObject();

        for(int i = 0; i < body_data_fields_count - 1; i = i + 2) {

            // checks if there is content (validation)
            if(strlen(body_data[i+1]) == 0)
                continue;

            if(strcmp(body_data[i], "page_count") == 0) {
                
                // checks if number is valid (validation)
                if(atoi(body_data[i+1]) == 0)
                    continue;

                // adds to object if validated
                cJSON_AddNumberToObject(body_message, body_data[i], atoi(body_data[i+1]));
                continue;
            }

            // adds to object if validated
            cJSON_AddItemToObject(body_message, body_data[i], cJSON_CreateString(body_data[i+1]));
        }

        // prints the json object into a string
        char *body_message_string = cJSON_Print(body_message);

        //gets it's length
        body_data_size = (int) strlen(body_message_string);

        // prints the necessary headers
        sprintf(line, "Content-Type: %s\r\nContent-Length: %d", content_type, body_data_size);
        compute_message(message, line);


        // adds authorization header
        if(header != NULL) {
            memset(line, 0, 1000000);
            sprintf(line, "Authorization: Bearer %s", header);
            compute_message(message, line); 
        }

        // adds cookies
        if (cookies != NULL) {
            memset(line, 0, 1000000);
            strcat(line, "Cookie: ");

            for (int i = 0; i < cookies_count - 1; i++) {
                strcat(line, cookies[i]);
                strcat(line, ";");
            }

            strcat(line, cookies[cookies_count - 1]);
            compute_message(message,line);
        }

        compute_message(message, "");

        // add json data
        memset(line, 0, 1000000);
        strcat(message, body_message_string);

        free(body_message_string);
        cJSON_Delete(body_message);
        free(line);
        return message;
    }

    return NULL;
}
