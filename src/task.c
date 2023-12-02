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
void task_display(Task *task){
    printf("%s %d (%s priority):\n\t%s",(task->status==DONE ? "DONE":"TODO"),
            task->id,
            (task->priority==HIGH ? "high" : (task->priority==MEDIUM ? "medium" : "low")),
            task->description);
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
        if(first_part==NULL){
            memset(command,0,128);
            continue;
        }
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
        else if(strcmp(first_part,"priority")==0){
            first_part = strtok(NULL," \t\n");
            if(first_part==NULL){
                printf("In what order?\n");
            }
            else{
                if(strcmp(first_part,"asc")==0){
                    todo_sort_priority(todo,ASC);
                }
                else if(strcmp(first_part,"dsc")==0){
                    todo_sort_priority(todo,DSC);
                }
                else{
                    printf("Wrong order\n");
                }
            }
        }
        else if(strcmp(first_part,"high")==0){
            first_part = strtok(NULL," \t\n");
            if(first_part==NULL){
                printf("To which task set high priority?\n");
            }
            else{
                sscanf(first_part,"%d",&param);
                todo_priority(todo,param,HIGH);
            }
        }
        else if(strcmp(first_part,"medium")==0){
            first_part = strtok(NULL," \t\n");
            if(first_part==NULL){
                printf("To which task set medium priority?\n");
            }
            else{
                sscanf(first_part,"%d",&param);
                todo_priority(todo,param,MEDIUM);
            }
        }
        else if(strcmp(first_part,"low")==0){
            first_part = strtok(NULL," \t\n");
            if(first_part==NULL){
                printf("To which task set low priority?\n");
            }
            else{
                sscanf(first_part,"%d",&param);
                todo_priority(todo,param,LOW);
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
                    todo_group(todo,DONE);
                }
                else if(strcmp(first_part,"todo")==0){
                    todo_group(todo,TODO);
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
        else if(strcmp(first_part,"clear")==0){
            todo_clear(todo);
        }
        else if(strcmp(first_part,"renumerate")==0){
            todo_renumerate(todo);
            todo_view(todo);
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
    printf("Describe task:\n");
    fgets(description,256,stdin);
    description[255] = 0;
    to_add = (Task*)malloc(sizeof(Task));
    if(to_add==NULL){
        return;
    }
    memset(to_add->description,0,256);
    memmove(to_add->description,description,strlen(description));
    to_add->status = TODO;
    to_add->id = todo->last_id;
    to_add->priority = LOW;
    if(list_append(todo->tasklist,to_add)!=0){
        return;
    }
    todo->last_id += 1;
    todo_view(todo);
    printf("Task added successfully\n");
}
void todo_remove(ToDoList *todo, int id){
    Task *to_remove = NULL;
    int ret = list_remove(todo->tasklist,&id,(void**)&to_remove);
    if(ret==0){
        task_destroy(to_remove);
        todo_view(todo);
        printf("Element deleted successfully\n");
    }
    else{
        printf("Could not delete task\n");
    }
}
void todo_change(ToDoList *todo, int id, Status status){
    Task *to_change = (Task*)list_get(todo->tasklist,&id);
    if(to_change==NULL){
        printf("No such task\n");
    }
    else{
        to_change->status = status;
        todo_view(todo);
        printf("Marked task %d as %s\n",id,status==DONE ? "done" : "not done");
    }
}
void todo_priority(ToDoList *todo, int id, int priority){
    Task *to_change = (Task*)list_get(todo->tasklist,&id);
    if(to_change==NULL){
        printf("No such task\n");
    }
    else{
        to_change->priority = priority;
        todo_view(todo);
        printf("Marked task %d as having %s priority\n",id,
            priority==HIGH ? "high" : (priority==MEDIUM ? "medium" : "low"));
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
        task_display(task);
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
        fprintf(db,"%s%d\n%d\n",task->description,task->status,task->priority);
        element = element->next;
    }
    fclose(db);
    printf("List saved successfully\n");
}
void todo_readin(ToDoList *todo){
    Task *to_add = NULL;
    char description[256] = {0};
    char completed_string[256] = {0};
    char priority_string[256] = {0};
    int priority;
    int completed;
    FILE *db = NULL;
    if((db=fopen(todo->db_path,"r"))==NULL){
        printf("Could not open database\n");
        return;
    }
    while(fgets(description,256,db)!=NULL && 
          fgets(completed_string,256,db)!=NULL &&
          fgets(priority_string,256,db)!=NULL){
        description[255] = 0;
        completed_string[255] = 0;
        priority_string[255] = 0;
        sscanf(completed_string,"%d",&completed);
        sscanf(priority_string,"%d",&priority);
        to_add = (Task*)malloc(sizeof(Task));
        if(to_add==NULL){
            return;
        }
        memset(to_add->description,0,256);
        memmove(to_add->description,description,strlen(description));
        to_add->status = completed ? DONE : TODO;
        to_add->id = todo->last_id;
        to_add->priority = priority;
        if(list_append(todo->tasklist,to_add)!=0){
            return;
        }
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
        if(removed->status==DONE){
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
    todo_view(todo);
}
void todo_clear(ToDoList *todo){
    list_clear(todo->tasklist);
    todo->last_id = 1;
}
void todo_renumerate(ToDoList *todo){
    todo->last_id = 1;
    List *tasks = todo->tasklist;
    Task *task = NULL;
    Element *element = tasks->head;
    while(element!=NULL){
        task = (Task*)element->data;
        task->id = todo->last_id;
        todo->last_id += 1;
        element = element->next;
    }
}
void todo_sort_priority(ToDoList *todo, Order order){
    List *lows = NULL;
    List *mediums = NULL;
    List *highs = NULL;
    List *tasklist = todo->tasklist;
    Task *task = NULL;
    if((lows=(List*)malloc(sizeof(List)))==NULL){
        return;
    }
    if((mediums=(List*)malloc(sizeof(List)))==NULL){
        free(lows);
        return;
    }
    if((highs=(List*)malloc(sizeof(List)))==NULL){
        free(lows);
        free(mediums);
        return;
    }
    list_init(lows,NULL,NULL);
    list_init(mediums,NULL,NULL);
    list_init(highs,NULL,NULL);
    while(list_shift(tasklist,(void**)&task)==0){
        switch(task->priority){
            case LOW:
                list_append(lows,task);
                break;
            case MEDIUM:
                list_append(mediums,task);
                break;
            case HIGH:
                list_append(highs,task);
                break;
        }
    }
    while(list_shift(mediums,(void**)&task)==0){
        list_append(tasklist,task);
    }
    if(order==ASC){
        while(list_shift(lows,(void**)&task)==0){
            list_prepend(tasklist,task);
        }
        while(list_shift(highs,(void**)&task)==0){
            list_append(tasklist,task);
        }
    }
    else{
        while(list_shift(lows,(void**)&task)==0){
            list_append(tasklist,task);
        }
        while(list_shift(highs,(void**)&task)==0){
            list_prepend(tasklist,task);
        }
    }
    list_destroy(lows);
    list_destroy(mediums);
    list_destroy(highs);
    free(lows);
    free(mediums);
    free(highs);
    todo_view(todo);
}
void todo_only_status(ToDoList *todo, Status status){
    printf("\033[2J\033[H");
    if(todo->has_read==0){
        todo_readin(todo);
    }
    List *tasklist = todo->tasklist;
    Element *element = tasklist->head;
    Task *task = NULL;

}
void todo_only_priority(ToDoList *todo, Priority priority){
    printf("\033[2J\033[H");
    if(todo->has_read==0){
        todo_readin(todo);
    }

}
void todo_help(){
    printf("Type:\n");
    printf("list                   ===> to see your todo list\n");
    printf("done x                 ===> to mark item x as done\n");
    printf("undo x                 ===> to mark item x as not done\n");
    printf("commit                 ===> to save changes\n");
    printf("clear                  ===> to clear current list\n");
    printf("add                    ===> to add a new todo item\n");
    printf("remove x               ===> to remove item x from todo list\n");
    printf("first [done|todo]      ===> to display done or todo first\n");
    printf("priority [asc|dsc]     ===> to sort tasks by priority\n");
    printf("only [done|todo]       ===> to only display done or not done tasks\n");
    printf("only [low|medium|high] ===> to only display tasks with given priority\n");
    printf("high x                 ===> to mark item x with high priority\n");
    printf("medium x               ===> to mark item x with medium priority\n");
    printf("low x                  ===> to mark item x with low priority\n");
    printf("readin                 ===> to read in saved todo list again\n");
    printf("renumerate             ===> to renumber the tasks so they start at 1\n");
    printf("help                   ===> to see the list of available commands\n");
    printf("Ctrl+d                 ===> to exit\n");
}