// -lpthread
//
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <pthread.h>
#include <unistd.h>

#define MAX_THREADS 10

// Structure to hold the search parameters
typedef struct {
    char* dir;
    pthread_t tid;
} SearchParams;

// Function to check if a file/folder has the SUID bit set
int hasSUIDBitSet(const char* path) {
    struct stat fileStat;
    if (stat(path, &fileStat) == -1) {
        perror("stat");
        return 0;
    }
    return (fileStat.st_mode & S_ISUID);
}

// Function to recursively search directories for files/folders with the SUID bit set
void* searchDirectories(void* arg) {
    SearchParams* params = (SearchParams*)arg;
    DIR* dir = opendir(params->dir);
    if (dir == NULL) {
        perror("opendir");
        pthread_exit(NULL);
    }

    struct dirent* entry;
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_DIR) {
            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
                continue;

            char newPath[1024];
            snprintf(newPath, sizeof(newPath), "%s/%s", params->dir, entry->d_name);

            SearchParams newParams;
            newParams.dir = newPath;
            pthread_create(&newParams.tid, NULL, searchDirectories, &newParams);
            pthread_join(newParams.tid, NULL);
        } else {
            char filePath[1024];
            snprintf(filePath, sizeof(filePath), "%s/%s", params->dir, entry->d_name);
            if (hasSUIDBitSet(filePath)) {
                printf("Found SUID bit set: %s\n", filePath);
            }
        }
    }

    closedir(dir);
    pthread_exit(NULL);
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("Usage: %s <directory>\n", argv[0]);
        return 1;
    }

    // Create the root search parameters
    SearchParams rootParams;
    rootParams.dir = argv[1];

    // Create the initial thread for the root directory
    pthread_create(&rootParams.tid, NULL, searchDirectories, &rootParams);
    pthread_join(rootParams.tid, NULL);

    return 0;
}

