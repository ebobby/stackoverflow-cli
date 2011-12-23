#include "stackoverflow-cli.h"

#include <json/json.h>

int searchCommand (stackoverflow_cli_opts *opts) {
    responseObject response;
    char *url = buildUrl("search", 5,
                         "nottagged", opts->nottagged,
                         "tagged", opts->tagged,
                         "intitle", opts->intitle,
                         "pagesize", opts->pagesize,
                         "page", opts->page);

    if (url == NULL)
        return 0;

    makeWebRequest(url, &response);

    if (response.size == 0)
        return 0;

    json_object *jobj = json_tokener_parse(response.data);

    if (jobj == NULL || is_error(jobj)) {
        fprintf(stderr, "Not a valid JSON response?!\n");
        freeWebResponse(&response);
        free(url);
        return 0;
    }

    json_object *jerror = json_object_object_get(jobj, "error");

    if (jerror != NULL) {
        fprintf(stderr, "%s\n", json_object_get_string(json_object_object_get(jerror, "message")));
        freeWebResponse(&response);
        free(url);
        return 0;
    }

    printf("%s\n", response.data);

    freeWebResponse(&response);
    free(url);
}
