#include "header.h"

void pcg(
 mesh_struct *mesh,
 matrix_int_crs_struct *splat_matrix,
 int *blur_matrix[5],
 double *Cn_matrix,
 int *Cs_matrix,
 double lambda,
 int *dep_arr,
 double *con_arr,
 double *b,
 double *x,
 int n,
 double epsilon
)

/*
Solve the linear system using Preconditioned Conjugate Gradients
*/

{

 arrayHeader *mesh_verts;
 int maxInd;
 int nbr_verts;
 int i;
 double *r;
 double *A_x;
 int vert;
 double *d;
 double delta_new;
 double delta_old;
 double *q;
 double *s;
 double den;
 double alpha;
 double beta;
 double delta_0;
 double error;
 double *M1_arr;
 int print_flag= 0;

 /*
 Get nbr of verts
 */

 mesh_verts= mesh->mesh_verts;
 maxInd= arrayMaxId(*mesh_verts);
 nbr_verts= maxInd+1;

 /*
 Allocate memory for the preconditioner M1_arr
 */

 myCalloc(M1_arr,double,nbr_verts,sizeof(double));

 /*
 Compute the preconditioner M1_arr
 */

 compute_preconditioner(
  mesh,
  splat_matrix,
  blur_matrix,
  Cn_matrix,
  Cs_matrix,
  lambda,
  con_arr,
  M1_arr
 );

 /*
 Initialize iteration
 */

 i= 1;

 /*
 Allocate memory for r (the residual)
 */

 myCalloc(r,double,nbr_verts,sizeof(double));

 /*
 Allocate memory for A_x
 */

 myCalloc(A_x,double,nbr_verts,sizeof(double));

 /*
 Initialize the residual r =
 b-A(x)
 */

 compute_A_x(
  mesh,
  splat_matrix,
  blur_matrix,
  Cn_matrix,
  Cs_matrix,
  lambda,
  con_arr,
  x,
  A_x
 );

 for ( vert= 0 ; vert< nbr_verts ; vert++ ) {
    r[vert]= b[vert]-A_x[vert];
 }

 /*
 Allocate memory for d
 */

 myCalloc(d,double,nbr_verts,sizeof(double));

 /*
 Initialize d =
 M^-1(r)
 */

 for ( vert= 0 ; vert< nbr_verts ; vert++ ) {
    d[vert]= M1_arr[vert]*r[vert];
 }

 /*
 Initialize delta_new =
 r(trans)d
 */

 delta_new= 0;
 for ( vert= 0 ; vert< nbr_verts ; vert++ ) {
    delta_new+= r[vert]*d[vert];
 }

 /*
 Set delta_0 =
 delta_new
 */

 delta_0= delta_new;

 /*
 Allocate memory for q
 */

 myCalloc(q,double,nbr_verts,sizeof(double));

 /*
 Allocate memory for s
 */

 myCalloc(s,double,nbr_verts,sizeof(double));

 START:

 if ( print_flag == 1 ) {
    fprintf(stdout,"i = %d\n",i);
 }

 /*
 Compute q =
 A(d)
 */

 compute_A_x(
  mesh,
  splat_matrix,
  blur_matrix,
  Cn_matrix,
  Cs_matrix,
  lambda,
  con_arr,
  d,
  q
 );

 /*
 Compute alpha =
 delta_new/d(trans)q
 */

 den= 0;
 for ( vert= 0 ; vert< nbr_verts ; vert++ ) {
    den+= d[vert]*q[vert];
 }

 if ( den == 0 ) {
    error_handler("pcg");
 }

 alpha= delta_new/den;

 /*
 Update x =
 x + alpha*d
 */

 for ( vert= 0 ; vert< nbr_verts ; vert++ ) {
    x[vert]= x[vert] + alpha*d[vert];
 }

 /*
 Update r =
 r - alpha*q
 */

 for ( vert= 0 ; vert< nbr_verts ; vert++ ) {
    r[vert]= r[vert] - alpha*q[vert];
 }

 /*
 Update s =
 M^-1(r)
 */

 for ( vert= 0 ; vert< nbr_verts ; vert++ ) {
    s[vert]= M1_arr[vert]*r[vert];
 }

 /*
 Update delta_old =
 delta_new
 */

 delta_old= delta_new;

 /*
 Update delta_new =
 r(trans)s
 */

 delta_new= 0;
 for ( vert= 0 ; vert< nbr_verts ; vert++ ) {
    delta_new+= r[vert]*s[vert];
 }

 /*
 Update beta =
 delta_new/delta_old
 */

 if ( delta_old  == 0 ) {
    error_handler("pcg");
 }

 beta= delta_new/delta_old;

 /*
 Update d =
 s + beta*d
 */

 for ( vert= 0 ; vert< nbr_verts ; vert++ ) {
    d[vert]= s[vert] + beta*d[vert];
 }

 error= delta_new/delta_0;

 if ( print_flag == 1 ) {
    fprintf(stdout,"error = %g\n",error);
 }

 /*
 Update i =
 i + 1
 */

 i= i + 1;

 /*
 Check for termination
 */

 if ( i <= n && error > epsilon*epsilon )
  goto START;

 /*
 Free the preconditioner M1_arr
 */

 myFree(M1_arr);

 /*
 Free r (the residual)
 */

 myFree(r);

 /*
 Free A_x
 */

 myFree(A_x);

 /*
 Free d
 */

 myFree(d);
 
 /*
 Free q
 */

 myFree(q);
 
 /*
 Free s
 */

 myFree(s);

}
