#include "header.h"

void mesh_free(
 mesh_struct *mesh
)

{

 arrayHeader *mesh_verts;

 mesh_verts= mesh->mesh_verts;
 arrayDelAll(*mesh_verts);
 myFree(mesh_verts);

 myFree(mesh);
 
}
