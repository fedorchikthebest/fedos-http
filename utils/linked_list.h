typedef struct node
{
    struct node* slave;
    void* data;
} list_node;

typedef struct llist
{
    struct node* start;
    struct node* end;
} linked_list;



linked_list* linked_list_create(void* data);
void linked_list_add(linked_list* list, void* data);
list_node* search_elem(linked_list list, void* data);
void list_node_add(list_node* start, void* data);