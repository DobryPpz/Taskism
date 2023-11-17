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
    todo->last_id = 1;
    todo->has_read = 0;
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
    char *first_part = NULL;
    int param;
    while(fgets(command,128,stdin)!=NULL){
        command[127] = 0;
        first_part = strtok(command," \t\n");
        if(strcmp(first_part,"list")==0){
            todo_view(todo);
        }
        else if(strcmp(first_part,"commit")==0){
            todo_commit(todo);
        }
        else if(strcmp(first_part,"add")==0){
            todo_add(todo);
        }
        else if(strcmp(first_part,"remove")==0){
            first_part = strtok(NULL," \t\n");
            if(first_part==NULL){
                printf("What to remove?\n");
            }
            else{
                sscanf(first_part,"%d",&param);
                todo_remove(todo,param);
            }
        }
        else if(strcmp(first_part,"done")==0){
            first_part = strtok(NULL," \t\n");
            if(first_part==NULL){
                printf("Which one done?\n");
            }
            else{
                sscanf(first_part,"%d",&param);
                todo_change(todo,param,1);
            }
        }
        else if(strcmp(first_part,"undo")==0){
            first_part = strtok(NULL," \t\n");
            if(first_part==NULL){
                printf("Which one to undo?\n");
            }
            else{
                sscanf(first_part,"%d",&param);
                todo_change(todo,param,0);
            }
        }
        else if(strcmp(first_part,"first")==0){
            first_part = strtok(NULL," \t\n");
            if(first_part==NULL){
                printf("Done or not done first?\n");
            }
            else{
                if(strcmp(first_part,"done")==0){
                    todo_group(todo,1);
                }
                else if(strcmp(first_part,"todo")==0){
                    todo_group(todo,0);
                }
                else{
                    printf("wrong parameter\n");
                }
            }
        }
        else if(strcmp(first_part,"readin")==0){
            todo_clear(todo);
            todo_readin(todo);
            todo_view(todo);
        }
        else if(strcmp(first_part,"help")==0){
            todo_help();
        }
        else{
            printf("Wrong command!\n");
        }
        memset(command,0,128);
    }
}
void todo_add(ToDoList *todo){
    Task *to_add = NULL;
    char description[256] = {0};
    int id;
    printf("Describe task:\n");
    fgets(description,256,stdin);
    description[255] = 0;
    to_add = (Task*)malloc(sizeof(Task));
    if(to_add==NULL){
        return;
    }
    memset(to_add->description,0,256);
    memmove(to_add->description,description,strlen(description));
    to_add->completed = 0;
    to_add->id = todo->last_id;
    if(list_append(todo->tasklist,to_add)!=0){
        return;
    }
    todo->last_id += 1;
    printf("Task added successfully\n");
}
void todo_remove(ToDoList *todo, int id){
    Task *to_remove = NULL;
    int ret = list_remove(todo->tasklist,&id,(void**)&to_remove);
    if(ret==0){
        printf("Element deleted successfully\n");
        task_destroy(to_remove);
    }
    else{
        printf("Could not delete task\n");
    }
}
void todo_change(ToDoList *todo, int id, int completed){
    Task *to_change = (Task*)list_get(todo->tasklist,&id);
    if(to_change==NULL){
        printf("No such task\n");
    }
    else{
        to_change->completed = completed;
        printf("Marked task %d as %s\n",id,completed ? "done" : "not done");
    }
}
void todo_view(ToDoList *todo){
    printf("\033[2J\033[H");
    if(todo->has_read==0){
        todo_readin(todo);
    }
    Element *element = todo->tasklist->head;
    Task *task = NULL;
    while(element!=NULL){
        task = (Task*)element->data;
        printf("%s %d:\n\t%s",(task->completed ? "DONE":"TODO"),task->id,task->description);
        element = element->next;
    }
}
void todo_commit(ToDoList *todo){
    if(is_empty(todo->tasklist)){
        todo_readin(todo);
    }
    FILE *db = NULL;
    if((db=fopen(todo->db_path,"w"))==NULL){
        return;
    }
    Element *element = todo->tasklist->head;
    Task *task = NULL;
    while(element!=NULL){
        task = (Task*)element->data;
        fprintf(db,"%s%d\n",task->description,task->completed);
        element = element->next;
    }
    fclose(db);
    printf("List saved successfully\n");
}
void todo_readin(ToDoList *todo){
    Task *to_add = NULL;
    char description[256] = {0};
    char id_string[256] = {0};
    int completed;
    int id;
    FILE *db = NULL;
    if((db=fopen(todo->db_path,"r"))==NULL){
        printf("Could not open database\n");
        return;
    }
    while(fgets(description,256,db)!=NULL && fgets(id_string,256,db)!=NULL){
        description[255] = 0;
        id_string[255] = 0;
        sscanf(id_string,"%d",&id);
        to_add = (Task*)malloc(sizeof(Task));
        if(to_add==NULL){
            return;
        }
        memset(to_add->description,0,256);
        memmove(to_add->description,description,strlen(description));
        to_add->completed = id;
        to_add->id = todo->last_id;
        if(list_append(todo->tasklist,to_add)!=0){
            return;
        }
        printf("Succesfully read in a task\n");
        todo->last_id += 1;
    }
    todo->has_read = 1;
    fclose(db);
}
void todo_group(ToDoList *todo, int first){
    if(is_empty(todo->tasklist)){
        return;
    }
    List *done = NULL;
    List *not_done = NULL;
    void *data = NULL;
    Task *removed = NULL;
    if((done=(List*)malloc(sizeof(List)))==NULL){
        return;
    }
    if((not_done=(List*)malloc(sizeof(List)))==NULL){
        free(done);
        return;
    }
    list_init(done,NULL,NULL);
    list_init(not_done,NULL,NULL);
    while(list_shift(todo->tasklist,(void**)&data)==0){
        removed = (Task*)data;
        if(removed->completed){
            list_append(done,removed);
        }
        else{
            list_append(not_done,removed);
        }
    }
    if(first==1){
        while(list_shift(done,(void**)&data)==0){
            list_append(todo->tasklist,data);
        }
        while(list_shift(not_done,(void**)&data)==0){
            list_append(todo->tasklist,data);
        }
    }
    else{
        while(list_shift(not_done,(void**)&data)==0){
            list_append(todo->tasklist,data);
        }
        while(list_shift(done,(void**)&data)==0){
            list_append(todo->tasklist,data);
        }
    }
    list_destroy(not_done);
    list_destroy(done);
    free(not_done);
    free(done);
}
void todo_clear(ToDoList *todo){
    list_clear(todo->tasklist);
    todo->last_id = 1;
}
void todo_help(){
    printf("Type:\n");
    printf("list              ===> to see your todo list\n");
    printf("commit            ===> to save changes\n");
    printf("add               ===> to add a new todo item\n");
    printf("remove x          ===> to remove item x from todo list\n");
    printf("done x            ===> to mark item x as done\n");
    printf("undo x            ===> to mark item x as not done\n");
    printf("first [done|todo] ===> to display done or todo first\n");
    printf("readin            ===> to read in saved todo list again\n");
    printf("help              ===> to see the list of available commands\n");
    printf("Ctrl+d            ===> to exit\n");
}