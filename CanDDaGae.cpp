//
// Created by csrc on 20. 9. 11..
//

#include "CanDDaGae.h"
#include <iostream>

void AccessOrder::push(NonPersistentElement* np) {
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
NonPersistentElement* AccessOrder::pop() {
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
NonPersistentElement* AccessOrder::remove(NonPersistentElement* np) {
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

int ContentFileTracker::checkReadOnly(const char* modes) {
    int idx = 0;
    while(modes[idx] != 0) {
        if (modes[idx]!='r' || modes[idx]!='b')
            return 0;
    }
    return 1;
}
int ContentFileTracker::cleanup_condition() {
    if (ac.size() > CLEAN_ACTIVATION_THRESHOLD) {
        return 1;
    }
    return 0;
}
void ContentFileTracker::do_swapouts() {
    while(ac.size() > CLEAN_UNTIL_THRESHOLD) {
        std::string path = ac.pop()->get_path();
        swapout(path);
    }
    return;
}
// TODO
void ContentFileTracker::swapout(std::string key) {
}
// TODO
void ContentFileTracker::swapin(std::string key) {
}
FILE* ContentFileTracker::open(const char* filename, const char* modes) {
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
void ContentFileTracker::debug_stats() {
    std::cout<<"element count: "<<elems.size()<<"\n";
    std::cout<<"element lists\n";
    std::cout<<"IDX\tPATHt\TYPE\n";
    auto it = elems.begin();
    while(it != elems.end()) {

        it++;
    }
}