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
        printf("Failed to open directory: %s", pathname);
        exit(EXIT_FAILURE);
    }

    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        char path[1024];
        snprintf(path, sizeof(path), "%s/%s", pathname, entry->d_name);

        if (stat(path, &statbuf) == -1) {
            printf("Failed to get file status for: %s", path);
            continue;
        }

        // Indent based on depth
        for (int i = 0; i < depth; ++i) {
            printf("  ");
        }

        // Check if it is a directory or a file
        if (S_ISDIR(statbuf.st_mode)) {
            printf("%s", entry->d_name);  // Print directory name
            showAllContents(path, depth + 1); // Recurse into the directory
        } else {
            printf("  %s", entry->d_name);  // Print file name with additional indentation
        }
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
