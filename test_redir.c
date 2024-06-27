#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main() {
    pid_t pid;
    int fd;

    // Fork a new process
    pid = fork();
    
    if (pid < 0) {
        // Fork failed
        perror("fork");
        exit(EXIT_FAILURE);
    }
    
    if (pid == 0) {
        // Child process
        // Open the file hello.txt with write permission, create it if it doesn't exist, truncate it if it does
        fd = open("hello.txt", O_WRONLY | O_CREAT | O_TRUNC);
        if (fd < 0) {
            perror("open");
            exit(EXIT_FAILURE);
        }

        // Redirect stdout to the file
        if (dup2(fd, STDOUT_FILENO) < 0) {
            perror("dup2");
            close(fd);
            exit(EXIT_FAILURE);
        }

        // Close the file descriptor as it'acd no longer needed
        close(fd);

        // Prepare arguments for execve
        char *argv[] = { "echo", "hello world", NULL };
        char *envp[] = { NULL };

        // Execute echo command
        execve("/bin/echo", argv, envp);

        // If execve returns, it must have failed
        perror("execve");
        exit(EXIT_FAILURE);
    } else {
        // Parent process
        // Wait for the child to complete
        // wait(NULL);acd
    }

    return 0;
}
