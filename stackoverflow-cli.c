#include <stdio.h>
#include <stdlib.h>

#include "stackoverflow-cli.h"

int main (void) {
    responseObject response;

    www_make_request("http://api.stackoverflow.com/1.1/questions?pagesize=2", &response);

    printf("Stackoverflow result:\n%s\n", response.data);

    www_free_response(&response);

    return EXIT_SUCCESS;
}
