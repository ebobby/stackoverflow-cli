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

void makeWebRequest (const char *url, responseObject *response);
void freeWebResponse (responseObject *response);
/********************************************************/

/** Helpers from arguments.c ****************************/
enum stackoverflow_operation { none = 0, search, question, user };

typedef struct stackoverflow_cli_opts {
    enum stackoverflow_operation operation;
    char *nottagged;
    char *tagged;
    char *intitle;
    char *pagesize;
    char *page;
} stackoverflow_cli_opts;

void processArguments (int argc, char **argv, stackoverflow_cli_opts *opts);
void printUsage (const char *name, const char *msg);
/********************************************************/

/*** Helpers from stackoverflow.c ***********************/
int searchCommand (stackoverflow_cli_opts *opts);
/********************************************************/

#define STACKOVERFLOW_API_URL "http://api.stackoverflow.com/1.1/"

#endif
