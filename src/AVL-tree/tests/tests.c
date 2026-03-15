#include "AVL_tree.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int testsPassed = 0;
int testsFailed = 0;

void checkStr(const char* testName, const char* expected, const char* actual)
{
    if (strcmp(expected, actual) != 0) {
        fprintf(stderr, "FAILED: %s | expected=%s actual=%s\n", testName, expected, actual);
        testsFailed++;
    } else {
        testsPassed++;
    }
}

void checkBool(const char* testName, bool expected, bool actual)
{
    if (expected != actual) {
        fprintf(stderr, "FAILED: %s | expected=%d actual=%d\n", testName, expected, actual);
        testsFailed++;
    } else {
        testsPassed++;
    }
}

void checkPtrNotNull(const char* testName, void* ptr)
{
    if (ptr == NULL) {
        fprintf(stderr, "FAILED: %s | pointer is NULL\n", testName);
        testsFailed++;
    } else {
        testsPassed++;
    }
}

void checkPtrNull(const char* testName, void* ptr)
{
    if (ptr != NULL) {
        fprintf(stderr, "FAILED: %s | pointer is NOT NULL\n", testName);
        testsFailed++;
    } else {
        testsPassed++;
    }
}

void testAVLInsertEmpty()
{
    bool error = true;

    Node* root = NULL;
    root = add(root, "AMS", "Amsterdam Airport", &error);

    checkBool("insert empty error", true, error);
    checkPtrNotNull("insert empty root", root);
    checkStr("insert empty code", "AMS", root->code);
    checkPtrNull("insert empty left", root->left);
    checkPtrNull("insert empty right", root->right);
}

void testAVLInsertTwo()
{
    bool error = true;

    Node* root = NULL;

    root = add(root, "AMS", "Amsterdam Airport", &error);
    root = add(root, "JFK", "John F Kennedy", &error);

    checkBool("insert two error", true, error);
    checkStr("root code", "AMS", root->code);
    checkPtrNotNull("right child exists", root->right);
    checkStr("right child code", "JFK", root->right->code);
}

void testAVLInsertDuplicate()
{
    bool error = true;

    Node* root = NULL;

    root = add(root, "AMS", "Amsterdam Airport", &error);
    root = add(root, "AMS", "Duplicate", &error);

    checkBool("duplicate error flag", false, error);
}

void testAVLInsertMany()
{
    bool error = true;
    Node* root = NULL;

    char code[4];
    char name[32];

    for (int i = 0; i < 1000; i++) {
        code[0] = 'A' + (i / (26 * 26)) % 26;
        code[1] = 'A' + (i / 26) % 26;
        code[2] = 'A' + i % 26;
        code[3] = '\0';

        snprintf(name, sizeof(name), "Airport %d", i);

        root = add(root, code, name, &error);
    }

    checkBool("many insert error", true, error);
    checkPtrNotNull("many insert root", root);
}

void testDeleteEmpty()
{
    bool error = true;

    Node* root = NULL;

    root = delete (root, "AMS", &error);

    checkBool("delete empty error", false, error);
    checkPtrNull("delete empty root", root);
}

void testDeleteSingleNode()
{
    bool error = true;

    Node* root = NULL;

    root = add(root, "AMS", "Amsterdam Airport", &error);
    root = delete (root, "AMS", &error);

    checkBool("delete single error", true, error);
    checkPtrNull("delete single root", root);
}

void testDeleteLeaf()
{
    bool error = true;

    Node* root = NULL;

    root = add(root, "AMS", "Amsterdam", &error);
    root = add(root, "AAA", "AAA Airport", &error);

    root = delete (root, "AAA", &error);

    checkBool("delete leaf error", true, error);
    checkStr("delete leaf root code", "AMS", root->code);
}

void testDeleteNotFound()
{
    bool error = true;

    Node* root = NULL;

    root = add(root, "AMS", "Amsterdam Airport", &error);

    root = delete (root, "SVO", &error);

    checkBool("delete not found error", false, error);
}

void testDeleteRoot()
{
    bool error = true;

    Node* root = NULL;

    root = add(root, "AMS", "Amsterdam Airport", &error);
    root = add(root, "AAA", "AAA Airport", &error);
    root = add(root, "JFK", "JFK Airport", &error);

    root = delete (root, "AMS", &error);

    checkBool("delete root error", true, error);
    checkPtrNotNull("delete root exists", root);
}

void testDeleteMany()
{
    bool error = true;

    Node* root = NULL;

    char code[4];
    char name[32];

    for (int i = 0; i < 100; i++) {
        sprintf(code, "A%02d", i);
        sprintf(name, "Airport %d", i);
        root = add(root, code, name, &error);
    }

    for (int i = 0; i < 100; i++) {
        sprintf(code, "A%02d", i);
        root = delete (root, code, &error);
    }

    checkBool("delete many error", true, error);
    checkPtrNull("delete many tree empty", root);
}

void testDeleteOneChild()
{
    bool error = true;

    Node* root = NULL;

    root = add(root, "AMS", "Amsterdam", &error);
    root = add(root, "JFK", "JFK Airport", &error);

    root = delete (root, "AMS", &error);

    checkBool("delete one child error", true, error);
    checkStr("delete one child new root", "JFK", root->code);
}

void testDeleteTwoChildren()
{
    bool error = true;

    Node* root = NULL;

    root = add(root, "JFK", "JFK", &error);
    root = add(root, "AMS", "Amsterdam", &error);
    root = add(root, "SVO", "Sheremetyevo", &error);
    root = add(root, "LED", "Pulkovo", &error);

    root = delete (root, "JFK", &error);

    checkBool("delete two children error", true, error);
}

void testDeleteRotation()
{
    bool error = true;

    Node* root = NULL;

    root = add(root, "AAA", "A", &error);
    root = add(root, "BBB", "B", &error);
    root = add(root, "CCC", "C", &error);

    root = delete (root, "AAA", &error);

    checkStr("delete rotation root", "BBB", root->code);
}

void testFindEmpty()
{
    AVL* tree = initTree();

    bool result = find(tree, "AMS");

    checkBool("find empty tree", false, result);

    avlFree(&tree);
}

void testFindExisting()
{
    bool error = true;

    AVL* tree = initTree();

    tree->root = add(tree->root, "AMS", "Amsterdam Airport", &error);
    tree->root = add(tree->root, "JFK", "John F Kennedy", &error);

    bool result = find(tree, "AMS");

    checkBool("find existing", true, result);

    avlFree(&tree);
}

void testFindNotExisting()
{
    bool error = true;

    AVL* tree = initTree();

    tree->root = add(tree->root, "AMS", "Amsterdam Airport", &error);
    tree->root = add(tree->root, "JFK", "John F Kennedy", &error);

    bool result = find(tree, "SVO");

    checkBool("find not existing", false, result);

    avlFree(&tree);
}

void testFindRoot()
{
    bool error = true;

    AVL* tree = initTree();

    tree->root = add(tree->root, "AMS", "Amsterdam Airport", &error);

    bool result = find(tree, "AMS");

    checkBool("find root", true, result);

    avlFree(&tree);
}

void testFindLeaf()
{
    bool error = true;

    AVL* tree = initTree();

    tree->root = add(tree->root, "AMS", "Amsterdam Airport", &error);
    tree->root = add(tree->root, "AAA", "AAA Airport", &error);

    bool result = find(tree, "AAA");

    checkBool("find leaf", true, result);

    avlFree(&tree);
}

void testFindAfterDelete()
{
    bool error = true;

    AVL* tree = initTree();

    tree->root = add(tree->root, "AMS", "Amsterdam Airport", &error);

    tree->root = delete (tree->root, "AMS", &error);

    bool result = find(tree, "AMS");

    checkBool("find after delete", false, result);

    avlFree(&tree);
}

void testFindMany()
{
    bool error = true;

    AVL* tree = initTree();

    char code[4];
    char name[32];

    for (int i = 0; i < 100; i++) {
        sprintf(code, "A%02d", i);
        sprintf(name, "Airport %d", i);
        tree->root = add(tree->root, code, name, &error);
    }

    bool result = find(tree, "A50");

    checkBool("find many", true, result);

    avlFree(&tree);
}

int runTests()
{
    testAVLInsertEmpty();
    testAVLInsertTwo();
    testAVLInsertDuplicate();
    testAVLInsertMany();

    testDeleteEmpty();
    testDeleteSingleNode();
    testDeleteLeaf();
    testDeleteOneChild();
    testDeleteTwoChildren();
    testDeleteNotFound();
    testDeleteRoot();
    testDeleteMany();
    testDeleteRotation();

    testFindEmpty();
    testFindExisting();
    testFindNotExisting();
    testFindRoot();
    testFindLeaf();
    testFindAfterDelete();
    testFindMany();

    fprintf(stderr, "\nTests passed: %d\nTests failed: %d\n", testsPassed, testsFailed);

    if (testsFailed == 0) {
        fprintf(stderr, "ALL TESTS PASSED\n");
    } else {
        return 1;
    }

    return 0;
}

int main()
{
    return runTests();
}
