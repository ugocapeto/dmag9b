#include "header.h"

void gradient_y(
 int width,
 int height,
 double *image,
 double *gradient
)

{

 int i;
 int j;
 double I;
 double I_bottom;
 int ind;
 int ind_top;
 int ind_bottom;
 int i_top;
 int i_bottom;

 for ( j= 0 ; j< width ; j++ ) {
    for ( i= 0 ; i< height-1 ; i++ ) {
       ind= i*width+j;
       I= image[ind];
       i_bottom= i+1;
       ind_bottom= i_bottom*width+j;
       I_bottom= image[ind_bottom];
       gradient[ind]= I_bottom-I;
    }
    i= height-1;
    ind= i*width+j;
    i_top= i-1;
    ind_top= i_top*width+j;
    gradient[ind]= gradient[ind_top];
 }

}
