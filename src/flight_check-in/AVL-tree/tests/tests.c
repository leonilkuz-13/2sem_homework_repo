#include "AVL_tree.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int testsPassed = 0;
int testsFailed = 0;

void checkStr(const char* testName, const char* expected, const char* actual)
{
    if (actual == NULL || strcmp(expected, actual) != 0) {
        fprintf(stderr, "FAILED: %s | expected=%s actual=%s\n", testName, expected,
                actual ? actual : "NULL");
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
    if (root)
        checkStr("insert empty code", "AMS", root->code);
}

void testAVLInsertDuplicate()
{
    bool error = true;
    Node* root = NULL;

    root = add(root, "AMS", "Amsterdam Airport", &error);
    root = add(root, "AMS", "Duplicate", &error);

    checkBool("duplicate error flag", false, error);
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

void testDeleteNotFound()
{
    bool error = true;
    Node* root = NULL;

    root = add(root, "AMS", "Amsterdam Airport", &error);
    root = delete (root, "SVO", &error);

    checkBool("delete not found error", false, error);
}

void testFindExisting()
{
    bool error = true;
    Avl* tree = initTree();

    tree->root = add(tree->root, "AMS", "Amsterdam Airport", &error);
    tree->root = add(tree->root, "JFK", "John F Kennedy", &error);

    Node* result = find(tree, "AMS");

    checkPtrNotNull("find existing", result);
    if (result) {
        checkStr("find content check", "AMS", result->code);
    }

    avlFree(&tree);
}

void testFindNotExisting()
{
    bool error = true;
    Avl* tree = initTree();

    tree->root = add(tree->root, "AMS", "Amsterdam Airport", &error);
    Node* result = find(tree, "SVO");

    checkPtrNull("find not existing", result);

    avlFree(&tree);
}

void testFindAfterDelete()
{
    bool error = true;
    Avl* tree = initTree();

    tree->root = add(tree->root, "AMS", "Amsterdam Airport", &error);
    tree->root = delete (tree->root, "AMS", &error);

    Node* result = find(tree, "AMS");

    checkPtrNull("find after delete", result);

    avlFree(&tree);
}

int runTests()
{
    testAVLInsertEmpty();
    testAVLInsertDuplicate();

    testDeleteSingleNode();
    testDeleteNotFound();

    testFindExisting();
    testFindNotExisting();
    testFindAfterDelete();

    printf("\nTests passed: %d\nTests failed: %d\n", testsPassed, testsFailed);

    if (testsFailed == 0) {
        puts("ALL TESTS PASSED");
        return 0;
    }
    return 1;
}

int main()
{
    return runTests();
}