#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>
#include <fcntl.h>
#include <semaphore.h>

#define MAX_STUDENTS 100

typedef struct {
    int student_id;
    char name[50];
    char class[20];
} Student;

sem_t *semaphore;

volatile sig_atomic_t shared_data_ready = 0;

void sigint_handler(int signum) {
    if (signum == SIGINT) {
        shared_data_ready = 1;
    }
}


int main() {
    signal(SIGINT, sigint_handler);

    Student students[MAX_STUDENTS];
    int num_students = 0;
    
    int select = 0;
    
    semaphore = sem_open("my_semaphore", O_CREAT, 0644, 1);
    if (semaphore == SEM_FAILED) {
        perror("sem_open");
        exit(EXIT_FAILURE);
    }
    
    while(select != 3) {
    	printf("Chọn\n");
    	printf("1. Nhập vào danh sách sinh viên (mã sv, họ tên, lớp)\n");
    	printf("2. Nhấn Ctrl + C để copy danh sách sinh viên vào vùng nhớ chia sẻ\n");
    	printf("3. Kết thúc\n");
    	scanf("%d", &select);
    	switch (select) {
    	   case 1:
    	      int tiep = -1;
    	      do {
    	        printf("Nhập mã sinh viên: ");
       	      	scanf("%d", &students[num_students].student_id);
             	printf("Nhập tên sinh viên: ");
              	getchar(); // Clear input buffer
            	fgets(students[num_students].name, sizeof(students[num_students].name), stdin);
              	students[num_students].name[strcspn(students[num_students].name, "\n")] = '\0'; // Remove newline character
              	printf("Nhập lớp: ");
              	fgets(students[num_students].class, sizeof(students[num_students].class), stdin);
              	students[num_students].class[strcspn(students[num_students].class, "\n")] = '\0'; // Remove newline character
              	num_students++;
              	printf("Kết thúc thì nhập 0 \n");
              	scanf("%d", &tiep);
    	      }while(tiep != 0);
    	      break;
    	   case 2:
    	      printf("Nhấn Ctrl + C để tiếp tục \n");
    	      while(shared_data_ready != 1) {
    	      	
    	      }   
    	      int shared_mem_fd = shm_open("/shared_memory", O_CREAT | O_RDWR, 0666);
    	      ftruncate(shared_mem_fd, sizeof(Student) * num_students);
    	      void *shared_memory = mmap(NULL, sizeof(Student) * num_students, PROT_READ | PROT_WRITE, MAP_SHARED, shared_mem_fd, 0);
    	      if (shared_memory == MAP_FAILED) {
            	  perror("mmap");
         	  exit(EXIT_FAILURE);
              }

    		// Copy data to shared memory
    	      sem_wait(semaphore);
    	      memcpy(shared_memory, students, sizeof(Student) * num_students);
    	      sem_post(semaphore);
    	      munmap(shared_memory, sizeof(Student) * num_students);
    	      close(shared_mem_fd);
    	      shared_data_ready = 0;
    	      break;   
    	}
    }
    sem_close(semaphore);
    return 0;
}