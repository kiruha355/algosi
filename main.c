#include "avl.h"
#include <stdio.h>
#include <string.h>


int main(int argc, char *argv[]) {
    AVLTree tree;
    avl_init(&tree);

    int exit_code = 0;

    if (argc == 4 && strcmp(argv[1], "-f") == 0) {
        if (avl_run_file(&tree, argv[2], argv[3]) != 0) {
            fprintf(stderr, "Failed to process file.\n");
            exit_code = 1;
        } else {
            printf("Results written to '%s'\n", argv[3]);
        }
    } else if (argc == 1) {
        avl_run_interactive(&tree);
    } else {
        exit_code = 1;
    }

    avl_free(&tree);
    return exit_code;
}