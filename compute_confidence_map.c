#include "header.h"

void compute_confidence_map(
 int width,
 int height,
 int *ref_arr,
 int *dep_arr,
 double sigma_s,
 double sigma_r,
 int num_iterations,
 double *con_arr
)

/*
dep_arr is the depth map
(grayscale image)
*/

{

 int nbr_pixels;
 double *Z_arr;
 double *Z2_arr;
 int i;
 int j;
 int pixel;
 int *joint_image_arr;
 int cind;
 double *DT_Z_arr;
 double *DT_Z2_arr;
 double sigma_dt;
 double *V_arr;
 int *DT_Z_normalized_arr;
 int *DT_Z2_normalized_arr;
 int *V_normalized_arr;
 double deviation;
 double factor;

 nbr_pixels= width*height;

 /*
 Compute local variance
 V =  DT(Z^2) - (DT(Z))^2
 */

 /*
 Allocate memory for Z array
 */

 myCalloc(Z_arr,double,nbr_pixels,sizeof(double));

 /*
 Allocate memory for Z^2 array
 */

 myCalloc(Z2_arr,double,nbr_pixels,sizeof(double));

 /*
 Fill Z_arr and Z2_arr
 */

 /*
 The Z array is the normalized depth (between 0.0 and 1.0)
 */

 for ( i= 0 ; i< height ; i++ ) {
    for ( j= 0 ; j< width ; j++ ) {
       pixel= i*width+j;
       Z_arr[pixel]= (double)dep_arr[pixel]/255;
       Z2_arr[pixel]= Z_arr[pixel]*Z_arr[pixel];
    }
 }

 /*
 Allocate memory for the joint image array
 */

 myCalloc(joint_image_arr,int,3*nbr_pixels,sizeof(int));

 /*
 Fill the joint image array
 */

 /*
 The joint image array is the depth map
 */

 /*
 for ( i= 0 ; i< height ; i++ ) {
    for ( j= 0 ; j< width ; j++ ) {
       pixel= i*width+j;
       for ( cind= 0 ; cind< 3 ; cind++ ) {
          joint_image_arr[3*pixel+cind]= dep_arr[pixel];
       }
    }
 }
 */

 /*
 The joint image array is the reference image
 */

 for ( i= 0 ; i< height ; i++ ) {
    for ( j= 0 ; j< width ; j++ ) {
       pixel= i*width+j;
       for ( cind= 0 ; cind< 3 ; cind++ ) {
          joint_image_arr[3*pixel+cind]= ref_arr[3*pixel+cind];
       }
    }
 }

 /*
 Allocate memory for DT(Z) array
 */

 myCalloc(DT_Z_arr,double,nbr_pixels,sizeof(double));

 /*
 Compute the Domain Transform of Z
 */

 domain_transform_recursive_filter_dmag9b(
  width,
  height,
  Z_arr,
  sigma_s,
  sigma_r,
  num_iterations,
  joint_image_arr,
  DT_Z_arr
 );

 /*
 For debugging purposes,
 write out DT_Z_arr in image form
 */

 /*
 myCalloc(DT_Z_normalized_arr,int,nbr_pixels,sizeof(int));

 normalize_image(
  DT_Z_arr,
  DT_Z_normalized_arr,
  nbr_pixels
 );

 write_image_arr_into_jpeg(
  "DT_Z.jpg",
  width,
  height,
  DT_Z_normalized_arr
 );

 myFree(DT_Z_normalized_arr);
 */

 /*
 Allocate memory for DT(Z^2) array
 */

 myCalloc(DT_Z2_arr,double,nbr_pixels,sizeof(double));

 /*
 Compute the Domain Transform of Z^2
 */

 domain_transform_recursive_filter_dmag9b(
  width,
  height,
  Z2_arr,
  sigma_s,
  sigma_r,
  num_iterations,
  joint_image_arr,
  DT_Z2_arr
 );

 /*
 For debugging purposes,
 write out DT_Z2_arr in image form
 */

 /*
 myCalloc(DT_Z2_normalized_arr,int,nbr_pixels,sizeof(int));

 normalize_image(
  DT_Z2_arr,
  DT_Z2_normalized_arr,
  nbr_pixels
 );

 write_image_arr_into_jpeg(
  "DT_Z2.jpg",
  width,
  height,
  DT_Z2_normalized_arr
 );

 myFree(DT_Z2_normalized_arr);
 */

 /*
 Allocate memory for the variance array
 */

 myCalloc(V_arr,double,nbr_pixels,sizeof(double));

 /*
 Fill the variance array
 */

 for ( i= 0 ; i< height ; i++ ) {
    for ( j= 0 ; j< width ; j++ ) {
       pixel= i*width+j;
       V_arr[pixel]= DT_Z2_arr[pixel] - DT_Z_arr[pixel]*DT_Z_arr[pixel];
       /* 
       if ( V_arr[pixel] < 0.0 ) {
          error_handler("compute_confidence_map");
       }
       */
       /*
       if ( V_arr[pixel] > 1.0 ) {
          error_handler("compute_confidence_map");
       }
       */
    }
 }

 /*
 For debugging purposes,
 write out V_arr in image form
 */

 /*
 myCalloc(V_normalized_arr,int,nbr_pixels,sizeof(int));

 normalize_image(
  V_arr,
  V_normalized_arr,
  nbr_pixels
 );

 write_image_arr_into_jpeg(
  "V.jpg",
  width,
  height,
  V_normalized_arr
 );

 myFree(V_normalized_arr);
 */

 /*
 Compute the confidence
 using Jon Barron's method 
 The larger the variance (the square of the standard deviation),
 the lower the confidence
 */

 sigma_dt= 2;
 for ( i= 0 ; i< height ; i++ ) {
    for ( j= 0 ; j< width ; j++ ) {
       pixel= i*width+j;
       con_arr[pixel]= exp(-V_arr[pixel]/(2*sigma_dt*sigma_dt));
    }
 }

 /*
 Compute the confidence
 using my method
 The larger the deviation from the mean,
 the lower the confidence
 */

 /*
 for ( i= 0 ; i< height ; i++ ) {
    for ( j= 0 ; j< width ; j++ ) {
       pixel= i*width+j;
       deviation= fabs(Z_arr[pixel]-DT_Z_arr[pixel]);
       factor= 10.;
       con_arr[pixel]= exp(-deviation*factor);
    }
 }
 */

 /*
 Free memory for Z array
 */

 myFree(Z_arr);

 /*
 Free memory for Z^2 array
 */

 myFree(Z2_arr);

 /*
 Free memory for the joint image array
 */

 myFree(joint_image_arr);

 /*
 Free memory for DT(Z) array
 */

 myFree(DT_Z_arr);

 /*
 Free memory for DT(Z2) array
 */

 myFree(DT_Z2_arr);

 /*
 free memory for the variance array
 */

 myFree(V_arr);

}
