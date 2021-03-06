#include "stackoverflow-cli.h"

int main (int argc, char **argv) {
    stackoverflow_cli_opts global_opts;
    int result = EXIT_FAILURE;

    if (!processArguments(argc, argv, &global_opts))
        return result;

    if (global_opts.operation == none)
        result = EXIT_FAILURE;
    else if (global_opts.operation == search)
        result = searchCommand(&global_opts) ? EXIT_SUCCESS : EXIT_FAILURE;
    else if (global_opts.operation == users)
        result = usersCommand(&global_opts) ? EXIT_SUCCESS : EXIT_FAILURE;
    else if (global_opts.operation == questions)
        result = questionsCommand(&global_opts) ? EXIT_SUCCESS : EXIT_FAILURE;

    return result;
}
