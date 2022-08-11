#include "header.h"

void compute_depth_confidence(
 int *image_arr,
 int *depthmap_arr,
 int width,
 int height,
 int radius,
 double gamma_p,
 double gamma_c,
 double sigma,
 double *conf_arr
)

{

 double dist_dbl;
 int i;
 int j;
 int dist_c_siz;
 double *dist_c;
 int dist_c_ind;
 int dist_p_siz;
 double *dist_p;
 int dist_p_ind;
 int pixel;
 int depth;
 double weight_sum;
 int i_radius;
 int j_radius;
 int i2;
 int j2;
 int pixel2;
 int depth2;
 double weight_p;
 double weight_c;
 int cind;
 double weight;
 int nbr_pixels;
 double *E_depth_arr;
 double *E_depth_squared_arr;
 double E_depth;
 double E_depth_squared;
 double V_depth;
 double *V_depth_arr;
 double conf;
 double val;

 /*
 Tabulate proximity strengths dist_p
 (array of size (2*radius+1)^2)
 */

 dist_p_siz= (2*radius+1)*(2*radius+1);
 dist_p= (double *)calloc(dist_p_siz,sizeof(double));

 dist_p_ind= 0;
 for ( i= -radius ; i<= +radius ; i++ ) {
    for ( j= -radius ; j<= +radius ; j++ ) {
       dist_dbl= (double)(i*i+j*j);
       dist_dbl= sqrt(dist_dbl);
       dist_p[dist_p_ind]= exp(-2.*dist_dbl/gamma_p);

       if ( i == 0 ) {
          fprintf(stdout,"proximity weight (i=0) = %g\n",
           dist_p[dist_p_ind]);
       }

       dist_p_ind++;
    }
 }

 /*
 Tabulate color similarity strengths dist_c
 (array of size 3*255+1)
 */

 dist_c_siz= 3*255+1;
 dist_c= (double *)calloc(dist_c_siz,sizeof(double));

 for ( dist_c_ind= 0 ;
       dist_c_ind< dist_c_siz ;
       dist_c_ind++ ) {
    dist_dbl= (double)dist_c_ind;
    dist_dbl/= 3.;
    dist_c[dist_c_ind]= exp(-dist_dbl/gamma_c);
 }

 /*
 Compute the expectation for the depth
 by applying the convolution kernel to the depth
 Compute the expectation for the depth squared
 by applying the convolution kernel to the depth squared
 Compute the variance for the depth
 */

 nbr_pixels= width*height;
 E_depth_arr= (double *)calloc(nbr_pixels,sizeof(double));
 E_depth_squared_arr= (double *)calloc(nbr_pixels,sizeof(double));
 V_depth_arr= (double *)calloc(nbr_pixels,sizeof(double));

 for ( i= 0 ; i< height ; i++ ) {
    for ( j= 0 ; j< width ; j++ ) {
       pixel= i*width+j;
       depth= depthmap_arr[pixel];

       /*
       Apply edge-preserving filter
       */

       weight_sum= 0;
       E_depth= 0;
       E_depth_squared= 0;

       for ( i_radius= -radius ; i_radius<= +radius ; i_radius++ ) {
          for ( j_radius= -radius ; j_radius<= +radius ; j_radius++ ) {
             i2= i+i_radius;
             j2= j+j_radius;
             pixel2= i2*width+j2;

             /*
             Check if neighboring pixel is out of bounds
             */

             if ( i2 < 0 )
              continue;
             if ( i2 > height-1 )
              continue;
             if ( j2 < 0 )
              continue;
             if ( j2 > width-1 )
              continue;

             /*
             Neighboring pixel is in bounds
             */

             depth2= depthmap_arr[pixel2];

             /*
             Get proximity weight 
             */

             dist_p_ind= (radius+i_radius)*(2*radius+1)+radius+j_radius;
             weight_p= dist_p[dist_p_ind];

             /*
             Get color similarity weight
             */

             dist_c_ind= 0;
             for ( cind= 0 ; cind< 3 ; cind++ ) {
                dist_c_ind+= abs(image_arr[3*pixel2+cind]-image_arr[3*pixel+cind]);
             }
             weight_c= dist_c[dist_c_ind];

             /*
             Compute total weight
             */

             weight= weight_p * weight_c;

             /*
             Add weighted contribution of neighboring pixel
             */

             E_depth+= weight*(double)depth2;
             E_depth_squared+= weight*(double)depth2*(double)depth2;
             weight_sum+= weight;
          }
       }

       E_depth/= weight_sum;
       E_depth_squared/= weight_sum;

       /*
       Compute the depth variance
       */

       V_depth= E_depth_squared-E_depth*E_depth;
       V_depth_arr[pixel]= V_depth;
    }
 }

 /*
 Compute the confidence
 */

 for ( i= 0 ; i< height ; i++ ) {
    for ( j= 0 ; j< width ; j++ ) {
       pixel= i*width+j;
       V_depth= V_depth_arr[pixel];
       val= -V_depth/(2.*sigma*sigma);
       conf= exp(val);
       conf_arr[pixel]= conf;
    }
 }

 /*
 Free dist_p
 */

 free(dist_p);

 /*
 Free dist_c
 */

 free(dist_c);

 /*
 Free E_depth_arr
 */

 free(E_depth_arr);

 /*
 Free E_depth_squared_arr
 */

 free(E_depth_squared_arr);

 /*
 Free V_depth_arr
 */

 free(V_depth_arr);

}
