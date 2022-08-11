#include "header.h"

void gradient_x(
 int width,
 int height,
 double *image,
 double *gradient
)

{

 int i;
 int j;
 double I;
 double I_right;
 int ind;
 int ind_left;
 int ind_right;
 int j_left;
 int j_right;

 for ( i= 0 ; i< height ; i++ ) {
    for ( j= 0 ; j< width-1 ; j++ ) {
       ind= i*width+j;
       I= image[ind];
       j_right= j+1;
       ind_right= i*width+j_right;
       I_right= image[ind_right];
       gradient[ind]= I_right-I;
    }
    j= width-1;
    ind= i*width+j;
    j_left= j-1;
    ind_left= i*width+j_left;
    gradient[ind]= gradient[ind_left];
 }

}
