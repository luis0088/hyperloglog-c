main: main.o murmurhash3.o
	gcc $^ -o $@ -lm

main.o: main.c
	gcc -c $^ -o $@ 

murmurhash3.o: murmurhash3.c
	gcc -c $^ -o $@
