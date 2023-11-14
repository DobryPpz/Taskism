#include <stdio.h>
#include <stdlib.h>
#include "task.h"

int main(int argc, char *argv[]){
    ToDoList my_todo_list;
    char *db_path = NULL;
    if(argc>1){
        db_path = argv[1];
        FILE *test_descriptor = fopen(db_path,"r");
        if(test_descriptor==NULL){
            printf("Could not open data storage location\n");
            return -1;
        }
        fclose(test_descriptor);
    }
    else{
        db_path = getenv("HOME");
        strcat(db_path,"/.taskismdb");
        FILE *ff = NULL;
        if((ff=fopen(db_path,"r"))==NULL){
            ff = fopen(db_path,"w");
        }
        fclose(ff);
    }
    todo_init(&my_todo_list,task_destroy,db_path,task_cmp);
    todo_run(&my_todo_list);
    todo_destroy(&my_todo_list);
    return 0;
}