/*
Macros on top of:
malloc
calloc
free
realloc
for error detection
*/

#include <stdio.h>
#include <stdlib.h>

#define myMalloc(v,type,size) { \
 v= (type *)malloc(size); \
 if ( (v) == 0 ) { \
    printf("malloc error in 'myMalloc'\n"); \
    exit(1); \
 } \
}

#define myCalloc(v,type,nelem,elsize) { \
 v= (type *)calloc(nelem,elsize); \
 if ( (v) == 0 ) { \
    printf("calloc error in 'myCalloc'\n"); \
    exit(1); \
 } \
}

#define myRealloc(v,type,size) { \
 v= (type *)realloc(v,size); \
 if ( (v) == 0 ) { \
    printf("realloc error in 'myRealloc'\n"); \
    exit(1); \
 } \
}

#define myFree(v) { \
 free(v); \
}
