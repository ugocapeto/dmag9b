/*
Mathematical functions
*/

#include <math.h>

#define FALSE 0
#define TRUE  1

/*
absolute value
*/
#ifndef ABS
#define ABS(x) \
( \
 ( (x) >= 0.0 ? (x) : -(x) ) \
)
#endif

/*
cross product
*/

#define cross(x,y,z) { \
 (z)[0] = ((x)[1])*((y)[2]) - ((x)[2])*((y)[1]); \
 (z)[1] = ((x)[2])*((y)[0]) - ((x)[0])*((y)[2]); \
 (z)[2] = ((x)[0])*((y)[1]) - ((x)[1])*((y)[0]); \
}

/*
3D dot product
*/

#define dot(x,y) \
 ( ((x)[0])*((y)[0]) + ((x)[1])*((y)[1]) + ((x)[2])*((y)[2]) )

/*
2D dot product
*/

#define dot_2d(x,y) \
 ( ((x)[0])*((y)[0]) + ((x)[1])*((y)[1]) )

/*
3D difference of 2 vectors
*/

#define vector_diff(x,y,z) { \
 (z)[0] = (x)[0] - (y)[0]; \
 (z)[1] = (x)[1] - (y)[1]; \
 (z)[2] = (x)[2] - (y)[2]; \
}

/*
2D difference of 2 vectors
*/

#define vector_diff_2d(x,y,z) { \
 (z)[0] = (x)[0] - (y)[0]; \
 (z)[1] = (x)[1] - (y)[1]; \
}

/*
3D norm of a vector
*/

#define vector_norm(x) \
 ( sqrt(dot(x,x)) )

/*
2D norm of a vector
*/

#define vector_norm_2d(x) \
 ( sqrt(dot_2d(x,x)) )

/*
3D squared norm of a vector
*/

#define vector_squared_norm(x) \
 ( dot(x,x) )
 
/*
2D squared norm of a vector
*/
 
#define vector_squared_norm_2d(x) \
 ( dot_2d(x,x) )

/*
normalize a 3D vector
with norm unknown
*/

#define vector_normalized(x) { \
 double norm; \
 norm = vector_norm(x); \
 if ( norm > 0.0 ) { \
    (x)[0] = (x)[0]/norm; \
    (x)[1] = (x)[1]/norm; \
    (x)[2] = (x)[2]/norm; \
 } \
}

/*
normalize a 2D vector
with norm unknown
*/

#define vector_normalized_2d(x) { \
 double norm; \
 norm = vector_norm_2d(x); \
 if ( norm > 0.0 ) { \
    (x)[0] = (x)[0]/norm; \
    (x)[1] = (x)[1]/norm; \
 } \
}

/*
normalize a 3D vector
with norm known
*/
 
#define vector_normalized_bis(x,norm) { \
 if ( norm > 0.0 ) { \
    (x)[0] = (x)[0]/norm; \
    (x)[1] = (x)[1]/norm; \
    (x)[2] = (x)[2]/norm; \
 } \
}
 
/*
normalize a 2D vector
with norm known 
*/
 
#define vector_normalized_bis_2d(x,norm) { \
 if ( norm > 0.0 ) { \
    (x)[0] = (x)[0]/norm; \
    (x)[1] = (x)[1]/norm; \
 } \
}

/*
compute deter of 3x3 matrix
*/

#define det_3x3(c1,c2,c3) \
 ( c1[0]*c2[1]*c3[2] + c2[0]*c3[1]*c1[2] + c1[1]*c2[2]*c3[0] - \
   c1[2]*c2[1]*c3[0] - c2[2]*c3[1]*c1[0] - c1[1]*c2[0]*c3[2] )

/*
compute deter of 2x2 matrix
*/

#define det_2x2(c1,c2) \
 ( c1[0]*c2[1] - c1[1]*c2[0] )

/*
get infinite norm of a 3D vector
*/

#define inf_norm(x,norm) { \
 int i; \
 for ( i= 0, (norm)= 0.0 ; i< 3 ; i++ ) \
  if ( ABS((x)[i]) > (norm) ) (norm)= ABS((x)[i]); \
}

/*
get infinite norm of a 2D vector
*/

#define inf_norm_2d(x,norm) { \
 int i; \
 for ( i= 0, (norm)= 0.0 ; i< 2 ; i++ ) \
  if ( ABS(x)[i]) > (norm) ) (norm)= ABS((x)[i]); \
}

/*
get max of n numbers
*/

#define myMax(x,n,max) { \
 int i; \
 for ( i= 0, (max)= (x)[0] ; i< (n) ; i++ ) \
  if ( (x)[i] > (max) ) (max)= (x)[i]; \
}

/*
get min of n numbers
*/

#define myMin(x,n,min) { \
 int i; \
 for ( i= 0, (min)= (x)[0] ; i< (n) ; i++ ) \
  if ( (x)[i] < (min) ) (min)= (x)[i]; \
}

/*
Reverse vector
*/

#define vector_reverse(x) { \
 int i; \
 for ( i= 0 ; i< 3 ; i++ ) x[i]= -x[i]; \
}

/*
Reverse vector (2d)
*/

#define vector_reverse_2d(x) { \
 int i; \
 for ( i= 0 ; i< 2 ; i++ ) x[i]= -x[i]; \
}

/*
Nullify vector
*/

#define vector_null(x) { \
 int i; \
 for ( i= 0 ; i< 3 ; i++ ) x[i]= 0.0; \
}

/* 
Nullify vector (2d)
*/ 

#define vector_null_2d(x) { \
 int i; \
 for ( i= 0 ; i< 2 ; i++ ) x[i]= 0.0; \
}

#define are_2_nbrs_equal(real1,real2,tolrnc) \
( \
 ( ABS((real1)-(real2)) > (tolrnc) ? 0 : 1 ) \
)
