#include "../code/heap.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

int testsPassed = 0;
int testsFailed = 0;

void checkBool(const char* testName, bool expected, bool actual)
{
    if (expected != actual) {
        fprintf(stderr, "FAILED: %s | expected=%d actual=%d\n", testName, expected, actual);
        testsFailed++;
    } else {
        testsPassed++;
    }
}

void checkInt(const char* testName, int expected, int actual)
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

void testInitHeap()
{
    Heap* heap = initHeap();
    checkPtrNotNull("initHeap", heap);
    if (heap) {
        checkInt("initHeap capacity", 32, heap->capacity);
        checkInt("initHeap len", 0, heap->len);
        checkPtrNotNull("initHeap array", heap->array);
        cleanHeap(&heap);
        checkPtrNull("cleanHeap", heap);
    }
}

void testPushPopOne()
{
    Heap* heap = initHeap();
    pushHeap(heap, 10, 5);
    NodeHeap result = popHeap(heap);
    checkInt("pushPopOne dist", 10, result.dist);
    checkInt("pushPopOne city", 5, result.city);
    cleanHeap(&heap);
}

void testPushPopTwo()
{
    Heap* heap = initHeap();
    pushHeap(heap, 30, 1);
    pushHeap(heap, 20, 2);
    NodeHeap first = popHeap(heap);
    checkInt("two first dist", 20, first.dist);
    checkInt("two first city", 2, first.city);
    NodeHeap second = popHeap(heap);
    checkInt("two second dist", 30, second.dist);
    checkInt("two second city", 1, second.city);
    cleanHeap(&heap);
}

void testOrder()
{
    Heap* heap = initHeap();
    int values[] = { 5, 3, 8, 1, 4, 7, 2, 6 };
    int expected[] = { 1, 2, 3, 4, 5, 6, 7, 8 };
    for (int i = 0; i < 8; i++)
        pushHeap(heap, values[i], i);
    for (int i = 0; i < 8; i++) {
        NodeHeap result = popHeap(heap);
        checkInt("order", expected[i], result.dist);
    }
    cleanHeap(&heap);
}

void testDuplicates()
{
    Heap* heap = initHeap();
    pushHeap(heap, 5, 10);
    pushHeap(heap, 5, 20);
    NodeHeap first = popHeap(heap);
    checkInt("duplicate first dist", 5, first.dist);
    NodeHeap second = popHeap(heap);
    checkInt("duplicate second dist", 5, second.dist);
    cleanHeap(&heap);
}

void testPopEmpty()
{
    Heap* heap = initHeap();
    NodeHeap result = popHeap(heap);
    checkInt("popEmpty dist", 0, result.dist);
    checkInt("popEmpty city", 0, result.city);
    cleanHeap(&heap);
}

void testRealloc()
{
    Heap* heap = initHeap();
    for (int i = 0; i < 64; i++)
        pushHeap(heap, 100 - i, i);
    checkBool("realloc capacity > 32", heap->capacity > 32, true);
    NodeHeap result = popHeap(heap);
    checkInt("realloc min dist", 37, result.dist); // 100 - (64-1) = 37
    cleanHeap(&heap);
}

void testCleanHeapTwice()
{
    Heap* heap = initHeap();
    cleanHeap(&heap);
    cleanHeap(&heap);
    checkPtrNull("cleanHeap twice", heap);
}

int runTests()
{
    testInitHeap();
    testPushPopOne();
    testPushPopTwo();
    testOrder();
    testDuplicates();
    testPopEmpty();
    testRealloc();
    testCleanHeapTwice();

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