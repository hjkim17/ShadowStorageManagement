//
// Created by csrc on 20. 9. 16..
//

#ifndef SHADOWSTORAGEMANAGEMENT_SHADOW_INTERFACE_H
#define SHADOWSTORAGEMANAGEMENT_SHADOW_INTERFACE_H

void shadow_shared_try_delete(const char * filename);
void shadow_shared_try_create(const char * filename, char* source, long int size, char** actual_path);

#endif //SHADOWSTORAGEMANAGEMENT_SHADOW_INTERFACE_H
