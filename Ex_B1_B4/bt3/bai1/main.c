#include <stdio.h>
#include <signal.h>
#include <unistd.h>

void sigint_handler(int signum) {
    printf("hello world\n");
}

int main() {
    // Thiết lập xử lý tín hiệu SIGINT
    signal(SIGINT, sigint_handler);

    while (1) {
        sleep(1);  
    }

    return 0;
}

