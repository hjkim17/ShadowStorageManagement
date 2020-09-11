//
// Created by csrc on 20. 9. 9..
//

#include "CanDDaGae.h"
#include <cstring>
#include <iostream>

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