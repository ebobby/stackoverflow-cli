#ifndef __STACKOVERFLOW_H__
#define __STACKOVERFLOW_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>

/** Helpers from requests.c *****************************/
typedef struct responseObject {
    size_t size;
    char *data;
} responseObject;

void makeWebRequest (const char *url, responseObject *response);
void freeWebResponse (responseObject *response);
/********************************************************/

/** Helpers from arguments.c ****************************/
enum stackoverflow_operation { none = 0, search, users, questions, user };

typedef struct stackoverflow_cli_opts {
    enum stackoverflow_operation operation;
    char *nottagged;
    char *tagged;
    char *intitle;
    char *filter;
    char *id;
    char *pagesize;
    char *page;
} stackoverflow_cli_opts;

int processArguments (int argc, char **argv, stackoverflow_cli_opts *opts);
void printUsage (const char *name, const char *msg);
/********************************************************/

/*** Helpers from stackoverflow.c ***********************/
char *buildUrl (const char *baseUrl, int numPath, int numKeyValues, ...);
int searchCommand (stackoverflow_cli_opts *opts);
int usersCommand (stackoverflow_cli_opts *opts);
int questionsCommand (stackoverflow_cli_opts *opts);
/********************************************************/

#define STACKOVERFLOW_API_URL "http://api.stackoverflow.com/1.1"

#endif
