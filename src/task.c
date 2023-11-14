#include "task.h"

void task_destroy(void *task){
    Task *to_destroy = (Task*)task;
    free(to_destroy);
}
int task_cmp(void *task, void *id){
    Task *task_ptr = (Task*)task;
    int *identifier = (int*)id;
    if(task_ptr==NULL || identifier==NULL){
        return -1;
    }
    if(task_ptr->id==*identifier){
        return 0;
    }
    return 1;
}
int todo_init(ToDoList *todo, void (*destroy)(void*), char *db_path, int (*cmp)(void*, void*)){
    List *tasklist = NULL;
    if((tasklist=(List*)malloc(sizeof(List)))==NULL){
        return -1;
    }
    list_init(tasklist,destroy,cmp);
    todo->tasklist = tasklist;
    todo->db_path = db_path;
    return 0;
}
void todo_destroy(ToDoList *todo){
    list_destroy(todo->tasklist);
    free(todo->tasklist);
    memset(todo,0,sizeof(ToDoList));
}
void todo_run(ToDoList *todo){
    todo_help();
    char command[128] = {0};
    while(fgets(command,128,stdin)!=NULL){
        command[127] = 0;

    }
}
void todo_add(ToDoList *todo){

}
void todo_remove(ToDoList *todo, int id){

}
void todo_shift(ToDoList *todo){

}
void todo_pop(ToDoList *todo){

}
void todo_change(ToDoList *todo, int id, int completed){

}
void todo_view(ToDoList *todo){

}
void todo_commit(ToDoList *todo){

}
void todo_help(){

}