#include "lab3.h"
#include <getopt.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <math.h>
#include <limits.h>

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

typedef unsigned long long int address;

/* Cache line struct 
contains 64 bit memory address, time access tracker, valid bit */
typedef struct{
    address tag; // 64 bit memory address
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


    cacheParam.S = (int) pow(2, cacheParam.s);
    cacheParam.B = (int) pow(2, cacheParam.b);

    // Dynamically allocate memory for cache
    Cache cache;
    cache.sets = malloc(cacheParam.S * sizeof(CacheSet)); // allocate memory for sets

    for(int i = 0; i < cacheParam.S; i++){
        // allocate size of line * number of lines (associativity) per set
        cache.sets[i].lines = malloc(cacheParam.E * sizeof(CacheLine)); 
    }

    // Hit, Miss, Eviction statistics
    int hitCount = 0;
    int missCount = 0;
    int evictionCount = 0;

    // Variables in " [Operation] address" 
    char operation; // Load, Store, Modify
    unsigned int addy;
    int size;

    // for verbose flag information
    int hit = 0;
    int evict = 0;

    // variable to track which index to evict
    int evictee = 0;

    // counter to help with LRU eviction
    int LRU = 0;


    while(fscanf(fp, " %c %11x,%d", &operation, &addy, &size) > 0){
        if(operation != 'I'){
            printf("Address: %x    Operation: %c\n", addy, operation);

            address addyTag = addy >> (cacheParam.b + cacheParam.s); // find tag bits
            int tagSize = (64 - (cacheParam.b + cacheParam.s)); // tag size is 64 bit memory - s - b
            unsigned long long temp = addy << (tagSize);
            unsigned long long setIndex = temp >> (tagSize + cacheParam.b); 

            CacheSet set = cache.sets[setIndex]; // go to proper set to search in
             int last = INT_MAX;

            /*
                for loop int i to run thru E
                    if lines[i] is valid then check to see if the tag matches the address
                        if it does, increment hit_counter, HIT = 1, set the time to LRU
                        and increment LRU

                        if it doesnt, find the oldest guy for eviction */

            for(int i = 0; i < cacheParam.E; i++){
                if(set.lines[i].valid == 1){
                    if(set.lines[i].tag == addy){
                        hitCount++;
                        hit = 1;
                        set.lines[i].lru = LRU;
                        LRU++;
                    }

                    else if(set.lines[i].lru < last){ // this else if is for finding the LRU to evict
                        last = set.lines.[i].lru;
                        evictee = i;
                    }
                }
            }
            
            /*

                    if not valid, check if its empty, if yes then set empty = i
                        this is because if it were valid we would be taking it

                if HIT wasnt set to 1, we got a miss
                    increment miss counter
                    check if there is an empty line
                        set valid, tag and LRU if there is and increment LRU
                    
                    if there's not an empty line
                        use LRU to find an eviction (we already did)
                        set time, and tag
                        incement evictions, timestamp

                data modify implies load and store, so we are gonna have a hit no mater what

                do verbose flag stuff

                reset empty, hit, evict




            */
        }
    }




    if(verboseFlag == 1){
        // print stuff do something
        // stub for compilation success
    }

    printSummary(hitCount, missCount, evictionCount);
    return 0;
}


