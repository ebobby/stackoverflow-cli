#include "stackoverflow-cli.h"

int main (int argc, char **argv) {
    stackoverflow_cli_opts global_opts;
    int result = EXIT_FAILURE;

    processArguments(argc, argv, &global_opts);

    if (global_opts.operation == none)
        result = EXIT_FAILURE;
    else if (global_opts.operation == search)
        result = searchCommand(&global_opts) ? EXIT_SUCCESS : EXIT_FAILURE;

    return result;
}
