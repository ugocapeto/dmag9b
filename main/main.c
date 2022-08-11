#include "header.h"
#include "proto.h"

/*
This is an implementation of the Fast Bilateral Solver of Barron et al.
*/

int main(
 int argc,
 char* argv[]
)

{

 FILE *fp;
 char filename[80];
 char filename_ref[80];
 char filename_dep[80];
 char filename_out[80];
 int width;
 int height;
 double lambda;
 int sample_rate_spatial;
 int sample_rate_range;
 int hash_table_size;
 int *ref_arr;
 int *dep_arr;
 double *con_arr;
 int *out_arr;
 int nbr_pixels;
 int n;
 double sigma_gm;
 int n_irls;
 int *con_map_arr;
 int radius;
 double gamma_p;
 double gamma_c;
 double sigma;
 int i;
 int j;
 int pixel;
 int err_flag;

 /*
 Let's read in the input file
 */

 fp= fopen("dmag9b_input.txt","r");

 /*
 Get filename for reference image
 */

 fscanf(fp,"%s",filename);

 fprintf(stdout,"reference image = %s\n",filename);

 strcpy(filename_ref,filename);

 /*
 Get filename for input disparity map
 */

 fscanf(fp,"%s",filename);

 fprintf(stdout,"input disparity map = %s\n",filename);

 strcpy(filename_dep,filename);

 /*
 Get spatial sample rate
 */

 fscanf(fp,"%d",&sample_rate_spatial);

 fprintf(stdout,"sample_rate_spatial = %d\n",sample_rate_spatial);

 /*
 Get range sample rate
 */

 fscanf(fp,"%d",&sample_rate_range);

 fprintf(stdout,"sample_rate_range = %d\n",sample_rate_range);

 /*
 Get lambda
 */

 fscanf(fp,"%lg",&lambda);

 fprintf(stdout,"lambda = %g\n",lambda);

 /*
 Get hash table size
 */

 fscanf(fp,"%d",&hash_table_size);

 fprintf(stdout,"hash_table_size = %d\n",hash_table_size);

 /*
 Get nbr of iterations (for linear solver)
 */

 fscanf(fp,"%d",&n);

 fprintf(stdout,"nbr of iterations (linear solver) = %d\n",n);

 /*
 Get sigma_gm
 */

 fscanf(fp,"%lg",&sigma_gm);

 fprintf(stdout,"sigma_gm = %g\n",sigma_gm);

 /*
 Get nbr of iterations (for irls)
 */

 fscanf(fp,"%d",&n_irls);

 fprintf(stdout,"nbr of iterations (irls) = %d\n",n_irls);

 /*
 Get radius (confidence map)
 */

 fscanf(fp,"%d",&radius);

 fprintf(stdout,"radius (confidence map) = %d\n",radius);

 /*
 Get gamma proximity (confidence map)
 */

 fscanf(fp,"%lg",&gamma_p);

 fprintf(stdout,"gamma proximity (confidence map) = %g\n",gamma_p);

 /*
 Get gamma color similarity (confidence map)
 */

 fscanf(fp,"%lg",&gamma_c);

 fprintf(stdout,"gamma color similarity (confidence map) = %g\n",gamma_c);

 /*
 Get sigma (confidence map)
 */

 fscanf(fp,"%lg",&sigma);

 fprintf(stdout,"sigma (confidence map) = %g\n",sigma);

 /*
 Get filename for output depth map image
 */

 fscanf(fp,"%s",filename);

 fprintf(stdout,"output depth map image = %s\n",filename);

 strcpy(filename_out,filename);

 /*
 Done reading the input file
 */

 fclose(fp);

 /*
 Load reference image
 */

 err_flag= load_rgb_image(
  filename_ref,
  &ref_arr,
  &width,
  &height
 );

 if ( err_flag == 1 )
  return 1;

 /*
 Load input disparity map
 */

 err_flag= load_image(
  filename_dep,
  &dep_arr,
  &width,
  &height
 );

 if ( err_flag == 1 )
  return 1;

 /*
 Allocate memory for the confidence array
 */

 nbr_pixels= width*height;
 myCalloc(con_arr,double,nbr_pixels,sizeof(double));

 /*
 Compute the confidence map
 */

 fprintf(stdout,"Computing the depth confidence ...\n");

 compute_depth_confidence(
  ref_arr,
  dep_arr,
  width,
  height,
  radius,
  gamma_p,
  gamma_c,
  sigma,
  con_arr
 );

 fprintf(stdout,"Computing the depth confidence ... done.\n");

 /*
 Good idea to normalize the confidence array
 and dump it into a grayscale image
 */

 nbr_pixels= width*height;
 myCalloc(con_map_arr,int,nbr_pixels,sizeof(int));
 normalize_image(
  con_arr,
  con_map_arr,
  nbr_pixels
 );
 err_flag= write_image(
  "con_map.jpg",
  con_map_arr,
  width,
  height
 );
 if ( err_flag == 1 ) {
    return 1;
 }
 myFree(con_map_arr);

 /*
 Do not allow the confidence to be equal to exactly 0
 */

 for ( i= 0 ; i< height ; i++ ) {
    for ( j= 0 ; j< width ; j++ ) {
       pixel= i*width+j;
       if ( con_arr[pixel] == 0 )
        con_arr[pixel]= 0.00001;
    }
 }

 /*
 Allocate memory for the output depth map image
 */

 nbr_pixels= width*height;
 myCalloc(out_arr,int,nbr_pixels,sizeof(int));

 /*
 Smooth the depth map
 */

 robust_bilateral_solver(
  width,
  height,
  ref_arr,
  dep_arr,
  con_arr,
  out_arr,
  sample_rate_spatial,
  sample_rate_range,
  lambda,
  hash_table_size,
  n,
  sigma_gm,
  n_irls
 );

 /*
 Let's dump the output depth map
 */

 err_flag= write_image(
  filename_out,
  out_arr,
  width,
  height
 );

 if ( err_flag == 1 ) {
    return 1;
 }

 /*
 Free memory
 */

 free(ref_arr);

 free(dep_arr);

 free(con_arr);

 free(out_arr);

 return 0;

}
