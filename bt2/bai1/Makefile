all: my_ls

my_ls.o: my_ls.c
	gcc -c my_ls.c

my_ls: my_ls.o
	gcc -o my_ls my_ls.o

clean:
	rm -f my_ls.o my_ls

