#include "linked_list.h"
#include <stdlib.h>


linked_list* linked_list_create(){
    struct llist* list = malloc(sizeof(struct llist));
    list->start = NULL;
    list->end = NULL;
    return list;
}


void linked_list_add(linked_list* list, void* data){
    struct node* new_node = malloc(sizeof(struct node));
    new_node->slave = NULL;
    new_node->data = data;
    if (list->start == NULL){
        list->start = new_node;
        list->end = new_node;
        return;
    }
    list->end->slave = new_node;
    list->end = new_node;
}


void list_node_add(list_node* start, void* data){
    struct node* i;
    for (i = start; i->slave != NULL; i = i->slave);
    i->slave = malloc(sizeof(struct node));
    i->slave->slave = NULL;
    i->slave->data = data;
}


void linked_list_remove(linked_list* list, int index){
    list_node* to_remove;
    int i = 0;

    if (index == 0){
        to_remove = list->start;
        list->start = to_remove->slave;
        if (to_remove->slave = NULL) list->end = NULL;
        free(to_remove);
        return;
    }

    for (to_remove = list->start; to_remove != NULL; to_remove = to_remove->slave){
        if (i == index - 1){
            if (to_remove->slave == list->end){
                list->end = to_remove;
            }
            to_remove->slave = to_remove->slave->slave;
            free(to_remove->slave);
            return;
        }
        i++;
    }
}