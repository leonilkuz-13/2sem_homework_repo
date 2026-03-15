#include "AVL_tree.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

AVL* initTree()
{
    AVL* tree = malloc(sizeof(AVL));
    if (tree == NULL) {
        return NULL;
    }

    tree->root = NULL;
    tree->size = 0;
    return tree;
}

static void cleanTree(Node* root)
{
    if (root == NULL) {
        return;
    }

    cleanTree(root->left);
    cleanTree(root->right);
    free(root);
}

void avlFree(AVL** treeRef)
{
    AVL* t = *treeRef;
    if (treeRef == NULL || *treeRef == NULL) {
        return;
    }

    cleanTree(t->root);
    free(t);
    *treeRef = NULL;
}

static int nodeHeight(const Node* node)
{
    if (node == NULL) {
        return -1; // nodeHeight(NULL) = -1;
    }

    int leftHeight = nodeHeight(node->left);
    int rightHeight = nodeHeight(node->right);

    return (1 + ((leftHeight >= rightHeight) ? leftHeight : rightHeight));
}

static int avlHeight(AVL* tree)
{
    if (tree == NULL) {
        return 0;
    }
    return nodeHeight(tree->root);
}

static Node* search(Node* node, const char* code)
{
    if (node == NULL) {
        return node;
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

static Node* rotateL(Node* node) // левое вращение правого поддерева
{
    if (node == NULL || node->right == NULL) {
        return NULL;
    }

    Node* root = node->right;
    node->right = root->left;
    root->left = node;

    updateHeight(root->left);
    updateHeight(root);

    return root;
}

static Node* rotateR(Node* node) // правое вращение левого поддерева
{
    if (node == NULL || node->left == NULL) {
        return NULL;
    }

    Node* root = node->left;
    node->left = root->right;
    root->right = node;

    updateHeight(root->right);
    updateHeight(root);

    return root;
}

static Node* rotateLR(Node* node) // left->right->root
{
    if (node == NULL) {
        return NULL;
    }

    Node* left = rotateL(node->left);
    node->left = left;
    Node* root = rotateR(node);
    return root;
}

static Node* rotateRL(Node* node) // right->left->root
{
    if (node == NULL) {
        return NULL;
    }

    Node* right = rotateR(node->right);
    node->right = right;
    Node* root = rotateL(node);
    return root;
}

static Node* balance(Node* node)
{
    if (node == NULL)
        return NULL;

    if (node->balanced == 2) {
        if (getBalance(node->left) >= 0) {
            return rotateR(node);
        } else {
            return rotateLR(node);
        }
    }

    if (node->balanced == -2) {
        if (getBalance(node->right) <= 0) {
            return rotateL(node);
        } else {
            return rotateRL(node);
        }
    }

    return node;
}

// error создан, чтобы показать, что что-то не вышло. // в вызывающем коде необходимо увеличивать
// размер tree.
Node* add(Node* root, const char* code, const char* fullname, bool* error)
{
    if (*error == false) {
        return root;
    }

    if (root == NULL) {
        Node* newNode = malloc(sizeof(Node));
        if (newNode == NULL) {
            *error = false;
            return root;
        }
        newNode->left = NULL;
        newNode->right = NULL;
        strcpy(newNode->code, code);
        strcpy(newNode->fullName, fullname);
        newNode->balanced = 0;
        return newNode;
    }

    int value = strcmp(root->code, code);
    if (value == 0) {
        *error = false;
        return root;
    }
    if (value < 0) {
        root->right = add(root->right, code, fullname, error);
        updateHeight(root);
        root = balance(root);
    } else if (value > 0) {
        root->left = add(root->left, code, fullname, error);
        updateHeight(root);
        root = balance(root);
    }
    return root;
}

bool find(AVL* tree, const char* code)
{
    if (tree == NULL) {
        return false;
    }
    if (search(tree->root, code) != NULL) {
        return true;
    }
    return false;
}

Node* delete(Node* root, const char* code, bool* error)
{
    if (root == NULL) {
        *error = false;
        return NULL;
    }

    if (*error == false) {
        return root;
    }

    int value = strcmp(root->code, code);
    if (value > 0) {
        root->left = delete (root->left, code, error);
    } else if (value < 0) {
        root->right = delete (root->right, code, error);
    } else {
        if (root->left == NULL) {
            Node* current = root->right;
            free(root);
            return current;
        }

        if (root->right == NULL) {
            Node* current = root->left;
            free(root);
            return current;
        }

        Node* node = rootMin(root->right);
        strcpy(root->code, node->code);
        root->right = delete (root->right, root->code, error);
    }

    updateHeight(root);
    return balance(root);
}
