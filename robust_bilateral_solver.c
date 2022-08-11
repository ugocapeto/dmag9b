#include "header.h"

void robust_bilateral_solver(
 int width,
 int height,
 int *ref_arr,
 int *dep_arr,
 double *inp_con_arr,
 int *out_arr,
 int sample_rate_spatial,
 int sample_rate_range,
 double lambda,
 int hash_table_size,
 int n,
 double sigma_gm,
 int n_irls
)

{

 mesh_struct *mesh;
 matrix_int_crs_struct *splat_matrix;
 int *blur_matrix[5];
 double *Cn_matrix;
 int *Cs_matrix;
 int nbr_pixels;
 arrayHeader *mesh_verts;
 int maxInd;
 int nbr_verts;
 double *b;
 int vert;
 int ind;
 int pixel;
 int dep;
 double con;
 double *x;
 double num;
 double den;
 int *pixel2vert_arr;
 double d_dbl;
 int d_int;
 int dim;
 double epsilon;
 double *con_arr;
 double *e;
 int iter;
 double e2;
 double sigma_gm2;
 int print_flag= 1;
 int i;
 int j;

 /*
 Get nbr of pixels
 */

 nbr_pixels= width*height;

 /*
 Create the bilateral space verts
 */

 fprintf(stdout,"Creating the bilateral space verts ...\n");

 stereo_create_verts(
  ref_arr,
  width,
  height,
  sample_rate_spatial,
  sample_rate_range,
  hash_table_size,
  &mesh,
  &splat_matrix,
  blur_matrix
 );

 fprintf(stdout,"Creating the bilateral space verts ... done.\n");

 /*
 Get nbr of verts
 */

 mesh_verts= mesh->mesh_verts;
 maxInd= arrayMaxId(*mesh_verts);
 nbr_verts= maxInd+1;

 /*
 Build the Cn and Cs matrix
 */

 fprintf(stdout,"Building the Cn and Cs matrix ...\n");

 stereo_build_Cn_Cs_matrix(
  ref_arr,
  width,
  height,
  mesh,
  splat_matrix,
  blur_matrix,
  &Cn_matrix,
  &Cs_matrix
 );

 fprintf(stdout,"Building the Cn and Cs matrix ... done.\n");

 /*
 Note that in the paper "Fast Bilateral Solver" by Barron et al.,
 Cn is called Dn
 Cs is called Dm
 */

 /*
 Allocate memory for b
 */

 myCalloc(b,double,nbr_verts,sizeof(double));

 /*
 Allocate memory for con_arr
 */

 myCalloc(con_arr,double,nbr_pixels,sizeof(double));

 /*
 Initialize con_arr
 */

 for ( pixel= 0 ; pixel< nbr_pixels ; pixel++ ) {
    con_arr[pixel]= inp_con_arr[pixel];
 }

 /*
 Allocate memory for the solution x
 */

 myCalloc(x,double,nbr_verts,sizeof(double));

 /*
 Allocate memory for e
 */

 myCalloc(e,double,nbr_pixels,sizeof(double));

 /*
 Initialize number of iterations
 */

 iter= 1;

 START:

 if ( print_flag == 1 ) {
    fprintf(stdout,"iter = %d\n",iter);
 }

 /*
 Compute b
 b = S ( c x t) where
 S is the splat matrix
 c is the confidence matrix
 t is the depth map (the input depth map)
 */

 for ( vert= 0 ; vert< nbr_verts ; vert++ ) {

    b[vert]= 0;

    for ( ind= splat_matrix->row_ptr[vert] ;
          ind< splat_matrix->row_ptr[vert+1] ;
          ind++ ) {
       pixel= splat_matrix->col_ind[ind];
       dep= dep_arr[pixel];
       con= con_arr[pixel];
       b[vert]+= con*(double)dep;
    }
 }

 /*
 Initialize x =
 S(c x t)/S(c)
 */

 for ( vert= 0 ; vert< nbr_verts ; vert++ ) {

    num= 0;
    den= 0;

    for ( ind= splat_matrix->row_ptr[vert] ;
          ind< splat_matrix->row_ptr[vert+1] ;
          ind++ ) {
       pixel= splat_matrix->col_ind[ind];
       dep= dep_arr[pixel];
       con= con_arr[pixel];
       num+= con*(double)dep;
       den+= con;
    }

    if ( den == 0 ) {
       error_handler("fast_bilateral_solver");
    }

    x[vert]= num/den;
 }

 /*
 Solve the linear system
 */

 fprintf(stdout,"Solving the linear system ...\n");

 epsilon= 1.0e-16;

 pcg(
  mesh,
  splat_matrix,
  blur_matrix,
  Cn_matrix,
  Cs_matrix,
  lambda,
  dep_arr,
  con_arr,
  b,
  x,
  n,
  epsilon
 );

 /*
 cg(
  mesh,
  splat_matrix,
  blur_matrix,
  Cn_matrix,
  Cs_matrix,
  lambda,
  dep_arr,
  con_arr,
  b,
  x,
  n,
  epsilon
 );
 */

 fprintf(stdout,"Solving the linear system ... done.\n");

 /*
 Transfer the solution
 from bilateral space to pixel space
 */

 fprintf(stdout,"Transfering the disparities to pixel-space ...\n");

 /*
 Go from bilateral space solution x
 to pixel space solution out_arr
 */

 myCalloc(pixel2vert_arr,int,nbr_pixels,sizeof(int));

 for ( vert= 0 ; vert< nbr_verts ; vert++ ) {
    for ( ind= splat_matrix->row_ptr[vert] ;
          ind< splat_matrix->row_ptr[vert+1] ;
          ind++ ) {
       pixel= splat_matrix->col_ind[ind];
       pixel2vert_arr[pixel]= vert;
    }
 }

 for ( i= 0 ; i< height ; i++ ) {
    for ( j= 0 ; j< width ; j++ ) {
       pixel= i*width+j;

       vert= pixel2vert_arr[pixel];

       /*
       Get the disparity for that vert
       Note that it's a double and it could be out of bounds
       */

       d_dbl= x[vert];

       if ( d_dbl >= 0 )
        d_int= (int)(d_dbl+0.5);
       else
        d_int= (int)(d_dbl-0.5);

       /*
       If not in bounds, put in bounds
       */

       if ( d_int < 0 )
        d_int= 0;
       if ( d_int > 255 )
        d_int= 255;

       /*
       Store the pixel disparity
       */

       out_arr[pixel]= d_int;
    }
 }

 fprintf(stdout,"Transfering the disparities to pixel-space ... done.\n");

 /*
 Update e =
 x(pixel space) - t
 */

 for ( pixel= 0 ; pixel< nbr_pixels ; pixel++ ) {
    e[pixel]= out_arr[pixel]-dep_arr[pixel];
 }

 /*
 Update c =
 2 sigma_gm^2/(sigma_gm^2+e^2)^2
 */

 sigma_gm2= sigma_gm*sigma_gm;
 for ( i= 0 ; i< height ; i++ ) {
    for ( j= 0 ; j< width ; j++ ) {
       pixel= i*width+j;
       num= 2*sigma_gm2;
       e2= e[pixel]*e[pixel];
       den= (sigma_gm2+e2)*(sigma_gm2+e2);
       if ( den == 0 ) {
          error_handler("robust_bilateral_solver"); 
       }
       con_arr[pixel]= num/den;
    }
 }

 /*
 Update iter =
 iter + 1
 */

 iter= iter + 1;

 /*
 Free pixel2vert_arr
 */

 myFree(pixel2vert_arr);

 /*
 Check for termination
 */

 if ( iter <= n_irls )
  goto START;

 /*
 Free the mesh
 */

 mesh_free(mesh);

 /*
 Free the splat matrix
 */

 myFree(splat_matrix->val);
 myFree(splat_matrix->col_ind);
 myFree(splat_matrix->row_ptr);
 myFree(splat_matrix);

 /*
 Free the blur matrix
 */

 for ( dim= 0 ; dim< 5 ; dim++ ) {
    myFree(blur_matrix[dim]);
 }

 /*
 Free the Cn matrix
 */

 myFree(Cn_matrix);

 /*
 Free the Cs matrix
 */

 myFree(Cs_matrix);

 /*
 Free b
 */

 myFree(b);

 /*
 Free con_arr
 */

 myFree(con_arr);

 /*
 Free x
 */

 myFree(x);

 /*
 Free e
 */

 myFree(e);

 /*
 Free pixel2vert_arr
 */

 /*
 myFree(pixel2vert_arr);
 */

}
