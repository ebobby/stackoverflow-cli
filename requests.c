#include "stackoverflow-cli.h"

#include <curl/curl.h>

#define URL_BUILDER_ALLOCATION_BLOCK 512

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

/*
 * This is not the most beautiful piece of code ever but I really tried to keep it general enough for
 * this little app url building needs. --fms
 *
 * - baseUrl : base string in which to append everything else, must NOT end with '/' !!
 * - numPath : number of path "pieces" passed in the vararg list.
 * - numKeyValues : number of key and values pairs passed in the vararg list (after path pieces).
 *
 * If the num* parameters do not match exactly the number of char * passed, it will crash,
 * there's no way to be sure the numbers are actually correct inside the function (that I know of at least).
 */
char *buildUrl (const char *baseUrl, int numPath, int numKeyValues, ...) {
    char *result = malloc(URL_BUILDER_ALLOCATION_BLOCK);
    int cur_len = 0, cur_alloc = URL_BUILDER_ALLOCATION_BLOCK;
    va_list ap;
    char *key = NULL, *escaped_key = NULL;
    int key_len, i;
    CURL *curlHandle = NULL;
    char *eos = NULL;

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
    strncat(result, baseUrl, URL_BUILDER_ALLOCATION_BLOCK - 1);
    cur_len = strlen(result);

    va_start(ap, numKeyValues);
    for (i = 0; i < numPath + numKeyValues; i++) {
        key = va_arg(ap, char *);

        if (key != NULL) {
            key_len = strlen(key);

            escaped_key = curl_easy_escape(curlHandle, key, key_len);

            if (escaped_key == NULL) {
                fprintf(stderr, "Not enough memory!\n");
                curl_easy_cleanup(curlHandle);
                free(result);
                return NULL;
            }

            key_len = strlen(escaped_key);
        } else {
            key_len = 0;
        }

        if (cur_len + key_len + 1 > cur_alloc) {
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

        if (i < numPath)
            eos = "/";
        else if (i == numPath)
            eos = "?";
        else if (eos[0] == '=')
            eos = "&";
        else
            eos = "=";

        strncat(result, eos, 1);
        cur_len++;

        if (key != NULL) {
            strncat(result, escaped_key, cur_alloc - cur_len - 1);
            cur_len += key_len;

            curl_free(escaped_key);
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
