#include "avl.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

static int max2(int a, int b) { return a > b ? a : b; }

static int height(const AVLNode *n) { return n ? n->height : 0; }

static void update_height(AVLNode *n) {
    if (n) n->height = 1 + max2(height(n->left), height(n->right));
}

static int balance_factor(const AVLNode *n) {           //если не -1,0,1 то поворот
    return n ? height(n->left) - height(n->right) : 0;
}

static AVLNode *new_node(const char *key, double value) {  
    AVLNode *n = (AVLNode *)malloc(sizeof(AVLNode));
    if (!n) return NULL;
    strncpy(n->key, key, MAX_KEY_LEN - 1);
    n->key[MAX_KEY_LEN - 1] = '\0';
    n->value  = value;
    n->left   = n->right = NULL;
    n->height = 1;
    return n;
}


static AVLNode *rotate_right(AVLNode *y) { //y у кого высота не сбалансированна ,x новый корень,t2 првый ребенок левого узла
    AVLNode *x  = y->left;
    AVLNode *t2 = x->right;
    x->right = y;
    y->left  = t2;
    update_height(y);
    update_height(x);
    return x;
}

static AVLNode *rotate_left(AVLNode *x) {
    AVLNode *y  = x->right;
    AVLNode *t2 = y->left;
    y->left  = x;
    x->right = t2;
    update_height(x);
    update_height(y);
    return y;
}

static AVLNode *rebalance(AVLNode *n) { //вызывается на каждом узле от вставленного до корня
    update_height(n);
    int bf = balance_factor(n);


    if (bf > 1) {
        if (balance_factor(n->left) < 0)
            n->left = rotate_left(n->left);  
        return rotate_right(n);
    }
    if (bf < -1) {
        if (balance_factor(n->right) > 0)
            n->right = rotate_right(n->right); 
        return rotate_left(n);
    }
    return n;
}

static AVLNode *insert_rec(AVLNode *n, const char *key, double value, int *err) {  //рекурсивно идут до того места где вставляем или удаляем пото рекурсивно вверх для балансировки на кждом узле
    if (!n) return new_node(key, value);

    int cmp = strcmp(key, n->key);
    if (cmp < 0)
        n->left  = insert_rec(n->left,  key, value, err);
    else if (cmp > 0)
        n->right = insert_rec(n->right, key, value, err);
    else {
        n->value = value;
        *err = 2;
        return n;
    }
    return rebalance(n);
}


static AVLNode *min_node(AVLNode *n) {  //нижний левый эл
    while (n->left) n = n->left;
    return n;
}

static AVLNode *delete_rec(AVLNode *n, const char *key, int *found) {  
    if (!n) return NULL;

    int cmp = strcmp(key, n->key);
    if (cmp < 0) {                                      //ребенок удаление когда 2 ребенка
        n->left  = delete_rec(n->left,  key, found);
    } else if (cmp > 0) {                               //удаляем корень у которого только один реб
        n->right = delete_rec(n->right, key, found);
    } else {                                            //два ребенка
        *found = 1;
        if (!n->left || !n->right) {
            AVLNode *tmp = n->left ? n->left : n->right;
            free(n);
            return tmp;
        }
        AVLNode *succ = min_node(n->right);
        strncpy(n->key, succ->key, MAX_KEY_LEN);
        n->value = succ->value;
        n->right = delete_rec(n->right, succ->key, found);
    }
    return rebalance(n);
}


void avl_init(AVLTree *tree) { tree->root = NULL; }

int avl_insert(AVLTree *tree, const char *key, double value) {
    int err = 0;
    tree->root = insert_rec(tree->root, key, value, &err);
    return err;
}

int avl_delete(AVLTree *tree, const char *key) {
    int found = 0;
    tree->root = delete_rec(tree->root, key, &found);
    return found; 
}

int avl_search(const AVLTree *tree, const char *key, double *out_value) {
    AVLNode *cur = tree->root;
    while (cur) {
        int cmp = strcmp(key, cur->key);
        if      (cmp < 0) cur = cur->left;
        else if (cmp > 0) cur = cur->right;
        else {
            if (out_value) *out_value = cur->value;
            return 1;
        }
    }
    return 0;
}

static void print_rec(const AVLNode *n, FILE *out, const char *prefix, int is_last) {
    if (!n) return;

    fprintf(out, "%s%s %s: %.4g\n",
            prefix,
            is_last ? "\\--" : "+--",
            n->key, n->value);

    
    char child_prefix[512];
    snprintf(child_prefix, sizeof(child_prefix), "%s%s",
            prefix, is_last ? "    " : "|   ");

    
    if (n->right && n->left) {
        print_rec(n->right, out, child_prefix, 0);
        print_rec(n->left,  out, child_prefix, 1);
    } else if (n->right) {
        print_rec(n->right, out, child_prefix, 1);
    } else if (n->left) {
        print_rec(n->left,  out, child_prefix, 1);
    }
}

void avl_print(const AVLTree *tree, FILE *out) {
    if (!tree->root) {
        fprintf(out, "(empty tree)\n");
        return;
    }
    fprintf(out, "%s: %.4g\n", tree->root->key, tree->root->value);
    if (tree->root->right && tree->root->left) {
        print_rec(tree->root->right, out, "", 0);
        print_rec(tree->root->left,  out, "", 1);
    } else if (tree->root->right) {
        print_rec(tree->root->right, out, "", 1);
    } else if (tree->root->left) {
        print_rec(tree->root->left,  out, "", 1);
    }
}



static void free_rec(AVLNode *n) {
    if (!n) return;
    free_rec(n->left);
    free_rec(n->right);
    free(n);
}

void avl_free(AVLTree *tree) {
    free_rec(tree->root);
    tree->root = NULL;
}


int avl_is_valid_key(const char *key) {
    if (!key || key[0] == '\0') return 0;
    int len = 0;
    for (int i = 0; key[i]; i++) {
        if (!isalpha((unsigned char)key[i])) return 0;
        len++;
    }
    return len >= 1 && len <= 6;
}

static void split_rec(AVLNode *n, const char *pivot, AVLTree *left, AVLTree *right) {   //рекурсивно проходит и разбивает на левое и правое дерево 
    if (!n) return;
    split_rec(n->left,  pivot, left, right);
    split_rec(n->right, pivot, left, right);
    if (strcmp(n->key, pivot) <= 0)
        avl_insert(left,  n->key, n->value);
    else
        avl_insert(right, n->key, n->value);
}

void avl_split(AVLTree *src, const char *pivot, AVLTree *left, AVLTree *right) {
    avl_init(left);
    avl_init(right);
    split_rec(src->root, pivot, left, right);
}

static void merge_rec(AVLNode *n, AVLTree *result) {    //такие же обходит балансировкой авл 
    if (!n) return;
    avl_insert(result, n->key, n->value);
    merge_rec(n->left,  result);
    merge_rec(n->right, result);
}

void avl_merge(AVLTree *left, AVLTree *right, AVLTree *result) {
    avl_init(result);
    merge_rec(left->root,  result);
    merge_rec(right->root, result);
}

static int process_line(AVLTree *tree, const char *line, FILE *out) {
    
    while (*line == ' ' || *line == '\t') line++;
    if (*line == '\0' || *line == '#' || *line == '\n') return 0;

    int op = 0;
    if (sscanf(line, "%d", &op) != 1) return -1;

    
    while (*line >= '0' && *line <= '9') line++;
    while (*line == ' ' || *line == '\t') line++;

    char key[MAX_KEY_LEN + 16] = {0};
    double val = 0.0;

    switch (op) {
        case 1: { 
            if (sscanf(line, "%15s %lf", key, &val) < 2) {
                fprintf(out, "ERROR: op 1 requires <key> <value>\n");
                return -1;
            }
            if (!avl_is_valid_key(key)) {
                fprintf(out, "ERROR: invalid key '%s' (1-6 latin letters)\n", key);
                return -1;
            }
            int r = avl_insert(tree, key, val);
            if (r == 2)
                fprintf(out, "OK: updated key '%s' = %.4g\n", key, val);
            else
                fprintf(out, "OK: inserted key '%s' = %.4g\n", key, val);
            break;
        }
        case 2: { 
            if (sscanf(line, "%15s", key) < 1) {
                fprintf(out, "ERROR: op 2 requires <key>\n");
                return -1;
            }
            if (!avl_is_valid_key(key)) {
                fprintf(out, "ERROR: invalid key '%s'\n", key);
                return -1;
            }
            int r = avl_delete(tree, key);
            if (r) fprintf(out, "OK: deleted key '%s'\n", key);
            else   fprintf(out, "ERROR: key '%s' not found\n", key);
            break;
        }
        case 3: { 
            avl_print(tree, out);
            break;
        }
        case 4: { 
            if (sscanf(line, "%15s", key) < 1) {
                fprintf(out, "ERROR: op 4 requires <key>\n");
                return -1;
            }
            double found_val = 0.0;
            if (avl_search(tree, key, &found_val))
                fprintf(out, "FOUND: '%s' = %.4g\n", key, found_val);
            else
                fprintf(out, "NOT FOUND: '%s'\n", key);
            break;
        }
        case 0: 
            return 1;
        default:
            fprintf(out, "ERROR: unknown operation %d\n", op);
            return -1;
    }
    return 0;
}

int avl_run_file(AVLTree *tree, const char *in_path, const char *out_path) {
    FILE *fin = fopen(in_path, "r");
    if (!fin) { perror(in_path); return -1; }

    FILE *fout = fopen(out_path, "w");
    if (!fout) { perror(out_path); fclose(fin); return -1; }

    char line[256];
    while (fgets(line, sizeof(line), fin)) {
        
        line[strcspn(line, "\r\n")] = '\0';

        fprintf(fout, "> %s\n", line);

        process_line(tree, line, fout);
        fprintf(fout, "\n");
    }

    fclose(fin);
    fclose(fout);
    return 0;
}

static char *read_line_dynamic(FILE *fp) {
    size_t cap = 128, len = 0;
    char *buf = (char *)malloc(cap);
    if (!buf) return NULL;

    int c;
    while ((c = fgetc(fp)) != EOF && c != '\n') {
        if (len + 2 > cap) {
            cap *= 2;
            char *tmp = (char *)realloc(buf, cap);
            if (!tmp) { free(buf); return NULL; }
            buf = tmp;
        }
        buf[len++] = (char)c;
    }

    if (len == 0 && c == EOF) { free(buf); return NULL; }
    if (len > 0 && buf[len - 1] == '\r') len--;
    buf[len] = '\0';
    return buf;
}


int avl_run_interactive(AVLTree *tree) {
    printf("AVL Tree CLI\n");
    printf("Commands: 1.isert <key> <val>  2.delete <key>  3.print tree  4.find <key>  0.exit\n");

    while (1) {
        printf("> ");
        fflush(stdout);
        char *line = read_line_dynamic(stdin);
        if (!line) break;
        int r = process_line(tree, line, stdout);
        free(line);
        if (r == 1) break;
    }
    return 0;
}
