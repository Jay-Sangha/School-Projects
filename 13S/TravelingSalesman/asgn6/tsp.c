#include "graph.h"
#include "path.h"
#include "stack.h"
#include "vertices.h"

#include <inttypes.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void dfs(Graph *g, uint32_t ver, Path *p, Path *smalp) {
    graph_visit_vertex(g, ver);
    path_add(p, ver, g);

    for (uint32_t i = 0; i < graph_vertices(g); ++i) {
        if (graph_get_weight(g, ver, i) != 0 && !graph_visited(g, i)) {

            dfs(g, i, p, smalp);
        }
    }

    if (path_vertices(p) == graph_vertices(g)) {
        if ((graph_get_weight(g, ver, START_VERTEX) > 0)) {
            path_add(p, START_VERTEX, g);

            if ((path_distance(smalp) == 0) || (path_distance(p) < path_distance(smalp))) {
                path_copy(smalp, p);
            }

            path_remove(p, g);
        }
    }

    path_remove(p, g);
    graph_unvisit_vertex(g, ver);
}

int main(int argc, char *argv[]) {
    int opt;
    FILE *in = stdin;
    FILE *out = stdout;
    bool dir = false;
    bool help = false;

    while ((opt = getopt(argc, argv, "dhi:o:")) != -1) {
        switch (opt) {
        case 'd': dir = true; break;

        case 'h': help = true; break;

        case 'o':
            out = fopen(optarg, "w");

            if (out == NULL) {
                fprintf(stderr, "tsp:  error writing output file %s\n", optarg);
                help = true;
                return 1;
            }

            break;

        case 'i':
            in = fopen(optarg, "r");

            if (in == NULL) {
                fprintf(stderr, "tsp:  error reading input file %s\n", optarg);
                help = true;
                return 1;
            }

            break;

        default: break;
        }
    }
    if (help == true) {
        fprintf(stdout, "Usage: tsp [options]\n\n");
        fprintf(
            stdout, "-i infile    Specify the input file path containing the cities and edges\n");
        fprintf(stdout, "             of a graph. If not specified, the default input should be\n");
        fprintf(stdout, "             set as stdin.\n\n");
        fprintf(
            stdout, "-o outfile   Specify the output file path to print to. If not specified,\n");
        fprintf(stdout, "	     the default output should be set as stdout.\n\n");
        fprintf(stdout, "-d	     Specifies the graph to be directed.\n\n");
        fprintf(stdout, "-h	     Prints out a help message describing the purpose of the\n");
        fprintf(stdout, "	     graph and the command-line options it accepts, exiting the\n");
        fprintf(stdout, "	     program afterwards.\n");

        return 0;
    }

    uint32_t vertex;

    if (fscanf(in, "%u\n", &vertex) != 1) {
        fprintf(stderr, "tsp: error reading number of vertices\n");
        return 1;
    }

    Graph *g = graph_create(vertex, dir);

    char name[PATH_MAX];

    for (uint32_t i = 0; i < vertex; ++i) {
        if (fgets(name, sizeof(name), in) != NULL) {
            name[strlen(name) - 1] = '\0';
            graph_add_vertex(g, name, i);
        }
    }

    uint32_t edge;

    if (fscanf(in, "%u", &edge) != 1) {
        fprintf(stderr, "tsp: error reading number of edges\n");
        return 1;
    }

    uint32_t start;
    uint32_t end;
    uint32_t weight;

    for (uint32_t i = 0; i < edge; ++i) {
        fscanf(in, "%u %u %u", &start, &end, &weight);
        graph_add_edge(g, start, end, weight);
    }

    fclose(in);

    Path *active = path_create(vertex);
    Path *min = path_create(vertex + 1);

    dfs(g, START_VERTEX, active, min);

    if (path_distance(min) == 0) {
        fprintf(out, "No path found! Alissa is lost!\n");

    }

    else {
        path_add(min, START_VERTEX, g);
        fprintf(out, "Alissa starts at:\n");
        path_print(min, out, g);
        fprintf(out, "Total Distance: %d\n", path_distance(min));
    }

    path_free(&active);
    path_free(&min);
    graph_free(&g);

    fclose(out);

    return 0;
}
