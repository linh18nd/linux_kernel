#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

// Biến toàn cục để lưu PID của tiến trình con
pid_t child_pid;

// Hàm xử lý tín hiệu SIGUSR1
void signal_handler(int signum) {
    if (signum == SIGUSR1) {
        printf("Received SIGUSR1 signal from child process. Synchronizing...\n");
    }
}

int main() {
    // Đăng ký hàm xử lý tín hiệu SIGUSR1
    signal(SIGUSR1, signal_handler);

    // Tạo tệp counter.txt nếu chưa tồn tại và ghi giá trị ban đầu
    FILE *file = fopen("counter.txt", "w");
    if (file == NULL) {
        perror("Failed to open file");
        exit(EXIT_FAILURE);
    }
    fprintf(file, "0");
    fclose(file);

    // Fork một tiến trình con
    child_pid = fork();
    if (child_pid < 0) {
        // Fork không thành công
        perror("Fork failed");
        exit(EXIT_FAILURE);
    } else if (child_pid == 0) {
        // Tiến trình con (B)
        execl("./B", "B", NULL);
        perror("execl failed");
        exit(EXIT_FAILURE);
    } else {
        // Tiến trình cha (A)
        // Đợi tín hiệu từ tiến trình con
        pause();
        
        // Đọc giá trị counter từ tệp counter.txt
        int counter;
        file = fopen("counter.txt", "r");
        if (file == NULL) {
            perror("Failed to open file");
            exit(EXIT_FAILURE);
        }
        fscanf(file, "%d", &counter);
        fclose(file);

        // Tăng counter lên 1 và ghi lại vào tệp counter.txt
        counter++;
        file = fopen("counter.txt", "w");
        if (file == NULL) {
            perror("Failed to open file");
            exit(EXIT_FAILURE);
        }
        fprintf(file, "%d\n", counter);
        fclose(file);

        // Gửi tín hiệu SIGUSR1 cho tiến trình con để đồng bộ
        kill(child_pid, SIGUSR1);

        // Chờ tiến trình con kết thúc
        wait(NULL);
    }

    return 0;
}

