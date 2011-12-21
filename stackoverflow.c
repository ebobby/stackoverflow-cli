#include "stackoverflow-cli.h"

#include <stdarg.h>

#define URL_BUILDER_ALLOCATION_BLOCK 256

static char *build_url (const char *op, int argc, ...) {
    char *result = malloc(URL_BUILDER_ALLOCATION_BLOCK);
    int cur_len = 0, cur_alloc = URL_BUILDER_ALLOCATION_BLOCK;
    va_list ap;
    char *key = NULL, *value = NULL;
    int key_len, value_len, i;
    char buffer[URL_BUILDER_ALLOCATION_BLOCK];

    if (result == NULL) {
        fprintf(stderr, "Not enough memory!");
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

            /* current url, plus the new key/value, plus the =, & and the null terminator */
            if (cur_len + key_len + value_len + 3 > cur_alloc) {
                if (!realloc(result, cur_alloc + URL_BUILDER_ALLOCATION_BLOCK)) {
                    fprintf(stderr, "Not enough memory!");
                    free(result);
                    return NULL;
                } else {
                    memset(result + cur_alloc, 0, URL_BUILDER_ALLOCATION_BLOCK);
                    cur_alloc += URL_BUILDER_ALLOCATION_BLOCK;
                }
            }

            memset(buffer, 0, URL_BUILDER_ALLOCATION_BLOCK);
            snprintf(buffer, URL_BUILDER_ALLOCATION_BLOCK, "%s=%s&", key, value);

            strncat(result, buffer, cur_alloc - cur_len - 1);
            cur_len += strlen(buffer);
        }
    }
    va_end(ap);

    return result;
}

int run_search_command (stackoverflow_cli_opts *opts) {
    responseObject response;
    char *url = build_url("search", 5,
                          "nottagged", opts->nottagged,
                          "tagged", opts->tagged,
                          "intitle", opts->intitle,
                          "pagesize", opts->pagesize,
                          "page", opts->page);

    if (url == NULL)
        return 0;

    www_make_request(url, &response);

    if (response.size == 0)
        return 0;

    printf("%s\n", response.data);

    www_free_response(&response);
    free(url);
}
