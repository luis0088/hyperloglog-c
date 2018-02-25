all: main zero
CFLAG=-I./include
zero: obj/zeroMQ_hll.o obj/murmurhash3.o obj/hyperloglogC.o
	gcc $^ -o $@ -lzmq -lm


main: obj/main.o obj/murmurhash3.o obj/hyperloglogC.o
	gcc -o $@ $^  -lm

obj/main.o: src/main.c
	gcc -c $^ $(CFLAG)  -o $@ 
obj/zeroMQ_hll.o: src/zeroMQ_hll.c
	gcc -c $^ $(CFLAG) -o $@ 

obj/murmurhash3.o: src/murmurhash3.c
	gcc -c $^ $(CFLAG) -o $@

obj/hyperloglogC.o: src/hyperloglogC.c
	gcc -c $^ $(CFLAG) -o $@

