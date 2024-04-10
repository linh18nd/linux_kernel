#include <stdio.h>
#include <signal.h>
#include <unistd.h>

// Hàm xử lý signal SIGUSR1
void signal_handler(int signum) {
    if (signum == SIGUSR1) {
        printf("Received SIGUSR1 signal. Waking up...\n");
    }
}

int main() {
    // Đăng ký hàm xử lý signal
    signal(SIGUSR1, signal_handler);
    printf("Sleeping indefinitely. Send SIGUSR1 signal to wake up.\n");
    pid_t pid = getpid();
    printf("PID of this program: %d\n", pid);


    sleep(3600);
    while(1) {
        sleep(3600);
    }

    return 0;
}

