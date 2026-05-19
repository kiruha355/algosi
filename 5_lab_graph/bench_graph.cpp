#include <benchmark/benchmark.h>

extern "C" {
#include "graph.h"
}

static Graph *make_graph(int n) {
    Graph *g = graph_create(n);
    srand(42);
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            if (rand() % 2) {
                int w = (rand() % 100) + 1;
                graph_add_edge(g, i, j, w);
            }
        }
    }
    return g;
}

static void BM_FloydWarshall(benchmark::State &state) {
    int n = state.range(0);
    Graph *g = make_graph(n);

    for (auto _ : state) {
        int **dist = floyd_warshall(g);
        benchmark::DoNotOptimize(dist);
        benchmark::ClobberMemory();
        free_matrix(dist, n);
    }

    graph_free(g);
}

BENCHMARK(BM_FloydWarshall)
    ->Arg(10)
    ->Arg(50)
    ->Arg(100)
    ->Arg(200)
    ->Arg(300);

BENCHMARK_MAIN();
