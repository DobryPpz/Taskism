#ifndef TASK_H_
#define TASK_H_
#include "list.h"
#include "stringutils.h"
#include <regex.h>

typedef enum Priority{HIGH,MEDIUM,LOW} Priority;
typedef enum Order{ASC,DSC} Order;
typedef enum Status{TODO,DONE} Status;

typedef struct Task{
    char description[256];
    Status status;
    int id;
    Priority priority;
}Task;

typedef struct ToDoList{
    List *tasklist;
    char *db_path;
    int last_id;
    int has_read;
}ToDoList;

void task_destroy(void *task);
int task_cmp(void *task, void *id);
void task_display(Task *task);
int todo_init(ToDoList *todo, void (*destroy)(void*), char *db_path, int (*cmp)(void*, void*));
void todo_destroy(ToDoList *todo);
void todo_run(ToDoList *todo);
void todo_add(ToDoList *todo);
void todo_remove(ToDoList *todo, int id);
void todo_change(ToDoList *todo, int id, Status status);
void todo_priority(ToDoList *todo, int id, int priority);
void todo_view(ToDoList *todo);
void todo_commit(ToDoList *todo);
void todo_readin(ToDoList *todo);
void todo_group(ToDoList *todo, int first);
void todo_clear(ToDoList *todo);
void todo_renumerate(ToDoList *todo);
void todo_sort_priority(ToDoList *todo, Order order);
void todo_only_status(ToDoList *todo, Status status);
void todo_only_priority(ToDoList *todo, Priority priority);
void todo_like(ToDoList *todo, char *pattern);
void todo_regex(ToDoList *todo, char *pattern);
void todo_help();

#endif