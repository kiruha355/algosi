#include "graph.h"

static int **alloc_matrix(int n, int fill) {
    int **m = malloc(n * sizeof(int *));
    if (!m) return NULL;
    for (int i = 0; i < n; i++) {
        m[i] = malloc(n * sizeof(int));
        if (!m[i]) {
            for (int j = 0; j < i; j++) free(m[j]);
            free(m);
            return NULL;
        }
        for (int j = 0; j < n; j++)
            m[i][j] = fill;
    }
    return m;
}

void free_matrix(int **m, int n) {
    if (!m) return;
    for (int i = 0; i < n; i++) free(m[i]);
    free(m);
}

Graph *graph_create(int n) {
    if (n <= 0 || n > MAX_VERTICES) return NULL;
    Graph *g = malloc(sizeof(Graph));
    if (!g) return NULL;
    g->n = n;
    g->matrix = alloc_matrix(n, INF);
    if (!g->matrix) { free(g); return NULL; }
    for (int i = 0; i < n; i++) g->matrix[i][i] = 0;
    return g;
}

void graph_free(Graph *g) {
    if (!g) return;
    free_matrix(g->matrix, g->n);
    free(g);
}

void graph_add_edge(Graph *g, int u, int v, int w) {
    if (!g || u < 0 || v < 0 || u >= g->n || v >= g->n) return;
    g->matrix[u][v] = w;
    g->matrix[v][u] = w;
}

void graph_remove_edge(Graph *g, int u, int v) {
    if (!g || u < 0 || v < 0 || u >= g->n || v >= g->n) return;
    g->matrix[u][v] = INF;
    g->matrix[v][u] = INF;
}

int graph_get_weight(const Graph *g, int u, int v) {
    if (!g || u < 0 || v < 0 || u >= g->n || v >= g->n) return INF;
    return g->matrix[u][v];
}

Graph *graph_load_from_stream(FILE *f) {
    int n;
    if (fscanf(f, "%d", &n) != 1 || n <= 0 || n > MAX_VERTICES)
        return NULL;
    Graph *g = graph_create(n);
    if (!g) return NULL;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            char token[32];
            if (fscanf(f, "%31s", token) != 1) { graph_free(g); return NULL; }
            if (i == j) {
                g->matrix[i][j] = 0;
            } else if (strcmp(token, "INF") == 0 || strcmp(token, "inf") == 0) {
                g->matrix[i][j] = INF;
            } else {
                int w = atoi(token);
                g->matrix[i][j] = (w == 0) ? INF : w;
            }
        }
    }
    return g;
}

Graph *graph_load(const char *filename) {
    FILE *f = fopen(filename, "r");
    if (!f) { perror("graph_load"); return NULL; }
    Graph *g = graph_load_from_stream(f);
    fclose(f);
    return g;
}

int graph_save_to_stream(const Graph *g, FILE *f) {
    if (!g || !f) return -1;
    fprintf(f, "%d\n", g->n);
    for (int i = 0; i < g->n; i++) {
        for (int j = 0; j < g->n; j++) {
            if (j) fputc(' ', f);
            if (g->matrix[i][j] == INF) fprintf(f, "INF");
            else                        fprintf(f, "%d", g->matrix[i][j]);
        }
        fputc('\n', f);
    }
    return 0;
}

int graph_save(const Graph *g, const char *filename) {
    FILE *f = fopen(filename, "w");
    if (!f) { perror("graph_save"); return -1; }
    int rc = graph_save_to_stream(g, f);
    fclose(f);
    return rc;
}

int **floyd_warshall(const Graph *g) {
    if (!g) return NULL;
    int n = g->n;

    int **dist = alloc_matrix(n, 0);
    if (!dist) return NULL;

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++)
            dist[i][j] = g->matrix[i][j];
        dist[i][i] = 0;
    }

    for (int k = 0; k < n; k++) {
        for (int i = 0; i < n; i++) {
            if (dist[i][k] == INF) continue;
            for (int j = 0; j < n; j++) {
                if (dist[k][j] == INF) continue;
                long long nd = (long long)dist[i][k] + dist[k][j];
                if (nd < dist[i][j])
                    dist[i][j] = (int)nd;
            }
        }
    }

    return dist;
}


int fw_save(int **dist, int n, const char *filename) {
    FILE *f = fopen(filename, "w");
    if (!f) { perror("fw_save"); return -1; }

    fprintf(f, "Distance matrix (%d x %d):\n", n, n);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (j) fprintf(f, " ");
            if (dist[i][j] == INF) fprintf(f, "  INF");
            else                   fprintf(f, "%5d", dist[i][j]);
        }
        fprintf(f, "\n");
    }

    fclose(f);
    return 0;
}