#include "header.h"

void hash_table_insert_vert(
 arrayHeader *hash_table,
 int hash_table_size,
 mesh_struct *mesh,
 int grid_size[5],
 int vert
)

{

 int grid_cell_ind[5];
 hash_table_vert_struct hash_table_vert;
 int ind;
 int hash;
 int R=31;
 int i;

 mesh_vert_get_data(
  mesh,
  vert,
  grid_cell_ind
 );

 /*
 Compute hashing function value for that list of integers
 See http://algs4.cs.princeton.edu/34hash/
 */

 hash= 0;
 for ( i= 0 ; i< 5 ; i++ ) {
    hash= (R*hash+grid_cell_ind[i])%hash_table_size;
 }

 /*
 Add vert to the hash table bucket defined by the hashing function value
 */

 hash_table_vert.tag= 0;
 hash_table_vert.vert= vert;

 arrayPutNext(hash_table[hash],hash_table_vert,hash_table_vert_struct,hash_table_vert_struct_key,ind);

}
