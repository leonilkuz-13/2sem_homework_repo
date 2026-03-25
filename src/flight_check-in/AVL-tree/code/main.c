#include "AVL_tree.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void load(Avl* tree, const char* fileName)
{
    FILE* file = fopen(fileName, "r");
    if (!file) {
        perror("Error opening file");
        return;
    }

    char line[256];
    bool err = true;

    while (fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\r\n")] = 0;
        if (strlen(line) < 5) {
            continue;
        }

        char code[4];
        strncpy(code, line, 3);
        code[3] = '\0';

        tree->root = add(tree->root, code, line + 4, &err);
        if (err) {
            tree->size++;
        }
        err = true;
    }
    fclose(file);
    printf("Loaded %d airports.\n\n", tree->size);
}

void handleFind(Avl* tree, char* cmd)
{
    char* code = cmd + 5;
    Node* ptr = find(tree, code);
    if (ptr) {
        printf("%s -> %s\n\n", ptr->code, ptr->fullName);
    } else {
        printf("Airport '%s' not found.\n\n", code);
    }
}

void handleAdd(Avl* tree, char* cmd)
{
    char* data = cmd + 4;
    char* split = strchr(data, ':');
    if (split) {
        *split = '\0';
        bool err = true;
        tree->root = add(tree->root, data, split + 1, &err);
        if (err) {
            tree->size++;
            puts("Added.");
            puts("");
        } else {
            puts("Error: Airport already exists.");
            puts("");
        }
    }
}

void handleDelete(Avl* tree, char* cmd)
{
    char* code = cmd + 7;
    if (find(tree, code)) {
        bool err = true;
        tree->root = delete (tree->root, code, &err);
        tree->size--;
        puts("Deleted.");
        puts("");
    } else {
        puts("Error: Code not found.");
        puts("");
    }
}

void save(Node* root, FILE* file)
{
    if (!root) {
        return;
    }

    save(root->left, file);
    fprintf(file, "%s %s\n", root->code, root->fullName);
    save(root->right, file);
}

int main(int argc, char* argv[])
{
    if (argc < 2) {
        puts("Usage: ./utility_airports ../src/airports.txt");
        return 1;
    }

    Avl* tree = initTree();
    if (!tree) {
        return 1;
    }

    load(tree, argv[1]);

    char cmd[256];
    while (true) {
        puts("> ");
        if (!fgets(cmd, sizeof(cmd), stdin)) {
            break;
        }
        cmd[strcspn(cmd, "\r\n")] = 0;

        if (strncmp(cmd, "quit", 4) == 0) {
            break;
        } else if (strncmp(cmd, "find ", 5) == 0) {
            handleFind(tree, cmd);
        } else if (strncmp(cmd, "add ", 4) == 0) {
            handleAdd(tree, cmd);
        } else if (strncmp(cmd, "delete ", 7) == 0) {
            handleDelete(tree, cmd);
        } else if (strcmp(cmd, "save") == 0) {
            FILE* out = fopen(argv[1], "w");
            if (out) {
                save(tree->root, out);
                fclose(out);
                printf("Saved %d records.\n\n", tree->size);
            }
        } else if (strlen(cmd) > 0) {
            puts("Unknown command.");
            puts("");
        }
    }

    avlFree(&tree);
    return 0;
}