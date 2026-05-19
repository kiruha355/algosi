#ifndef GRAPH_H
#define GRAPH_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define INF INT_MAX / 2
#define MAX_VERTICES 512

typedef struct {
    int n;
    int **matrix;
} Graph;

Graph *graph_create(int n);
void   graph_free(Graph *g);

void graph_add_edge(Graph *g, int u, int v, int w);
void graph_remove_edge(Graph *g, int u, int v);
int  graph_get_weight(const Graph *g, int u, int v);

Graph *graph_load(const char *filename);
int    graph_save(const Graph *g, const char *filename);
Graph *graph_load_from_stream(FILE *f);
int    graph_save_to_stream(const Graph *g, FILE *f);

int **floyd_warshall(const Graph *g);
int   fw_save(int **dist, int n, const char *filename);

void free_matrix(int **m, int n);

#endif