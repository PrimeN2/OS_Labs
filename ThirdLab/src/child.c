// child.c
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>
#include <semaphore.h>

typedef struct {
    int value;
    int done;
} shared_data_t;

int is_prime(int n) {
    if (n <= 1) return 0;
    if (n == 2) return 1;
    if (n % 2 == 0) return 0;
    for (int i = 3; i * i <= n; i += 2) {
        if (n % i == 0) return 0;
    }
    return 1;
}

int main(int argc, char *argv[]) {
    if (argc != 5) {
        fprintf(stderr, "Usage: %s <filename> <shm_name> <sem_empty> <sem_full>\n", argv[0]);
        exit(1);
    }

    char *filename = argv[1];
    char *shm_name = argv[2];
    char *sem_empty_name = argv[3];
    char *sem_full_name = argv[4];

    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Cannot open file");
        exit(1);
    }

    int shm_fd = shm_open(shm_name, O_RDWR, 0666);
    if (shm_fd == -1) {
        perror("child: shm_open");
        exit(1);
    }

    shared_data_t *shared_mem = mmap(NULL, sizeof(shared_data_t), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (shared_mem == MAP_FAILED) {
        perror("child: mmap");
        exit(1);
    }

    sem_t *sem_empty = sem_open(sem_empty_name, 0);
    sem_t *sem_full = sem_open(sem_full_name, 0);

    if (sem_empty == SEM_FAILED || sem_full == SEM_FAILED) {
        perror("child: sem_open");
        exit(1);
    }

    int num;
    while (fscanf(file, "%d", &num) == 1) {
        if (num <= 0 || is_prime(num)) {
            sem_wait(sem_empty); 
            shared_mem->done = 1;
            sem_post(sem_full);
            break;
        }

        if (!is_prime(num)) {
            sem_wait(sem_empty); 
            shared_mem->value = num;
            shared_mem->done = 0;
            sem_post(sem_full); 
        }
    }

    fclose(file);

    munmap(shared_mem, sizeof(shared_data_t));
    sem_close(sem_empty);
    sem_close(sem_full);

    return 0;
}