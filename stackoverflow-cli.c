#include "stackoverflow-cli.h"

int main (int argc, char **argv) {
    stackoverflow_cli_opts global_opts;

    process_arguments(argc, argv, &global_opts);

    if (global_opts.operation == none)
        return EXIT_FAILURE;

    if (global_opts.operation == search)
        run_search_command(&global_opts);

    return EXIT_SUCCESS;
}
