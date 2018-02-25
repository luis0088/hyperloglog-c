#include <string.h>
#include <zmq.h>
#include <assert.h>
#include <inttypes.h>
#include <unistd.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include "hyperloglogC.h"
#include "k_v_benchmark.h"
static long OBJECT_LIMIT=0;
static FILE *result_file;
static char *result_path= "./Result/hll_Results.txt";
static char filename[50];
int main(int argc, char ** argv){
    int opt;
    
    int b=5;
    while((opt = getopt(argc,argv,"l:b:"))!=-1){
        switch(opt){
            case 'l':
            	OBJECT_LIMIT = strtol(optarg, NULL, 10);
            	break;
            case 'b':
                b = atoi(optarg);
                break;
            default:
                break;
        }
    }
    if(b<4){
    	fprintf(stderr, "The bits value must be greater than 3\n");
    	exit(1);
    }
    if(OBJECT_LIMIT<=0){
    	fprintf(stderr, "The object limit must be greater than zero\n");
    	exit(1);
    }

	HyperloglogC * hll = new_hll(5,sizeof(uint64_t));
    unsigned int epoch =1;
    int number_of_objects=0;
    int cnt =0;

	void *context = zmq_ctx_new ();
    void *subscriber = zmq_socket (context, ZMQ_SUB);
    int rc = zmq_connect (subscriber, "tcp://localhost:5555");
    assert (rc == 0);

    int zeromq_socket_opt_value = 0;
  
    //rc = zmq_setsockopt (subscriber, ZMQ_SUBSCRIBE, filter, strlen (filter));
    rc = zmq_setsockopt (subscriber, ZMQ_SUBSCRIBE, NULL, 0);
    assert (rc == 0);
    bm_op_t rec_op = {BM_WRITE_OP, 0, 0};
    uint64_t *object = malloc(sizeof(uint64_t));
    *object = rec_op.key_hv;


    fprintf(stderr,"LISTENING FOR OBJECTS....\n");

    while(1){

        zmq_recv(subscriber,&rec_op , sizeof(bm_op_t), 0);

        *object = rec_op.key_hv;

        number_of_objects ++;
        add_hll(hll,object);

        if(number_of_objects==OBJECT_LIMIT){
        	time_t now = time(&now);

        	if (now == -1) {

        		fprintf(stderr, "Error getting time\n" );
        		exit(1);
        	}

        	struct tm *ptm = gmtime(&now);

        	if (ptm == NULL) {

        		fprintf(stderr, "Error generating gmtime\n" );
        		exit(1);
        	}    

        	int count = size_hll(hll);
        	result_file = fopen(result_path,"a");
        	char time_str[100];
        	strcpy(time_str,asctime(ptm));
        	time_str[strlen(time_str)-1]='\0';
        	fprintf(result_file,"epoch: %d, time: %s, unique objects: %d\n",epoch, time_str, count);
        	printf("Unique objects: %d\n", count);
        	fclose(result_file);
        	epoch++;
        	number_of_objects = 0;
        }
    	//object = malloc(sizeof(uint64_t)); 


    }
    zmq_close (subscriber);
    zmq_ctx_destroy (context);
    return 0;
}