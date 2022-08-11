/*
Dynamic array manager
with garbage collection
*/

typedef struct arrayHeader {
 int size;     /* Total size of array */
 int blck;     /* Amount to allocate when need be */
 int maxId;    /* Index of last used slot in array */
 int avlId;    /* First available slot in array */
 void *vecPtr; /* Points to actual array */
} arrayHeader;

#include "allocMacros.h"

/*
Initialize the header without allocating
*/

#define arrayInit(Header,block) { \
 (Header).size= 0; \
 (Header).blck= (block); \
 (Header).maxId= -1; \
 (Header).avlId= 0; \
 (Header).vecPtr= 0; \
}

/*
Given an index id,
return the corresponding structure variable
*/

#define arrayGet(Header,type,id) \
 ((type *)(Header).vecPtr)[(id)]

/*
Put data into array at given id location
Allocate or Reallocate space if necessary
id must be less or equal to header's size
*/

#define arrayPut(Header,id,data,type) { \
 if ( ((id)==(Header).size) ) { \
    int new_size; \
    new_size= (Header).size + (Header).blck; \
    if ((id)==0) { \
       myCalloc((Header).vecPtr,type,new_size,sizeof(type)); \
    } \
    else { \
       myRealloc((Header).vecPtr,type,new_size*sizeof(type)); \
    } \
    (Header).size= new_size; \
 } \
 ((type *)(Header).vecPtr)[(id)]= (data); \
 if ( (id)>(Header).maxId ) { \
    (Header).maxId= (id); \
 } \
}

/*
Put data into next available spot
Allocate or Reallocate space if necessary
member is an int member of the structure type
id indicates where (in array) the data was put
*/

#define arrayPutNext(Header,data,type,member,id) { \
 int new_id; \
 id= (Header).avlId; \
 if ( id==((Header).maxId)+1 ) { \
    ((Header).avlId)++; \
 } \
 else { \
    new_id= -(((type *)(Header).vecPtr)[(id)].member)-1; \
    (Header).avlId= new_id; \
 } \
 arrayPut(Header,id,data,type); \
}

/*
Get current max index
*/

#define arrayMaxId(Header) \
 (Header).maxId

/*
Delete an indexed entry in the array
member is an int member of the structure type
*/

#define arrayDel(Header,id,type,member) { \
 ((type *)(Header).vecPtr)[(id)].member= \
 -((Header).avlId+1); \
 (Header).avlId= (id); \
}

/*
Delete all space occupied by the array
and reinitialize header fields
*/

#define arrayDelAll(Header) { \
 if ( (Header).vecPtr != 0 ) { \
    myFree( (Header).vecPtr ); \
    (Header).vecPtr= 0; \
 } \
 (Header).size= 0; \
 (Header).maxId= -1; \
 (Header).avlId= 0; \
}

/*
Reset the array
without deleting the vector
*/

#define arrayReset(Header) { \
 (Header).maxId = -1; \
 (Header).avlId= 0; \
}
