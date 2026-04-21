#include "avl.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>

static int g_pass = 0;

#define TEST(name) do { printf("  [TEST] %s ... ", name); fflush(stdout); } while(0)
#define PASS       do { printf("OK\n"); g_pass++; } while(0)


static void test_split(void) {
    TEST("split");

    AVLTree src; avl_init(&src);
    avl_insert(&src, "aaa", 1.0);
    avl_insert(&src, "bbb", 2.0);
    avl_insert(&src, "ccc", 3.0);
    avl_insert(&src, "ddd", 4.0);
    avl_insert(&src, "eee", 5.0);

    AVLTree left, right;
    avl_split(&src, "ccc", &left, &right);

    assert(avl_search(&left, "aaa", NULL) == 1);
    assert(avl_search(&left, "bbb", NULL) == 1);
    assert(avl_search(&left, "ccc", NULL) == 1);
    assert(avl_search(&left, "ddd", NULL) == 0);
    assert(avl_search(&left, "eee", NULL) == 0);

    assert(avl_search(&right, "ddd", NULL) == 1);
    assert(avl_search(&right, "eee", NULL) == 1);
    assert(avl_search(&right, "aaa", NULL) == 0);

    avl_free(&src); avl_free(&left); avl_free(&right);
    PASS;
}


static void test_merge(void) {
    TEST("merge");

    AVLTree t1, t2, result;
    avl_init(&t1); avl_init(&t2);

    avl_insert(&t1, "aaa", 1.0);
    avl_insert(&t1, "bbb", 2.0);
    avl_insert(&t2, "ccc", 3.0);
    avl_insert(&t2, "ddd", 4.0);

    avl_merge(&t1, &t2, &result);

    double v;
    assert(avl_search(&result, "aaa", &v) == 1); assert(fabs(v-1.0)<1e-9);
    assert(avl_search(&result, "bbb", &v) == 1); assert(fabs(v-2.0)<1e-9);
    assert(avl_search(&result, "ccc", &v) == 1); assert(fabs(v-3.0)<1e-9);
    assert(avl_search(&result, "ddd", &v) == 1); assert(fabs(v-4.0)<1e-9);
    assert(avl_search(&result, "zzz", NULL) == 0);

    avl_free(&t1); avl_free(&t2); avl_free(&result);
    PASS;
}


static void test_insert_search(void) {
    TEST("insert and search ");

    AVLTree t; avl_init(&t);
    avl_insert(&t, "abc", 3.14);
    avl_insert(&t, "xyz", 2.71);

    double v;
    assert(avl_search(&t, "abc", &v) == 1); assert(fabs(v-3.14)<1e-9);
    assert(avl_search(&t, "xyz", &v) == 1); assert(fabs(v-2.71)<1e-9);
    assert(avl_search(&t, "zzz", NULL) == 0);

    avl_free(&t);
    PASS;
}


static void test_delete(void) {
    TEST("delete");

    AVLTree t; avl_init(&t);
    avl_insert(&t, "aa", 1.0);
    avl_insert(&t, "bb", 2.0);
    avl_insert(&t, "cc", 3.0);

    assert(avl_delete(&t, "bb") == 1);
    assert(avl_search(&t, "bb", NULL) == 0);
    assert(avl_search(&t, "aa", NULL) == 1);
    assert(avl_search(&t, "cc", NULL) == 1);
    assert(avl_delete(&t, "zz") == 0);

    avl_free(&t);
    PASS;
}

static void test_split_then_merge(void) {
    TEST("split + merge");

    AVLTree src; avl_init(&src);
    const char *keys[] = {"aaa","bbb","ccc","ddd","eee","fff"};
    double      vals[] = {1.1,   2.2,  3.3,  4.4,  5.5,  6.6};
    for (int i = 0; i < 6; i++) avl_insert(&src, keys[i], vals[i]);

    AVLTree left, right;
    avl_split(&src, "ccc", &left, &right);

    AVLTree merged;
    avl_merge(&left, &right, &merged);

    double v;
    for (int i = 0; i < 6; i++) {
        assert(avl_search(&merged, keys[i], &v) == 1);
        assert(fabs(v - vals[i]) < 1e-9);
    }

    avl_free(&src); avl_free(&left); avl_free(&right); avl_free(&merged);
    PASS;
}

int main(void) {

    test_split();
    test_merge();
    test_insert_search();
    test_delete();
    test_split_then_merge();

}