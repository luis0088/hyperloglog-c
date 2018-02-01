#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <time.h>
#include <math.h>
#include <limits.h>
#include "murmurhash3.h"
#ifndef max
	#define max( a, b ) ( ((a) > (b)) ? (a) : (b) )
#endif
int leading_zeros(uint64_t x);
int bitcount(uint64_t value);
int get_bucket_pos(uint64_t x, int n);

uint64_t createMask(uint64_t n, uint64_t x){
   uint64_t r = 0;
   for (uint64_t i=n; i<=x; i++)
       r |= 1lu << i;

   return r;
}


int main(int argc, char **argv){
	uint64_t arr[2];
	
	
	int n = 16;
	int mBuckets = pow(2,n);
	int buckets[mBuckets];
	for(int i=0;i<mBuckets;i++){
		buckets[i]=INT_MIN;
	}	
	for(int i = 0;i<50;i++){
		qhashmurmur3_128(&i, sizeof(int), arr);
		uint64_t res = arr[1];
	
		uint64_t bitmask = createMask(0lu,((uint64_t) (64-n)));
		uint64_t pos = bitmask & res;
		buckets[get_bucket_pos(res,n)] = max(leading_zeros(pos),buckets[get_bucket_pos(res,n)]);

	}
	double z =0;
	for(int i=0;i<mBuckets;i++){
		z+=pow(2,-buckets[i]) ;
	}
	printf("%f\n",z);
	z= pow(z,-1);
	printf("%f\n",z);
	printf("%f\n",(0.673*mBuckets*mBuckets)*z);
	return 0;
}

int leading_zeros(uint64_t x){
	if(!x) return -1;
	return __builtin_clzl(x);
}
int get_bucket_pos(uint64_t x, int n){
	return x>>(64-n);
}
int bitcount(uint64_t x){
	/*
    const int nbits= sizeof(uint64_t)*8;
	const uint64_t msb = 1UL << (nbits -1);
	int count =0;
	int i =0;
	while(i<nbits && !((x<<i)&msb)){
		i++;
	}
	return i;
	*/
	
	x = x | (x >> 1);
	x = x | (x >> 2);
	x = x | (x >> 4);
	x = x | (x >> 8);
	x = x | (x >>16);
	return __builtin_popcountl(~x);
}