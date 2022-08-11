#include "header.h"

void normalize_image(
 double *inp_arr,
 int *out_arr,
 int size
)

/*
out_I must be allocated outside this function
*/

{

 double min_val;
 double max_val;
 int ind;
 double val;
 double diff;
 int val_int;

 /*
 Compute the min and max values for the input array
 */

 min_val= 1.0e32;
 max_val=-1.0e32;

 for ( ind= 0 ; ind< size ; ind++ ) {
    val= inp_arr[ind];
    if ( val < min_val )
     min_val= val;
    if ( val > max_val )
     max_val= val;
 }

 diff= max_val-min_val;

 /*
 Normalize the image values between 0 and 255
 */

 for ( ind= 0 ; ind< size ; ind++ ) {
    val= inp_arr[ind];
    val= 255.0*(val-min_val)/diff;
    val_int= (int)(val+0.5);
    if ( val_int < 0 )
     val_int= 0;
    if ( val_int > 255 )
     val_int= 255;
    out_arr[ind]= val_int;
 }

}
