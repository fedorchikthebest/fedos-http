#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "path_tree.h"


int32_t adler32(char* data, int size){
    uint64_t a=1, b=0;
    for (int i = 0; i < size; i++){
        a += data[i];
        b += a;
    }
    a %= 65521;
    b %= 65521;
    return (uint32_t) (b * 65536 + a);
}


tree_node* create_tree(void* data){
    tree_node* tree_root = malloc(sizeof(tree_node));
    tree_root->child = NULL;
    tree_root->id = 0;
    tree_root->brother = NULL;
    tree_root->data = data;
}


void add_tree_node(uint32_t* path, void* data, int path_len, tree_node* root){
    tree_node* current_node = root;
    tree_node* current;
    tree_node* end_node;
    char is_find;
    for (int i = 1; i < path_len; i++){
        if (current_node->child == NULL){
            current_node->child = malloc(sizeof(tree_node));
            current_node->child->id = path[i];
            current_node->child->brother = NULL;
            current_node->child->child = NULL;
        }
        current_node = current_node->child;
        is_find = 0;
        for (current = current_node; current != NULL; current = current->brother){
            if (current->id == path[i]){
                current_node = current;
                is_find = 1;
                break;
            }
            if (current->brother == NULL) end_node = current;
        }
        if (!is_find){
            current = end_node;
            current->brother = malloc(sizeof(tree_node));
            current->brother->id = path[i];
            current->brother->brother = NULL;
            current->brother->child = NULL;
            current = current->brother;
            current_node = current;
        }
    }
    current_node->data = data;
}


void* get_tree_node(uint32_t* path, int path_len, tree_node* root){
    tree_node* current_node = root;
    tree_node* current;
    tree_node* end_node;
    char is_find;
    for (int i = 1; i < path_len; i++){
        if (current_node->child == NULL){
            return NULL;
        }
        current_node = current_node->child;
        is_find = 0;
        for (current = current_node; current != NULL; current = current->brother){
            if (current->id == path[i]){
                current_node = current;
                is_find = 1;
                break;
            }
        }
        if (!is_find) return NULL;
    }
    return current_node->data;
}


int tst(char* a, char* b){
    printf("YAY");
}


void runner(int (*op)(char*, char*)){
    op("", "");
}