#include <stdio.h>
#include <dirent.h>

int main() {
    DIR *dir;
    struct dirent *entry;

    // Open the current directory
    dir = opendir(".");

    if (dir == NULL) {
        perror("Error opening directory");
        return 1;
    }

    entry = readdir(dir);
    printf("Name: %s\n", entry->d_name);
    long position = telldir(dir);
    // printf("%ld", position);

    // Read and print directory entries
    while ((entry = readdir(dir)) != NULL) {
        printf("Name: %s\n", entry->d_name);
    }

    // Seek to the saved position in the directory stream
    seekdir(dir, position);

    // Read and print directory entries again from the saved position
    printf("\nReading again from the saved position:\n");
    while ((entry = readdir(dir)) != NULL) {
        printf("Name: %s\n", entry->d_name);
    }

    // Close the directory stream
    closedir(dir);

    return 0;
}
