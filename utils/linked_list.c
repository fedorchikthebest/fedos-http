#include "linked_list.h"
#include <stdlib.h>
#include <stdio.h>


linked_list* linked_list_create(void* data){
    struct llist* list = malloc(sizeof(struct llist));
    struct node* start_node = malloc(sizeof(struct node));
    start_node->data = data;
    start_node->slave = NULL;
    list->start = start_node;
    list->end = start_node;
    return list;
}


void linked_list_add(linked_list* list, void* data){
    struct node* new_node = malloc(sizeof(struct node));
    new_node->slave = NULL;
    new_node->data = data;
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