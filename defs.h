typedef struct mesh_struct {
 arrayHeader *mesh_verts;
} mesh_struct;

typedef struct mesh_vert_struct {
 int tag;
 int grid_cell_ind[5];
} mesh_vert_struct;
#define mesh_vert_struct_key tag /* for garbage collection */

typedef struct hash_table_vert_struct {
 int tag;
 int vert;
} hash_table_vert_struct;
#define hash_table_vert_struct_key tag /* for garbage collection */

typedef struct matrix_int_crs_struct {
 int nnz; /* number of nonzeros in the matrix */
 int *val;
 int *col_ind;
 int n; /* number of rows in the matrix */
 int *row_ptr;
} matrix_int_crs_struct;
