#ifndef TASK_H_
#define TASK_H_
#include "list.h"

typedef struct Task{
    char description[256];
    int completed;
    int id;
}Task;

typedef struct ToDoList{
    List *tasklist;
    char *db_path;
}ToDoList;

void todo_init(ToDoList *todo, void (*destroy)(void*), char *db_path, int (*cmp)(void*, void*));
void todo_destroy(ToDoList *todo);
void todo_run(ToDoList *todo);
void todo_add(ToDoList *todo);
void todo_remove(ToDoList *todo, int id);
void todo_shift(ToDoList *todo);
void todo_pop(ToDoList *todo);
void todo_change(ToDoList *todo, int id, int completed);
void todo_view(ToDoList *todo);
void todo_help();

#endif