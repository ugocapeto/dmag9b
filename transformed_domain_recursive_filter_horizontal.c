#include "header.h"

void transformed_domain_recursive_filter_horizontal(
 int width,
 int height,
 double *I[3],
 double *D,
 double sigma
)

{

 double a;
 int rgb_ind;
 double *F[3];
 int i;
 int j;
 int pixel;
 double *V;
 int pixel1;

 /*
 Feedback coefficient
 */

 a= exp(-sqrt(2)/sigma);

 /*
 Copy I into F
 */

 for ( rgb_ind= 0 ; rgb_ind< 3 ; rgb_ind++ ) {

    myCalloc(F[rgb_ind],double,width*height,sizeof(double));

    for ( i= 0 ; i< height ; i++ ) {
       for ( j= 0 ; j< width ; j++ ) {
          pixel= i*width+j;
          F[rgb_ind][pixel]= I[rgb_ind][pixel];
       }
    }
 }

 /*
 Compute V = a^D
 */

 myCalloc(V,double,width*height,sizeof(double));

 for ( i= 0 ; i< height ; i++ ) {
    for ( j= 0 ; j< width ; j++ ) {
       pixel= i*width+j;
       V[pixel]= pow(a,D[pixel]);
    }
 }

 /*
 Left -> right filter
 */

 for ( rgb_ind= 0 ; rgb_ind< 3 ; rgb_ind++ ) {

    for ( i= 0 ; i< height ; i++ ) {
       for ( j= 1 ; j< width ; j++ ) {
          pixel= i*width+j;
          pixel1= i*width+(j-1);

          F[rgb_ind][pixel]= F[rgb_ind][pixel]+
           V[pixel]*(F[rgb_ind][pixel1]-F[rgb_ind][pixel]);
       }
    }
 }

 /*
 Right -> left filter
 */

 for ( rgb_ind= 0 ; rgb_ind< 3 ; rgb_ind++ ) {

    for ( i= 0 ; i< height ; i++ ) {
       for ( j= width-2 ; j>= 0 ; j-- ) {
          pixel= i*width+j;
          pixel1= i*width+(j+1);

          F[rgb_ind][pixel]= F[rgb_ind][pixel]+
           V[pixel1]*(F[rgb_ind][pixel1]-F[rgb_ind][pixel]);
       }
    }
 }

 /*
 Copy F into I
 */

 for ( rgb_ind= 0 ; rgb_ind< 3 ; rgb_ind++ ) {

    for ( i= 0 ; i< height ; i++ ) {
       for ( j= 0 ; j< width ; j++ ) {
          pixel= i*width+j;
          I[rgb_ind][pixel]= F[rgb_ind][pixel];
       }
    }
 }

 /*
 Free what was allocated
 */

 for ( rgb_ind= 0 ; rgb_ind< 3 ; rgb_ind++ ) {
    myFree(F[rgb_ind]);
 }

 myFree(V);

}
