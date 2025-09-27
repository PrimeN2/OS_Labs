#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>

int main() {
    char filename[255];
    int n = read(0, filename, sizeof(filename) - 1);
    
    if (n > 0) {
        filename[n] = '\0';
    }

    for (int i = 0; filename[i] != '\0'; ++i) {
        if (filename[i] == '\n') {
            filename[i] = 0;
            break;
        }
    }
    
    int fd = open(filename, O_RDONLY);

    if (fd < 0) {
        write(2, "Can't open the file\n", 20);
        return 1;
    }

    int pipefd[2];
    pipe(pipefd);

    pid_t pid = fork();

    if (pid == 0) {
        close(pipefd[0]);
        dup2(fd, 0);
        dup2(pipefd[1], 1);
        close(fd);
        close(pipefd[1]);

        execl("./child", "child", NULL);

        write(2, "exec did't work out\n", 20);
        _exit(1);

    } else {
        close(pipefd[1]);
        close(fd);

        char buffer[1024];
        int n;
        while ((n = read(pipefd[0], buffer, sizeof(buffer))) > 0) {
            write(1, buffer, n);
        }

        close(pipefd[0]);
        wait(NULL);
    }
}