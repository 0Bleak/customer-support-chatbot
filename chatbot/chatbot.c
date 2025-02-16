#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include <cjson/cJSON.h>

// Singleton API Handler
typedef struct {
    CURL *curl;
} APIHandler;

APIHandler *get_api_handler() {
    static APIHandler *instance = NULL;
    if (!instance) {
        instance = (APIHandler *)malloc(sizeof(APIHandler));
        if (!instance) {
            fprintf(stderr, "Memory allocation failed for APIHandler\n");
            exit(1);
        }
        instance->curl = curl_easy_init();
        if (!instance->curl) {
            fprintf(stderr, "cURL initialization failed\n");
            free(instance);
            exit(1);
        }
    }
    return instance;
}

// Callback function to handle API response
size_t write_callback(void *ptr, size_t size, size_t nmemb, void *userdata) {
    size_t total_size = size * nmemb;
    char **response = (char **)userdata;
    *response = realloc(*response, total_size + 1);
    if (*response) {
        memcpy(*response, ptr, total_size);
        (*response)[total_size] = '\0';
    }
    return total_size;
}

// Adapter Pattern: Parse JSON response
char *parse_json_response(const char *json_str, const char *service) {
    cJSON *json = cJSON_Parse(json_str);
    if (!json) {
        fprintf(stderr, "Error parsing JSON: %s\n", json_str);
        return NULL;
    }

    char *result = NULL;
    if (strcmp(service, "crm") == 0) {
        cJSON *name = cJSON_GetObjectItemCaseSensitive(json, "name");
        cJSON *email = cJSON_GetObjectItemCaseSensitive(json, "email");
        if (name && email) {
            result = (char *)malloc(256 * sizeof(char));
            snprintf(result, 256, "Name: %s, Email: %s", name->valuestring, email->valuestring);
        }
    } else if (strcmp(service, "ticketing") == 0) {
        cJSON *status = cJSON_GetObjectItemCaseSensitive(json, "status");
        cJSON *ticket_id = cJSON_GetObjectItemCaseSensitive(json, "ticket_id");
        if (status && ticket_id) {
            result = (char *)malloc(256 * sizeof(char));
            snprintf(result, 256, "Status: %s, Ticket ID: %s", status->valuestring, ticket_id->valuestring);
        }
    } else if (strcmp(service, "faq") == 0) {
        cJSON *answer = cJSON_GetObjectItemCaseSensitive(json, "answer");
        if (answer) {
            result = strdup(answer->valuestring);
        }
    }

    cJSON_Delete(json);
    return result;
}

// Factory Pattern: Create API URL based on input
char *create_api_url(const char *service, const char *query) {
    char *url = (char *)malloc(256 * sizeof(char));
    if (strcmp(service, "crm") == 0) {
        snprintf(url, 256, "http://127.0.0.1:5000/crm?user_id=%s", query);
    } else if (strcmp(service, "ticketing") == 0) {
        snprintf(url, 256, "http://127.0.0.1:5000/ticketing?query=%s", query);
    } else if (strcmp(service, "faq") == 0) {
        snprintf(url, 256, "http://127.0.0.1:5000/faq?question=%s", query);
    } else {
        free(url);
        return NULL;
    }
    return url;
}

// Scatter-Gather Pattern: Query multiple services
void scatter_gather(const char *query) {
    APIHandler *handler = get_api_handler();
    char *services[] = {"crm", "ticketing", "faq"};
    int valid_response_found = 0;

    for (int i = 0; i < 3; i++) {
        char *url = create_api_url(services[i], query);
        if (!url) continue;

        char *response = NULL;
        curl_easy_setopt(handler->curl, CURLOPT_URL, url);
        curl_easy_setopt(handler->curl, CURLOPT_WRITEFUNCTION, write_callback);
        curl_easy_setopt(handler->curl, CURLOPT_WRITEDATA, &response);
        CURLcode res = curl_easy_perform(handler->curl);
        if (res != CURLE_OK) {
            fprintf(stderr, "cURL error: %s\n", curl_easy_strerror(res));
        } else {
            char *parsed_response = parse_json_response(response, services[i]);
            if (parsed_response) {
                printf("Response from %s: %s\n", services[i], parsed_response);
                free(parsed_response);
                valid_response_found = 1;
            }
        }
        free(response);
        free(url);
    }

    if (!valid_response_found) {
        printf("Sorry, I couldn't find any information for your query: %s\n", query);
        printf("Would you like to speak to a human agent? (yes/no): ");
        char choice[10];
        fgets(choice, 10, stdin);
        choice[strcspn(choice, "\n")] = 0; // Remove newline
        if (strcmp(choice, "yes") == 0) {
            printf("Your request has been forwarded to a human agent. They will contact you shortly.\n");
        } else {
            printf("Okay, let me know if you need further assistance!\n");
        }
    }
}

int main() {
    char input[256];
    printf("Welcome to the Customer Support Chatbot!\n");
    while (1) {
        printf("Enter your query (or 'exit' to quit): ");
        fgets(input, 256, stdin);
        input[strcspn(input, "\n")] = 0; // Remove newline
        if (strcmp(input, "exit") == 0) break;
        scatter_gather(input);
    }
    return 0;
}