#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>

void showAllContents(char *pathname, int depth) {
    DIR *dir;
    struct dirent *entry;
    struct stat statbuf;

    dir = opendir(pathname);
    if (dir == NULL) {
        perror("opendir");
        exit(EXIT_FAILURE);
    }

    entry = readdir(dir);  // Read the first directory entry
    while (entry != NULL) {
        // Skip the entries "." and ".."
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            entry = readdir(dir);  // Read the next directory entry
            continue;
        }

        char path[1024];
        // Construct the full path using strcpy and strcat
        strcpy(path, pathname);
        strcat(path, "/");
        strcat(path, entry->d_name);

        // Print the entry with the depth indentation
        for (int i = 0; i < depth; ++i) {
            printf("  ");
        }

        printf("%s\n", entry->d_name);  // Print the entry name

        if (S_ISDIR(statbuf.st_mode)) {
            showAllContents(path, depth + 1);  // Recurse into the directory
        }

        entry = readdir(dir);  // Read the next directory entry
    }

    closedir(dir);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <directory>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    showAllContents(argv[1], 0);  // Start with depth 0
    exit(EXIT_SUCCESS);
}
