#include "stackoverflow-cli.h"

#include <getopt.h>

void print_usage (const char *name, const char *msg) {
    if (msg != NULL)
        fprintf(stderr, "%s\n", msg);

    printf("Usage: %s <operation> [options...]\n", name);

}

void process_arguments (int argc, char **argv, stackoverflow_cli_opts *opts) {
    static const struct option getopt_long_opts[] = {
        { "search",    no_argument,       NULL, 'S' },
        { "nottagged", required_argument, NULL, 'n' },
        { "tagged",    required_argument, NULL, 'g' },
        { "intitle",   required_argument, NULL, 't' },
        { "pagesize",  required_argument, NULL, 's' },
        { "page",      required_argument, NULL, 'p' },
        { NULL, no_argument, NULL, 0 }
    };

    char *optstring = "Sn:g:t:s:p:h?";

    memset(opts, 0, sizeof(stackoverflow_cli_opts));

    /* Some sane defaults */
    opts->operation = none;

    for (;;) {
        int opt_index = 0;
        int opt = getopt_long(argc, argv, optstring, getopt_long_opts, &opt_index);

        if (opt == -1)
            break;

        switch (opt) {
        case 'S':
            opts->operation = search;
            break;
        case 'n':
            opts->nottagged = optarg;
            break;
        case 'g':
            opts->tagged = optarg;
            break;
        case 't':
            opts->intitle = optarg;
            break;
        case 's':
            opts->pagesize = optarg;
            break;
        case 'p':
            opts->page = optarg;
            break;
        case 'h':
        case '?':
        default:
            break;

        }
    }

    if (opts->operation == none) {
        print_usage(argv[0], NULL);
        return;
    }
}
