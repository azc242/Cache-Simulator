#include "lab3.h"
#include <getopt.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

//Name: Alan Chen
//UserID: azc242

// cache parameters
typedef struct{
    int s; // 2^s cache sets
    int S; // number of sets
    int E; // cache lines in a set
    int b; // parameter used for getting B
    int B; // B = 2^b (bytes per cache block)
    char *traceFile; // valgrind trace file
} cacheParameter;


/* Usage Info */
void csim_help_info()
{
  printf("Usage: ./csim [-hv] -s <num> -E <num> -b <num> -t <file>\n");
  printf("Options:\n");
  printf("  -h         Print this help message.\n");
  printf("  -v         Optional verbose flag.\n");
  printf("  -s <num>   Number of set index bits.\n");
  printf("  -E <num>   Number of lines per set.\n");
  printf("  -b <num>   Number of block offset bits.\n");
  printf("  -t <file>  Trace file.\n\n");
  printf("Examples:\n");
  printf("  linux>  ./csim -s 4 -E 1 -b 4 -t traces/yi.trace\n");
  printf("  linux>  ./csim -v -s 8 -E 2 -b 4 -t traces/yi.trace\n");

}

int main(int argc, char** argv){
    int c;
    cacheParameter cacheParam; 
    int helpFlag = 0;
    int verboseFlag = 0;
    int sFlag = 0;
    int EFlag = 0;
    int bFlag = 0;
    int tFlag = 0;

    /* read arvg with getopt()
    optarg does not need to be defined, it is a pointer to the original argv array

    */
    while((c = getopt(argc, argv, "hvsEbt:")) != -1){
        switch(c){
            case 'h':
                helpFlag = 1;
                break;
            case 'v':
                verboseFlag = 1;
                break;
            case 's':
                cacheParam.s = (atoi(optarg));
                sFlag =1;
                break;
            case 'E':
                cacheParam.E = (atoi(optarg));
                EFlag = 1;
                break;
            case 'b':
                cacheParam.E = (atoi(optarg));
                bFlag = 1;
                break;
            case 't':
                cacheParam.E = (atoi(optarg));
                tFlag = 1;
                break;

        }
    }

    printSummary(0, 0, 0);
    return 0;
}


