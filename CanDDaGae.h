//
// Created by csrc on 20. 9. 11..
//

#ifndef SHADOWSTORAGEMANAGEMENT_CANDDAGAE_H
#define SHADOWSTORAGEMANAGEMENT_CANDDAGAE_H

#include "StorageElement.h"
#include <memory>
#include <unordered_map>

class AccessOrder {
    NonPersistentElement *head, *tail;
    int cnt;
public:
    AccessOrder() {
        head = nullptr;
        tail = nullptr;
        cnt = 0;
    }
    void push(NonPersistentElement* np);
    NonPersistentElement* pop();
    NonPersistentElement* remove(NonPersistentElement* np);
    int size() const {return cnt;}
};

class ContentFileTracker {
private:
    std::unordered_map<std::string, std::shared_ptr<StorageElement>> elems;
#define CLEAN_ACTIVATION_THRESHOLD 2
#define CLEAN_UNTIL_THRESHOLD 2
    AccessOrder ac;

    int checkReadOnly(const char* modes);
    int cleanup_condition();
    void do_swapouts();
    void swapout(std::string key);
    void swapin(std::string key);
public:
    FILE* open(const char* filename, const char* modes);
    void debug_stats();
};

// Opener
namespace CanDDaGae {
    ContentFileTracker tracker;
    FILE* fopen(const char* filename, const char* modes) {
        return tracker.open(filename, modes);
    }
}

#endif //SHADOWSTORAGEMANAGEMENT_CANDDAGAE_H
