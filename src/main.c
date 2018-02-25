#include <stdio.h>
#include <stdint.h>

#include "hyperloglogC.h"

int main(int argc, char **argv){
	HyperloglogC *hll = new_hll(5,sizeof(int));
	for(int i=0;i<10000;i++){
		add_hll(hll,&i);
	}

	printf("%d\n",size_hll(hll));
	
	free_hll(hll);
	return 0;
}

