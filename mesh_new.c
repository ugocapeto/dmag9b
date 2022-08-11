#include "header.h"

void mesh_new(
 mesh_struct **ptr_mesh
)

{

 mesh_struct *mesh;
 int block;

 myMalloc(mesh,mesh_struct,sizeof(mesh_struct));

 myMalloc(mesh->mesh_verts,arrayHeader,sizeof(arrayHeader));
 block= 100;
 arrayInit(*(mesh->mesh_verts),block);

 (*ptr_mesh)= mesh;

}
