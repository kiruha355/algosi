#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "graph.h"

static void usage(const char *prog) {
    printf("Usage: %s --load <file> --result <file> [--save-graph <file>]\n", prog);
}

int main(int argc, char *argv[]) {
    const char *load_file   = NULL;
    const char *save_file   = NULL;
    const char *result_file = NULL;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--help") == 0) {
            usage(argv[0]);
            return 0;
        } else if (strcmp(argv[i], "--load") == 0 && i+1 < argc) {
            load_file = argv[++i];
        } else if (strcmp(argv[i], "--save-graph") == 0 && i+1 < argc) {
            save_file = argv[++i];
        } else if (strcmp(argv[i], "--result") == 0 && i+1 < argc) {
            result_file = argv[++i];
        } else {
            fprintf(stderr, "Unknown argument: %s\n", argv[i]);
            usage(argv[0]);
            return 1;
        }
    }

    if (!load_file) {
        fprintf(stderr, "Error: --load is required.\n");
        usage(argv[0]);
        return 1;
    }
    if (!result_file) {
        fprintf(stderr, "Error: --result is required.\n");
        usage(argv[0]);
        return 1;
    }

    
    Graph *g = graph_load(load_file);
    if (!g) {
        fprintf(stderr, "Failed to load graph from '%s'\n", load_file);
        return 1;
    }
    printf("Graph loaded: %d vertices\n", g->n);

    
    if (save_file) {
        if (graph_save(g, save_file) == 0)
            printf("Graph saved to '%s'\n", save_file);
        else
            fprintf(stderr, "Warning: could not save graph to '%s'\n", save_file);
    }

    
    int **dist = floyd_warshall(g);
    if (!dist) {
        fprintf(stderr, "failed to load graph\n");
        graph_free(g);
        return 1;
    }
    printf("Floyd-Warshall completed.\n");

    if (fw_save(dist, g->n, result_file) == 0)
        printf("Result written to '%s'\n", result_file);
    else
        fprintf(stderr, "Warning: could not write result to '%s'\n", result_file);

free_matrix(dist, g->n);
    graph_free(g);
    return 0;
}
