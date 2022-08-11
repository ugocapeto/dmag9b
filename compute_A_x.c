#include "header.h"

void compute_A_x(
 mesh_struct *mesh,
 matrix_int_crs_struct *splat_matrix,
 int *blur_matrix[5],
 double *Cn_matrix,
 int *Cs_matrix,
 double lambda,
 double *con_arr,
 double *x_arr,
 double *A_x_arr
)

/*
Compute A x
where A = lambda (Dm - Dn B Dn) + diag (S c)
Dm is Cs
Dm is Cn 
*/

/*
Inspiration comes from stereo_compute_cost() in dmag7
*/

{

 arrayHeader *mesh_verts;
 int maxInd;
 int nbr_verts;
 double *temp_vector;
 double *Cnv;
 double *BCnv;
 double *CnBCnv;
 double *Csv;
 int dim;
 int vert;
 int i;
 int k;
 double vert_con;
 int ind;
 int pixel;
 double con;

 /*
 Get nbr of verts
 */

 mesh_verts= mesh->mesh_verts;
 maxInd= arrayMaxId(*mesh_verts);
 nbr_verts= maxInd+1;

 /*
 Initialize A_x_arr
 */

 for ( vert= 0 ; vert< nbr_verts ; vert++ ) {

    /*
    vert represents the row index
    */

    A_x_arr[vert]= 0;
 }

 /*
 Allocate memory for a temporary vector
 */

 myCalloc(temp_vector,double,nbr_verts,sizeof(double));

 /*
 Allocate memory for Cnv
 */

 myCalloc(Cnv,double,nbr_verts,sizeof(double));

 /*
 Allocate memory for BCnv
 */

 myCalloc(BCnv,double,nbr_verts,sizeof(double));

 /*
 Allocate memory for CnBCnv
 */

 myCalloc(CnBCnv,double,nbr_verts,sizeof(double));

 /*
 Allocate memory for Csv
 */

 myCalloc(Csv,double,nbr_verts,sizeof(double));

 /*
 Let's compute Cn v
 */

 /*
 Recall that Cn is a diagonal matrix that is stored as a vector
 */

 for ( i= 0 ; i< nbr_verts ; i++ ) {
    Cnv[i]= Cn_matrix[i]*x_arr[i];
 }

 /*
 Let's compute B Cn v
 as B1 Cn v + B2 Cn v + B3 Cn v + B4 Cn v + B5 Cn v
 */

 for ( dim= 0 ; dim< 5 ; dim++ ) {

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
       temp_vector[i]= 0;

       k= blur_matrix[dim][i*3+0];
       temp_vector[i]+=
        (1./4.)*
        Cnv[k];

       k= blur_matrix[dim][i*3+1];
       if ( k != i ) {
          error_handler("stereo_compute_cost");
       }
       temp_vector[i]+=
        (2./4.)*
        Cnv[k];

       k= blur_matrix[dim][i*3+2];
       temp_vector[i]+=
        (1./4.)*
        Cnv[k];
    }

    /*
    Add to B Cn v
    */

    for ( vert= 0 ; vert< nbr_verts ; vert++ ) {

       /*
       vert represents the row index
       */

       BCnv[vert]+= temp_vector[vert];
    }
 }

 /*
 Let's compute Cn B Cn v
 */

 /*
 Recall that Cn is a diagonal matrix that is stored as a vector
 */

 for ( i= 0 ; i< nbr_verts ; i++ ) {
    CnBCnv[i]= Cn_matrix[i]*BCnv[i];
 }

 /*
 Let's compute Csv
 */

 /*
 Recall that Cs is a diagonal matrix that is stored as a vector
 */

 for ( i= 0 ; i< nbr_verts ; i++ ) {
    Csv[i]= Cs_matrix[i]*x_arr[i];
 }

 /*
 Add to A_x_arr
 */

 for ( vert= 0 ; vert< nbr_verts ; vert++ ) {

    /*
    vert represents the row index
    */

    A_x_arr[vert]+=
     lambda*(Csv[vert]-CnBCnv[vert]);
 }

 /*
 Add diag(Sc) to A_x_arr
 S is the splat matrix
 nbr of rows = nbr_verts
 nbr of columns = nbr_pixels
 c is the confidence matrix
 */

 for ( vert= 0 ; vert< nbr_verts ; vert++ ) {

    vert_con= 0;

    for ( ind= splat_matrix->row_ptr[vert] ;
          ind< splat_matrix->row_ptr[vert+1] ;
          ind++ ) {
       pixel= splat_matrix->col_ind[ind];
       con= con_arr[pixel];
       vert_con+= con;
    }

    A_x_arr[vert]+=
     vert_con*x_arr[vert];
 }

 /*
 Free temp_vector
 */

 if ( nbr_verts > 0 )
  myFree(temp_vector);

 /*
 Free memory for Cnv
 */

 if ( nbr_verts > 0 )
  myFree(Cnv);

 /*
 Free memory for BCnv
 */

 if ( nbr_verts > 0 )
  myFree(BCnv);

 /*
 Free memory for CnBCnv
 */

 if ( nbr_verts > 0 )
  myFree(CnBCnv);

 /*
 Free memory for Csv
 */

 if ( nbr_verts > 0 )
  myFree(Csv);

}
