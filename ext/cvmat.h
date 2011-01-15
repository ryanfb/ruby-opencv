/************************************************************

   cvmat.h -

   $Author: lsxi $

   Copyright (C) 2005-2007 Masakazu Yonekura

************************************************************/
#ifndef RUBY_OPENCV_CVMAT_H
#define RUBY_OPENCV_CVMAT_H

#include "opencv.h"

#define __NAMESPACE_BEGIN_CVMAT namespace cCvMat{
#define __NAMESPACE_END_CVMAT }

__NAMESPACE_BEGIN_OPENCV
__NAMESPACE_BEGIN_CVMAT

VALUE rb_class();

void define_ruby_class();

VALUE rb_allocate(VALUE klass);
VALUE rb_initialize(int argc, VALUE *argv, VALUE self);
VALUE rb_load_imageM(int argc, VALUE *argv, VALUE self);

VALUE rb_method_missing(int argc, VALUE *argv, VALUE self);
VALUE rb_to_s(VALUE self);
VALUE rb_has_parent_q(VALUE self);
VALUE rb_parent(VALUE self);
VALUE rb_inside_q(VALUE self, VALUE object);
VALUE rb_to_IplConvKernel(VALUE self, VALUE anchor);
VALUE rb_create_mask(VALUE self);

VALUE rb_width(VALUE self);
VALUE rb_height(VALUE self);
VALUE rb_depth(VALUE self);
VALUE rb_channel(VALUE self);
VALUE rb_data(VALUE self);

VALUE rb_clone(VALUE self);
VALUE rb_copy(int argc, VALUE *argv, VALUE self);
VALUE copy(VALUE mat);

VALUE rb_to_8u(VALUE self);
VALUE rb_to_8s(VALUE self);
VALUE rb_to_16u(VALUE self);
VALUE rb_to_16s(VALUE self);
VALUE rb_to_32s(VALUE self);
VALUE rb_to_32f(VALUE self);
VALUE rb_to_64f(VALUE self);
VALUE rb_vector_q(VALUE self);
VALUE rb_square_q(VALUE self);
// cxcore function
VALUE rb_to_CvMat(VALUE self);
VALUE rb_to_IplImage(VALUE self);
VALUE rb_sub_rect(VALUE self, VALUE args);
VALUE rb_slice_width(VALUE self, VALUE num);
VALUE rb_slice_height(VALUE self, VALUE num);
VALUE rb_row(VALUE self, VALUE args);
VALUE rb_col(VALUE self, VALUE args);
VALUE rb_each_row(VALUE self);
VALUE rb_each_col(VALUE self);
VALUE rb_diag(int argc, VALUE *argv, VALUE self);
VALUE rb_size(VALUE self);
VALUE rb_dims(VALUE self);
VALUE rb_dim_size(VALUE self, VALUE index);
VALUE rb_aref(VALUE self, VALUE args);
VALUE rb_aset(VALUE self, VALUE args);
VALUE rb_fill(int argc, VALUE *argv, VALUE self);
VALUE rb_fill_bang(int argc, VALUE *argv, VALUE self);
VALUE rb_clear(VALUE self);
VALUE rb_clear_bang(VALUE self);
VALUE rb_set_identity(int argc, VALUE *argv, VALUE self);
VALUE rb_set_identity_bang(int argc, VALUE *argv, VALUE self);
VALUE rb_range(int argc, VALUE *argv, VALUE self);
VALUE rb_range_bang(int argc, VALUE *argv, VALUE self);
/* Transforms and Permutations */
VALUE rb_reshape(VALUE self, VALUE hash);
VALUE rb_repeat(VALUE self, VALUE object);
VALUE rb_flip(int argc, VALUE *argv, VALUE self);
VALUE rb_flip_bang(int argc, VALUE *argv, VALUE self);
VALUE rb_split(VALUE self);
VALUE rb_merge(VALUE klass, VALUE args);
VALUE rb_mix_channels(int argc, VALUE *argv, VALUE klass);
VALUE rb_rand_shuffle(int argc, VALUE *argv, VALUE klass);
VALUE rb_rand_shuffle_bang(int argc, VALUE *argv, VALUE klass);

VALUE rb_lut(VALUE self, VALUE lut);
VALUE rb_convert_scale(VALUE self, VALUE hash);
VALUE rb_convert_scale_abs(VALUE self, VALUE hash);
VALUE rb_add(int argc, VALUE *argv, VALUE self);
VALUE rb_sub(int argc, VALUE *argv, VALUE self);    
VALUE rb_mul(int argc, VALUE *argv, VALUE self);
VALUE rb_mat_mul(int argc, VALUE *argv, VALUE self);
VALUE rb_div(int argc, VALUE *argv, VALUE self);    
VALUE rb_and(int argc, VALUE *argv, VALUE self);
VALUE rb_or(int argc, VALUE *argv, VALUE self);
VALUE rb_xor(int argc, VALUE *argv, VALUE self);
VALUE rb_not(VALUE self);
VALUE rb_not_bang(VALUE self);
VALUE rb_cmp_internal(VALUE self, VALUE val, int operand);
VALUE rb_eq(VALUE self, VALUE val);
VALUE rb_gt(VALUE self, VALUE val);
VALUE rb_ge(VALUE self, VALUE val);
VALUE rb_lt(VALUE self, VALUE val);
VALUE rb_le(VALUE self, VALUE val);
VALUE rb_ne(VALUE self, VALUE val);
VALUE rb_in_range(VALUE self, VALUE min, VALUE max);
VALUE rb_abs_diff(VALUE self, VALUE val);
/* Statistics */
VALUE rb_count_non_zero(VALUE self);
VALUE rb_sum(VALUE self);
VALUE rb_avg(int argc, VALUE *argv, VALUE self);
VALUE rb_avg_sdv(int argc, VALUE *argv, VALUE self);
VALUE rb_sdv(int argc, VALUE *argv, VALUE self);
VALUE rb_min_max_loc(int argc, VALUE *argv, VALUE self);

//VALUE rb_norm();
VALUE rb_dot_product(VALUE self, VALUE mat);
VALUE rb_cross_product(VALUE self, VALUE mat);
// VALUE rb_gemm();
VALUE rb_transform(int argc, VALUE *argv, VALUE self);
VALUE rb_perspective_transform(VALUE self, VALUE mat);
VALUE rb_mul_transposed(VALUE self, VALUE args);
VALUE rb_trace(VALUE self);
VALUE rb_transpose(VALUE self);
VALUE rb_transpose_bang(VALUE self);
VALUE rb_det(VALUE self);
VALUE rb_invert(int argc, VALUE *argv, VALUE self);
VALUE rb_solve(int argc, VALUE *argv, VALUE self);
VALUE rb_svd(int argc, VALUE *argv, VALUE self);
VALUE rb_svbksb(int argc, VALUE *argv, VALUE self);
VALUE rb_eigenvv(int argc, VALUE *argv, VALUE self);
VALUE rb_eigenvv_bang(int argc, VALUE *argv, VALUE self);
VALUE rb_calc_covar_matrix(int argc, VALUE *argv, VALUE self);
VALUE rb_mahalonobis(int argc, VALUE *argv, VALUE self);

VALUE rb_dft(int argc, VALUE *argv, VALUE self);
//VALUE rb_optimal_dft_size(VALUE self);
//VALUE rb_mul_spectrums(int argc, VALUE *argv, VALUE self);
VALUE rb_dct(int argc, VALUE *argv, VALUE self);

/* drawing function*/
VALUE rb_line(int argc, VALUE *argv, VALUE self);
VALUE rb_line_bang(int argc, VALUE *argv, VALUE self);
VALUE rb_rectangle(int argc, VALUE *argv, VALUE self);
VALUE rb_rectangle_bang(int argc, VALUE *argv, VALUE self);
VALUE rb_circle(int argc, VALUE *argv, VALUE self);
VALUE rb_circle_bang(int argc, VALUE *argv, VALUE self);
VALUE rb_ellipse(int argc, VALUE *argv, VALUE self);
VALUE rb_ellipse_bang(int argc, VALUE *argv, VALUE self);
VALUE rb_ellipse_box(int argc, VALUE *argv, VALUE self);
VALUE rb_ellipse_box_bang(int argc, VALUE *argv, VALUE self);
VALUE rb_fill_poly(int argc, VALUE *argv, VALUE self);
VALUE rb_fill_poly_bang(int argc, VALUE *argv, VALUE self);
VALUE rb_fill_convex_poly(int argc, VALUE *argv, VALUE self);
VALUE rb_fill_convex_poly_bang(int argc, VALUE *argv, VALUE self);
VALUE rb_poly_line(int argc, VALUE *argv, VALUE self);
VALUE rb_poly_line_bang(int argc, VALUE *argv, VALUE self);

VALUE rb_put_text(int argc, VALUE *argv, VALUE self);
VALUE rb_put_text_bang(int argc, VALUE *argv, VALUE self);
/* cv function */
VALUE rb_sobel(int argc, VALUE *argv, VALUE self);
VALUE rb_laplace(int argc, VALUE *argv, VALUE self);
VALUE rb_canny(int argc, VALUE *argv, VALUE self);
VALUE rb_pre_corner_detect(int argc, VALUE *argv, VALUE self);
VALUE rb_corner_eigenvv(int argc, VALUE *argv, VALUE self);
VALUE rb_corner_min_eigen_val(int argc, VALUE *argv, VALUE self);
VALUE rb_corner_harris(int argc, VALUE *argv, VALUE self);
VALUE rbi_find_corner_sub_pix(int argc, VALUE *argv, VALUE self);
VALUE rb_good_features_to_track(int argc, VALUE *argv, VALUE self);

VALUE rb_sample_line(int argc, VALUE *argv, VALUE self);
VALUE rb_rect_sub_pix(VALUE self, VALUE center, VALUE size);
VALUE rb_quadrangle_sub_pix(VALUE self, VALUE map_matrix, VALUE size);
VALUE rb_resize(int argc, VALUE *argv, VALUE self);
VALUE rb_warp_affine(int argc, VALUE *argv, VALUE self);
VALUE rb_rotation(VALUE self, VALUE center, VALUE angle, VALUE scale);
VALUE rb_warp_perspective(int argc, VALUE *argv, VALUE self);
//VALUE rb_perspective_transform();
VALUE rb_remap(int argc, VALUE *argv, VALUE self);
VALUE rb_log_polar(int argc, VALUE *argv);

VALUE rb_erode(int argc, VALUE *argv, VALUE self);
VALUE rb_erode_bang(int argc, VALUE *argv, VALUE self);
VALUE rb_dilate(int argc, VALUE *argv, VALUE self);
VALUE rb_dilate_bang(int argc, VALUE *argv, VALUE self);
VALUE rb_morphology_open(int argc, VALUE *argv, VALUE self);
VALUE rb_morphology_close(int argc, VALUE *argv, VALUE self);
VALUE rb_morphology_gradient(int argc, VALUE *argv, VALUE self);
VALUE rb_morphology_tophat(int argc, VALUE *argv, VALUE self);
VALUE rb_morphology_blackhat(int argc, VALUE *argv, VALUE self);

VALUE rb_smooth_blur_no_scale(int argc, VALUE *argv, VALUE self);
VALUE rb_smooth_blur(int argc, VALUE *argv, VALUE self);
VALUE rb_smooth_gaussian(int argc, VALUE *argv, VALUE self);
VALUE rb_smooth_median(int argc, VALUE *argv, VALUE self);
VALUE rb_smooth_bilateral(int argc, VALUE *argv, VALUE self);
VALUE rb_filter2d(int argc, VALUE *argv, VALUE self);
VALUE rb_copy_make_border_constant(int argc, VALUE *argv, VALUE self);
VALUE rb_copy_make_border_replicate(int argc, VALUE *argv, VALUE self);
VALUE rb_integral(int argc, VALUE *argv, VALUE self);
VALUE rb_threshold_binary(int argc, VALUE *argv, VALUE self);
VALUE rb_threshold_binary_inverse(int argc, VALUE *argv, VALUE self);
VALUE rb_threshold_trunc(int argc, VALUE *argv, VALUE self);
VALUE rb_threshold_to_zero(int argc, VALUE *argv, VALUE self);
VALUE rb_threshold_to_zero_inverse(int argc, VALUE *argv, VALUE self);
VALUE rb_adaptive_threshold(int argc, VALUE *argv, VALUE self);

VALUE rb_pyr_down(int argc, VALUE *argv, VALUE self);
VALUE rb_pyr_up(int argc, VALUE *argv, VALUE self);

VALUE rb_flood_fill(int argc, VALUE *argv, VALUE self);
VALUE rb_flood_fill_bang(int argc, VALUE *argv, VALUE self);
VALUE rb_find_contours(int argc, VALUE *argv, VALUE self);
VALUE rb_find_contours_bang(int argc, VALUE *argv, VALUE self);
VALUE rb_pyr_segmentation(int argc, VALUE *argv, VALUE self);
VALUE rb_pyr_mean_shift_filtering(int argc, VALUE *argv, VALUE self);
VALUE rb_watershed(VALUE self);

VALUE rb_moments(int argc, VALUE *argv, VALUE self);

VALUE rb_hough_lines_standard(int argc, VALUE *argv, VALUE self);
VALUE rb_hough_lines_probabilistic(int argc, VALUE *argv, VALUE self);
VALUE rb_hough_lines_multi_scale(int argc, VALUE *argv, VALUE self);
VALUE rb_hough_circles_gradient(int argc, VALUE *argv, VALUE self);
VALUE rb_dist_transform(int argc, VALUE *argv, VALUE self);
VALUE rb_inpaint_ns(VALUE self, VALUE mask, VALUE radius);
VALUE rb_inpaint_telea(VALUE self, VALUE mask, VALUE radius);

VALUE rb_equalize_hist(VALUE self);
/* Matching*/
VALUE rb_match_template(int argc, VALUE *argv, VALUE self);
VALUE rb_match_shapes_i1(int argc, VALUE *argv, VALUE self);
VALUE rb_match_shapes_i2(int argc, VALUE *argv, VALUE self);
VALUE rb_match_shapes_i3(int argc, VALUE *argv, VALUE self);
// VALUE rb_calc_emd(int argc, VALUE *argv, VALUE self);
/* Object Tracking */
VALUE rb_mean_shift(VALUE self, VALUE window, VALUE criteria);
VALUE rb_cam_shift(VALUE self, VALUE window, VALUE criteria);
VALUE rb_snake_image(int argc, VALUE *argv, VALUE self);
/* Optical Flow */
VALUE rb_optical_flow_hs(int argc, VALUE *argv, VALUE self);
VALUE rb_optical_flow_lk(int argc, VALUE *argv, VALUE self);
VALUE rb_optical_flow_bm(int argc, VALUE *argv, VALUE self);
VALUE rb_optical_flow_pyr_lk(int argc, VALUE *argv, VALUE self);

/* Epipolar Geometory */
VALUE rb_find_fundamental_mat_7point(int argc, VALUE *argv, VALUE klass);
VALUE rb_find_fundamental_mat_8point(int argc, VALUE *argv, VALUE klass);
VALUE rb_find_fundamental_mat_ransac(int argc, VALUE *argv, VALUE klass);
VALUE rb_find_fundamental_mat_lmeds(int argc, VALUE *argv, VALUE klass);
VALUE rb_compute_correspond_epilines(VALUE klass, VALUE points, VALUE which_image, VALUE fundamental_matrix);
  
// HighGUI function
VALUE rb_save_image(VALUE self, VALUE filename);

VALUE new_object(int rows, int cols, int type);
VALUE new_object(CvSize size, int type);

__NAMESPACE_END_CVMAT

inline CvMat*
CVMAT(VALUE object)
{
  CvMat *ptr, stub;
  Data_Get_Struct(object, CvMat, ptr);
  return cvGetMat(ptr, &stub);
}

inline CvMat*
MASK(VALUE object)
{
  if(NIL_P(object))
    return NULL;
  else if(rb_obj_is_kind_of(object, cCvMat::rb_class()) &&
	  CV_MAT_DEPTH(CVMAT(object)->type) == CV_8UC1 &&
	  CV_MAT_CN(CVMAT(object)->type) == 1)
    return CVMAT(object);
  else
    rb_raise(rb_eTypeError, "object is not mask.");
}

__NAMESPACE_END_OPENCV

#endif // RUBY_OPENCV_CVMAT_H
