#include "stackoverflow-cli.h"

#include <curl/curl.h>

#define URL_BUILDER_ALLOCATION_BLOCK 256

static size_t requestCallback (char *ptr, size_t size, size_t nmemb, void *usr) {
    size_t realsize = size * nmemb;
    responseObject *response = (responseObject *)usr;

    if (realsize != 0) {
        response->data = realloc(response->data, response->size + realsize + 1);

        if (response->data == NULL) {
            printf("Not enough memory!");
            exit(EXIT_FAILURE);
        }

        memcpy(&response->data[response->size], ptr, realsize);
        response->size += realsize;
        response->data[response->size] = 0;
    } else {
        response->data = NULL;
        response->size = 0;
    }

    return realsize;
}

char *buildUrl (const char *op, int argc, ...) {
    char *result = malloc(URL_BUILDER_ALLOCATION_BLOCK);
    int cur_len = 0, cur_alloc = URL_BUILDER_ALLOCATION_BLOCK;
    va_list ap;
    char *key = NULL, *value = NULL, *escaped_key = NULL, *escaped_value = NULL;
    int key_len, value_len, i;
    char buffer[URL_BUILDER_ALLOCATION_BLOCK];
    CURL *curlHandle = NULL;

    curl_global_init(CURL_GLOBAL_ALL);

    curlHandle = curl_easy_init();

    if (result == NULL) {
        fprintf(stderr, "Not enough memory!\n");
        return NULL;
    }

    if (curlHandle == NULL) {
        free(result);
        fprintf(stderr, "Could not create libcurl handle.\n");
        return NULL;
    }

    memset(result, 0, URL_BUILDER_ALLOCATION_BLOCK);
    snprintf(result, URL_BUILDER_ALLOCATION_BLOCK, "%s%s?", STACKOVERFLOW_API_URL, op);
    cur_len = strlen(result);

    va_start(ap, argc);
    for (i = 0; i < argc; i++) {
        key = va_arg(ap, char *);
        value = va_arg(ap, char *);

        if (key != NULL && value != NULL) {
            key_len = strlen(key);
            value_len = strlen(value);

            escaped_key = curl_easy_escape(curlHandle, key, key_len);
            escaped_value = curl_easy_escape(curlHandle, value, value_len);

            if (escaped_key == NULL || escaped_value == NULL) {
                fprintf(stderr, "Not enough memory!\n");
                curl_easy_cleanup(curlHandle);
                free(result);
                return NULL;
            }

            key_len = strlen(escaped_key);
            value_len = strlen(escaped_value);

            /* current url, plus the new key/value, plus the =, & and the null terminator */
            if (cur_len + key_len + value_len + 3 > cur_alloc) {
                if (!realloc(result, cur_alloc + URL_BUILDER_ALLOCATION_BLOCK)) {
                    fprintf(stderr, "Not enough memory!\n");
                    curl_easy_cleanup(curlHandle);
                    free(result);
                    return NULL;
                } else {
                    memset(result + cur_alloc, 0, URL_BUILDER_ALLOCATION_BLOCK);
                    cur_alloc += URL_BUILDER_ALLOCATION_BLOCK;
                }
            }

            memset(buffer, 0, URL_BUILDER_ALLOCATION_BLOCK);
            snprintf(buffer, URL_BUILDER_ALLOCATION_BLOCK, "%s=%s&", escaped_key, escaped_value);

            strncat(result, buffer, cur_alloc - cur_len - 1);
            cur_len += strlen(buffer);

            curl_free(escaped_key);
            curl_free(escaped_value);
        }
    }
    va_end(ap);

    curl_easy_cleanup(curlHandle);

    return result;
}

void makeWebRequest (const char *url, responseObject *response) {
    CURL *curlHandle = NULL;

    memset(response, 0, sizeof(responseObject));

    curl_global_init(CURL_GLOBAL_ALL);

    curlHandle = curl_easy_init();

    if (curlHandle == NULL) {
        fprintf(stderr, "Could not create libcurl handle.\n");
        return;
    }

    curl_easy_setopt(curlHandle, CURLOPT_URL, url);
    curl_easy_setopt(curlHandle, CURLOPT_ENCODING, "\0");
    curl_easy_setopt(curlHandle, CURLOPT_WRITEFUNCTION, requestCallback);
    curl_easy_setopt(curlHandle, CURLOPT_WRITEDATA, (void *)response);
    curl_easy_perform(curlHandle);
    curl_easy_cleanup(curlHandle);
}

void freeWebResponse (responseObject *response) {
    if (response == NULL)
        return;

    if (response->data != NULL)
        free(response->data);

    response->size = 0;
}
