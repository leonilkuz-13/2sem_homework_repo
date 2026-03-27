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
    (void)fclose(file);
    (void)printf("Loaded %d airports.\n\n", tree->size);
}

void handleFind(Avl* tree, char* cmd)
{
    char* code = cmd + 5;
    Node* ptr = find(tree, code);
    if (ptr) {
        (void)printf("%s -> %s\n\n", ptr->code, ptr->fullName);
    } else {
        (void)printf("Airport '%s' not found.\n\n", code);
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
            (void)puts("Added.");
            (void)puts("");
        } else {
            (void)puts("Error: Airport already exists.");
            (void)puts("");
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
        (void)puts("Deleted.");
        (void)puts("");
    } else {
        (void)puts("Error: Code not found.");
        (void)puts("");
    }
}

void save(Node* root, FILE* file)
{
    if (!root) {
        return;
    }

    save(root->left, file);
    (void)fprintf(file, "%s %s\n", root->code, root->fullName);
    save(root->right, file);
}

int main(int argc, char* argv[])
{
    if (argc < 2) {
        (void)puts("Usage: ./utility_airports ../src/airports.txt");
        return 1;
    }

    Avl* tree = initTree();
    if (!tree) {
        return 1;
    }

    load(tree, argv[1]);

    char cmd[256];
    while (true) {
        (void)puts("> ");
        if (!fgets(cmd, sizeof(cmd), stdin)) {
            break;
        }
        cmd[strcspn(cmd, "\r\n")] = 0;

        if (strncmp(cmd, "quit", 4) == 0) {
            break;
        }
        if (strncmp(cmd, "find ", 5) == 0) {
            handleFind(tree, cmd);
        } else if (strncmp(cmd, "add ", 4) == 0) {
            handleAdd(tree, cmd);
        } else if (strncmp(cmd, "delete ", 7) == 0) {
            handleDelete(tree, cmd);
        } else if (strcmp(cmd, "save") == 0) {
            FILE* out = fopen(argv[1], "w");
            if (out) {
                save(tree->root, out);
                (void)fclose(out);
                (void)printf("Saved %d records.\n\n", tree->size);
            }
        } else if (strlen(cmd) > 0) {
            (void)puts("Unknown command.");
            (void)puts("");
        }
    }

    avlFree(&tree);
    return 0;
}