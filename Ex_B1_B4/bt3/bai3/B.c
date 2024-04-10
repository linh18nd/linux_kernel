#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

// Hàm xử lý tín hiệu SIGUSR1
void signal_handler(int signum) {
    // Không cần làm gì trong trường hợp này
}

int main() {
    // Đăng ký hàm xử lý tín hiệu SIGUSR1
    signal(SIGUSR1, signal_handler);

    // Đợi tín hiệu từ tiến trình cha (A)
    pause();

    // Đọc giá trị counter từ tệp counter.txt
    int counter;
    FILE *file = fopen("counter.txt", "r");
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

    // Gửi tín hiệu SIGUSR1 cho tiến trình cha để đồng bộ
    kill(getppid(), SIGUSR1);

    return 0;
}

