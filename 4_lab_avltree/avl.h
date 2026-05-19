#ifndef AVL_H
#define AVL_H

#include <stdio.h>

#define MAX_KEY_LEN 7

typedef struct AVLNode {
    char key[MAX_KEY_LEN];
    double value;
    struct AVLNode *left;
    struct AVLNode *right;
    int height;
} AVLNode;

typedef struct {
    AVLNode *root;
} AVLTree;

void avl_init(AVLTree *tree);
int  avl_insert(AVLTree *tree, const char *key, double value);
int  avl_delete(AVLTree *tree, const char *key);
int  avl_search(const AVLTree *tree, const char *key, double *out_value);
void avl_print(const AVLTree *tree, FILE *out);
void avl_free(AVLTree *tree);

int avl_run_file(AVLTree *tree, const char *in_path, const char *out_path);
int avl_run_interactive(AVLTree *tree);

int avl_is_valid_key(const char *key);

void avl_split(AVLTree *src, const char *pivot, AVLTree *left, AVLTree *right);
void avl_merge(AVLTree *left, AVLTree *right, AVLTree *result);

#endif