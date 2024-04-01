#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

// Khai báo semaphore và biến global
sem_t semaphore_p1, semaphore_p2, semaphore_p3;

// Hàm in và release semaphore
void print_and_release(sem_t *sem, char *str) {
    printf("%s ", str);
    sem_post(sem);
}

// Tiến trình P1
void *p1(void *arg) {
    sem_wait(&semaphore_p1);
    print_and_release(&semaphore_p2, "R");
    sem_wait(&semaphore_p1);
    print_and_release(&semaphore_p1, "OK");
    pthread_exit(NULL);
}

// Tiến trình P2
void *p2(void *arg) {
    sem_wait(&semaphore_p2);
    print_and_release(&semaphore_p3, "I");
    sem_wait(&semaphore_p2);
    print_and_release(&semaphore_p2, "OK");
    pthread_exit(NULL);
}

// Tiến trình P3
void *p3(void *arg) {
    sem_wait(&semaphore_p3);
    print_and_release(&semaphore_p3, "O");
    sem_post(&semaphore_p1);
    sem_post(&semaphore_p1);
    print_and_release(&semaphore_p1, "OK");
    pthread_exit(NULL);
}

int main() {
    pthread_t tid1, tid2, tid3;

    // Khởi tạo semaphore
    sem_init(&semaphore_p1, 0, 1);
    sem_init(&semaphore_p2, 0, 0);
    sem_init(&semaphore_p3, 0, 0);

    // Tạo các tiến trình
    pthread_create(&tid1, NULL, p1, NULL);
    pthread_create(&tid2, NULL, p2, NULL);
    pthread_create(&tid3, NULL, p3, NULL);

    // Chờ tiến trình hoàn thành
    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);
    pthread_join(tid3, NULL);

    // Hủy semaphore
    sem_destroy(&semaphore_p1);
    sem_destroy(&semaphore_p2);
    sem_destroy(&semaphore_p3);

    printf("\nFinished.\n");

    return 0;
}

