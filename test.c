#define NULL ((void*) 0)
#include <inttypes.h>
#include <stdio.h>
#include <string.h>

uint32_t adler32(char* data, int size){
    uint64_t a=1, b=0;
    for (int i = 0; i < size; i++){
        a += data[i];
        b += a;
    }
    a %= 65521;
    b %= 65521;
    return (uint32_t) (b * 65536 + a);
}

int main(){
    printf("%d\n", strstr("abcde", "a") == NULL);
}