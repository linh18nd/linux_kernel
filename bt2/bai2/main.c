#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>

int main() {
    // Mở một file trước khi fork
    int fd = open("output.txt", O_RDWR | O_CREAT | O_TRUNC, 0666);

    // Kiểm tra lỗi khi mở file
    if (fd == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    // Fork để tạo ra process con
    pid_t pid = fork();

    // Kiểm tra lỗi fork
    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    // Process cha
    if (pid > 0) {
        // In ra PID của cha
        printf("PID của cha: %d\n", getpid());
        // In ra thông báo
        printf("I am parent\n");
        // Ghi thông tin vào file
        dprintf(fd, "PID của cha: %d\nI am parent\n", getpid());
        // Chờ process con kết thúc
        wait(NULL);
    }
    // Process con
    else {
        // In ra PID của con
        printf("PID của con: %d\n", getpid());
        // In ra thông báo
        printf("I am child\n");
        // Ghi thông tin vào file
        dprintf(fd, "PID của con: %d\nI am child\n", getpid());
    }

    // Đóng file
    close(fd);

    // Kiểm tra PID sử dụng lệnh PS
    printf("Sử dụng lệnh PS để kiểm tra PID\n");

    // Dùng biến thông thường để kiểm tra
    int variable = 42;
    printf("Biến thông thường: %d\n", variable);

    // Sử dụng lệnh kill để giết một trong 2 process
    if (pid > 0) {
        printf("Process cha sẽ giết process con\n");
        kill(pid, SIGKILL);
        wait(NULL); // Chờ process con kết thúc
    } else {
        printf("Process con đã bị giết\n");
    }

    // Kiểm tra xem process còn lại có bị giết không
    if (pid > 0) {
        printf("Process cha đã bị giết\n");
    } else {
        printf("Process con đã bị giết\n");
    }

    return 0;
}

