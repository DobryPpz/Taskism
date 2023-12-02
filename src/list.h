#ifndef LIST_H_
#define LIST_H_
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct{
    void *data;
    struct Element *next;
}Element;

typedef struct{
    void (*destroy)(void *data);
    Element *head;
    Element *tail;
    int (*cmp)(void*, void*);
    int length;
}List;

void list_init(List *list, void (*destroy)(void*), int (*cmp)(void*, void*));
void list_destroy(List *list);
void list_clear(List *list);
int list_append(List *list, void *data);
int list_prepend(List *list, void *data);
void *list_get(List *list, void *key);
int list_shift(List *list, void **data);
int list_pop(List *list, void **data);
int list_remove(List *list, void *key, void **data);
int is_empty(List *list);

#endif