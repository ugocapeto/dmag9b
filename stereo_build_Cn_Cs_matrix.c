#include "header.h"

void stereo_build_Cn_Cs_matrix(
 int *image_arr,
 int width,
 int height,
 mesh_struct *mesh,
 matrix_int_crs_struct *splat_matrix,
 int *blur_matrix[5],
 double **ptr_Cn_matrix,
 int **ptr_Cs_matrix
)

{

 arrayHeader *mesh_verts;
 int maxInd;
 int nbr_verts;
 int nbr_pixels;
 int *m;
 int vert;
 int mass;
 int pixel;
 double *n;
 int iter;
 double *Cn;
 int *Cs;
 int i;
 double val;
 double new_val;
 double max_norm;
 double tol;
 int ind;
 double *Bn;
 int dim;
 int k;

 /*
 Get nbr of pixels
 */

 nbr_pixels= width*height;

 /*
 Get nbr of verts
 */

 mesh_verts= mesh->mesh_verts;
 maxInd= arrayMaxId(*mesh_verts);
 nbr_verts= maxInd+1;

 /*
 Build the vert mass vector
 */

 myCalloc(m,int,nbr_verts,sizeof(int));

 for ( vert= 0 ; vert< nbr_verts ; vert++ ) {

    /*
    vert represents the row index
    */

    /*
    Compute the mass of the vert
    */

    mass= 0;
    for ( ind= splat_matrix->row_ptr[vert] ;
          ind< splat_matrix->row_ptr[vert+1] ;
          ind++ ) {
       pixel= splat_matrix->col_ind[ind];
       mass++;
    }

    /*
    Print out the mass
    */

    /*
    fprintf(stdout,"vert= %d mass= %d\n",vert,mass);
    */

    /*
    Store the mass
    */

    m[vert]= mass;
 }

 /*
 Allocate memory for the n vector
 */

 myCalloc(n,double,nbr_verts,sizeof(double));

 /*
 Allocate memory for the B n vector
 */

 myCalloc(Bn,double,nbr_verts,sizeof(double));

 /*
 Initialize the n vector
 */

 for ( vert= 0 ; vert< nbr_verts ; vert++ )
  n[vert]= 1;

 iter= 0;

 tol= 1.0e-6;

 START:

 iter++;

 /*
 Let's compute B n
 as B1 n + B2 n + B3 n + B4 n + B5 n
 */

 /*
 Initialize B n
 */

 for ( vert= 0 ; vert< nbr_verts ; vert++ )
  Bn[vert]= 0;

 for ( dim= 0 ; dim< 5 ; dim++ ) {

    /*
    Print blur_matrix[dim]
    */

    /*
    for ( i= 0 ; i< nbr_verts ; i++ ) {
       fprintf(stdout,"i= %d blur_matrix[%d]= %d %d %d\n",
        i,dim,
        blur_matrix[dim][i*3+0],
        blur_matrix[dim][i*3+1],
        blur_matrix[dim][i*3+2]);
    }
    */

    /*
    Recall that the blur matrix is
    nbr of row = nbr of verts
    nbr of colums = 3
    column 0 corresponds to neighboring vert on the left
    column 1 corresponds to vert
    column 2 corresponds to neighboring vert on the right
    It's a 1,2,1 blur
    */

    for ( i= 0 ; i< nbr_verts ; i++ ) {

       k= blur_matrix[dim][i*3+0];
       Bn[i]+= (1./4.)*n[k];

       k= blur_matrix[dim][i*3+1];
       if ( k != i ) {
          error_handler("stereo_build_Cn_Cs_matrix");
       }
       Bn[i]+= (2./4.)*n[k];

       k= blur_matrix[dim][i*3+2];
       Bn[i]+= (1./4.)*n[k];
    }
 }

 /*
 Assign sqrt ( ( n x m ) / ( B n ) ) to n
 */

 /*
 Let's compute the max norm of vector n
 to check for convergence
 */

 max_norm= 0;

 for ( vert= 0 ; vert< nbr_verts ; vert++ ) {

    val= n[vert];

    new_val= sqrt(n[vert]*(double)m[vert]/Bn[vert]);

    if ( fabs(new_val-val) > max_norm )
     max_norm= fabs(new_val-val);

    n[vert]= new_val;
 }

 fprintf(stdout,"iter = %d max_norm= %lg\n",iter,max_norm);

 /*
 Use max_iter to stop
 */

 /*
 if ( iter < max_iter )
  goto START;
 */

 /*
 Use tolerance on the max norm to stop
 */

 if ( !( max_norm < tol ) )
  goto START;

 /*
 Allocate memory for the Cn matrix
 Cn = diag n
 It is a diagonal matrix, so we're gonna store it as a vector
 */

 myCalloc(Cn,double,nbr_verts,sizeof(double));

 for ( vert= 0 ; vert< nbr_verts ; vert++ )
  Cn[vert]= n[vert];

 /*
 Allocate memory for the Cs matrix
 Cs = diag m
 It is a diagonal matrix, so we're gonna store it as a vector
 */
 
 myCalloc(Cs,int,nbr_verts,sizeof(int));

 for ( vert= 0 ; vert< nbr_verts ; vert++ ) 
  Cs[vert]= m[vert];

 /*
 Check if ( Cn B Cn ) 1 = m
 */

 /*
 stereo_build_Cn_Cs_matrix_check(
  image_arr,
  width,
  height,
  mesh,
  splat_matrix,
  blur_matrix,
  m,
  Cn,
  Cs
 );
 */

 /*
 Free m
 */

 myFree(m);

 /*
 Free n
 */

 myFree(n);

 /*
 Free Bn
 */

 myFree(Bn);

 (*ptr_Cn_matrix)= Cn;

 (*ptr_Cs_matrix)= Cs;

}
