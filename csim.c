#include "lab3.h"
#include <getopt.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <stdint.h>

//Name: Alan Chen
//UserID: azc242

/* Cache parameters struct*/
typedef struct{
    int s; // 2^s cache sets
    int S; // number of sets
    int E; // cache lines in a set
    int b; // parameter used for getting B
    int B; // B = 2^b (bytes per cache block)
    char *traceFile; // valgrind trace file
} CacheParameter;

/* Cache line struct 
contains 64 bit memory address, time access tracker, valid bit */
typedef struct{
    int64_t tag; // 64 bit memory address
    int lru;
    int valid;
} CacheLine;

/* Cache set struct */
typedef struct{
    CacheLine *lines;
} CacheSet;

/* Cache struct */
typedef struct{
    CacheSet *sets;
} Cache;

/* Usage Info */
void displayUsage()
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
    CacheParameter cacheParam; 
    int helpFlag = 0;
    int verboseFlag = 0;
    int sFlag = 0;
    int EFlag = 0;
    int bFlag = 0;
    int tFlag = 0;

    /* read arvg with getopt()
    optarg does not need to be defined, it is a pointer to the original argv array
    */
    while((c = getopt(argc, argv, "hv::s:E:b:t:")) != -1){
        switch(c){
            case 'h':
                // Optional flag for displaying usage information
                helpFlag = 1;
                break;

            case 'v':
                // Optional flag for displaying trace information
                verboseFlag = 1;
                break;

            case 's':
                // 2^s cache sets 
                cacheParam.s = (atoi(optarg));
                sFlag = 1;
                break;

            case 'E':
                // Associativity (lines per set)
                cacheParam.E = (atoi(optarg));
                EFlag = 1;
                break;

            case 'b':
                // Block bits (2^b block size in total)
                cacheParam.b = (atoi(optarg));
                bFlag = 1;
                break;

            case 't':
                // Valgrind trace
                cacheParam.traceFile = optarg;
                tFlag = 1;
                break;
            
            default:
                displayUsage();
                break;
        }
    }

    /* Setting the -h flag causes the program to print the usage information
    It also forces the program to terminate regardless of any other flags set
    */
    if(helpFlag == 1){
        displayUsage();
        exit(EXIT_SUCCESS); // successful termination
    }

    if(sFlag == 0 || EFlag == 0 || bFlag == 0 || tFlag == 0){
        printf("./csim: Missing required command line argument\n");
        displayUsage();
        exit(EXIT_FAILURE); // unsuccessful termination
    }

    FILE *fp; // FILE pointer to open
    fp = fopen(cacheParam.traceFile, "r"); // read valgrind trace file

    if(fp == NULL){
        printf("%s: No such file or directory boiii", cacheParam.traceFile);
        exit(EXIT_FAILURE); // unsuccessful termination
    }


    cacheParam.S = (int) pow(2, param.s);
    cacheParam.B = (int) pow(2, param.b);

    // Dynamically allocate memory for cache
    Cache cache;
    cache.sets = malloc(cacheParam.S * sizeof(CacheSet)); // allocate memory for sets

    for(int i = 0; i < cacheParam.S; i++){
        // allocate size of line * number of lines (associativity) per set
        cache.sets[i].lines = malloc(cacheParam.E * sizeof(CacheLine)); 
    }

    // Hit, Miss, Eviction statistics
    int hit_count = 0;
    int miss_count = 0;
    int eviction_count = 0;

    // Variables in " [Operation] address,size" 
    char operation; // Load, Store, Modify
    int size; //
    int64_t = addy;








    if(verboseFlag == 1){
        // print stuff do something
        // stub for compilation success
    }

    printSummary(hit_count, miss_count, eviction_count);
    return 0;
}


