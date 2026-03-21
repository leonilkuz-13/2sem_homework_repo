#include "AVL_tree.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Avl* initTree()
{
    Avl* tree = malloc(sizeof(Avl));
    if (tree == NULL) {
        return NULL;
    }

    tree->root = NULL;
    tree->size = 0;
    return tree;
}

Node* initNode(const char* code, const char* fullName)
{
    Node* node = malloc(sizeof(Node));
    if (node == NULL) {
        return NULL;
    }

    node->balanced = 0;
    node->left = NULL;
    node->right = NULL;

    node->code = malloc(strlen(code) + 1);
    node->fullName = malloc(strlen(fullName) + 1);
    if (node->code == NULL || node->fullName == NULL) {
        free(node->code);
        free(node->fullName);
        free(node);
        return NULL;
    }

    strcpy(node->code, code);
    strcpy(node->fullName, fullName);

    return node;
}

void freeNode(Node* node)
{
    if (node == NULL) {
        return;
    }
    free(node->code);
    free(node->fullName);
    free(node);
}

static void cleanTree(Node* root)
{
    if (root == NULL) {
        return;
    }

    cleanTree(root->left);
    cleanTree(root->right);
    freeNode(root);
}

void avlFree(Avl** treeRef)
{
    if (treeRef == NULL || *treeRef == NULL) {
        return;
    }

    cleanTree((*treeRef)->root);
    free(*treeRef);
    *treeRef = NULL;
}

static int nodeHeight(const Node* node)
{
    if (node == NULL) {
        return -1;
    }

    int leftHeight = nodeHeight(node->left);
    int rightHeight = nodeHeight(node->right);

    return (1 + ((leftHeight >= rightHeight) ? leftHeight : rightHeight));
}

static Node* search(Node* node, const char* code)
{
    if (node == NULL) {
        return NULL;
    }

    int value = strcmp(node->code, code);
    if (value == 0) {
        return node;
    }

    if (value > 0) {
        return search(node->left, code);
    }
    return search(node->right, code);
}

static Node* rootMin(Node* root)
{
    if (root == NULL) {
        return NULL;
    }

    while (root->left != NULL) {
        root = root->left;
    }

    return root;
}

static int getBalance(Node* node)
{
    if (node == NULL) {
        return 0;
    }
    return nodeHeight(node->left) - nodeHeight(node->right);
}

static void updateHeight(Node* node)
{
    if (node == NULL) {
        return;
    }
    node->balanced = getBalance(node);
}

static Node* rotateL(Node* node)
{
    if (node == NULL || node->right == NULL) {
        return node;
    }

    Node* root = node->right;
    node->right = root->left;
    root->left = node;

    updateHeight(node);
    updateHeight(root);

    return root;
}

static Node* rotateR(Node* node)
{
    if (node == NULL || node->left == NULL) {
        return node;
    }

    Node* root = node->left;
    node->left = root->right;
    root->right = node;

    updateHeight(node);
    updateHeight(root);

    return root;
}

static Node* rotateLR(Node* node)
{
    if (node == NULL) {
        return NULL;
    }
    node->left = rotateL(node->left);
    return rotateR(node);
}

static Node* rotateRL(Node* node)
{
    if (node == NULL) {
        return NULL;
    }
    node->right = rotateR(node->right);
    return rotateL(node);
}

static Node* balance(Node* node)
{
    if (node == NULL) {
        return NULL;
    }

    updateHeight(node);

    if (node->balanced == 2) {
        if (getBalance(node->left) >= 0) {
            return rotateR(node);
        }
        return rotateLR(node);
    }

    if (node->balanced == -2) {
        if (getBalance(node->right) <= 0) {
            return rotateL(node);
        }
        return rotateRL(node);
    }

    return node;
}

Node* add(Node* root, const char* code, const char* fullname, bool* error)
{
    if (*error == false) {
        return root;
    }

    if (root == NULL) {
        Node* newNode = initNode(code, fullname);
        if (newNode == NULL) {
            *error = false;
        }
        return newNode;
    }

    int value = strcmp(root->code, code);
    if (value == 0) {
        *error = false;
        return root;
    }

    if (value > 0) {
        root->left = add(root->left, code, fullname, error);
    } else {
        root->right = add(root->right, code, fullname, error);
    }

    return balance(root);
}

Node* find(Avl* tree, const char* code)
{
    if (tree == NULL) {
        return NULL;
    }
    return search(tree->root, code);
}

Node* delete(Node* root, const char* code, bool* error)
{
    if (root == NULL) {
        *error = false;
        return NULL;
    }

    int value = strcmp(root->code, code);
    if (value > 0) {
        root->left = delete (root->left, code, error);
    } else if (value < 0) {
        root->right = delete (root->right, code, error);
    } else {
        if (root->left == NULL || root->right == NULL) {
            Node* temp = root->left ? root->left : root->right;
            freeNode(root);
            return temp;
        }

        Node* temp = rootMin(root->right);

        free(root->code);
        free(root->fullName);
        root->code = malloc(strlen(temp->code) + 1);
        root->fullName = malloc(strlen(temp->fullName) + 1);

        if (root->code && root->fullName) {
            strcpy(root->code, temp->code);
            strcpy(root->fullName, temp->fullName);
        }

        root->right = delete (root->right, temp->code, error);
    }

    return balance(root);
}