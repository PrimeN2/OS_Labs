#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <unistd.h>
#include <semaphore.h>

#define SHM_SIZE sizeof(shared_data_t)

typedef struct {
    int value;      
    int done;       
} shared_data_t;

int main() {
    char filename[256];
    printf("Enter filename: ");
    fflush(stdout);
    if (!fgets(filename, sizeof(filename), stdin)) {
        perror("Failed to read filename");
        exit(1);
    }
    filename[strcspn(filename, "\n")] = 0;

    char shm_name[64];
    char sem_empty_name[64];
    char sem_full_name[64];
    sprintf(shm_name, "/shm_%d", getpid());
    sprintf(sem_empty_name, "/sem_empty_%d", getpid());
    sprintf(sem_full_name, "/sem_full_%d", getpid());

    int shm_fd = shm_open(shm_name, O_CREAT | O_RDWR, 0666);
    if (shm_fd == -1) {
        perror("shm_open");
        exit(1);
    }

    if (ftruncate(shm_fd, SHM_SIZE) == -1) {
        perror("ftruncate");
        exit(1);
    }

    shared_data_t *shared_mem = mmap(NULL, SHM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (shared_mem == MAP_FAILED) {
        perror("mmap");
        exit(1);
    }

    shared_mem->value = 0;
    shared_mem->done = 0;

    sem_t *sem_empty = sem_open(sem_empty_name, O_CREAT | O_EXCL, 0666, 1); 
    sem_t *sem_full = sem_open(sem_full_name, O_CREAT | O_EXCL, 0666, 0);   

    pid_t pid = fork();
    if (pid == 0) {
        execl("./third_lab_child", "child", filename, shm_name, sem_empty_name, sem_full_name, NULL);
        perror("execl failed");
        exit(1);
    } else if (pid > 0) {
        while (1) {
            sem_wait(sem_full); 

            if (shared_mem->done) {
                printf("Parent: received termination signal.\n");
                break;
            }

            printf("Composite number: %d\n", shared_mem->value);
            fflush(stdout);

            sem_post(sem_empty); 
        }

        wait(NULL);

        munmap(shared_mem, SHM_SIZE);
        shm_unlink(shm_name);
        sem_close(sem_empty);
        sem_close(sem_full);
        sem_unlink(sem_empty_name);
        sem_unlink(sem_full_name);
    } else {
        perror("fork");
        exit(1);
    }

    return 0;
}