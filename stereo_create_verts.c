#include "header.h"

typedef struct vert_pixel_struct {
 int vert;
 int pixel;
} vert_pixel_struct;

static int compare_vert_pixel(
 const void *p1,
 const void *p2
)

{

 int vert1;
 int vert2;
 int pixel1;
 int pixel2;

 /*
 Compare the verts
 */

 vert1= ((vert_pixel_struct *)p1)->vert;
 vert2= ((vert_pixel_struct *)p2)->vert;
 if ( vert1 < vert2 )
  return -1;
 if ( vert1 > vert2 )
  return +1;

 /*
 Compare the pixels
 */

 pixel1= ((vert_pixel_struct *)p1)->pixel;
 pixel2= ((vert_pixel_struct *)p2)->pixel;
 if ( pixel1 < pixel2 )
  return -1;
 if ( pixel1 > pixel2 )
  return +1;

 return 0;

}

void stereo_create_verts(
 int *image_arr,
 int width,
 int height,
 int sample_rate_spatial,
 int sample_rate_range,
 int hash_table_size,
 mesh_struct **ptr_mesh,
 matrix_int_crs_struct **ptr_splat_matrix,
 int *blur_matrix[5]
)

/*
Create the bilateral space verts
Build the splat matrix
Build the blur matrices
*/

/*
sample_rate_spatial defines the size of the grid cells
in the spatial dimension
sample_rate_range defines the size of the grid cells
in the range dimension
*/

{

 arrayHeader *hash_table;
 int img_i;
 int img_j;
 int ind;
 int rgb_ind;
 int rgb[3];
 double x;
 double y;
 int grid_cell_ind[5];
 int vert;
 mesh_struct *mesh;
 int nbr_pixels;
 int nbr_verts;
 matrix_int_crs_struct *splat_matrix;
 int dim;
 arrayHeader *mesh_verts;
 int maxInd;
 int ind2;
 int dim2;
 int grid_cell_ind2[5];
 int vert2;
 double grid_cell_length[5];
 int hind;
 int block;
 int pixel;
 vert_pixel_struct *vert_pixel_arr;
 int vert_old;
 double val_dbl;
 int val_int;
 int grid_size[5];
 int count;
 int max_count;

 /*
 Get nbr of pixels
 */

 nbr_pixels= width*height;

 /*
 Initialize nbr of verts
 */

 nbr_verts= 0;

 /*
 The splat matrix has
 "nbr of verts" rows
 "nbr of pixels" columns
 but we're gonna use a Compressed Row Storage (CRS) to store it
 For that, we are gonna need to first store doublets
 containing the pixel and the vert it's associated with
 Then, we are gonna sort w/r to vert first and pixel second
 Finally, we are gonna build the splat matrix in CRS format
 */

 myCalloc(vert_pixel_arr,vert_pixel_struct,nbr_pixels,sizeof(vert_pixel_struct));

 /*
 We are gonna need to be able to find a bilateral space vert
 given coordinates in bilateral space
 */

 /*
 For that,
 it makes sense to create a hash table
 A hash table is basically an array of buckets
 Each bucket stores a bunch of entries according to a hash key
 */

 myCalloc(hash_table,arrayHeader,hash_table_size,sizeof(arrayHeader));

 for ( hind= 0 ; hind< hash_table_size ; hind++) {
    block= 100;
    arrayInit(hash_table[hind],block);
 }

 /*
 Create the mesh to hold the verts
 */

 mesh_new(
  &mesh
 );

 /*
 Let's get the grid cell length in all dimensions
 */

 val_dbl= (double)width/(double)sample_rate_spatial;
 val_int= (int)(val_dbl+.5);
 grid_size[0]= val_int;
 grid_cell_length[0]= (double)width/(double)val_int;

 val_dbl= (double)height/(double)sample_rate_spatial;
 val_int= (int)(val_dbl+.5);
 grid_size[1]= val_int;
 grid_cell_length[1]= (double)height/(double)val_int;

 val_dbl= 256./(double)sample_rate_range;
 val_int= (int)(val_dbl+.5);
 grid_size[2]= val_int;
 grid_size[3]= val_int;
 grid_size[4]= val_int;
 grid_cell_length[2]= 256./(double)val_int;
 grid_cell_length[3]= 256./(double)val_int;
 grid_cell_length[4]= 256./(double)val_int;

 fprintf(stdout,"Size of bilateral grid = %d x %d x %d x %d x %d\n",
  grid_size[0],grid_size[1],grid_size[2],grid_size[3],grid_size[4]);

 /*
 Let's go through each pixel of the image
 */

 for ( img_i= 0 ; img_i< height ; img_i++ ) {
    for ( img_j= 0 ; img_j< width ; img_j++ ) {
       pixel= img_i*width+img_j;
       for ( rgb_ind= 0 ; rgb_ind< 3 ; rgb_ind++ )
        rgb[rgb_ind]= image_arr[3*pixel+rgb_ind];

       /*
       Get grid cell in 5-dimensional space (x,y,r,g,b)
       where this pixel falls in
       */

       /*
       Let's take care of x coordinate
       */

       x= (double)img_j;

       /*
       Get grid cell index the x coordinate falls in
       */

       ind= x/grid_cell_length[0];

       if ( ind > grid_size[0]-1 )
        ind= grid_size[0]-1;

       grid_cell_ind[0]= ind;

       /*
       Let's take care of x coordinate
       */

       y= (double)img_i;

       /*
       Get grid cell index the y coordinate falls in
       */

       ind= y/grid_cell_length[1];

       if ( ind > grid_size[1]-1 )
        ind= grid_size[1]-1;

       grid_cell_ind[1]= ind;

       /*
       Let's take care of rgb colors
       */

       for ( rgb_ind= 0 ; rgb_ind< 3 ; rgb_ind++ ) {

          /*
          Get grid cell index the range coordinate falls in
          */

          ind= (double)rgb[rgb_ind]/grid_cell_length[2+rgb_ind];

          if ( ind > grid_size[2+rgb_ind]-1 )
           ind= grid_size[2+rgb_ind]-1;

          grid_cell_ind[2+rgb_ind]= ind;
       }

       /*
       grid_cell_ind[5] are in fact the coordinates in bilateral space
       */

       /*
       At this point,
       you are supposed to create a vert if it doesn't already exist
       */

       /*
       Let's see if vert has already been created
       */

       hash_table_get_vert(
        hash_table,
        hash_table_size,
        mesh,
        grid_size,
        grid_cell_ind,
        &vert
       );

       if ( vert == -1 ) {

          /*
          Create bilateral space vert
          */

          mesh_create_vert(
           mesh,
           grid_cell_ind,
           &vert
          );

          /*
          Insert vert into hash table
          */

          hash_table_insert_vert(
           hash_table,
           hash_table_size,
           mesh,
           grid_size,
           vert
          );

          nbr_verts++;
       }

       vert_pixel_arr[pixel].vert= vert;
       vert_pixel_arr[pixel].pixel= pixel;
    }
 }

 fprintf(stdout,"Nbr of pixels = %d\n",nbr_pixels);
 fprintf(stdout,"Nbr of verts = %d\n",nbr_verts);
 fprintf(stdout,"Ratio of pixels to verts = %f\n",
  (double)nbr_pixels/(double)nbr_verts);

 /*
 Print some info about the hash table
 For each hash table bucket,
 print the number of verts that are in
 */

 max_count= 0;
 for ( hind= 0 ; hind< hash_table_size ; hind++ ) {
    maxInd= arrayMaxId(hash_table[hind]);
    count= maxInd+1;
    if ( count > max_count )
     max_count= count;
 }
 fprintf(stdout,"Max nbr of verts in hash table = %d\n",
  max_count);

 /*
 Sort vert_pixel_arr w/r to vert and pixel
 */

 /*
 We're gonna use qsort that's part of C's standard library
 */

 qsort(
  vert_pixel_arr,
  nbr_pixels,
  sizeof(vert_pixel_struct),
  compare_vert_pixel
 );

 /*
 Print the vert pixel array after the sort
 to make sure it's correctly sorted
 */

 /*
 for ( ind= 0 ; ind< nbr_pixels ; ind++ ) {
    vert= vert_pixel_arr[ind].vert;
    pixel= vert_pixel_arr[ind].pixel;
    fprintf(stdout,"vert= %d pixel= %d\n",vert,pixel);
 }
 */

 /*
 Build the splat matrix in Compressed Row Storage (CRS) format 
 */

 myMalloc(splat_matrix,matrix_int_crs_struct,sizeof(matrix_int_crs_struct));
 splat_matrix->nnz= nbr_pixels;
 myCalloc(splat_matrix->val,int,nbr_pixels,sizeof(int));
 myCalloc(splat_matrix->col_ind,int,nbr_pixels,sizeof(int));
 splat_matrix->n= nbr_verts;
 myCalloc(splat_matrix->row_ptr,int,nbr_verts+1,sizeof(int));

 vert_old= -1;

 for ( ind= 0 ; ind< nbr_pixels ; ind++ ) {
    vert= vert_pixel_arr[ind].vert;
    pixel= vert_pixel_arr[ind].pixel;

    /*
    Vert is the row
    Pixel is the column
    for the splat matrix in full form
    */

    splat_matrix->val[ind]= 1;
    splat_matrix->col_ind[ind]= pixel;

    if ( vert != vert_old ) {

       /*
       ind start the row defined by vert
       */

       splat_matrix->row_ptr[vert]= ind;

       vert_old= vert;
    }
 }

 splat_matrix->row_ptr[nbr_verts]= nbr_pixels;

 /*
 We are gonna build the blur matrices
 One per dimension
 Recall that we have 5 dimensions (x,y,r,g,b)
 The blur is a narrow [1,2,1] blur
 (don't forget to divide by 4 to get the actual blur)
 */

 /*
 Allocate memory for each blur matrix
 */

 /*
 The format of the blur matrix is:
 nbr of rows = nbr_verts
 nbr of columns = 3
 column 0 contains the vert corresponding to the ngb grid cell
 column 1 contains vert
 column 2 contains the vert corresponding to the ngb grid cell
 */

 for ( dim= 0 ; dim< 5 ; dim++ )
  myCalloc(blur_matrix[dim],int,3*nbr_verts,sizeof(int));

 /*
 Process the verts
 */

 mesh_verts= mesh->mesh_verts;
 maxInd= arrayMaxId(*mesh_verts);
 nbr_verts= maxInd+1;

 for ( vert= 0 ; vert<= maxInd ; vert++ ) {

    mesh_vert_get_data(
     mesh,
     vert,
     grid_cell_ind
    );

    for ( dim= 0 ; dim< 5 ; dim++ ) {
       ind= grid_cell_ind[dim];

       if ( ind < 0 ) {
          error_handler("stereo_create_verts");
       }
       if ( ind > grid_size[dim]-1 ) {
          error_handler("stereo_create_verts");
       }

       /*
       Store vert in column 1
       */

       blur_matrix[dim][vert*3+1]= vert;

       /*
       Get vert2 that's before vert along that dimension
       Vert2 gets a weight of 1
       */

       /*
       Copy the coordinates
       */

       for ( dim2= 0 ; dim2< 5 ; dim2++ )
        grid_cell_ind2[dim2]= grid_cell_ind[dim2];

       /*
       Shift to the left along that dimension
       */

       ind2= ind-1;
       if ( ind2 < 0 )
        ind2= 0;

       grid_cell_ind2[dim]= ind2;

       /*
       Get the vert using the hash table
       */

       hash_table_get_vert(
        hash_table,
        hash_table_size,
        mesh,
        grid_size,
        grid_cell_ind2,
        &vert2
       );

       if ( vert2 == -1 ) {

          /*
          Do as if vert were the neighboring vert
          */

          vert2= vert; 
       }

       /* 
       Store vert2 in column 0
       */

       blur_matrix[dim][vert*3+0]= vert2;

       /*
       Get vert2 that's after vert along that dimension
       Vert2 gets a weight of 1
       */

       /*
       Copy the coordinates
       */

       for ( dim2= 0 ; dim2< 5 ; dim2++ )
        grid_cell_ind2[dim2]= grid_cell_ind[dim2];

       /*
       Shift to the right along that dimension
       */

       ind2= ind+1;
       if ( ind2 > grid_size[dim]-1 )
        ind2= grid_size[dim]-1;

       grid_cell_ind2[dim]= ind2;

       /*
       Get the vert using the hash table
       */

       hash_table_get_vert(
        hash_table,
        hash_table_size,
        mesh,
        grid_size,
        grid_cell_ind2,
        &vert2
       );

       if ( vert2 == -1 ) {

          /*
          Do as if vert were the neighboring vert
          */

          vert2= vert; 
       }

       /* 
       Store vert2 in column 2
       */

       blur_matrix[dim][vert*3+2]= vert2;
    }
 }

 /*
 Free the vert pixel array
 that was used to build the splat matrix
 */

 myFree(vert_pixel_arr);

 /*
 Free the hash table
 */

 for ( hind= 0 ; hind< hash_table_size ; hind++ ) {
    arrayDelAll(hash_table[hind]);
 }
 if ( hash_table_size > 0 )
  myFree(hash_table);

 (*ptr_mesh)= mesh;

 (*ptr_splat_matrix)= splat_matrix;

}
