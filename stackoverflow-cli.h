#ifndef __STACKOVERFLOW_H__
#define __STACKOVERFLOW_H__

#include <stdlib.h>
#include <string.h>

/** Helpers from requests.c *****************************/
typedef struct responseObject {
    size_t size;
    char *data;
} responseObject;

void www_make_request (char *url, responseObject *response);
void www_free_response (responseObject *response);
/********************************************************/

#endif
