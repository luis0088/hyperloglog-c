main: main.o murmurhash3.o hyperloglogC.o
	gcc -o $@ $^  -lm

main.o: main.c
	gcc -c $^ -o $@ 

murmurhash3.o: murmurhash3.c
	gcc -c $^ -o $@

hyperloglogC.o: hyperloglogC.c
	gcc -c $^ -o $@

