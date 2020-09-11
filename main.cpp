//
// Created by csrc on 20. 9. 9..
//

#include <stdio.h>
#include <iostream>
#include <string>
#include <unordered_map>
#include <memory>
#include <cstring>
#include <vector>

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
    PersistentElement(std::string path) : StorageElement(path, STORAGE_PERSISTENT) {
    }
    FILE* requestReadOnly(const char* modes) {
        // normal fopen
        return ::fopen(get_path().c_str(), modes);
    }
    // TODO 2: implement request for PersistentElement
    FILE* request(const char* modes) {
        // LOCK the element
        // shadow refDown call
        /* SHADOW CONTEXT:
         *   LOCK the Shadow Tracker
         *   copy to memory
         *   --ref on target
         *   UNLOCK
         *   return the copy
         */
        //
        return nullptr;
    }
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
        // normal fmemopen
        return fmemopen(data, filesize_limit, modes);
    }
    FILE* request(const char* modes) {
        // normal fmemopen
        return fmemopen(data, filesize_limit, modes);
    }
};
class AccessOrder {
    NonPersistentElement *head, *tail;
    int cnt;
public:
    AccessOrder() {
        head = NULL;
        tail = NULL;
        cnt = 0;
    }
    void push(NonPersistentElement* np) {
        if (head==NULL) {
            head = np;
            tail = np;
        } else {
            np->next = head;
            np->prev = NULL;
            head->prev = np;
            head = np;
        }
        cnt++;
    }
    NonPersistentElement* pop() {
        if (cnt == 0) {
            return nullptr;
        } else {
            NonPersistentElement* np = tail;
            if(cnt == 1) {
                head = NULL;
                tail = NULL;
            } else {
                tail = tail->prev;
                tail->next = NULL;
                np->prev = NULL;
            }
            cnt--;
            return np;
        }
    }
    NonPersistentElement* remove(NonPersistentElement* np) {
        if(!np)
            return nullptr;
        NonPersistentElement* prev = np->prev;
        NonPersistentElement* next = np->next;
        if(prev) {
            prev->next = next;
            if (next) {
                next->prev = prev;
            }
        } else if(next) {
            next->prev = prev;
        }
        np->next = NULL;
        np->prev = NULL;
        return np;
    }
    int size() {return cnt;}
};

class ContentFileTracker {
private:
    std::unordered_map<std::string, std::shared_ptr<StorageElement>> elems;
#define CLEAN_ACTIVATION_THRESHOLD 2
#define CLEAN_UNTIL_THRESHOLD 2
    AccessOrder ac;

    int checkReadOnly(const char* modes) {
        int idx = 0;
        while(modes[idx] != 0) {
            if (modes[idx]!='r' || modes[idx]!='b')
                return 0;
        }
        return 1;
    }
    int cleanup_condition() {
        if (ac.size() > CLEAN_ACTIVATION_THRESHOLD) {
            return 1;
        }
        return 0;
    }
    void do_swapouts() {
        while(ac.size() > CLEAN_UNTIL_THRESHOLD) {
            std::string path = ac.pop()->get_path();
            swapout(path);
        }
        return;
    }
    // TODO
    void swapout(std::string key) {
    }
    // TODO
    void swapin(std::string key) {
    }
public:
    FILE* open(const char* filename, const char* modes) {
        int readOnly = checkReadOnly(modes);
        auto it = elems.find(filename);
        if (it != elems.end()) {
            if (it->second->get_storage_type()) {
                FILE* file = it->second->request(modes);
                NonPersistentElement* np = ac.remove(std::dynamic_pointer_cast<NonPersistentElement>(it->second).get());
                ac.push(np);
                return file;
            } else if(readOnly){
                // TODO 2: read persistent
            } else {
                // TODO 2: write persistent
                // add to non-persistent
                // add elem as recently accessed element
                // TODO 2: massive migration
                if (cleanup_condition()) {
                    do_swapouts();
                }
            }
        } else {
#define FIXED_FILESIZE 0x1000
            NonPersistentElement* np_element = new NonPersistentElement(filename, FIXED_FILESIZE);
            auto elem = std::shared_ptr<StorageElement>(np_element);
            auto it = elems.insert({filename, elem});
            ac.push(std::dynamic_pointer_cast<NonPersistentElement>(elem).get());
            if (cleanup_condition()) {
                do_swapouts();
            }
            return it.first->second->request(modes);
        }
    }
    // TODO EXT: make debug print
    void debug_stats() {
        std::cout<<"element count: "<<elems.size()<<"\n";
        std::cout<<"element lists\n";
        std::cout<<"IDX\tPATHt\TYPE\n";
        auto it = elems.begin();
        while(it != elems.end()) {

            it++;
        }
    }
};

// Opener
namespace CanDDaGae {
    ContentFileTracker tracker;
    FILE* fopen(const char* filename, const char* modes) {
        return tracker.open(filename, modes);
    }
}

int main() {
    char src[10]="test";
    char dest[10];
    memset(dest, 0, sizeof(char)*10);
    FILE* file = CanDDaGae::fopen("test.txt", "w");
    fwrite(src, sizeof(char), 5, file);
    fclose(file);
    file = CanDDaGae::fopen("test.txt", "r");
    fread(dest,sizeof(char), 5, file);
    fclose(file);
    std::cout<<dest<<"\n";
    return 0;
}