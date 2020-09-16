//
// Created by csrc on 20. 9. 9..
//

#include "CanDDaGae.h"
#include <cstring>
#include <iostream>

void simple_read_test() {
    // read from in-memory file
    char in_memory_file[11] = "1234567890";
    char result[11];
    memset(result, 0, sizeof(char)*11);
    FILE* file = CanDDaGae::fopen("simple_read_in_memory.txt", "r");
    fread(result,sizeof(char), 11, file);
    CanDDaGae::fclose(file);
    printf("simple read from in-memory file: %s\n", result);
    // read from disk file
    memset(result, 0, sizeof(char)*11);
    file = CanDDaGae::fopen("simple_read_disk_file.txt", "r");
    fread(result,sizeof(char), 11, file);
    CanDDaGae::fclose(file);
    printf("simple read from in-memory file: %s\n", result);
}
void simple_write_test() {
    // write to in-memory file
    // write to disk file
}
void read_on_read_test() {
    // read on read in-memory file

    // read on read disk file
}
void read_on_write_test() {
    // read on write in-memory file

    // read on write disk file
}
void write_on_write_test() {
	// write on write in-memory file

	// write on write disk file
}

int main() {
    simple_read_test();
    return 0;
}