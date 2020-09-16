//
// Created by csrc on 20. 9. 16..
//

#include "shadow_interface.h"

/*
 * (virtual path, unique data), actual path, refcnt
 */
struct simplenode {
    struct simplenode* prev;
    struct simplenode* next;
    char* key;
    void* data;
};
struct simplenode_header {
    struct simplenode* head;
    struct simplenode* tail;
};

void shadow_shared_try_delete(const char * filename) {

}
void shadow_shared_try_create(const char * filename, char* source, long int size, char** actual_path) {

}