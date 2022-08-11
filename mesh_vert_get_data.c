#include "header.h"

void mesh_vert_get_data(
 mesh_struct *mesh,
 int vert,
 int grid_cell_ind[5]
)

{

 arrayHeader *mesh_verts;
 mesh_vert_struct mesh_vert;
 int next;
 int i;

 mesh_verts= mesh->mesh_verts;

 mesh_vert= arrayGet(*mesh_verts,mesh_vert_struct,vert);

 next= mesh_vert.mesh_vert_struct_key;
 if ( next < 0 )
  error_handler("mesh_vert_get_data");

 for ( i= 0 ; i< 5 ; i++ )
  grid_cell_ind[i]= mesh_vert.grid_cell_ind[i];

}
