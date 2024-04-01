all: app

mylib.a: mylib.o
	ar rcs mylib.a mylib.o
mylib.o: mylib.c
	gcc -c mylib.c
app: main.o mylib.a
	gcc -o app main.o -L. -l:mylib.a
main.o: main.c
	gcc -c main.c

clean:	
	rm -f *.o *.a app


