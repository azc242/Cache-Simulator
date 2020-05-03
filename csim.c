#include "lab3.h"

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
} cacheParam;


int main()
{
    printSummary(0, 0, 0);
    return 0;
}
