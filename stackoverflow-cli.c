#include "stackoverflow-cli.h"

int main (int argc, char **argv) {
    stackoverflow_cli_opts global_opts;


    process_arguments(argc, argv, &global_opts);

    printf("%i\n", global_opts.operation);

    if (global_opts.nottagged != NULL)
        printf("%s\n", global_opts.nottagged);
    if (global_opts.tagged != NULL)
        printf("%s\n", global_opts.tagged);
    if (global_opts.intitle != NULL)
        printf("%s\n", global_opts.intitle);
    printf("%i\n", global_opts.pagesize);
    printf("%i\n", global_opts.page);

    return EXIT_SUCCESS;
}
