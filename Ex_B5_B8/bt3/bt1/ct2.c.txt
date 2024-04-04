#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

#define MAX_STUDENTS 100

typedef struct {
    int student_id;
    char name[50];
    char class[20];
} Student;

int main() {
    
    int select = 0;
    int num_students = 0;
    Student *students = NULL;
    int shared_mem_fd;
    void *shared_memory;
    struct stat stat_buf;
    
    while(select != 3) {
    	printf("Chọn\n");
    	printf("1. Nhấn Ctrl + V để đọc dữ liệu từ vùng nhớ chia sẻ từ ctr 1\n");
    	printf("2. Hiển thị danh sách sinh viên\n");
    	printf("3. Kết thúc\n");
    	scanf("%d", &select);
    	switch (select) {
    	   case 1:
    	      printf("Nhấn Ctrl + V để tiếp tục \n"); 
    	      int ch;
              while ((ch = getchar()) != EOF && ch != 0x16) {
                    // Đợi người dùng nhấn Ctrl + V
              }
    	      shared_mem_fd = shm_open("/shared_memory", O_CREAT | O_RDWR, 0666);
    	      if (shared_mem_fd == -1) {
                    perror("shm_open");
                    exit(EXIT_FAILURE);
                }
    	      fstat(shared_mem_fd, &stat_buf);
    	      if (fstat(shared_mem_fd, &stat_buf) == -1) {
                    perror("fstat");
                    exit(EXIT_FAILURE);
                }
              shared_memory = mmap(NULL, stat_buf.st_size, PROT_READ, MAP_SHARED, shared_mem_fd, 0);
              if (shared_memory == MAP_FAILED) {
            	  perror("mmap");
         	  exit(EXIT_FAILURE);
              }
              num_students = stat_buf.st_size / sizeof(Student);
              
   	      students = (Student *)shared_memory;
    	      break; 
    	   case 2:
    	      printf("Số sinh viên: %d\n", num_students);
    	      printf("Danh sách sinh viên:\n");
   	      for (int i = 0; i < num_students; ++i) {
        	printf("Mã sinh viên: %d\n", students[i].student_id);
     	      	printf("Tên sinh viên: %s\n", students[i].name);
              	printf("Lớp: %s\n", students[i].class);
      	      	printf("\n");
    	      }
    	      break;     
    	}
    }
    munmap(shared_memory, stat_buf.st_size);
    close(shared_mem_fd);
    return 0;
}