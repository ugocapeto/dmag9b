#include "header.h"

void hash_table_get_vert(
 arrayHeader *hash_table,
 int hash_table_size,
 mesh_struct *mesh,
 int grid_size[5],
 int inp_grid_cell_ind[5],
 int *ptr_vert
)

{

 int maxInd;
 int ind;
 hash_table_vert_struct hash_table_vert;
 int next;
 int vert;
 int grid_cell_ind[5];
 int count;
 int i;
 int hash;
 int R=31;

 (*ptr_vert)=-1;

 /*
 Compute hashing function value for that list of integers
 See http://algs4.cs.princeton.edu/34hash/
 */

 hash= 0;
 for ( i= 0 ; i< 5 ; i++ ) {
    hash= (R*hash+inp_grid_cell_ind[i])%hash_table_size;
 }

 /*
 Get vert with same coordinates as input coordinates
 looking through the verts in the hash table bucket
 defined by the hashing function value
 */

 maxInd= arrayMaxId(hash_table[hash]);

 for ( ind= 0 ; ind<= maxInd ; ind++ ) {
    hash_table_vert= arrayGet(hash_table[hash],hash_table_vert_struct,ind);

    next= hash_table_vert.hash_table_vert_struct_key;
    if ( next < 0 )
     continue;

    vert= hash_table_vert.vert;

    /*
    See if that vert has the same coordinates
    */

    mesh_vert_get_data(
     mesh,
     vert,
     grid_cell_ind
    );

    count= 0;
    for ( i= 0 ; i< 5 ; i++ ) {
       if ( grid_cell_ind[i] == inp_grid_cell_ind[i] )
        count++;
    }

    if ( count == 5 ) {

       /*
       We have found the vert
       */

       (*ptr_vert)= vert;
    }
 }

}
