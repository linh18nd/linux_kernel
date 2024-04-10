#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <string.h>
#include <signal.h>

void handle_signal(int sig) {
    // Xử lý tín hiệu SIGCHLD
}

int main() {
    // Thiết lập xử lý tín hiệu SIGCHLD
    struct sigaction sa;
    sa.sa_handler = handle_signal;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    sigaction(SIGCHLD, &sa, NULL);

    // Mở một file để ghi trước khi fork
    int fd = open("output.txt", O_CREAT | O_WRONLY | O_TRUNC, 0644);
    if (fd == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    // Fork ra process con
    pid_t pid = fork();

    if (pid < 0) {
        perror("fork");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        // Process con
        printf("Child PID: %d\n", getpid());
        printf("I am child\n");

        // Ghi vào file từ process con
        char *msg = "Message from child\n";
        if (write(fd, msg, strlen(msg)) == -1) {
            perror("write");
            exit(EXIT_FAILURE);
        }
    } else {
        // Process cha
        printf("Parent PID: %d\n", getpid());
        printf("I am parent\n");

        // Ghi vào file từ process cha
        char *msg = "Message from parent\n";
        if (write(fd, msg, strlen(msg)) == -1) {
            perror("write");
            exit(EXIT_FAILURE);
        }

        // Đóng file descriptor
        close(fd);

        // Đợi cho process con kết thúc
        pause();

        // Mở file để đọc và in ra nội dung
        fd = open("output.txt", O_RDONLY);
        if (fd == -1) {
            perror("open");
            exit(EXIT_FAILURE);
        }

        char buffer[100];
        ssize_t bytes_read;
        while ((bytes_read = read(fd, buffer, sizeof(buffer))) > 0) {
            write(STDOUT_FILENO, buffer, bytes_read);
        }

        // Đóng file descriptor
        close(fd);
    }

    return 0;
}

