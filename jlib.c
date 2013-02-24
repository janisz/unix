////////////////////////////////////////////////////////////////////
// jLib
// Set of usefull functions and macros
////////////////////////////////////////////////////////////////////

#define _GNU_SOURCE 
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <time.h>
#include <errno.h>
#include <string.h>

//Print error info and exit
#define ERR(source) (fprintf(stderr,"%s:%d\n",__FILE__,__LINE__),\
                     perror(source),kill(0,SIGKILL),			\
		     		     exit(EXIT_FAILURE))
//Call function and exit on error		     		     
#define CALL_AND_CHECK(f, r, ...)                               \
    do {                                                        \
        if (f(__VA_ARGS__) != r)                                \
        {                                                       \
            ERR(#f "(): ");                                  	\          
        }                                                       \
    } while (0)		     		     
    
//Iterate over table   
#define foreach(item, array) 									\
    for(int keep=1, 											\
            count=0,											\
            size=sizeof (array)/sizeof *(array); 				\
        count != size; 											\
        keep=1, count++) 										\
      for(item = (array)+count; keep; keep = !keep)    
      

#define IMPLIES(x, y) (!(x) || (y))
#define COMPARE(x, y) (((x) > (y)) - ((x) < (y)))
#define SIGN(x) COMPARE(x, 0)
#define ARRAY_SIZE(a) (sizeof(a) / sizeof(*a))
#define SWAP(x, y, T) do { T tmp = (x); (x) = (y); (y) = tmp; } while(0)
#define SORT2(a, b, T) do { if ((a) > (b)) SWAP((a), (b), T); } while (0)
#define SET(d, n, v) do{ size_t i_, n_; for (n_ = (n), i_ = 0; n_ > 0; --n_, ++i_) (d)[i_] = (v); } while(0)
#define ZERO(d, n) SET(d, n, 0)
//Checking whether a floating point x is Not A Number
#define ISNAN(x) ((x) != (x))

      
