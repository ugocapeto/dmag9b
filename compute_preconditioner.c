#include "header.h"

void compute_preconditioner(
 mesh_struct *mesh,
 matrix_int_crs_struct *splat_matrix,
 int *blur_matrix[5],
 double *Cn_matrix,
 int *Cs_matrix,
 double lambda,
 double *con_arr,
 double *M1_arr
)

/*
Compute M^-1, the (Jacobi) preconditioner
*/

{

 arrayHeader *mesh_verts;
 int maxInd;
 int nbr_verts;
 int vert;
 double *diag_A;
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
 Allocate memory for diag_A
 */

 myCalloc(diag_A,double,nbr_verts,sizeof(double));

 /*
 Compute diag(A) =
 lambda*(diag(Dm)-diag(Dn)*Bdiag*diag(Dn))+Sc
 */

 for ( vert= 0 ; vert< nbr_verts ; vert++ ) {

    diag_A[vert]= 0.0;

    diag_A[vert]+= lambda*Cs_matrix[vert];

    diag_A[vert]-= lambda*Cn_matrix[vert]*(2./4.)*Cn_matrix[vert];

    vert_con= 0.0;

    for ( ind= splat_matrix->row_ptr[vert] ;
          ind< splat_matrix->row_ptr[vert+1] ;
          ind++ ) {
       pixel= splat_matrix->col_ind[ind];
       con= con_arr[pixel];
       vert_con+= con;
    }

    diag_A[vert]+= vert_con;
 }

 /*
 Compute M^-1 =
 diag(A)^-1
 */

 for ( vert= 0 ; vert< nbr_verts ; vert++ ) {
    if ( diag_A[vert] == 0 ) {
       error_handler("pb !");
    }
    M1_arr[vert]= 1./diag_A[vert];
 }

 /*
 Free diag_A
 */

 myFree(diag_A);

}
