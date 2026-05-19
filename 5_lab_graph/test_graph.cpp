#include <gtest/gtest.h>

extern "C" {
#include "graph.h"
}

TEST(Graph, CreateAndFree) {        //Граф создаётся, все веса по умолчанию INF
    Graph *g = graph_create(4);
    ASSERT_NE(g, nullptr);
    EXPECT_EQ(g->n, 4);
    EXPECT_EQ(graph_get_weight(g, 0, 0), 0);
    EXPECT_EQ(graph_get_weight(g, 0, 1), INF);
    graph_free(g);
}

TEST(Graph, AddAndRemoveEdge) {          // Добавление и удаление 
    Graph *g = graph_create(4);
    graph_add_edge(g, 0, 1, 7);
    EXPECT_EQ(graph_get_weight(g, 0, 1), 7);
    EXPECT_EQ(graph_get_weight(g, 1, 0), 7);
    graph_remove_edge(g, 0, 1);
    EXPECT_EQ(graph_get_weight(g, 0, 1), INF);
    graph_free(g);
}

TEST(Graph, SaveAndLoad) {              //Сохранение графа в файл и загрузка
    Graph *g = graph_create(3);
    graph_add_edge(g, 0, 1, 2);
    graph_add_edge(g, 1, 2, 5);
    graph_save(g, "lab5_test_tmp.txt");
    graph_free(g);

    Graph *g2 = graph_load("lab5_test_tmp.txt");
    ASSERT_NE(g2, nullptr);
    EXPECT_EQ(graph_get_weight(g2, 0, 1), 2);
    EXPECT_EQ(graph_get_weight(g2, 0, 2), INF);
    graph_free(g2);
}

TEST(FloydWarshall, ShortestPaths) {        // Floyd-Warshall находит правильные
    Graph *g = graph_create(4);
    graph_add_edge(g, 0, 1, 1);
    graph_add_edge(g, 1, 2, 2);
    graph_add_edge(g, 2, 3, 1);
    graph_add_edge(g, 0, 3, 100);
    int **dist = floyd_warshall(g);
    ASSERT_NE(dist, nullptr);
    EXPECT_EQ(dist[0][3], 4);
    EXPECT_EQ(dist[0][2], 3);
    EXPECT_EQ(dist[1][3], 3);
    free_matrix(dist, g->n);
    graph_free(g);
}

TEST(FloydWarshall, Disconnected) {         //Недостижимые вершины остаются
    Graph *g = graph_create(3);
    graph_add_edge(g, 0, 1, 5);
    int **dist = floyd_warshall(g);
    ASSERT_NE(dist, nullptr);
    EXPECT_EQ(dist[0][2], INF);
    EXPECT_EQ(dist[1][2], INF);
    free_matrix(dist, g->n);
    graph_free(g);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}