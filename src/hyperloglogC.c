#include <stdlib.h>
#include <math.h>
#include <tgmath.h>
#include <stdint.h>

#include "murmurhash3.h"
#ifndef max
	#define max( a, b ) ( ((a) > (b)) ? (a) : (b) )
#endif
typedef struct _hyperloglog {
	int *registers;
	int nregisters;
	size_t elements_size;
	int b;
	double factor;
} HyperloglogC;

static inline int leftmostbit(uint64_t x){
	return (!x)?1:(__builtin_clzl(x)+1);
}


static inline int registerPos(uint64_t x, int b){
	return (x>>(64-b))+1;
}

void free_hll(HyperloglogC *hll){
	if(hll==NULL){
		return;
	}
	free(hll->registers);
	free(hll);
}

HyperloglogC * new_hll(const int b, const size_t elements_size){
	HyperloglogC *hll = (HyperloglogC *) malloc(sizeof(HyperloglogC));
	hll->elements_size = elements_size;
	hll->b = b;
	hll->nregisters= pow(2,b);
	hll->registers = calloc(hll->nregisters,sizeof(int));
	switch(b){
		case 4:
			hll->factor = 0.673;
		break;

		case 5:
			hll->factor = 0.697;
		break;

		case 6:
			hll->factor = 0.709;
		break;

		default:
			hll->factor = 0.7213/(1.0+(1.079/hll->nregisters));
		break;

	}
	return hll;
}
void add_hash_hll(HyperloglogC *hll, const uint64_t hash){
	uint64_t w =hash<<(hll->b);
	int j = registerPos(hash,hll->b);
	hll->registers[j] = max(leftmostbit(w),hll->registers[j]);
}
void add_hll(HyperloglogC *hll, const void *element){
	uint64_t arr[2];
	qhashmurmur3_128(element, hll->elements_size, arr);
	uint64_t hash = arr[0] ^ arr[1];
	add_hash_hll(hll,hash);	
}


int size_hll(const HyperloglogC *hll){

	long double z =0;
	for(int i=1;i<(hll->nregisters);i++){
		z+=powl(2,-(hll->registers[i]));
	}
	z=  powl(z,-1);
	z= (hll->factor*powl(hll->nregisters,2))*z;
	if(z<=((5.0/2.0)*hll->nregisters)){
		int counter =0;
		for(int i=1;i<hll->nregisters;i++){
			if(hll->registers[i]==0)
				counter++;
		}
		if(counter!=0){
			z= hll->nregisters*(log(((double)hll->nregisters)/counter));
		}
	}else if(z>(((1.0/30.0))*powl(2,32))  ){
		long double power2 = powl(2,32);
		z = -powl(2,32)*log(1-(z/(power2)));
	}
	return (int)z;
}

void clear_hll(HyperloglogC *hll){
	for(int i=1;i<(hll->nregisters);i++){
		hll->registers[i]=0;
	}
}

