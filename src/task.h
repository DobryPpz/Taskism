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
    int last_id;
    int has_read;
}ToDoList;

void task_destroy(void *task);
int task_cmp(void *task, void *id);
int todo_init(ToDoList *todo, void (*destroy)(void*), char *db_path, int (*cmp)(void*, void*));
void todo_destroy(ToDoList *todo);
void todo_run(ToDoList *todo);
void todo_add(ToDoList *todo);
void todo_remove(ToDoList *todo, int id);
void todo_change(ToDoList *todo, int id, int completed);
void todo_view(ToDoList *todo);
void todo_commit(ToDoList *todo);
void todo_readin(ToDoList *todo);
void todo_group(ToDoList *todo, int first);
void todo_help();

#endif