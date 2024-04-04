all: Tinh

Tinh:  hinhVuong.o hinhTron.o hinhTamGiac.o  Tinh.o
	gcc -o  Tinh Tinh.o hinhVuong.o hinhTron.o  hinhTamGiac.o -lm
Tinh.o: Tinh.c
	gcc -c Tinh.c
hinhVuong.o: hinhVuong.c
	gcc -c hinhVuong.c
hinhTron.o: hinhTron.c 
	gcc -c hinhTron.c
hinhTamGiac.o: hinhTamGiac.c
	gcc -c hinhTamGiac.c

clean: 
	rm   *.o Tinh
