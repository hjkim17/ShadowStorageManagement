//
// Created by csrc on 20. 9. 11..
//

#ifndef SHADOWSTORAGEMANAGEMENT_STORAGEELEMENT_H
#define SHADOWSTORAGEMANAGEMENT_STORAGEELEMENT_H

#include <string>

#define STORAGE_PERSISTENT 0
#define STORAGE_NONPERSISTENT 1
class StorageElement {
private:
    std::string path;
    int storage_type; // memory: 0, storage: 1
public:
    StorageElement(std::string path, int storage_type) {
        this->path = path;
        this->storage_type = storage_type;
    }
    std::string get_path() {return path;}
    int get_storage_type() {return storage_type;}
    virtual FILE* requestReadOnly(const char* modes) = 0;
    virtual FILE* request(const char* modes) = 0;
};
class PersistentElement : public StorageElement{
private:
public:
    PersistentElement(std::string path) : StorageElement(path, STORAGE_PERSISTENT) {}
    FILE* requestReadOnly(const char* modes);
    FILE* request(const char* modes);
};
class NonPersistentElement : public StorageElement{
private:
    void* data;
    int filesize_limit;
public:
    NonPersistentElement *prev, *next;
    NonPersistentElement(std::string path, int filesize) : StorageElement(path, STORAGE_NONPERSISTENT) {
        this->filesize_limit = filesize;
        data = (void*)malloc(sizeof(char)*filesize);
        prev = NULL;
        next = NULL;
    }
    ~NonPersistentElement() {
        free(data);
    }
    FILE* requestReadOnly(const char* modes) {
        return fmemopen(data, filesize_limit, modes);
    }
    FILE* request(const char* modes) {
        return fmemopen(data, filesize_limit, modes);
    }
};
#endif //SHADOWSTORAGEMANAGEMENT_STORAGEELEMENT_H
