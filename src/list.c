#include "list.h"

void list_init(List *list, void (*destroy)(void*), int (*cmp)(void*, void*)){
    list->destroy = destroy;
    list->cmp = cmp;
    list->head = NULL;
    list->tail = NULL;
    list->length = 0;
}
void list_destroy(List *list){
    void *data = NULL;
    while(list_pop(list,(void**)&data)==0){
        if(list->destroy!=NULL){
            list->destroy(data);
        }
    }
    memset(list,0,sizeof(List));
}
int list_append(List *list, void *data){
    Element *new_element = NULL;
    if((new_element=(Element*)malloc(sizeof(Element)))==NULL){
        return -1;
    }
    new_element->data = data;
    new_element->next = NULL;
    if(list->head==NULL){
        list->head = new_element;
        list->tail = new_element;
    }
    else{
        list->tail->next = new_element;
        list->tail = new_element;
    }
    list->length += 1;
    return 0;
}
int list_prepend(List *list, void *data){
    Element *new_element = NULL;
    if((new_element=(Element*)malloc(sizeof(Element)))==NULL){
        return -1;
    }
    new_element->data = data;
    new_element->next = NULL;
    if(list->head==NULL){
        list->head = new_element;
        list->tail = new_element;
    }
    else{
        new_element->next = list->head;
        list->head = new_element;
    }
    list->length += 1;
    return 0;
}
void *list_get(List *list, void *key){
    if(list==NULL || list->head==NULL){
        return NULL;
    }
    Element *traverser = list->head;
    while(traverser!=NULL && list->cmp(traverser->data,key)!=0){
        traverser = traverser->next;
    }
    if(traverser!=NULL){
        return traverser->data;
    }
    return NULL;
}
int list_shift(List *list, void **data){
    if(list==NULL || list->head==NULL){
        return -1;
    }
    Element *old_element = NULL;
    if(list->head==list->tail){
        old_element = list->head;
        list->head = NULL;
        list->tail = NULL;
    }
    else{
        old_element = list->head;
        list->head = list->head->next;
        old_element->next = NULL;
    }
    *data = old_element->data;
    free(old_element);
    list->length -= 1;
    return 0;
}
int list_pop(List *list, void **data){
    if(list==NULL || list->head==NULL){
        return -1;
    }
    Element *old_element = NULL;
    if(list->head==list->tail){
        old_element = list->head;
        list->head = NULL;
        list->tail = NULL;
    }
    else{
        Element *traverser = list->head;
        while(traverser->next!=list->tail){
            traverser = traverser->next;
        }
        old_element = traverser->next;
        traverser->next = NULL;
        list->tail = traverser;
    }
    *data = old_element->data;
    free(old_element);
    list->length -= 1;
    return 0;
}
int list_remove(List *list, void *key, void **data){
    if(list==NULL || list->head==NULL){
        return -1;
    }
    if(list->cmp(list->head->data,key)==0){
        int ret = list_shift(list,(void**)&data);
        list->destroy(data);
        return ret;
    }
    if(list->cmp(list->tail->data,key)==0){
        int ret = list_pop(list,(void**)&data);
        list->destroy(data);
        return ret;
    }
    Element *traverser = list->head;
    Element *old_element = NULL;
    while(traverser!=NULL && list->cmp(traverser->next->data,key)!=0){
        traverser = traverser->next;
    }
    if(traverser==NULL){
        return -1;
    }
    old_element = traverser->next;
    traverser->next = traverser->next->next;
    old_element->next = NULL;
    *data = old_element->data;
    free(old_element);
    list->length -= 1;
    return 0;
}