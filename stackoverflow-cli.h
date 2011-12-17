#ifndef __STACKOVERFLOW_H__
#define __STACKOVERFLOW_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/** Helpers from requests.c *****************************/
typedef struct responseObject {
    size_t size;
    char *data;
} responseObject;

void www_make_request (const char *url, responseObject *response);
void www_free_response (responseObject *response);
/********************************************************/

/** Helpers from arguments.c ****************************/
enum stackoverflow_operation { none = 0, search, question, user };

typedef struct stackoverflow_cli_opts {
    enum stackoverflow_operation operation;
    char *nottagged;
    char *tagged;
    char *intitle;
    int  pagesize;
    int  page;
} stackoverflow_cli_opts;

void process_arguments (int argc, char **argv, stackoverflow_cli_opts *opts);
void print_usage (const char *name, const char *msg);
/********************************************************/

#endif
