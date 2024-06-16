#include <inttypes.h>

typedef struct tree_node
{
    struct tree_node* child;
    struct tree_node* brother;
    uint32_t id;
    void* data;
} tree_node;


tree_node* create_tree(void* data);
void add_tree_node(uint32_t* path, void* data, int path_len, tree_node* root);
void* get_tree_node(uint32_t* path, int path_len, tree_node* root);
int32_t adler32(char* data, int size);