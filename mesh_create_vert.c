#include "header.h"

void mesh_create_vert(
 mesh_struct *mesh,
 int grid_cell_ind[5],
 int *ptr_vert
)

{

 arrayHeader *mesh_verts;
 mesh_vert_struct mesh_vert;
 int i;
 int vert;

 mesh_verts= mesh->mesh_verts;

 mesh_vert.tag= 0;

 for ( i= 0 ; i< 5 ; i++ )
  mesh_vert.grid_cell_ind[i]= grid_cell_ind[i];

 arrayPutNext(*mesh_verts,mesh_vert,mesh_vert_struct,mesh_vert_struct_key,vert);

 (*ptr_vert)= vert;

}
