#include "../utils/linked_list.h"
#include <inttypes.h>

typedef enum request_type{
    GET,
    POST
} request_type;

typedef struct request
{
    request_type type;
    uint32_t path;
    int fd;
    char* header;
    int header_size;
    char* body;
    int body_size;
} request;


request* parse_request(char* request);