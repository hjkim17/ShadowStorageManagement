//
// Created by csrc on 20. 9. 11..
//

#include "StorageElement.h"

FILE* PersistentElement::requestReadOnly(const char* modes) {
    // normal fopen
    return ::fopen(get_path().c_str(), modes);
}
// TODO 2: implement request for PersistentElement
FILE* PersistentElement::request(const char* modes) {
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