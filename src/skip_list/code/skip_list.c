#include "skip_list.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// максимальный ранг узла
#define MAX_LEVEL 16

Node* initNode()
{
    Node* node = malloc(sizeof(Node));
    if (node == NULL) {
        return NULL;
    }

    node->levelNode = NULL;
    node->key = 0;
    node->level = 0;
    return node;
}

List* intiList()
{
    List* list = malloc(sizeof(List));
    if (list == NULL) {
        return NULL;
    }

    list->head = NULL;
    list->tail = NULL;
    list->len = 0;
    list->maxLevel = 0;
    return list;
}

static void buildLevel(Node** node)
{
    if ((*node) == NULL) {
        return;
    }

    size_t level = 0;

    while ((double)rand() / RAND_MAX > 0.5 && level < MAX_LEVEL) {
        level++;
    }

    Node** array = calloc(level + 1, sizeof(Node*));
    if (array == NULL) {
        return;
    }

    (*node)->levelNode = array;
    (*node)->level = level;
}

Node* find(List* list, int value)
{
    if (list == NULL) {
        return NULL;
    }

    Node* current = list->head;
    if (current == NULL) {
        return NULL;
    }

    int level = list->maxLevel;
    while (level >= 0) {
        while (level <= current->level && current->levelNode[level] != NULL
               && current->levelNode[level]->key < value) {
            current = current->levelNode[level];
        }

        if (level <= current->level && current->levelNode[level] != NULL
            && current->levelNode[level]->key == value) {
            return current->levelNode[level];
        }

        level--;
    }

    return NULL;
}

void insert(List* list, int value)
{
    if (list == NULL) {
        return;
    }

    // первый элемент
    if (list->head == NULL) {
        Node* node = initNode();
        if (node == NULL)
            return;

        buildLevel(&node);
        node->key = value;

        list->head = node;
        list->maxLevel = node->level;
        list->len++;
        return;
    }

    // массив указателей для изменения связей
    Node* update[MAX_LEVEL + 1];

    Node* current = list->head;

    // заполнение update
    for (int level = list->maxLevel; level >= 0; level--) {
        while (level <= current->level && current->levelNode[level] != NULL
               && current->levelNode[level]->key < value) {
            current = current->levelNode[level];
        }

        update[level] = current;
    }

    // спавн узла
    Node* node = initNode();
    if (node == NULL) {
        return;
    }

    buildLevel(&node);
    node->key = value;

    // если максимальный уровень поменялся, то обновляем update
    int newLevel = node->level;
    if (newLevel > list->maxLevel) {
        for (int index = list->maxLevel + 1; index <= newLevel; index++) {
            update[index] = list->head;
        }
        list->maxLevel = newLevel;
    }

    // изменение связей
    for (int index = 0; index <= newLevel; index++) {
        Node* prev = update[index];
        Node* next = NULL;

        if (index <= prev->level) {
            next = prev->levelNode[index];
        }

        prev->levelNode[index] = node;
        node->levelNode[index] = next;
    }

    list->len++;
}

void delete(List* list, int value)
{
    if (list == NULL) {
        return;
    }

    Node* update[MAX_LEVEL + 1];
    Node* current = list->head;

    // заполнение update
    for (int level = list->maxLevel; level >= 0; level--) {
        while (level <= current->level && current->levelNode[level] != NULL
               && current->levelNode[level]->key < value) {
            current = current->levelNode[level];
        }

        update[level] = current;
    }

    // кандидат на удаление
    Node* target = NULL;

    if (0 <= current->level) {
        target = current->levelNode[0];
    }

    if (target == NULL || target->key != value) {
        return;
    }

    // удаляем на всех уровнях
    for (int level = 0; level <= list->maxLevel; level++) {
        if (level > update[level]->level || update[level]->levelNode[level] != target) {
            break;
        }

        update[level]->levelNode[level] = target->levelNode[level];
    }

    // чистим незаполненные уровни списка
    while (list->maxLevel > 0 && list->head->levelNode[list->maxLevel] == NULL) {
        list->maxLevel--;
    }

    free(target->levelNode);
    free(target);

    list->len--;
}