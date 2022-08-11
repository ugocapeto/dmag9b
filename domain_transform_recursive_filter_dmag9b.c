#include "header.h"

void domain_transform_recursive_filter_dmag9b(
 int width,
 int height,
 double *inp_arr,
 double sigma_s,
 double sigma_r,
 int num_iterations,
 int *joint_image_arr,
 double *out_arr
)

/*
See eps7/domain_transform_recursive_filter.c
*/

/*
Here,
the input is a one-dimensional signal (inp_arr)
the output is the filtered one-dimensional signal (out_arr)
the joint image is assumed to be an rgb image
*/

{

 double *F[3];
 int rgb_ind;
 double *joint_image_rgb_arr[3];
 int i;
 int j;
 int pixel;
 double *dIcdx[3];
 double *dIcdy[3];
 double *dIdx;
 double *dIdy;
 int N;
 double sigma_H;
 int iter;
 double sigma_H_iter;
 double *dHdx;
 double *dVdy;

 /*
 Separate the color channels of the joint image
 */

 for ( rgb_ind= 0 ; rgb_ind< 3 ; rgb_ind++ ) {

    myCalloc(joint_image_rgb_arr[rgb_ind],double,width*height,sizeof(double));

    for ( i= 0 ; i< height ; i++ ) {
       for ( j= 0 ; j< width ; j++ ) {
          pixel= i*width+j;
          joint_image_rgb_arr[rgb_ind][pixel]= (double)joint_image_arr[3*pixel+rgb_ind];
       }
    }
 }

 /*
 Get the horizontal derivatives of the joint image for each color channel
 */

 for ( rgb_ind= 0 ; rgb_ind< 3 ; rgb_ind++ ) {

    myCalloc(dIcdx[rgb_ind],double,width*height,sizeof(double));

    gradient_x(
     width,
     height,
     joint_image_rgb_arr[rgb_ind],
     dIcdx[rgb_ind]
    );
 }

 /*
 Get the vertical derivatives of the joint image for each color channel
 */

 for ( rgb_ind= 0 ; rgb_ind< 3 ; rgb_ind++ ) {

    myCalloc(dIcdy[rgb_ind],double,width*height,sizeof(double));

    gradient_y(
     width,
     height,
     joint_image_rgb_arr[rgb_ind],
     dIcdy[rgb_ind]
    );
 }

 /*
 Compute L1-norm distance of neighboring pixels
 */

 myCalloc(dIdx,double,width*height,sizeof(double));

 myCalloc(dIdy,double,width*height,sizeof(double));

 for ( rgb_ind= 0 ; rgb_ind< 3 ; rgb_ind++ ) {
    for ( i= 0 ; i< height ; i++ ) {
       for ( j= 0 ; j< width ; j++ ) {
          pixel= i*width+j;
          dIdx[pixel]= dIdx[pixel]+fabs(dIcdx[rgb_ind][pixel]);
          dIdy[pixel]= dIdy[pixel]+fabs(dIcdy[rgb_ind][pixel]);
       }
    }
 }

 /*
 Compute the derivatives of the horizontal and vertical domain transforms
 */

 myCalloc(dHdx,double,width*height,sizeof(double));

 myCalloc(dVdy,double,width*height,sizeof(double));

 for ( i= 0 ; i< height ; i++ ) {
    for ( j= 0 ; j< width ; j++ ) {
       pixel= i*width+j;
       dHdx[pixel]= 1+(sigma_s/sigma_r)*dIdx[pixel];
       dVdy[pixel]= 1+(sigma_s/sigma_r)*dIdy[pixel];
    }
 }

 /*
 Perform the filtering
 */

 N= num_iterations;

 for ( rgb_ind= 0 ; rgb_ind< 3 ; rgb_ind++ ) {

    myCalloc(F[rgb_ind],double,width*height,sizeof(double));

    for ( i= 0 ; i< height ; i++ ) {
       for ( j= 0 ; j< width ; j++ ) {
          pixel= i*width+j;
          F[rgb_ind][pixel]= inp_arr[pixel];
       }
    }
 }

 sigma_H= sigma_s; 

 for ( iter= 0 ; iter< num_iterations ; iter++ ) {

    /*
    Compute the sigma value for this iteration
    */

    sigma_H_iter= sigma_H*sqrt(3)*pow(2,N-(iter+1))/sqrt(pow(4,N)-1);

    transformed_domain_recursive_filter_horizontal(
     width,
     height,
     F,
     dHdx,
     sigma_H_iter
    );

    transformed_domain_recursive_filter_vertical(
     width,
     height,
     F,
     dVdy,
     sigma_H_iter
    );
 }

 for ( rgb_ind= 0 ; rgb_ind< 3 ; rgb_ind++ ) {

    for ( i= 0 ; i< height ; i++ ) {
       for ( j= 0 ; j< width ; j++ ) {
          pixel= i*width+j;

          out_arr[pixel]= F[rgb_ind][pixel];
       }
    }
 }

 /*
 Free whatever was allocated
 */

 for ( rgb_ind= 0 ; rgb_ind< 3 ; rgb_ind++ ) {
    myFree(joint_image_rgb_arr[rgb_ind]);
 }

 for ( rgb_ind= 0 ; rgb_ind< 3 ; rgb_ind++ ) {
    myFree(dIcdx[rgb_ind]);
 }

 for ( rgb_ind= 0 ; rgb_ind< 3 ; rgb_ind++ ) {
    myFree(dIcdy[rgb_ind]);
 }

 myFree(dIdx);

 myFree(dIdy);

 myFree(dHdx);

 myFree(dVdy);

 for ( rgb_ind= 0 ; rgb_ind< 3 ; rgb_ind++ ) {
    myFree(F[rgb_ind]);
 }

}
