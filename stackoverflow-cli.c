#include "stackoverflow-cli.h"

int main (int argc, char **argv) {
    stackoverflow_cli_opts global_opts;

    process_arguments(argc, argv, &global_opts);

    return EXIT_SUCCESS;
}
