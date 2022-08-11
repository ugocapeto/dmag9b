void cg(
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
);

void compute_A_x(
 mesh_struct *mesh,
 matrix_int_crs_struct *splat_matrix,
 int *blur_matrix[5],
 double *Cn_matrix,
 int *Cs_matrix,
 double lambda,
 double *con_arr,
 double *v_arr,
 double *A_v_arr
);

void compute_confidence_map(
 int width,
 int height,
 int *ref_arr,
 int *dep_arr,
 double sigma_s,
 double sigma_r,
 int num_iterations,
 double *con_arr
);

void compute_depth_confidence(
 int *image_arr,
 int *depthmap_arr,
 int width,
 int height,
 int radius,
 double gamma_p,
 double gamma_c,
 double sigma,
 double *conf_arr
);

void compute_preconditioner(
 mesh_struct *mesh,
 matrix_int_crs_struct *splat_matrix,
 int *blur_matrix[5],
 double *Cn_matrix,
 int *Cs_matrix,
 double lambda,
 double *con_arr,
 double *M1_arr
);

void domain_transform_recursive_filter(
 int width,
 int height,
 int *image_arr,
 double sigma_s,
 double sigma_r,
 int num_iterations,
 int *joint_image_arr,
 int *filter_image_arr
);

void domain_transform_recursive_filter_dmag9b(
 int width,
 int height,
 double *inp_arr,
 double sigma_s,
 double sigma_r,
 int num_iterations,
 int *joint_image_arr,
 double *out_arr
);

void gradient_x(
 int width,
 int height,
 double *image,
 double *gradient
);

void gradient_y(
 int width,
 int height,
 double *image,
 double *gradient
);

void fast_bilateral_solver(
 int width,
 int height,
 int *ref_arr,
 int *dep_arr,
 double *con_arr,
 int *out_arr,
 int sample_rate_spatial,
 int sample_rate_range,
 double lambda,
 int hash_table_size,
 int n
);

void hash_table_get_vert(
 arrayHeader *hash_table,
 int hash_table_size,
 mesh_struct *mesh,
 int grid_size[5],
 int inp_grid_cell_ind[5],
 int *ptr_vert
);

void hash_table_insert_vert(
 arrayHeader *hash_table,
 int hash_table_size,
 mesh_struct *mesh,
 int grid_size[5],
 int vert
);

void mesh_create_vert(
 mesh_struct *mesh,
 int grid_cell_ind[5],
 int *ptr_vert
);

void mesh_free(
 mesh_struct *mesh
);

void mesh_new(
 mesh_struct **ptr_mesh
);

void mesh_vert_get_data(
 mesh_struct *mesh,
 int vert,
 int grid_cell_ind[5]
);

void normalize_image(
 double *inp_arr,
 int *out_arr,
 int size
);

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
);

void robust_bilateral_solver(
 int width,
 int height,
 int *ref_arr,
 int *dep_arr,
 double *inp_con_arr,
 int *out_arr,
 int sample_rate_spatial,
 int sample_rate_range,
 double lambda,
 int hash_table_size,
 int n,
 double sigma_gm,
 int n_irls
);

void stereo_build_Cn_Cs_matrix(
 int *image_arr,
 int width,
 int height,
 mesh_struct *mesh,
 matrix_int_crs_struct *splat_matrix,
 int *blur_matrix[5],
 double **ptr_Cn_matrix,
 int **ptr_Cs_matrix
);

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
);

void transformed_domain_recursive_filter_horizontal(
 int width,
 int height,
 double *I[3],
 double *D,
 double sigma
);

void transformed_domain_recursive_filter_vertical(
 int width,
 int height,
 double *I[3],
 double *D,
 double sigma
);
