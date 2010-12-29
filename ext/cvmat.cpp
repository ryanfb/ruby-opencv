/************************************************************

   cvmat.cpp -

   $Author: lsxi $

   Copyright (C) 2005-2008 Masakazu Yonekura

************************************************************/
#include "cvmat.h"
/*
 * Document-class: OpenCV::CvMat
 *
 * CvMat is basic 2D matrix class in OpenCV.
 * 
 * C structure is here.
 *  typedef struct CvMat{
 *    int type;
 *    int step;
 *    int *refcount;
 *    union
 *    {
 *      uchar  *ptr;
 *      short  *s;
 *      int    *i;
 *      float  *fl;
 *      double *db;
 *    } data;
 *  #ifdef __cplusplus
 *    union
 *    {
 *      int rows;
 *      int height;    
 *    };
 *    union
 *    {
 *      int cols;
 *      int width;
 *    };
 *  #else
 *    int rows; // number of row
 *    int cols; // number of columns
 *  }CvMat;
 */
__NAMESPACE_BEGIN_OPENCV
__NAMESPACE_BEGIN_CVMAT

#define SUPPORT_8UC1_ONLY(value) if (cvGetElemType(CVARR(value)) != CV_8UC1) {rb_raise(rb_eNotImpError, "support single-channel 8bit unsigned image only.");}
#define SUPPORT_8U_ONLY(value) if (CV_MAT_DEPTH(cvGetElemType(CVARR(value))) != CV_8U) {rb_raise(rb_eNotImpError, "support 8bit unsigned image only.");}
#define SUPPORT_C1_ONLY(value) if (CV_MAT_CN(cvGetElemType(CVARR(value))) != 1) {rb_raise(rb_eNotImpError, "support single-channel image only.");}
#define SUPPORT_C1C3_ONLY(value) if (CV_MAT_CN(cvGetElemType(CVARR(value))) != 1 && CV_MAT_CN(cvGetElemType(CVARR(value))) != 3) {rb_raise(rb_eNotImpError, "support single-channel or 3-channnel image only.");}

#define DRAWING_OPTION(op) NIL_P(op) ? rb_const_get(rb_class(), rb_intern("DRAWING_OPTION")) : rb_funcall(rb_const_get(rb_class(), rb_intern("DRAWING_OPTION")), rb_intern("merge"), 1, op)
#define DO_COLOR(op) VALUE_TO_CVSCALAR(rb_hash_aref(op, ID2SYM(rb_intern("color"))))
#define DO_THICKNESS(op) FIX2INT(rb_hash_aref(op, ID2SYM(rb_intern("thickness"))))
#define DO_LINE_TYPE(op) FIX2INT(rb_hash_aref(op, ID2SYM(rb_intern("line_type"))) == ID2SYM("aa") ? INT2FIX(CV_AA) : rb_hash_aref(op, ID2SYM(rb_intern("line_type"))))
#define DO_SHIFT(op) FIX2INT(rb_hash_aref(op, ID2SYM(rb_intern("shift"))))
#define DO_IS_CLOSED(op) ({VALUE _is_closed = rb_hash_aref(op, ID2SYM(rb_intern("is_closed"))); NIL_P(_is_closed) ? 0 : _is_closed == Qfalse ? 0 : 1;})

#define GOOD_FEATURES_TO_TRACK_OPTION(op) NIL_P(op) ? rb_const_get(rb_class(), rb_intern("GOOD_FEATURES_TO_TRACK_OPTION")) : rb_funcall(rb_const_get(rb_class(), rb_intern("GOOD_FEATURES_TO_TRACK_OPTION")), rb_intern("merge"), 1, op)
#define GF_MAX(op) FIX2INT(rb_hash_aref(op, ID2SYM(rb_intern("max"))))
#define GF_MASK(op) MASK(rb_hash_aref(op, ID2SYM(rb_intern("mask"))))
#define GF_BLOCK_SIZE(op) FIX2INT(rb_hash_aref(op, ID2SYM(rb_intern("block_size"))))
#define GF_USE_HARRIS(op) TRUE_OR_FALSE(rb_hash_aref(op, ID2SYM(rb_intern("use_harris"))), 0)
#define GF_K(op) NUM2DBL(rb_hash_aref(op, ID2SYM(rb_intern("k"))))

#define FLOOD_FILL_OPTION(op) NIL_P(op) ? rb_const_get(rb_class(), rb_intern("FLOOD_FILL_OPTION")) : rb_funcall(rb_const_get(rb_class(), rb_intern("FLOOD_FILL_OPTION")), rb_intern("merge"), 1, op)
#define FF_CONNECTIVITY(op) NUM2INT(rb_hash_aref(op, ID2SYM(rb_intern("connectivity"))))
#define FF_FIXED_RANGE(op) TRUE_OR_FALSE(rb_hash_aref(op, ID2SYM(rb_intern("fixed_range"))), 0)
#define FF_MASK_ONLY(op) TRUE_OR_FALSE(rb_hash_aref(op, ID2SYM(rb_intern("mask_only"))), 0)

#define FIND_CONTOURS_OPTION(op) NIL_P(op) ? rb_const_get(rb_class(), rb_intern("FIND_CONTOURS_OPTION")) : rb_funcall(rb_const_get(rb_class(), rb_intern("FIND_CONTOURS_OPTION")), rb_intern("merge"), 1, op)
#define FC_MODE(op) FIX2INT(rb_hash_aref(op, ID2SYM(rb_intern("mode"))))
#define FC_METHOD(op) FIX2INT(rb_hash_aref(op, ID2SYM(rb_intern("method"))))
#define FC_OFFSET(op)VALUE_TO_CVPOINT(rb_hash_aref(op, ID2SYM(rb_intern("offset"))))

#define OPTICAL_FLOW_HS_OPTION(op) NIL_P(op) ? rb_const_get(rb_class(), rb_intern("OPTICAL_FLOW_HS_OPTION")) : rb_funcall(rb_const_get(rb_class(), rb_intern("OPTICAL_FLOW_HS_OPTION")), rb_intern("merge"), 1, op)
#define HS_LAMBDA(op) NUM2DBL(rb_hash_aref(op, ID2SYM(rb_intern("lambda"))))
#define HS_CRITERIA(op) VALUE_TO_CVTERMCRITERIA(rb_hash_aref(op, ID2SYM(rb_intern("criteria"))))

#define OPTICAL_FLOW_BM_OPTION(op) NIL_P(op) ? rb_const_get(rb_class(), rb_intern("OPTICAL_FLOW_BM_OPTION")) : rb_funcall(rb_const_get(rb_class(), rb_intern("OPTICAL_FLOW_BM_OPTION")), rb_intern("merge"), 1, op)
#define BM_BLOCK_SIZE(op) VALUE_TO_CVSIZE(rb_hash_aref(op, ID2SYM(rb_intern("block_size"))))
#define BM_SHIFT_SIZE(op) VALUE_TO_CVSIZE(rb_hash_aref(op, ID2SYM(rb_intern("shift_size"))))
#define BM_MAX_RANGE(op) VALUE_TO_CVSIZE(rb_hash_aref(op, ID2SYM(rb_intern("max_range"))))

#define FIND_FUNDAMENTAL_MAT_OPTION(op) NIL_P(op) ? rb_const_get(rb_class(), rb_intern("FIND_FUNDAMENTAL_MAT_OPTION")) : rb_funcall(rb_const_get(rb_class(), rb_intern("FIND_FUNDAMENTAL_MAT_OPTION")), rb_intern("merge"), 1, op)
#define FFM_WITH_STATUS(op) TRUE_OR_FALSE(rb_hash_aref(op, ID2SYM(rb_intern("with_status"))), 0)
#define FFM_MAXIMUM_DISTANCE(op) NUM2DBL(rb_hash_aref(op, ID2SYM(rb_intern("maximum_distance"))))
#define FFM_DESIRABLE_LEVEL(op) NUM2DBL(rb_hash_aref(op, ID2SYM(rb_intern("desirable_level"))))

VALUE rb_klass;

VALUE
rb_class()
{
  return rb_klass;
}

void define_ruby_class()
{
  if (rb_klass)
    return;
  /* 
   * opencv = rb_define_module("OpenCV");
   * 
   * note: this comment is used by rdoc.
   */
  VALUE opencv = rb_module_opencv();
  
  rb_klass = rb_define_class_under(opencv, "CvMat", rb_cObject);
  rb_define_alloc_func(rb_klass, rb_allocate);
  
  VALUE drawing_option = rb_hash_new();
  rb_define_const(rb_klass, "DRAWING_OPTION", drawing_option);
  rb_hash_aset(drawing_option, ID2SYM(rb_intern("color")), cCvScalar::new_object(cvScalarAll(0)));
  rb_hash_aset(drawing_option, ID2SYM(rb_intern("thickness")), INT2FIX(1));
  rb_hash_aset(drawing_option, ID2SYM(rb_intern("line_type")), INT2FIX(8));
  rb_hash_aset(drawing_option, ID2SYM(rb_intern("shift")), INT2FIX(0));

  VALUE good_features_to_track_option = rb_hash_new();
  rb_define_const(rb_klass, "GOOD_FEATURES_TO_TRACK_OPTION", good_features_to_track_option);
  rb_hash_aset(good_features_to_track_option, ID2SYM(rb_intern("max")), INT2FIX(0xFF));
  rb_hash_aset(good_features_to_track_option, ID2SYM(rb_intern("mask")), Qnil);
  rb_hash_aset(good_features_to_track_option, ID2SYM(rb_intern("block_size")), INT2FIX(3));
  rb_hash_aset(good_features_to_track_option, ID2SYM(rb_intern("use_harris")), Qfalse);
  rb_hash_aset(good_features_to_track_option, ID2SYM(rb_intern("k")), rb_float_new(0.04));

  VALUE flood_fill_option = rb_hash_new();
  rb_define_const(rb_klass, "FLOOD_FILL_OPTION", flood_fill_option);
  rb_hash_aset(flood_fill_option, ID2SYM(rb_intern("connectivity")), INT2FIX(4));
  rb_hash_aset(flood_fill_option, ID2SYM(rb_intern("fixed_range")), Qfalse);
  rb_hash_aset(flood_fill_option, ID2SYM(rb_intern("mask_only")), Qfalse);

  VALUE find_contours_option = rb_hash_new();
  rb_define_const(rb_klass, "FIND_CONTOURS_OPTION", find_contours_option);
  rb_hash_aset(find_contours_option, ID2SYM(rb_intern("mode")), INT2FIX(CV_RETR_LIST));
  rb_hash_aset(find_contours_option, ID2SYM(rb_intern("method")), INT2FIX(CV_CHAIN_APPROX_SIMPLE));
  rb_hash_aset(find_contours_option, ID2SYM(rb_intern("offset")), cCvPoint::new_object(cvPoint(0,0)));

  VALUE optical_flow_hs_option = rb_hash_new();
  rb_define_const(rb_klass, "OPTICAL_FLOW_HS_OPTION", optical_flow_hs_option);
  rb_hash_aset(optical_flow_hs_option, ID2SYM(rb_intern("lambda")), rb_float_new(0.0005));
  rb_hash_aset(optical_flow_hs_option, ID2SYM(rb_intern("criteria")), cCvTermCriteria::new_object(cvTermCriteria(CV_TERMCRIT_ITER+CV_TERMCRIT_EPS, 1, 0.001)));

  VALUE optical_flow_bm_option = rb_hash_new();
  rb_define_const(rb_klass, "OPTICAL_FLOW_BM_OPTION", optical_flow_bm_option);
  rb_hash_aset(optical_flow_bm_option, ID2SYM(rb_intern("block_size")), cCvSize::new_object(cvSize(4, 4)));
  rb_hash_aset(optical_flow_bm_option, ID2SYM(rb_intern("shift_size")), cCvSize::new_object(cvSize(1, 1)));
  rb_hash_aset(optical_flow_bm_option, ID2SYM(rb_intern("max_range")),  cCvSize::new_object(cvSize(4, 4)));
  
  VALUE find_fundamental_matrix_option = rb_hash_new();
  rb_define_const(rb_klass, "FIND_FUNDAMENTAL_MAT_OPTION", find_fundamental_matrix_option);
  rb_hash_aset(find_fundamental_matrix_option, ID2SYM(rb_intern("with_status")), Qfalse);
  rb_hash_aset(find_fundamental_matrix_option, ID2SYM(rb_intern("maximum_distance")), rb_float_new(1.0));
  rb_hash_aset(find_fundamental_matrix_option, ID2SYM(rb_intern("desirable_level")), rb_float_new(0.99));

  rb_define_method(rb_klass, "initialize", RUBY_METHOD_FUNC(rb_initialize), -1);
  // Ruby/OpenCV original functions
  rb_define_method(rb_klass, "method_missing", RUBY_METHOD_FUNC(rb_method_missing), -1);
  rb_define_method(rb_klass, "to_s", RUBY_METHOD_FUNC(rb_to_s), 0);
  rb_define_method(rb_klass, "has_parent?", RUBY_METHOD_FUNC(rb_has_parent_q), 0);
  rb_define_method(rb_klass, "parent", RUBY_METHOD_FUNC(rb_parent), 0);
  rb_define_method(rb_klass, "inside?", RUBY_METHOD_FUNC(rb_inside_q), 1);
  rb_define_method(rb_klass, "to_IplConvKernel", RUBY_METHOD_FUNC(rb_to_IplConvKernel), 1);
  rb_define_method(rb_klass, "create_mask", RUBY_METHOD_FUNC(rb_create_mask), 0);

  rb_define_method(rb_klass, "width", RUBY_METHOD_FUNC(rb_width), 0);
  rb_define_alias(rb_klass, "columns", "width");
  rb_define_alias(rb_klass, "cols", "width");
  rb_define_method(rb_klass, "height", RUBY_METHOD_FUNC(rb_height), 0);
  rb_define_alias(rb_klass, "rows", "height");
  rb_define_method(rb_klass, "depth", RUBY_METHOD_FUNC(rb_depth), 0);
  rb_define_method(rb_klass, "channel", RUBY_METHOD_FUNC(rb_channel), 0);
  rb_define_method(rb_klass, "data", RUBY_METHOD_FUNC(rb_data), 0);

  rb_define_method(rb_klass, "clone", RUBY_METHOD_FUNC(rb_clone), 0);
  rb_define_method(rb_klass, "copy", RUBY_METHOD_FUNC(rb_copy), -1);
  rb_define_method(rb_klass, "to_8u", RUBY_METHOD_FUNC(rb_to_8u), 0);      
  rb_define_method(rb_klass, "to_8s", RUBY_METHOD_FUNC(rb_to_8s), 0);
  rb_define_method(rb_klass, "to_16u", RUBY_METHOD_FUNC(rb_to_16u), 0);
  rb_define_method(rb_klass, "to_16s", RUBY_METHOD_FUNC(rb_to_16s), 0);
  rb_define_method(rb_klass, "to_32s", RUBY_METHOD_FUNC(rb_to_32s), 0);
  rb_define_method(rb_klass, "to_32f", RUBY_METHOD_FUNC(rb_to_32f), 0);
  rb_define_method(rb_klass, "to_64f", RUBY_METHOD_FUNC(rb_to_64f), 0);
  rb_define_method(rb_klass, "vector?", RUBY_METHOD_FUNC(rb_vector_q), 0);
  rb_define_method(rb_klass, "square?", RUBY_METHOD_FUNC(rb_square_q), 0);
  
  rb_define_method(rb_klass, "to_CvMat", RUBY_METHOD_FUNC(rb_to_CvMat), 0);
  rb_define_method(rb_klass, "sub_rect", RUBY_METHOD_FUNC(rb_sub_rect), -2);
  rb_define_alias(rb_klass, "subrect", "sub_rect");
  rb_define_method(rb_klass, "slice_width", RUBY_METHOD_FUNC(rb_slice_width), 1);
  rb_define_method(rb_klass, "slice_height", RUBY_METHOD_FUNC(rb_slice_height), 1);
  rb_define_method(rb_klass, "row", RUBY_METHOD_FUNC(rb_row), -2);
  rb_define_method(rb_klass, "col", RUBY_METHOD_FUNC(rb_col), -2);
  rb_define_alias(rb_klass, "column", "col");
  rb_define_method(rb_klass, "each_row", RUBY_METHOD_FUNC(rb_each_row), 0);
  rb_define_method(rb_klass, "each_col", RUBY_METHOD_FUNC(rb_each_col), 0);
  rb_define_alias(rb_klass, "each_column", "each_col");
  rb_define_method(rb_klass, "diag", RUBY_METHOD_FUNC(rb_diag), -1);
  rb_define_alias(rb_klass, "diagonal", "diag");
  rb_define_method(rb_klass, "size", RUBY_METHOD_FUNC(rb_size), 0);
  rb_define_method(rb_klass, "dims", RUBY_METHOD_FUNC(rb_dims), 0);
  rb_define_method(rb_klass, "dim_size", RUBY_METHOD_FUNC(rb_dim_size), 1);
  rb_define_method(rb_klass, "[]", RUBY_METHOD_FUNC(rb_aref), -2);
  rb_define_alias(rb_klass, "at", "[]");
  rb_define_method(rb_klass, "[]=", RUBY_METHOD_FUNC(rb_aset), -2);
  rb_define_method(rb_klass, "fill", RUBY_METHOD_FUNC(rb_fill), -1);
  rb_define_alias(rb_klass, "set", "fill");
  rb_define_method(rb_klass, "fill!", RUBY_METHOD_FUNC(rb_fill_bang), -1);
  rb_define_alias(rb_klass, "set!", "fill!");
  rb_define_method(rb_klass, "clear", RUBY_METHOD_FUNC(rb_clear), 0);
  rb_define_alias(rb_klass, "set_zero", "clear");
  rb_define_method(rb_klass, "clear!", RUBY_METHOD_FUNC(rb_clear_bang), 0);
  rb_define_alias(rb_klass, "set_zero!", "clear!");
  rb_define_method(rb_klass, "identity", RUBY_METHOD_FUNC(rb_set_identity), -1);
  rb_define_method(rb_klass, "identity!", RUBY_METHOD_FUNC(rb_set_identity_bang), -1);
  rb_define_method(rb_klass, "range", RUBY_METHOD_FUNC(rb_range), -1);
  rb_define_method(rb_klass, "range!", RUBY_METHOD_FUNC(rb_range_bang), -1);
  
  rb_define_method(rb_klass, "reshape", RUBY_METHOD_FUNC(rb_reshape), 1);
  rb_define_method(rb_klass, "repeat", RUBY_METHOD_FUNC(rb_repeat), 1);
  rb_define_method(rb_klass, "flip", RUBY_METHOD_FUNC(rb_flip), -1);
  rb_define_method(rb_klass, "flip!", RUBY_METHOD_FUNC(rb_flip_bang), -1);
  rb_define_method(rb_klass, "split", RUBY_METHOD_FUNC(rb_split), 0);
  rb_define_singleton_method(rb_klass, "merge", RUBY_METHOD_FUNC(rb_merge), -2);
  rb_define_method(rb_klass, "rand_shuffle", RUBY_METHOD_FUNC(rb_rand_shuffle), -1);
  rb_define_method(rb_klass, "rand_shuffle!", RUBY_METHOD_FUNC(rb_rand_shuffle_bang), -1);
  rb_define_method(rb_klass, "lut", RUBY_METHOD_FUNC(rb_lut), 1);
  rb_define_method(rb_klass, "convert_scale", RUBY_METHOD_FUNC(rb_convert_scale), 1);
  rb_define_method(rb_klass, "convert_scale_abs", RUBY_METHOD_FUNC(rb_convert_scale_abs), 1);
  rb_define_method(rb_klass, "add", RUBY_METHOD_FUNC(rb_add), -1);
  rb_define_alias(rb_klass, "+", "add");
  rb_define_method(rb_klass, "sub", RUBY_METHOD_FUNC(rb_sub), -1);
  rb_define_alias(rb_klass, "-", "sub");
  rb_define_method(rb_klass, "mul", RUBY_METHOD_FUNC(rb_mul), -1);
  rb_define_alias(rb_klass, "*", "mul");
  rb_define_method(rb_klass, "div", RUBY_METHOD_FUNC(rb_div), -1);
  rb_define_alias(rb_klass, "/", "div");
  rb_define_method(rb_klass, "and", RUBY_METHOD_FUNC(rb_and), -1);
  rb_define_alias(rb_klass, "&", "and");
  rb_define_method(rb_klass, "or", RUBY_METHOD_FUNC(rb_or), -1);
  rb_define_alias(rb_klass, "|", "or");
  rb_define_method(rb_klass, "xor", RUBY_METHOD_FUNC(rb_xor), -1);
  rb_define_alias(rb_klass, "^", "xor");
  rb_define_method(rb_klass, "not", RUBY_METHOD_FUNC(rb_not), 0);
  rb_define_method(rb_klass, "not!", RUBY_METHOD_FUNC(rb_not_bang), 0);
  rb_define_method(rb_klass, "eq", RUBY_METHOD_FUNC(rb_eq), 1);
  rb_define_alias(rb_klass, "==", "eq");
  rb_define_method(rb_klass, "gt", RUBY_METHOD_FUNC(rb_gt), 1);
  rb_define_alias(rb_klass, ">", "gt");
  rb_define_method(rb_klass, "ge", RUBY_METHOD_FUNC(rb_ge), 1);
  rb_define_alias(rb_klass, ">=", "ge");
  rb_define_method(rb_klass, "lt", RUBY_METHOD_FUNC(rb_lt), 1);
  rb_define_alias(rb_klass, "<", "lt");
  rb_define_method(rb_klass, "le", RUBY_METHOD_FUNC(rb_le), 1);
  rb_define_alias(rb_klass, "<=", "le");
  rb_define_method(rb_klass, "ne", RUBY_METHOD_FUNC(rb_ne), 1);
  rb_define_alias(rb_klass, "!=", "ne");
  rb_define_method(rb_klass, "in_range", RUBY_METHOD_FUNC(rb_in_range), 2);
  rb_define_method(rb_klass, "abs_diff", RUBY_METHOD_FUNC(rb_abs_diff), 1);
  rb_define_method(rb_klass, "count_non_zero", RUBY_METHOD_FUNC(rb_count_non_zero), 0);
  rb_define_method(rb_klass, "sum", RUBY_METHOD_FUNC(rb_sum), 0);
  rb_define_method(rb_klass, "avg", RUBY_METHOD_FUNC(rb_avg), -1);
  rb_define_method(rb_klass, "avg_sdv", RUBY_METHOD_FUNC(rb_avg_sdv), -1);
  rb_define_method(rb_klass, "sdv", RUBY_METHOD_FUNC(rb_sdv), -1);
  rb_define_method(rb_klass, "min_max_loc", RUBY_METHOD_FUNC(rb_min_max_loc), -1);
  rb_define_method(rb_klass, "dot_product", RUBY_METHOD_FUNC(rb_dot_product), 1);
  rb_define_method(rb_klass, "cross_product", RUBY_METHOD_FUNC(rb_cross_product), 1);
  rb_define_method(rb_klass, "transform", RUBY_METHOD_FUNC(rb_transform), -1);
  rb_define_method(rb_klass, "perspective_transform", RUBY_METHOD_FUNC(rb_perspective_transform), 1);
  rb_define_method(rb_klass, "mul_transposed", RUBY_METHOD_FUNC(rb_mul_transposed), -2);
  rb_define_method(rb_klass, "trace", RUBY_METHOD_FUNC(rb_trace), 0);
  rb_define_method(rb_klass, "transpose", RUBY_METHOD_FUNC(rb_transpose), 0);
  rb_define_alias(rb_klass, "t", "transpose");
  rb_define_method(rb_klass, "transpose!", RUBY_METHOD_FUNC(rb_transpose_bang), 0);
  rb_define_alias(rb_klass, "t!", "transpose!");
  rb_define_method(rb_klass, "det", RUBY_METHOD_FUNC(rb_det), 0);
  rb_define_alias(rb_klass, "determinant", "det");
  rb_define_method(rb_klass, "invert", RUBY_METHOD_FUNC(rb_invert), -1);
  rb_define_method(rb_klass, "solve", RUBY_METHOD_FUNC(rb_solve), -1);
  rb_define_method(rb_klass, "svd", RUBY_METHOD_FUNC(rb_svd), -1);
  rb_define_method(rb_klass, "svbksb", RUBY_METHOD_FUNC(rb_svbksb), -1);
  rb_define_method(rb_klass, "eigenvv", RUBY_METHOD_FUNC(rb_eigenvv), -1);
  rb_define_method(rb_klass, "eigenvv!", RUBY_METHOD_FUNC(rb_eigenvv_bang), -1);
  rb_define_method(rb_klass, "calc_covar_matrix", RUBY_METHOD_FUNC(rb_calc_covar_matrix), -1);
  rb_define_method(rb_klass, "mahalonobis", RUBY_METHOD_FUNC(rb_mahalonobis), -1);
  
  /* drawing function */
  rb_define_method(rb_klass, "line", RUBY_METHOD_FUNC(rb_line), -1);
  rb_define_method(rb_klass, "line!", RUBY_METHOD_FUNC(rb_line_bang), -1);
  rb_define_method(rb_klass, "rectangle", RUBY_METHOD_FUNC(rb_rectangle), -1);
  rb_define_method(rb_klass, "rectangle!", RUBY_METHOD_FUNC(rb_rectangle_bang), -1);
  rb_define_method(rb_klass, "circle", RUBY_METHOD_FUNC(rb_circle), -1);
  rb_define_method(rb_klass, "circle!", RUBY_METHOD_FUNC(rb_circle_bang), -1);
  rb_define_method(rb_klass, "ellipse", RUBY_METHOD_FUNC(rb_ellipse), -1);
  rb_define_method(rb_klass, "ellipse!", RUBY_METHOD_FUNC(rb_ellipse_bang), -1);
  rb_define_method(rb_klass, "ellipse_box", RUBY_METHOD_FUNC(rb_ellipse_box), -1);
  rb_define_method(rb_klass, "ellipse_box!", RUBY_METHOD_FUNC(rb_ellipse_box_bang), -1);
  rb_define_method(rb_klass, "fill_poly", RUBY_METHOD_FUNC(rb_fill_poly), -1);
  rb_define_method(rb_klass, "fill_poly!", RUBY_METHOD_FUNC(rb_fill_poly_bang), -1);
  rb_define_method(rb_klass, "fill_convex_poly", RUBY_METHOD_FUNC(rb_fill_convex_poly), -1);
  rb_define_method(rb_klass, "fill_convex_poly!", RUBY_METHOD_FUNC(rb_fill_convex_poly_bang), -1);
  rb_define_method(rb_klass, "poly_line", RUBY_METHOD_FUNC(rb_poly_line), -1);
  rb_define_method(rb_klass, "poly_line!", RUBY_METHOD_FUNC(rb_poly_line_bang), -1);
  rb_define_method(rb_klass, "put_text", RUBY_METHOD_FUNC(rb_put_text), -1);
  rb_define_method(rb_klass, "put_text!", RUBY_METHOD_FUNC(rb_put_text_bang), -1);

  rb_define_method(rb_klass, "dft", RUBY_METHOD_FUNC(rb_dft), -1);
  rb_define_method(rb_klass, "dct", RUBY_METHOD_FUNC(rb_dct), -1);
  
  rb_define_method(rb_klass, "sobel", RUBY_METHOD_FUNC(rb_sobel), -1);
  rb_define_method(rb_klass, "laplace", RUBY_METHOD_FUNC(rb_laplace), -1);
  rb_define_method(rb_klass, "canny", RUBY_METHOD_FUNC(rb_canny), -1);
  rb_define_method(rb_klass, "pre_corner_detect", RUBY_METHOD_FUNC(rb_pre_corner_detect), -1);
  rb_define_method(rb_klass, "corner_eigenvv", RUBY_METHOD_FUNC(rb_corner_eigenvv), -1);
  rb_define_method(rb_klass, "corner_min_eigen_val", RUBY_METHOD_FUNC(rb_corner_min_eigen_val), -1);
  rb_define_method(rb_klass, "corner_harris", RUBY_METHOD_FUNC(rb_corner_harris), -1);
  rb_define_private_method(rb_klass, "__find_corner_sub_pix", RUBY_METHOD_FUNC(rbi_find_corner_sub_pix), -1);
  rb_define_method(rb_klass, "good_features_to_track", RUBY_METHOD_FUNC(rb_good_features_to_track), -1);

  rb_define_method(rb_klass, "sample_line", RUBY_METHOD_FUNC(rb_sample_line), 2);
  rb_define_method(rb_klass, "rect_sub_pix", RUBY_METHOD_FUNC(rb_rect_sub_pix), 2);
  rb_define_method(rb_klass, "quadrangle_sub_pix", RUBY_METHOD_FUNC(rb_quadrangle_sub_pix), 2);
  rb_define_method(rb_klass, "resize", RUBY_METHOD_FUNC(rb_resize), -1);
  rb_define_method(rb_klass, "warp_affine", RUBY_METHOD_FUNC(rb_warp_affine), -1);
  rb_define_singleton_method(rb_klass, "rotation", RUBY_METHOD_FUNC(rb_rotation), 3);
  rb_define_method(rb_klass, "warp_perspective", RUBY_METHOD_FUNC(rb_warp_perspective), -1);
  //rb_define_method(rb_klass, "get_perspective_transform", RUBY_METHOD_FUNC(rb_get_perspective_transform), -1);
  //rb_define_alias(rb_klass, "warp_perspective_q_matrix", "get_perspective_transform");
  rb_define_method(rb_klass, "remap", RUBY_METHOD_FUNC(rb_remap), -1);
  //rb_define_method(rb_klass, "log_polar", RUBY_METHOD_FUNC(rb_log_polar), -1);
  
  rb_define_method(rb_klass, "erode", RUBY_METHOD_FUNC(rb_erode), -1);
  rb_define_method(rb_klass, "erode!", RUBY_METHOD_FUNC(rb_erode_bang), -1);
  rb_define_method(rb_klass, "dilate", RUBY_METHOD_FUNC(rb_dilate), -1);
  rb_define_method(rb_klass, "dilate!", RUBY_METHOD_FUNC(rb_dilate_bang), -1);
  rb_define_method(rb_klass, "morphology_open", RUBY_METHOD_FUNC(rb_morphology_open), -1);
  rb_define_method(rb_klass, "morphology_close", RUBY_METHOD_FUNC(rb_morphology_close), -1);
  rb_define_method(rb_klass, "morphology_gradient", RUBY_METHOD_FUNC(rb_morphology_gradient), -1);
  rb_define_method(rb_klass, "morphology_tophat", RUBY_METHOD_FUNC(rb_morphology_tophat), -1);
  rb_define_method(rb_klass, "morphology_blackhat", RUBY_METHOD_FUNC(rb_morphology_blackhat), -1);
  
  rb_define_method(rb_klass, "smooth_blur_no_scale", RUBY_METHOD_FUNC(rb_smooth_blur_no_scale), -1);
  rb_define_method(rb_klass, "smooth_blur", RUBY_METHOD_FUNC(rb_smooth_blur), -1);
  rb_define_method(rb_klass, "smooth_gaussian", RUBY_METHOD_FUNC(rb_smooth_gaussian), -1);
  rb_define_method(rb_klass, "smooth_median", RUBY_METHOD_FUNC(rb_smooth_median), -1);
  rb_define_method(rb_klass, "smooth_bilateral", RUBY_METHOD_FUNC(rb_smooth_bilateral), -1);
  rb_define_method(rb_klass, "filter2d", RUBY_METHOD_FUNC(rb_filter2d), -1);
  rb_define_method(rb_klass, "copy_make_border_constant", RUBY_METHOD_FUNC(rb_copy_make_border_constant), -1);
  rb_define_method(rb_klass, "copy_make_border_replicate", RUBY_METHOD_FUNC(rb_copy_make_border_replicate), -1);
  rb_define_method(rb_klass, "integral", RUBY_METHOD_FUNC(rb_integral), -1);
  rb_define_method(rb_klass, "threshold_binary", RUBY_METHOD_FUNC(rb_threshold_binary), -1);
  rb_define_method(rb_klass, "threshold_binary_inverse", RUBY_METHOD_FUNC(rb_threshold_binary_inverse), -1);
  rb_define_method(rb_klass, "threshold_trunc", RUBY_METHOD_FUNC(rb_threshold_trunc), -1);
  rb_define_method(rb_klass, "threshold_to_zero", RUBY_METHOD_FUNC(rb_threshold_to_zero), -1);
  rb_define_method(rb_klass, "threshold_to_zero_inverse", RUBY_METHOD_FUNC(rb_threshold_to_zero_inverse), -1);

  rb_define_method(rb_klass, "pyr_down", RUBY_METHOD_FUNC(rb_pyr_down), -1);
  rb_define_method(rb_klass, "pyr_up", RUBY_METHOD_FUNC(rb_pyr_up), -1);

  rb_define_method(rb_klass, "flood_fill", RUBY_METHOD_FUNC(rb_flood_fill), -1);
  rb_define_method(rb_klass, "flood_fill!", RUBY_METHOD_FUNC(rb_flood_fill_bang), -1);
  rb_define_method(rb_klass, "find_contours", RUBY_METHOD_FUNC(rb_find_contours), -1);
  rb_define_method(rb_klass, "find_contours!", RUBY_METHOD_FUNC(rb_find_contours_bang), -1);
  rb_define_method(rb_klass, "pyr_segmentation", RUBY_METHOD_FUNC(rb_pyr_segmentation), -1);
  rb_define_method(rb_klass, "pyr_mean_shift_filtering", RUBY_METHOD_FUNC(rb_pyr_mean_shift_filtering), -1);
  rb_define_method(rb_klass, "watershed", RUBY_METHOD_FUNC(rb_watershed), 0);

  rb_define_method(rb_klass, "moments", RUBY_METHOD_FUNC(rb_moments), -1);

  rb_define_method(rb_klass, "hough_lines_standard", RUBY_METHOD_FUNC(rb_hough_lines_standard), -1);
  rb_define_method(rb_klass, "hough_lines_probabilistic", RUBY_METHOD_FUNC(rb_hough_lines_probabilistic), -1);
  rb_define_method(rb_klass, "hough_lines_multi_scale", RUBY_METHOD_FUNC(rb_hough_lines_multi_scale), -1);
  rb_define_method(rb_klass, "hough_circles_gradient", RUBY_METHOD_FUNC(rb_hough_circles_gradient), -1);
  //rb_define_method(rb_klass, "dist_transform", RUBY_METHOD_FUNC(rb_dist_transform), -1);
  
  rb_define_method(rb_klass, "inpaint_ns", RUBY_METHOD_FUNC(rb_inpaint_ns), 2);
  rb_define_method(rb_klass, "inpaint_telea", RUBY_METHOD_FUNC(rb_inpaint_telea), 2);

  rb_define_method(rb_klass, "equalize_hist", RUBY_METHOD_FUNC(rb_equalize_hist), 0);
  rb_define_method(rb_klass, "match_template", RUBY_METHOD_FUNC(rb_match_template), -1);
  rb_define_method(rb_klass, "match_shapes_i1", RUBY_METHOD_FUNC(rb_match_shapes_i1), -1);
  rb_define_method(rb_klass, "match_shapes_i2", RUBY_METHOD_FUNC(rb_match_shapes_i2), -1);
  rb_define_method(rb_klass, "match_shapes_i3", RUBY_METHOD_FUNC(rb_match_shapes_i3), -1);
  
  rb_define_method(rb_klass, "mean_shift", RUBY_METHOD_FUNC(rb_mean_shift), 2);
  rb_define_method(rb_klass, "cam_shift", RUBY_METHOD_FUNC(rb_cam_shift), 2);
  rb_define_method(rb_klass, "snake_image", RUBY_METHOD_FUNC(rb_snake_image), -1);
  
  rb_define_method(rb_klass, "optical_flow_hs", RUBY_METHOD_FUNC(rb_optical_flow_hs), -1);
  rb_define_method(rb_klass, "optical_flow_lk", RUBY_METHOD_FUNC(rb_optical_flow_lk), -1);
  rb_define_method(rb_klass, "optical_flow_bm", RUBY_METHOD_FUNC(rb_optical_flow_bm), -1);
  
  rb_define_singleton_method(rb_klass, "find_fundamental_mat_7point", RUBY_METHOD_FUNC(rb_find_fundamental_mat_7point), -1);
  rb_define_singleton_method(rb_klass, "find_fundamental_mat_8point", RUBY_METHOD_FUNC(rb_find_fundamental_mat_8point), -1);
  rb_define_singleton_method(rb_klass, "find_fundamental_mat_ransac", RUBY_METHOD_FUNC(rb_find_fundamental_mat_ransac), -1);
  rb_define_singleton_method(rb_klass, "find_fundamental_mat_lmeds", RUBY_METHOD_FUNC(rb_find_fundamental_mat_lmeds), -1);
  rb_define_singleton_method(rb_klass, "compute_correspond_epilines", RUBY_METHOD_FUNC(rb_compute_correspond_epilines), 3);

  rb_define_method(rb_klass, "save_image", RUBY_METHOD_FUNC(rb_save_image), 1);
}

VALUE
rb_allocate(VALUE klass)
{
  return OPENCV_OBJECT(klass, 0);
}

/* 
 * call-seq:
 *   CvMat.new(<i>row, col[, depth = CV_8U][, channel = 3]</i>) -> cvmat
 *
 * Create col * row matrix. Each element set 0.
 *
 * Each element possigle range is set by <i>depth</i>. Default is unsigned 8bit.
 *
 * Number of channel is set by <i>channel</i>. <i>channel</i> should be 1..4.
 *
 */
VALUE
rb_initialize(int argc, VALUE *argv, VALUE self)
{
  VALUE row, column, depth, channel;
  rb_scan_args(argc, argv, "22", &row, &column, &depth, &channel);
  DATA_PTR(self) = cvCreateMat(FIX2INT(row), FIX2INT(column),
                               CV_MAKETYPE(CVMETHOD("DEPTH", depth, CV_8U), argc < 4 ? 3 : FIX2INT(channel)));
  return self;
}

/* 
 * nodoc
 */
VALUE
rb_method_missing(int argc, VALUE *argv, VALUE self)
{
  /*
  const char *to_str = "\\Ato_(\\w+)";
  VALUE name, args, str[3], method;
  rb_scan_args(argc, argv, "1*", &name, &args);
  if (RARRAY_LEN(args) != 0)
    return rb_call_super(argc, argv);
  if(rb_reg_match(rb_reg_new(to_str, strlen(to_str), 0), rb_funcall(name, rb_intern("to_s"), 0)) == Qnil)
    return rb_call_super(argc, argv);
  str[0] = rb_str_new2("%s2%s");
  str[1] = rb_color_model(self);
  str[2] = rb_reg_nth_match(1, rb_backref_get());
  method = rb_f_sprintf(3, str);
  if (rb_respond_to(rb_module_opencv(), rb_intern(StringValuePtr(method))))
    return rb_funcall(rb_module_opencv(), rb_intern(StringValuePtr(method)), 1, self);
  return rb_call_super(argc, argv);
  */
  VALUE name, args, method;
  rb_scan_args(argc, argv, "1*", &name, &args);
  method = rb_funcall(name, rb_intern("to_s"), 0);
  if (RARRAY_LEN(args) != 0 || !rb_respond_to(rb_module_opencv(), rb_intern(StringValuePtr(method))))
    return rb_call_super(argc, argv);
  return rb_funcall(rb_module_opencv(), rb_intern(StringValuePtr(method)), 1, self);
}

/*
 * call-seq:
 *   to_s -> string
 *
 * Return following string.
 *   m = CvMat.new(100, 100, :cv8u, 3)
 *   m.to_s # => <CvMat:100x100,depth=cv8u,channel=3>
 */
VALUE
rb_to_s(VALUE self)
{
  const int i = 6;
  VALUE str[i];
  str[0] = rb_str_new2("<%s:%dx%d,depth=%s,channel=%d>");
  str[1] = rb_str_new2(rb_class2name(CLASS_OF(self)));
  str[2] = rb_width(self);
  str[3] = rb_height(self);
  str[4] = rb_depth(self);
  str[5] = rb_channel(self);
  return rb_f_sprintf(i, str);
}

/*
 * call-seq:
 *   has_parent? -> true or false
 *
 * Return <tt>true</tt> if this matrix has parent object, otherwise <tt>false</tt>.
 */
VALUE
rb_has_parent_q(VALUE self)
{
  return lookup_root_object(CVMAT(self)) ? Qtrue : Qfalse;  
}

/*
 * call-seq:
 *   parent -> obj or nil
 *
 * Return root object that refer this object.
 */
VALUE
rb_parent(VALUE self)
{
  VALUE root = lookup_root_object(CVMAT(self));
  return root ? root : Qnil;
}

/*
 * call-seq:
 *   inside?(obj) -> true or false
 *
 * 
 */
VALUE 
rb_inside_q(VALUE self, VALUE object)
{
  if (cCvPoint::rb_compatible_q(cCvPoint::rb_class(), object)) {
    CvMat *mat = CVMAT(self);
    int x = NUM2INT(rb_funcall(object, rb_intern("x"), 0));
    int y = NUM2INT(rb_funcall(object, rb_intern("y"), 0));
    if (cCvRect::rb_compatible_q(cCvRect::rb_class(), object)) {
      int width = NUM2INT(rb_funcall(object, rb_intern("width"), 0));
      int height = NUM2INT(rb_funcall(object, rb_intern("height"), 0));
      return x >= 0 && y >= 0 && x < mat->width && x + width < mat->width && y < mat->height && y + height < mat->height ? Qtrue : Qfalse;
    } else {      
      return x >= 0 && y >= 0 && x < mat->width && y < mat->height ? Qtrue : Qfalse; 
    }
  }
  rb_raise(rb_eArgError, "argument 1 should have method \"x\", \"y\"");
}

/*
 * call-seq:
 *    to_IplConvKernel -> iplconvkernel
 *
 * Create IplConvKernel from this matrix.
 */
VALUE
rb_to_IplConvKernel(VALUE self, VALUE anchor)
{
  CvMat *src = CVMAT(self);
  CvPoint p = VALUE_TO_CVPOINT(anchor);
  IplConvKernel *kernel = cvCreateStructuringElementEx(src->cols, src->rows, p.x, p.y, CV_SHAPE_CUSTOM, src->data.i);
  return DEPEND_OBJECT(cIplConvKernel::rb_class(), kernel, self);
}

/*
 * call-seq:
 *   create_mask -> cvmat(single-channel 8bit unsinged image)
 * 
 * Create single-channel 8bit unsinged image that filled 0.
 */
VALUE
rb_create_mask(VALUE self)
{
  VALUE mask = cCvMat::new_object(cvGetSize(CVARR(self)), CV_8UC1);
  cvZero(CVARR(self));
  return mask;
}
    
/*
 * call-seq:
 *   width -> int
 *
 * Return number of columns.
 */
VALUE
rb_width(VALUE self)
{
  return INT2FIX(CVMAT(self)->width);
}
    
/*
 * call-seq:
 *   height -> int
 *
 * Return number of rows.
 */
VALUE
rb_height(VALUE self)
{
  return INT2FIX(CVMAT(self)->height);
}

/*
 * call-seq:
 *   depth -> symbol
 *
 * Return depth symbol. (see OpenCV::DEPTH)
 */
VALUE
rb_depth(VALUE self)
{
  return rb_hash_aref(rb_funcall(rb_const_get(rb_module_opencv(), rb_intern("DEPTH")), rb_intern("invert"), 0), INT2FIX(CV_MAT_DEPTH(CVMAT(self)->type)));
}

/*
 * call-seq:
 *   channel -> int (1 < channel < 4)
 *
 * Return number of channel.
 */
VALUE
rb_channel(VALUE self)  
{
  return INT2FIX(CV_MAT_CN(CVMAT(self)->type));
}

/*
 * call-seq:
 *   data -> binary (by String class)
 *
 * Return raw data of matrix.
 */
VALUE
rb_data(VALUE self)
{
  IplImage *image = IPLIMAGE(self);
  return rb_str_new((char *)image->imageData, image->imageSize);
}

/*
 * call-seq:
 *   clone -> cvmat
 *
 * Clone matrix. The parent and child relation is not succeeded.
 * Instance-specific method is succeeded.
 *
 *   module M
 *     def example
 *       true
 *     end
 *   end
 *
 *   mat.extend M
 *   mat.example   #=> true
 *   clone = mat.clone
 *   clone.example #=> true
 *   copy = mat.copy
 *   copy.example  #=> raise NoMethodError
 */
VALUE
rb_clone(VALUE self)
{
  VALUE clone = rb_obj_clone(self);
  DATA_PTR(clone) = cvClone(CVARR(self));
  return clone;
}

/*
 * call-seq:
 *   copy() -> cvmat
 *   copy(<i>mat</i>) -> mat
 *   copy(<i>val</i>) -> array(include cvmat)
 *
 * Copy matrix. The parent and child relation is not succeeded.
 * Instance-specific method is *NOT* succeeded. see also #clone.
 *
 * There are 3 kind behavior depending on the argument.
 *
 *   copy()
 *     Return one copied matrix.
 *   copy(mat)
 *     Copy own elements to target matrix. Return nil.
 *     Size (or ROI) and channel and depth should be match.
 *     If own width or height does not match target matrix, raise CvUnmatchedSizes
 *     If own channel or depth does not match target matrix, raise CvUnmatchedFormats
 *   copy(val)
 *     The amounts of the specified number are copied. Return Array with copies.
 *     If you give the 0 or negative value. Return nil.
 *       mat.copy(3)  #=> [mat1, mat2, mat3]
 *       mat.copy(-1) #=> nil
 *
 * When not apply to any, raise ArgumentError
 */
VALUE
rb_copy(int argc, VALUE *argv, VALUE self)
{
  VALUE value, copied;
  CvMat *src = CVMAT(self);
  rb_scan_args(argc, argv, "01", &value);
  if (argc == 0) {
    CvSize size = cvGetSize(src);
    copied = new_object(cvGetSize(src), cvGetElemType(src));
    cvCopy(src, CVMAT(copied));
    return copied;
  }else{
    if (rb_obj_is_kind_of(value, rb_klass)) {
      cvCopy(src, CVMAT(value));
      return Qnil;
    }else if (rb_obj_is_kind_of(value, rb_cFixnum)) {
      int n = FIX2INT(value);
      if (n > 0) {
        copied = rb_ary_new2(n);
        for (int i = 0; i < n; i++) {
          rb_ary_store(copied, i, new_object(src->rows, src->cols, cvGetElemType(src)));
        }
        return copied;
      }else{
        return Qnil;
      }
    }else
      rb_raise(rb_eArgError, "");
  }
}

VALUE
copy(VALUE mat)
{
  CvMat *src = CVMAT(mat);
  VALUE copied = new_object(cvGetSize(src), cvGetElemType(src));
  cvCopy(src, CVMAT(copied));
  return copied;
}


/*
 * call-seq:
 *   to_8u -> cvmat(depth = CV_8U)
 *
 * Return the new matrix that elements is converted to unsigned 8bit.
 */
VALUE
rb_to_8u(VALUE self)
{
  CvMat *src = CVMAT(self);
  VALUE dest = new_object(src->rows, src->cols, CV_MAKETYPE(CV_8U, CV_MAT_CN(src->type)));
  cvConvert(src, CVMAT(dest));
  return dest;
}

/*
 * call-seq:
 *   to_8s -> cvmat(depth = CV_8S)
 *
 * Return the new matrix that elements is converted to signed 8bit.
 */
VALUE
rb_to_8s(VALUE self)
{
  CvMat *src = CVMAT(self);
  VALUE dest = new_object(src->rows, src->cols, CV_MAKETYPE(CV_8S, CV_MAT_CN(src->type)));
  cvConvert(src, CVMAT(dest));
  return dest;
}

/*
 * call-seq:
 *   to_16u -> cvmat(depth = CV_16U)
 *
 * Return the new matrix that elements is converted to unsigned 16bit.
 */
VALUE rb_to_16u(VALUE self)
{
  CvMat *src = CVMAT(self);
  VALUE dest = new_object(src->rows, src->cols, CV_MAKETYPE(CV_16U, CV_MAT_CN(src->type)));
  cvConvert(src, CVMAT(dest));
  return dest;
}

/*
 * call-seq:
 *   to_16s -> cvmat(depth = CV_16s)
 *
 * Return the new matrix that elements is converted to signed 16bit.
 */
VALUE
rb_to_16s(VALUE self)
{
  CvMat *src = CVMAT(self);
  VALUE dest = new_object(src->rows, src->cols, CV_MAKETYPE(CV_16U, CV_MAT_CN(src->type)));
  cvConvert(src, CVMAT(dest));
  return dest;
}

/*
 * call-seq:
 *   to_32s -> cvmat(depth = CV_32S)
 *
 * Return the new matrix that elements is converted to signed 32bit.
 */
VALUE
rb_to_32s(VALUE self)
{
  CvMat *src = CVMAT(self);
  VALUE dest = new_object(src->rows, src->cols, CV_MAKETYPE(CV_32S, CV_MAT_CN(src->type)));
  cvConvert(src, CVMAT(dest));
  return dest;
}

/*
 * call-seq:
 *   to_32f -> cvmat(depth = CV_32F)
 *
 * Return the new matrix that elements is converted to 32bit floating-point.
 */
VALUE
rb_to_32f(VALUE self)
{
  CvMat *src = CVMAT(self);
  VALUE dest = new_object(src->rows, src->cols, CV_MAKETYPE(CV_32F, CV_MAT_CN(src->type)));
  cvConvert(src, CVMAT(dest));
  return dest;
}

/*
 * call-seq:
 *   to_64F -> cvmat(depth = CV_64F)
 *
 * Return the new matrix that elements is converted to 64bit floating-point.
 */
VALUE
rb_to_64f(VALUE self)
{
  CvMat *src = CVMAT(self);
  VALUE dest = new_object(src->rows, src->cols, CV_MAKETYPE(CV_64F, CV_MAT_CN(src->type)));
  cvConvert(src, CVMAT(dest));
  return dest;
}

/*
 * call-seq:
 *   vector? -> true or false
 *
 * If #width or #height is 1, return true. Otherwise return false.
 */
VALUE
rb_vector_q(VALUE self)
{
  CvMat *mat = CVMAT(self);
  return (mat->width == 1|| mat->height == 1) ? Qtrue : Qfalse;
}

/*
 * call-seq:
 *   square? -> true or false
 *
 * If #width == #height return true. Otherwise return false.
 */
VALUE
rb_square_q(VALUE self)
{
  CvMat *mat = CVMAT(self);
  return mat->width == mat->height ? Qtrue : Qfalse;
}

/************************************************************
       cxcore function
************************************************************/
/*
 * Return CvMat object with reference to caller-object.
 *
 *   src = CvMat.new(10, 10)
 *   src.has_parent?          #=> false
 *   src.parent               #=> nil
 *   mat = src.to_CvMat
 *   mat.has_parent?          #=> true
 *   mat.parent               #=> CvMat object "src"
 *
 * This case, 'src' is root-object. and 'mat' is child-object refer to 'src'.
 *   src <=refer= mat
 * In C, 'src->data' and 'mat->data' is common. Therefore, they cause the change each other.
 * object 'src' don't GC.
 */
VALUE
rb_to_CvMat(VALUE self)
{
  return DEPEND_OBJECT(rb_klass, cvGetMat(CVARR(self), CVALLOC(CvMat)), self);
}

/*
 * call-seq:
 *   sub_rect(<i>rect</i>) -> cvmat
 *   sub_rect(<i>topleft</i>, <i>size</i>) -> cvmat
 *   sub_rect(<i>x, y, width, height</i>) -> cvmat
 *
 * Return parts of self as CvMat.
 *
 * <i>p</i> or <i>x</i>,<i>y</i> mean top-left coordinate.
 * <i>size</i> or <i>width</i>,<i>height</i> is size.
 *
 * link:../images/CvMat_sub_rect.png
 */
VALUE
rb_sub_rect(VALUE self, VALUE args)
{
  CvRect area;
  CvPoint topleft;
  CvSize size;
  switch(RARRAY_LEN(args)) {
  case 1:
    area = VALUE_TO_CVRECT(RARRAY_PTR(args)[0]);
    break;
  case 2:
    topleft = VALUE_TO_CVPOINT(RARRAY_PTR(args)[0]);
    size = VALUE_TO_CVSIZE(RARRAY_PTR(args)[1]);
    area.x = topleft.x;
    area.y = topleft.y;
    area.width = size.width;
    area.height = size.height;
    break;
  case 4:
    area.x = NUM2INT(RARRAY_PTR(args)[0]);
    area.y = NUM2INT(RARRAY_PTR(args)[1]);
    area.width = NUM2INT(RARRAY_PTR(args)[2]);
    area.height = NUM2INT(RARRAY_PTR(args)[3]);
    break;
  default:
    rb_raise(rb_eArgError, "wrong number of arguments (%d of 1 or 2 or 4)", RARRAY_LEN(args));
  }
  return DEPEND_OBJECT(rb_klass,
                       cvGetSubRect(CVARR(self), CVALLOC(CvMat), area),
                       self);
}

/*
 * call-seq:
 *   slice_width(<i>n</i>)
 *
 * The matrix is divided into <i>n</i> piece by the width.
 * If it cannot be just divided, warning is displayed.
 *
 * e.g.
 *   m = OpenCV::CvMat.new(10, 10) #=> size 10x10 matrix
 *   ml, mr = m.slice_width(2)     #=> 5x10 and 5x10 matrix
 *
 *   ml, mm, mr = m.sclice_width(3)#=> 3x10 3x10 3x10 matrix
 *   warning : width does not div correctly.
 */
VALUE
rb_slice_width(VALUE self, VALUE num)
{
  int n = NUM2INT(num);
  if (n < 1) {rb_raise(rb_eArgError, "number of piece should be > 0");}
  CvSize size = cvGetSize(CVARR(self));
  if (size.width % n != 0) {rb_warn("width does not div correctly.");}
  int div_x = size.width / n;
  VALUE ary = rb_ary_new2(n);
  for (int i = 0; i < n; i++) {
    CvRect rect = {div_x * i, 0, div_x, size.height};
    rb_ary_push(ary, DEPEND_OBJECT(rb_klass, cvGetSubRect(CVARR(self), CVALLOC(CvMat), rect), self));
  }
  return ary;
}

/*
 * call-seq:
 *   slice_height(<i>n</i>)
 *
 * The matrix is divided into <i>n</i> piece by the height.
 * If it cannot be just divided, warning is displayed.
 *
 * see also #slice_width.
 */
VALUE
rb_slice_height(VALUE self, VALUE num)
{
  int n = NUM2INT(num);
  if (n < 1) {rb_raise(rb_eArgError, "number of piece should be > 0");}
  CvSize size = cvGetSize(CVARR(self));
  if (size.height % n != 0) {rb_warn("height does not div correctly.");}
  int div_y = size.height / n;
  VALUE ary = rb_ary_new2(n);
  for (int i = 0; i < n; i++) {
    CvRect rect = {0, div_y * i, size.width, div_y};
    rb_ary_push(ary, DEPEND_OBJECT(rb_klass, cvGetSubRect(CVARR(self), CVALLOC(CvMat), rect), self));
  }
  return ary;
}

/*
 * call-seq:
 *   row(<i>n</i>)            -> Return row
 *   row(<i>n1, n2, ...</i>)  -> Return Array of row
 *
 * Return row(or rows) of matrix.
 * argument should be Fixnum or CvSlice compatible object.
 */
VALUE
rb_row(VALUE self, VALUE args)
{
  int len = RARRAY_LEN(args);
  if (len < 1) {rb_raise(rb_eArgError, "wrong number of argument.(more than 1)");}
  VALUE ary = rb_ary_new2(len);
  for (int i = 0; i < len; i++) {
    VALUE value = rb_ary_entry(args, i);
    if (FIXNUM_P(value)) {
      rb_ary_store(ary, i, DEPEND_OBJECT(rb_klass, cvGetRow(CVARR(self), CVALLOC(CvMat), FIX2INT(value)), self));
    }else{
      CvSlice slice = VALUE_TO_CVSLICE(value);
      rb_ary_store(ary, i, DEPEND_OBJECT(rb_klass, cvGetRows(CVARR(self), CVALLOC(CvMat), slice.start_index, slice.end_index), self));
    }        
  }
  return RARRAY_LEN(ary) > 1 ? ary : rb_ary_entry(ary, 0);
}

/*
 * call-seq:
 *   col(<i>n</i>)            -> Return column
 *   col(<i>n1, n2, ...</i>)  -> Return Array of columns
 *
 * Return column(or columns) of matrix.
 * argument should be Fixnum or CvSlice compatible object.
 */
VALUE
rb_col(VALUE self, VALUE args)
{
  int len = RARRAY_LEN(args);
  if (len < 1) {rb_raise(rb_eArgError, "wrong number of argument.(more than 1)");}
  VALUE ary = rb_ary_new2(len);
  for (int i = 0; i < len; i++) {
    VALUE value = rb_ary_entry(args, i);
    if (FIXNUM_P(value)) {
      rb_ary_store(ary, i, DEPEND_OBJECT(rb_klass, cvGetCol(CVARR(self), CVALLOC(CvMat), FIX2INT(value)), self));
    }else{
      CvSlice slice = VALUE_TO_CVSLICE(value);
      rb_ary_store(ary, i, DEPEND_OBJECT(rb_klass, cvGetCols(CVARR(self), CVALLOC(CvMat), slice.start_index, slice.end_index), self));
    }        
  }
  return RARRAY_LEN(ary) > 1 ? ary : rb_ary_entry(ary, 0);
}

/*
 * call-seq:
 *   each_row{|row| ... } -> self
 *
 * Calls block once for each row in self, passing that element as a parameter.
 *
 * see also CvMat#each_col
 */
VALUE
rb_each_row(VALUE self)
{
  int rows = CVMAT(self)->rows;
  for (int i = 0; i < rows; i++) {
    rb_yield(DEPEND_OBJECT(rb_klass, cvGetRow(CVARR(self), CVALLOC(CvMat), i), self));
  }
  return self;
}

/*
 * call-seq:
 *   each_col{|col| ... } -> self
 *
 * Calls block once for each column in self, passing that element as a parameter.
 *
 * see also CvMat#each_row
 */
VALUE
rb_each_col(VALUE self)
{
  int cols = CVMAT(self)->cols;
  for (int i = 0; i < cols; i++) {
    rb_yield(DEPEND_OBJECT(rb_klass, cvGetCol(CVARR(self), CVALLOC(CvMat), i), self));
  }
  return self;
}

/*
 * call-seq:
 *   diag(<i>[val = 0]</i>) -> cvmat
 *
 * Return one of array diagonals.
 * <i>val</i> is zeo corresponds to the main diagonal, -1 corresponds to the diagonal above the main etc, 1 corresponds to the diagonal below the main etc.
 *
 */
VALUE
rb_diag(int argc, VALUE *argv, VALUE self)
{
  VALUE val;
  if (rb_scan_args(argc, argv, "01", &val) < 1) {
    val = INT2FIX(0);
  }      
  return DEPEND_OBJECT(rb_klass, cvGetDiag(CVARR(self), CVALLOC(CvMat), NUM2INT(val)), self);
}

/*
 * call-seq:
 *   size -> cvsize 
 *
 * Return size by CvSize
 */
VALUE
rb_size(VALUE self)
{
  return cCvSize::new_object(cvGetSize(CVARR(self)));
}

/*
  VALUE rb_elem_type(VALUE self) {
  return INT2FIX(cvGetElemType(CVARR(self)));
  }
*/

/*
 * call-seq:     
 *   dims -> array(int, int, ...)
 *
 * Return number of array dimensions and their sizes or the size of particular dimension.
 * In case of CvMat it always returns 2 regardless of number of matrix rows.
 */
VALUE
rb_dims(VALUE self)
{
  int size[CV_MAX_DIM];
  int dims = cvGetDims(CVARR(self), size);
  VALUE ary = rb_ary_new2(dims);
  for (int i = 0; i < dims; i++) {
    rb_ary_store(ary, i, INT2FIX(size[i]));
  }
  return ary;
}

/*
 * call-seq:
 *   dim_size(<i>index</i>) -> int
 *
 * Return number of dimension.
 * almost same as CvMat#dims[<i>index</i>].
 * If the dimension specified with index doesn't exist, CvStatusOutOfRange raise.
 */
VALUE
rb_dim_size(VALUE self, VALUE index)
{
  return INT2FIX(cvGetDimSize(CVARR(self), FIX2INT(index)));
}

/*
 * call-seq:
 *   [<i>idx1[,idx2]...</i>]
 *
 * Return value of the particular array element as CvScalar.
 */
VALUE
rb_aref(VALUE self, VALUE args)
{
  int index[CV_MAX_DIM];
  for (int i = 0; i < RARRAY_LEN(args); i++) {
    index[i] = NUM2INT(rb_ary_entry(args, i));
  }
  CvScalar scalar = cvScalarAll(0);
  switch(RARRAY_LEN(args)) {      
  case 1:
    scalar = cvGet1D(CVARR(self), index[0]);
    break;
  case 2:
    scalar = cvGet2D(CVARR(self), index[1], index[0]);
    break;
  case 3:
    scalar = cvGet3D(CVARR(self), index[2], index[1], index[0]);
    break;
  default:
    scalar = cvGetND(CVARR(self), index);
  }
  return cCvScalar::new_object(scalar);
}

/*
 * call-seq:
 *   [<i>idx1[,idx2]...</i>] = <i>value</i>
 *
 * Set value of the particular array element to <i>value</i>.
 * <i>value</i> should be CvScalar.
 */
VALUE
rb_aset(VALUE self, VALUE args)
{
  CvScalar scalar = VALUE_TO_CVSCALAR(rb_ary_pop(args));
  int index[CV_MAX_DIM];
  for (int i = 0; i < RARRAY_LEN(args); i++) {
    index[i] = NUM2INT(rb_ary_entry(args, i));
  }
  switch(RARRAY_LEN(args)) {
  case 1:
    cvSet1D(CVARR(self), index[0], scalar);
    break;
  case 2:
    cvSet2D(CVARR(self), index[1], index[0], scalar);
    break;
  case 3:
    cvSet3D(CVARR(self), index[2], index[1], index[0], scalar);
    break;
  default:
    cvSetND(CVARR(self), index, scalar);
  }
  return self;
}

/*
 * call-seq:
 *   fill(<i>value[, mask]</i>) -> cvmat
 *
 * Return CvMat copied value to every selected element. value should be CvScalar or compatible object.
 *   self[I] = value if mask(I)!=0
 *
 * note: This method support ROI on IplImage class. but COI not support. COI should not be set.
 *   image = IplImage.new(10, 20)         #=> create 3 channel image.
 *   image.coi = 1                        #=> set COI
 *   image.fill(CvScalar.new(10, 20, 30)) #=> raise CvBadCOI error.
 */
VALUE
rb_fill(int argc, VALUE *argv, VALUE self)
{
  return rb_fill_bang(argc, argv, copy(self));
}

/*
 * call-seq:
 *   fill!(<i>value[, mask]</i>) -> self
 *
 * Copie value to every selected element.
 *  self[I] = value if mask(I)!=0
 *
 * see also #fill.
 */
VALUE
rb_fill_bang(int argc, VALUE *argv, VALUE self)
{
  VALUE value, mask;
  rb_scan_args(argc, argv, "11", &value, &mask);
  cvSet(CVARR(self), VALUE_TO_CVSCALAR(value), MASK(mask));
  return self;
}

/*
 * call-seq:
 *   save_image(<i>filename</i>) -> self
 *
 * Saves an image to file. The image format is chosen depending on the filename extension.
 * Only 8bit single-channel or 3-channel(with 'BGR' channel order) image can be saved.
 *
 * e.g.
 *   image = OpenCV::CvMat.new(10, 10, CV_8U, 3)
 *   image.save_image("image.jpg") #=> save as JPEG format
 *   image.save_image("image.png") #=> save as PNG format
 */
VALUE
rb_save_image(VALUE self, VALUE filename)
{
  Check_Type(filename, T_STRING);      
  cvSaveImage(StringValueCStr(filename), CVARR(self));
  return self;
}

/*
 * call-seq:
 *   clear -> cvmat
 *
 * Return new matrix all element-value cleared.
 */
VALUE
rb_clear(VALUE self)
{
  return rb_clear_bang(copy(self));
}

/*
 * call-seq:
 *  clear! -> self
 *
 * Clear all element-value. Return self.
 */
VALUE
rb_clear_bang(VALUE self)
{
  cvSetZero(CVARR(self));
  return self;
}

/*
 * call-seq:
 *   identity(<i>[val = [1]]</i>) -> cvmat
 *
 * Return initializes scaled identity matrix.
 * <i>val</i> should be CvScalar.
 *
 *  arr(i, j) = val if i = j, 0 otherwise
 */
VALUE
rb_set_identity(int argc, VALUE *argv, VALUE self)
{
  return rb_set_identity_bang(argc, argv, copy(self));
}

/*
 * call-seq:
 *   identity!(<i>[val = [1]]</i>) -> self
 *
 * Initialize scaled identity matrix.
 * <i>val</i> should be CvScalar.
 *
 *  arr(i, j) = val if i = j, 0 otherwise
 */
VALUE
rb_set_identity_bang(int argc, VALUE *argv, VALUE self)
{
  VALUE val;
  CvScalar value;
  if (rb_scan_args(argc, argv, "01",  &val) < 1) {
    value = cvRealScalar(1);
  }else{
    value = VALUE_TO_CVSCALAR(val);
  }
  cvSetIdentity(CVARR(self), value);
  return self;
}

/*
 * call-seq:
 *   range(start, end) -> cvmat
 *
 * Create and return filled matrix with given range of numbers.
 *
 * see range!
 */
VALUE
rb_range(int argc, VALUE *argv, VALUE self)
{
  return rb_range_bang(argc, argv, copy(self));
}

/*
 * call-seq:
 *   range!(start, end) -> self
 *
 * Fills matrix with given range of numbers.
 *
 * initializes the matrix as following:
 *   arr(i,j)=(end-start)*(i*cols(arr)+j)/(cols(arr)*rows(arr))
 * For example, the following code will initilize 1D vector with subsequent integer numbers.
 *   m = CvMat.new(1, 10, :cv32s)
 *   m.range!(0, m.cols);            // m will be initialized as [0,1,2,3,4,5,6,7,8,9]
 */
VALUE
rb_range_bang(int argc, VALUE *argv, VALUE self)
{
  VALUE start, end;
  rb_scan_args(argc, argv, "20", &start, &end);
  cvRange(CVARR(self), NUM2DBL(start), NUM2DBL(end));
  return self;
}




/*
 * call-seq:
 *   reshape(<i>[:rows => num][, :channel => cn]</i>) -> cvmat(refer self)
 *
 * Change shape of matrix/image without copying data.
 *
 * e.g.
 *  mat = CvMat.new(3, 3, CV_8U, 3)  #=> 3x3 3-channel matrix
 *  vec = mat.reshape(:rows => 1)    #=> 1x9 3-channel matrix
 *  ch1 = mat.reshape(:channel => 1) #=> 9x9 1-channel matrix
 */
VALUE
rb_reshape(VALUE self, VALUE hash)
{
  if (TYPE(hash) != T_HASH)
    rb_raise(rb_eTypeError, "argument should be Hash that contaion key (:row, :channel).");
  VALUE channel = rb_hash_aref(hash, ID2SYM(rb_intern("channel")));
  VALUE rows = rb_hash_aref(hash, ID2SYM(rb_intern("rows")));
  return DEPEND_OBJECT(rb_klass, cvReshape(CVARR(self), CVALLOC(CvMat), NIL_P(rows) ? 0 : FIX2INT(rows), NIL_P(channel) ? 0 : FIX2INT(channel)), self);
}

/*
 * call-seq:
 *   repeat(<i>mat</i>) -> cvmat
 *
 * Tiled <i>mat</i> by self.
 */
VALUE
rb_repeat(VALUE self, VALUE object)
{
  if (rb_obj_is_kind_of(object, rb_class()))
    rb_raise(rb_eTypeError, "argument should be CvMat subclass.");
  cvRepeat(CVARR(self), CVARR(object));
  return object;
}

/*
 * call-seq:
 *   flip(:x) -> cvmat
 *   flip(:y) -> cvmat
 *   flip ->  -> cvmat
 *
 * Return new flipped 2D array.
 * * flip(:x) - flip around horizontal
 * * flip(:y) - flip around vertical
 * * flip - flip around both axises
 */
VALUE
rb_flip(int argc, VALUE *argv, VALUE self)
{
  return rb_flip_bang(argc, argv, copy(self));
}

/*
 * call-seq:
 *   flip!(:x) -> self
 *   flip!(:y) -> self
 *   flip!     -> self
 *
 * Flip 2D array. Return self.
 *
 * see also CvMat#flip
 */
VALUE
rb_flip_bang(int argc, VALUE *argv, VALUE self)
{
  VALUE format;
  int mode = -1;
  if (rb_scan_args(argc, argv, "01", &format) > 0) {
    if (rb_to_id(format) == rb_intern("x"))
      mode = 1;
    else if (rb_to_id(format) == rb_intern("y"))
      mode = 0;
    else
      rb_warn("argument may be :x or :y");        
  }
  cvFlip(CVARR(self), NULL, mode);
  return self;
}

/*
 * call-seq:
 *   split -> array(include cvmat)
 *
 * Divides multi-channel array into several single-chanel arrays.
 *
 * e.g.
 *  image = CvMat.new 640, 480, CV_8U, 3 #=> 3-channel image
 *  image.split                          #=> [image1, image2, image3] : each image have single-channel
 *
 * e.g. switch red <-> blue channel.
 *  image = IplImage.load "sample.bmp"
 *  i = image.split
 *  new_image = CvMat.merge i[2], i[1], i[0]
 */
VALUE
rb_split(VALUE self)
{
  int type = CVMAT(self)->type, depth = CV_MAT_DEPTH(type), channel = CV_MAT_CN(type);
  CvSize size = cvGetSize(CVARR(self));
  CvMat *dest[] = {NULL, NULL, NULL, NULL};      
  for (int i = 0; i < channel; i++)
    dest[i] = cvCreateMat(size.height, size.width, CV_MAKETYPE(depth, 1));      
  cvSplit(CVARR(self), dest[0], dest[1], dest[2], dest[3]);
  VALUE ary = rb_ary_new2(channel);
  for (int i = 0; i < channel; i++)
    rb_ary_store(ary, i, OPENCV_OBJECT(rb_klass, dest[i]));
  return ary;
}

/*
 * call-seq:
 *   CvMat.merge(<i>mat1[,mat2][,mat3][,mat4]</i>) -> cvmat
 *
 * Composes multi-channel array from several single-channel arrays.
 * Each argument should be single-channel image(CvMat or subclass).
 * All image should be same size and same depth.
 *
 * see also CvMat#split
 */
VALUE
rb_merge(VALUE klass, VALUE args)
{
  VALUE object, dest;
  int len = RARRAY_LEN(args);
  if (!(len > 0) || len > CV_CN_MAX) {
    rb_raise(rb_eArgError, "wrong number of argument (%d for 1..4)", len);
  }
  CvMat *src[] = {NULL, NULL, NULL, NULL}, *tmp = 0;
  for (int i = 0; i < len; i++) {
    if (rb_obj_is_kind_of((object = rb_ary_entry(args, i)), rb_klass)) {
      src[i] = CVMAT(object);
      if (CV_MAT_CN(src[i]->type) != 1) {
        rb_raise(rb_eStandardError, "image should be single-channel CvMat.");
      }
      if (!tmp)
        tmp = src[i];
      else{
        if (!CV_ARE_SIZES_EQ(tmp, src[i]))
          rb_raise(rb_eStandardError, "image size should be same.");
        if (!CV_ARE_DEPTHS_EQ(tmp, src[i]))
          rb_raise(rb_eStandardError, "image depth should be same.");
      }
    }else if (NIL_P(object)) {
      src[i] = NULL;
    }else
      rb_raise(rb_eTypeError, "argument should be CvMat or subclass of it.");
  }
  dest = new_object(cvGetSize(tmp), CV_MAKETYPE(CV_MAT_DEPTH(tmp->type), len));
  cvMerge(src[0], src[1], src[2], src[3], CVARR(dest));
  return dest;
}

/*
 * call-seq:
 *   CvMat.mix_channels(srcs,dests,from_to = {1 => 1, 2 => 2, 3 => 3, 4 => 4}) -> dests
 */
VALUE
rb_mix_channels(int argc, VALUE *argv, VALUE self)
{
  VALUE srcs, dests, from_to;
  rb_scan_args(argc, argv, "21", &srcs, &dests, &from_to);
  /* not yet */
  return Qnil;
}

/*
 * call-seq:
 *   rand_shuffle([seed = nil][,iter = 1])
 *
 * Return filled the destination array with values from the look-up table.
 * 
 * see rand_shuffle!
 */
VALUE
rb_rand_shuffle(int argc, VALUE *argv, VALUE self)
{
  return rb_rand_shuffle_bang(argc, argv, copy(self));
}

/*
 * call-seq:
 *   rand_shuffle!([seed = nil][,iter = 1])
 *
 * fills the destination array with values from the look-up table.
 * Indices of the entries are taken from the source array. That is, the function processes each element of src as following:
 *   dst(I)=lut[src(I)+DELTA]
 * where DELTA=0 if src has depth :cv8u, and DELTA=128 if src has depth :cv8s.
 */
VALUE
rb_rand_shuffle_bang(int argc, VALUE *argv, VALUE self)
{
  VALUE seed, iter;
  CvRNG rng;
  rb_scan_args(argc, argv, "02", &seed, &iter);
  if(NIL_P(seed))
    cvRandShuffle(CVARR(self), NULL, IF_INT(iter, 1));
  else{
    rng = cvRNG(rb_num2ll(seed));
    cvRandShuffle(CVARR(self), &rng, IF_INT(iter, 1));
  }
  return self;
}

/*
 * call-seq:
 *   lut(<i>lookup_table</i>) -> cvmat
 *
 * Return new matrix performed lookup-table transforme.
 *
 * <i>lookup_table</i> should be CvMat that have 256 element (e.g. 1x256 matrix).
 * Otherwise, raise CvStatusBadArgument error.
 *
 * And <i>lookup_table</i> should either have a single-channel, or the same number of channels.
 * When single-channel lookup-table given, same table is used for all channels.
 */
VALUE
rb_lut(VALUE self, VALUE lut)
{
  VALUE dest = copy(self);
  cvLUT(CVARR(self), CVARR(dest), CVARR(lut));
  return dest;
}

/*
 * call-seq:
 *   convert_scale(<i>:depth => nil, :scale => 1.0, :shift => 0.0</i>)
 *
 * Return new array with optional linear transformation.
 *   mat(I) = src(I) * scale + (shift, shift, ...)     
 */
VALUE
rb_convert_scale(VALUE self, VALUE hash)
{
  if (TYPE(hash) != T_HASH)
    rb_raise(rb_eTypeError, "argument should be Hash that contaion key [:depth, :scale, :shift].");
  VALUE depth = rb_hash_aref(hash, ID2SYM(rb_intern("depth"))),
    scale = rb_hash_aref(hash, ID2SYM(rb_intern("scale"))),
    shift = rb_hash_aref(hash, ID2SYM(rb_intern("shift"))),
    dest = new_object(cvGetSize(CVARR(self)), CV_MAKETYPE(IF_DEPTH(depth, CV_MAT_DEPTH(CVMAT(self)->type)), CV_MAT_CN(CVMAT(self)->type)));
  cvConvertScale(CVARR(self), CVARR(dest), IF_DBL(scale, 1.0), IF_DBL(shift, 0.0));
  return dest;
}

/*
 * call-seq:
 *   convert_scale_abs(<i>:scale => 1.0, :shift => 0.0</i>)
 *
 * Return new array with optional linear transformation.
 * It is similar to CvMat#convert_scale, but it stores absolute values of the conversion result
 *   mat(I) = (src(I) * scale + (shift, shift, ...)).abs
 */
VALUE
rb_convert_scale_abs(VALUE self, VALUE hash)
{
  if (TYPE(hash) != T_HASH)
    rb_raise(rb_eTypeError, "argument should be Hash that contaion key [:depth, :scale, :shift].");
  VALUE 
    scale = rb_hash_aref(hash, ID2SYM(rb_intern("scale"))),
    shift = rb_hash_aref(hash, ID2SYM(rb_intern("shift"))),
    dest = new_object(cvGetSize(CVARR(self)), CV_MAKETYPE(CV_8U, CV_MAT_CN(CVMAT(self)->type)));
  cvConvertScale(CVARR(self), CVARR(dest), IF_DBL(scale, 1.0), IF_DBL(shift, 0.0));
  return dest;
}

/*
 * call-seq:
 *   add(<i>val[,mask]</i>) -> cvmat
 *
 * Return new matrix computed per-element sum.
 * <i>val</i> should be CvMat or CvScalar.
 * If <i>val</i> is CvMat, it must have same type (depth and channel).
 * <i>mask</i> should be CvMat(8bit single-channel).
 * For each element (I) 
 *  dst(I) = src1(I) + src2(I) if mask(I) != 0
 */
VALUE
rb_add(int argc, VALUE *argv, VALUE self)
{
  VALUE val, mask, dest;
  rb_scan_args(argc, argv, "11", &val, &mask);
  dest = new_object(cvGetSize(CVARR(self)), cvGetElemType(CVARR(self)));
  if (rb_obj_is_kind_of(val, rb_klass))
    cvAdd(CVARR(self), CVARR(val), CVARR(dest), MASK(mask));
  else
    cvAddS(CVARR(self), VALUE_TO_CVSCALAR(val), CVARR(dest), MASK(mask));
  return dest;
}

/*
 * call-seq:
 *   sub(<i>val[,mask]</i>) -> cvmat
 *
 * Return new matrix computed per-element difference.
 * <i>val</i> should be CvMat or CvScalar.
 * If <i>val</i> is CvMat, it must have same type (depth and channel).
 * <i>mask</i> should be CvMat(8bit single-channel).
 * For each element (I) 
 *  dst(I) = src1(I) - src2(I) if mask(I) != 0
 */
VALUE
rb_sub(int argc, VALUE *argv, VALUE self)
{
  VALUE val, mask, dest;
  rb_scan_args(argc, argv, "11", &val, &mask);
  dest = new_object(cvGetSize(CVARR(self)), cvGetElemType(CVARR(self)));
  if (rb_obj_is_kind_of(val, rb_klass))
    cvSub(CVARR(self), CVARR(val), CVARR(dest), MASK(mask));
  else
    cvSubS(CVARR(self), VALUE_TO_CVSCALAR(val), CVARR(dest), MASK(mask));
  return dest;
}

/*
 * call-seq:
 *   mul(<i>val[,scale = 1.0]</i>) -> cvmat
 *
 * Return new matrix computed per-element product.
 * <i>val</i> should be CvMat or CvScalar.
 * If <i>val</i> is CvMat, it must have same type (depth and channel).
 * For each element (I)
 *  dst(I) = scale * src1(I) * src2(I)     
 */
VALUE
rb_mul(int argc, VALUE *argv, VALUE self)
{
  VALUE val, scale, dest;
  if (rb_scan_args(argc, argv, "11", &val, &scale) < 2)
    scale = rb_float_new(1.0);
  dest = new_object(cvGetSize(CVARR(self)), cvGetElemType(CVARR(self)));
  if (rb_obj_is_kind_of(val, rb_klass)) {        
    cvMul(CVARR(self), CVARR(val), CVARR(dest), NUM2DBL(scale));
  }else{
    CvScalar scl = VALUE_TO_CVSCALAR(val);
    VALUE mat = new_object(cvGetSize(CVARR(self)), cvGetElemType(CVARR(self)));
    cvSet(CVARR(mat), scl);
    cvMul(CVARR(self), CVARR(mat), CVARR(dest), NUM2DBL(scale));        
  }
  return dest;
}

/*
 * call-seq:
 *   div(<i>val[,scale = 1.0]</i>) -> cvmat
 *
 * Return new matrix computed per-element division.
 * <i>val</i> should be CvMat or CvScalar.
 * If <i>val</i> is CvMat, it must have same type (depth and channel).
 * For each element (I)
 *  dst(I) = scale * src1(I) / src2(I)     
 */
VALUE
rb_div(int argc, VALUE *argv, VALUE self)
{
  VALUE val, scale, dest;
  if (rb_scan_args(argc, argv, "11", &val, &scale) < 2)
    scale = rb_float_new(1.0);
  dest = new_object(cvGetSize(CVARR(self)), cvGetElemType(CVARR(self)));
  if (rb_obj_is_kind_of(val, rb_klass)) {        
    cvDiv(CVARR(self), CVARR(val), CVARR(dest), NUM2DBL(scale));
  }else{
    CvScalar scl = VALUE_TO_CVSCALAR(val);
    VALUE mat = new_object(cvGetSize(CVARR(self)), cvGetElemType(CVARR(self)));
    cvSet(CVARR(mat), scl);
    cvDiv(CVARR(self), CVARR(mat), CVARR(dest), NUM2DBL(scale));
  }
  return dest;
}

/*
 * call-seq:
 *   and(<i>val[,mask]</i>) -> cvmat
 *
 * Return new matrix computed per-element bit-wise conjunction.
 * <i>val</i> should be CvMat or CvScalar.
 * If <i>val</i> is CvMat, it must have same type (depth and channel).
 * For each element (I)
 *  dst(I) = src1(I) & src2(I) if mask(I) != 0
 */
VALUE
rb_and(int argc, VALUE *argv, VALUE self)
{
  VALUE val, mask, dest;
  rb_scan_args(argc, argv, "11", &val, &mask);
  dest = new_object(cvGetSize(CVARR(self)), cvGetElemType(CVARR(self)));
  if (rb_obj_is_kind_of(val, rb_klass))
    cvAnd(CVARR(self), CVARR(val), CVARR(dest), MASK(mask));
  else
    cvAndS(CVARR(self), VALUE_TO_CVSCALAR(val), CVARR(dest), MASK(mask));
  return dest;
}

/*
 * call-seq:
 *   or(<i>val[,mask]</i>) -> cvmat
 *
 * Return new matrix computed per-element bit-wise disjunction.
 * <i>val</i> should be CvMat or CvScalar.
 * If <i>val</i> is CvMat, it must have same type (depth and channel).
 * For each element (I)
 *  dst(I) = src1(I) | src2(I) if mask(I) != 0     
 */
VALUE
rb_or(int argc, VALUE *argv, VALUE self)
{
  VALUE val, mask, dest;
  rb_scan_args(argc, argv, "11", &val, &mask);
  dest = new_object(cvGetSize(CVARR(self)), cvGetElemType(CVARR(self)));
  if (rb_obj_is_kind_of(val, rb_klass))
    cvOr(CVARR(self), CVARR(val), CVARR(dest), MASK(mask));
  else
    cvOrS(CVARR(self), VALUE_TO_CVSCALAR(val), CVARR(dest), MASK(mask));
  return dest;
}

/*
 * call-seq:
 *   xor(<i>val[,mask]</i>) -> cvmat
 *
 * Return new matrix computed per-element bit-wise "exclusive or" operation.
 * <i>val</i> should be CvMat or CvScalar.
 * If <i>val</i> is CvMat, it must have same type (depth and channel).
 * For each element (I)
 *  dst(I) = src1(I) ^ src2(I) if mask(I) != 0
 */
VALUE
rb_xor(int argc, VALUE *argv, VALUE self)
{
  VALUE val, mask, dest;
  rb_scan_args(argc, argv, "11", &val, &mask);
  dest = new_object(cvGetSize(CVARR(self)), cvGetElemType(CVARR(self)));
  if (rb_obj_is_kind_of(val, rb_klass))
    cvXor(CVARR(self), CVARR(val), CVARR(dest), MASK(mask));
  else
    cvXorS(CVARR(self), VALUE_TO_CVSCALAR(val), CVARR(dest), MASK(mask));
  return dest;
}

/*
 * call-seq:
 *   not -> cvmat
 *
 * Return new matrix performed per-element bit-wise inversion.
 *  dst(I) =~ src(I)
 */
VALUE
rb_not(VALUE self)
{
  VALUE dest = new_object(cvGetSize(CVARR(self)), cvGetElemType(CVARR(self)));
  cvNot(CVARR(self), CVARR(dest));
  return dest;
}

/*
 * call-seq:
 *   not! -> self
 *
 * Performe per-element bit-wise inversion.
 */
VALUE
rb_not_bang(VALUE self)
{
  cvNot(CVARR(self), CVARR(self));
  return self;
}

VALUE
rb_cmp_internal(VALUE self, VALUE val, int operand)
{
  VALUE dest = new_object(cvGetSize(CVARR(self)), CV_8U);
  if (rb_obj_is_kind_of(val, rb_klass))
    cvCmp(CVARR(self), CVARR(val), CVARR(dest), operand);
  else if (CV_MAT_CN(cvGetElemType(CVARR(self))) == 1 && rb_obj_is_kind_of(val, rb_cNumeric)) {
    cvCmpS(CVARR(self), NUM2DBL(val), CVARR(dest), operand);
  }else{
    VALUE mat = new_object(cvGetSize(CVARR(self)), cvGetElemType(CVARR(self)));
    cvSet(CVARR(mat), VALUE_TO_CVSCALAR(val));
    cvCmp(CVARR(self), CVARR(mat), CVARR(dest), operand);
  }
  return dest;
}

/*
 * call-seq:
 *   eq(<i>val</i>) -> cvmat
 *
 * Return new matrix performed per-element comparision "equal".
 *  dst(I) = (self(I) == val(I) ? 0xFF : 0)   
 */
VALUE
rb_eq(VALUE self, VALUE val)
{
  return rb_cmp_internal(self, val, CV_CMP_EQ);
}

/*
 * call-seq:
 *   gt(<i>val</i>) -> cvmat
 *
 * Return new matrix performed per-element comparision "greater than".
 *  dst(I) = (self(I) > val(I) ? 0xFF : 0)   
 */
VALUE
rb_gt(VALUE self, VALUE val)
{
  return rb_cmp_internal(self, val, CV_CMP_GT);
}

/*
 * call-seq:
 *   ge(<i>val</i>) -> cvmat
 *
 * Return new matrix performed per-element comparision "greater or equal".
 *  dst(I) = (self(I) >= val(I) ? 0xFF : 0)   
 */
VALUE
rb_ge(VALUE self, VALUE val)
{
  return rb_cmp_internal(self, val, CV_CMP_GE);
}

/*
 * call-seq:
 *   lt(<i>val</i>) -> cvmat
 *
 * Return new matrix performed per-element comparision "less than".
 *  dst(I) = (self(I) < val(I) ? 0xFF : 0)   
 */
VALUE
rb_lt(VALUE self, VALUE val)
{
  return rb_cmp_internal(self, val, CV_CMP_LT);
}

/*
 * call-seq:
 *   le(<i>val</i>) -> cvmat
 *
 * Return new matrix performed per-element comparision "less or equal".
 *  dst(I) = (self(I) <= val(I) ? 0xFF : 0)   
 */
VALUE
rb_le(VALUE self, VALUE val)
{
  return rb_cmp_internal(self, val, CV_CMP_LE);
}

/*
 * call-seq:
 *   ne(<i>val</i>) -> cvmat
 *
 * Return new matrix performed per-element comparision "not equal".
 *  dst(I) = (self(I) != val(I) ? 0xFF : 0)
 */
VALUE
rb_ne(VALUE self, VALUE val)
{
  return rb_cmp_internal(self, val, CV_CMP_NE);
}

/*
 * call-seq:
 *   in_range(<i>min, max</i>) -> cvmat
 *
 * Check that element lie between two object.
 * <i>min</i> and <i>max</i> should be CvMat that have same size and type, or CvScalar.
 * Return new matrix performed per-element,
 *  dst(I) = within the range ? 0xFF : 0
 */
VALUE
rb_in_range(VALUE self, VALUE min, VALUE max)
{
  VALUE dest = dest = new_object(cvGetSize(CVARR(self)), CV_8UC1), tmp;
  if (rb_obj_is_kind_of(min, rb_klass) && rb_obj_is_kind_of(max, rb_klass)) {
    cvInRange(CVARR(self), CVARR(min), CVARR(max), CVARR(dest));
  }else if (rb_obj_is_kind_of(min, rb_klass)) {
    tmp = new_object(cvGetSize(CVARR(self)), cvGetElemType(CVARR(self)));
    cvSet(CVARR(tmp), VALUE_TO_CVSCALAR(max));
    cvInRange(CVARR(self), CVARR(min), CVARR(tmp), CVARR(dest));
  }else if (rb_obj_is_kind_of(max, rb_klass)) {        
    tmp = new_object(cvGetSize(CVARR(self)), cvGetElemType(CVARR(self)));
    cvSet(CVARR(tmp), VALUE_TO_CVSCALAR(min));
    cvInRange(CVARR(self), CVARR(tmp), CVARR(max), CVARR(dest));
  }else
    cvInRangeS(CVARR(self), VALUE_TO_CVSCALAR(min), VALUE_TO_CVSCALAR(max), CVARR(dest));
  return dest;
}

/*
 * call-seq:
 *   abs_diff(<i>val</i>) -> cvmat
 *
 * Calculate absolute difference between two.
 * <i>val</i> should be CvMat that have same size and same type, or CvScalar.
 *  dst(I) = (src(I) - val(I)).abs
 */
VALUE
rb_abs_diff(VALUE self, VALUE val)
{
  VALUE dest = new_object(cvGetSize(CVARR(self)), cvGetElemType(CVARR(self)));
  if (rb_obj_is_kind_of(val, rb_klass)) {
    cvAbsDiff(CVARR(self), CVARR(val), CVARR(dest));
  }else{
    cvAbsDiffS(CVARR(self), CVARR(dest), VALUE_TO_CVSCALAR(val));
  }
  return dest;
}

/*
 * call-seq:
 *   count_non_zero -> int
 *
 * Returns the number of non-zero elements.
 *   result = sumI arr(I)!=0
 *
 * In case of IplImage both ROI and COI are supported.
 */
VALUE
rb_count_non_zero(VALUE self)
{
  return INT2FIX(cvCountNonZero(CVARR(self)));
}

/*
 * call-seq:
 *   sum -> scalar
 *
 * Return summerizes elements as CvScalar. Independently for each channel.
 * 
 * note: If COI is setted in IplImage, the method processes the selected channel only and store the sum to the first component scalar[0].
 */
VALUE
rb_sum(VALUE self)
{
  return cCvScalar::new_object(cvSum(CVARR(self)));
}

/*
 * call-seq:
 *   avg(<i>[mask]</i>) -> mean(as scalar)
 *
 * Return the average(mean) of elements as CvScalar. Independently for each channel.
 */
VALUE
rb_avg(int argc, VALUE *argv, VALUE self)
{
  VALUE mask, mean;
  rb_scan_args(argc, argv, "01", &mask);
  
  return cCvScalar::new_object(cvAvg(CVARR(self), MASK(mask)));
}

/*
 * call-seq:
 *   avg_sdv(<i>mask</i>) -> [mean(as scalar), std_dev(as scalar)]
 *
 * Calculates the average value and standard deviation of array elements, independently for each channel.
 *
 * note: same as [CvMat#avg, CvMat#sdv]
 */
VALUE
rb_avg_sdv(int argc, VALUE *argv, VALUE self)
{
  VALUE mask, mean, std_dev;
  rb_scan_args(argc, argv, "01", &mask);
  mean = cCvScalar::new_object();
  std_dev = cCvScalar::new_object();
  cvAvgSdv(CVARR(self), CVSCALAR(mean), CVSCALAR(std_dev), MASK(mask));
  return rb_ary_new3(2, mean, std_dev);
}

/*
 * call-seq:
 *   sdv(<i>[mask]</i>) -> std_dev(as scalar)
 *
 * Return the standard deviation of elements as CvScalar. Independently for each channel.
 */
VALUE
rb_sdv(int argc, VALUE *argv, VALUE self)
{
  VALUE mask, std_dev;
  rb_scan_args(argc, argv, "01", &mask);
  std_dev = cCvScalar::new_object();
  cvAvgSdv(CVARR(self), NULL, CVSCALAR(std_dev), MASK(mask));
  return std_dev;
}

/*
 * call-seq:
 *   min_max_loc(<i>[mask]</i>) -> [min_val, max_val, min_loc(as point), max_loc(as point)]
 *
 * Finds minimum and maximum element values and their positions.
 * The extremums are searched over the whole array, selected ROI(in case of IplImage) or, if mask is not NULL, in the specified array region.
 * If the array has more than one channel, it must be IplImage with COI set.
 * In case if multi-dimensional arrays min_loc.x and max_loc.x will contain raw (linear) positions of the extremums.
 */
VALUE
rb_min_max_loc(int argc, VALUE *argv, VALUE self)
{
  VALUE mask, min_loc, max_loc;
  double min_val = 0.0, max_val = 0.0;
  rb_scan_args(argc, argv, "01", &mask);
  min_loc = cCvPoint::new_object();
  max_loc = cCvPoint::new_object();
  cvMinMaxLoc(CVARR(self), &min_val, &max_val, CVPOINT(min_loc), CVPOINT(max_loc), MASK(mask));
  return rb_ary_new3(4,
		     rb_float_new(min_val),
		     rb_float_new(max_val),
		     min_loc,
		     max_loc);
}

/*
 * call-seq:
 *   dot_product(<i>mat</i>) -> float
 *
 * Calculates dot product of two arrays in Euclidian metrics.
 * <i>mat</i> should be CvMat have same size and same type.
 * 
 *  src1.src2 = sum(src1(I) * src2(I))
 */
VALUE
rb_dot_product(VALUE self, VALUE mat)
{
  if (!rb_obj_is_kind_of(mat, rb_klass))
    rb_raise(rb_eTypeError, "argument should be CvMat.");
  return rb_float_new(cvDotProduct(CVARR(self), CVARR(mat)));
}

/*
 * call-seq:
 *   cross_product(<i>mat</i>) -> cvmat
 *
 * Calculate cross product of two 3D vectors.
 * <i>mat</i> should be CvMat have same size and same type.
 */
VALUE
rb_cross_product(VALUE self, VALUE mat)
{
  if (!rb_obj_is_kind_of(mat, rb_klass))
    rb_raise(rb_eTypeError, "argument should be CvMat.");
  VALUE dest = new_object(cvGetSize(CVARR(self)), cvGetElemType(CVARR(self)));
  cvCrossProduct(CVARR(self), CVARR(mat), CVARR(dest));
  return dest;
}

/*
 * call-seq:
 *   transform(<i>transmat[,shiftvec]</i>) -> cvmat
 *
 * performs matrix transform of every element.
 *  dst(I) = transmat * src(I) + shiftvec 
 */
VALUE
rb_transform(int argc, VALUE *argv, VALUE self)
{
  VALUE transmat, shiftvec;
  rb_scan_args(argc, argv, "11", &transmat, &shiftvec);
  VALUE dest = new_object(cvGetSize(CVARR(self)), cvGetElemType(CVARR(self)));
  cvTransform(CVARR(self), CVARR(dest), CVMAT(transmat), MASK(shiftvec));
  return dest;
}

/*
 * call-seq:
 *   perspective_transform(<i>mat</i>) -> cvmat
 *
 * Return performed perspective matrix transform of vector array.
 * <i>mat</i> should be 3x3 or 4x4 transform matrix (CvMat).
 * Every element (by treating it as 2D or 3D vector) in the following way:
 *   (x, y, z) -> (x'/w, y'/w, z'/w) or
 *   (x, y) -> (x'/w, y'/w)
 * where
 *   (x', y', z', w') = mat4x4*(x, y, z, 1) or
 *   (x', y', w') = mat3x3*(x, y, 1)
 * and
 *   w = w' if w'!=0, inf otherwise.
 */
VALUE
rb_perspective_transform(VALUE self, VALUE mat)
{
  VALUE dest = new_object(cvGetSize(CVARR(self)), cvGetElemType(CVARR(self)));
  cvPerspectiveTransform(CVARR(self), CVARR(dest), CVMAT(mat));
  return dest;
}

/*
 * call-seq:
 *  mul_transposed(<i>:order => :default or :inverse, :delta => nil or cvmat</i>)
 *
 * Calculates the product of self and its transposition.
 *
 * options
 * * :order -> should be :default or :inverse (default is :default)
 *    see below.
 * * :delta -> should be nil or CvMat (default is nil)
 *     An optional array, subtracted from source before multiplication.
 *
 * mul_transposed evaluates:
 *   :order => :default
 *     dst = (self - delta) * (self - delta)T
 *   :order => :inverse
 *     dst = (self - delta)T * (self - delta)
 *
 */
VALUE
rb_mul_transposed(VALUE self, VALUE args)
{
  //VALUE options = extract_options_from_args_bang(args);
  //assert_valid_keys(options, 2, "order", "delta");
  //VALUE order;
  //OPTIONS(order, options, "order", ID2SYM(rb_intern("default")));
  //ID2SYM(rb_intern("order")), rb_intern("")
  return Qnil;
}


/*
 * call-seq:
 *   trace -> scalar
 *
 * Returns trace of matrix. "trace" is sum of diagonal elements of the matrix.
 */
VALUE
rb_trace(VALUE self)
{
  return cCvScalar::new_object(cvTrace(CVARR(self)));
}

/*
 * call-seq:
 *   transpose -> cvmat
 *
 * Return transposed matrix.
 */
VALUE
rb_transpose(VALUE self)
{
  CvSize size = cvGetSize(CVARR(self));
  VALUE dest = new_object(size.width, size.height, cvGetElemType(CVARR(self)));
  cvTranspose(CVARR(self), CVARR(dest));
  return dest;
}

/*
 * call-seq:
 *   transpose! -> self
 *
 * Transposed matrix.
 *
 * <b>rectangular matrix only (CvMat#square? = true).</b>
 */
VALUE
rb_transpose_bang(VALUE self)
{
  cvTranspose(CVARR(self), CVARR(self));
  return self;
}

/*
 * call-seq:
 *   det -> float
 *
 * Return determinant of matrix.
 * <i>self</i> should be single-channel and floating-point depth.
 */
VALUE
rb_det(VALUE self)
{
  return rb_float_new(cvDet(CVARR(self)));
}

/*
 * call-seq:
 *   invert(<i>inversion_method=:lu[,delta]</i>) -> float
 *
 * Finds inverse or pseudo-inverse of matrix.
 * <i>inversion_method</i> should be following symbol.
 * * :lu
 *     Gaussian elimincation with optimal pivot element chose.
 *     Return self determinant (self must be square).
 * * :svd
 *     Singular value decomposition(SVD) method.
 *     Return the inversed condition number of self(ratio of the smallest singular value to the largest singular value)
 *     and 0 if self is all zeros. The SVD method calculate a pseudo-inverse matrix if self is singular.
 * * :svd_sym or :svd_symmetric
 *     SVD method for a symmetric positively-defined matrix.
 *
 * <i>self</i> type should be single-channel and floating-point matrix.
 */
VALUE
rb_invert(int argc, VALUE *argv, VALUE self)
{
  VALUE symbol;
  rb_scan_args(argc, argv, "01", &symbol);
  int method = CVMETHOD("INVERSION_METHOD", symbol, CV_LU);
  VALUE dest = new_object(cvGetSize(CVARR(self)), cvGetElemType(CVARR(self)));
  cvInvert(CVARR(self), CVARR(dest), method);
  return dest;
}

/*
 * call-seq:
 *   solve(<i>mat, inversion_method=:lu</i>)
 *
 * Solves linear system or least-squares problem (the latter is possible with SVD method).
 *
 * <i>inversion_method</i> should be following symbol.
 * * :lu
 *     Gaussian elimincation with optimal pivot element chose.
 *     Return self determinant (self must be square).
 * * :svd
 *     Singular value decomposition(SVD) method.
 *     Return the inversed condition number of self(ratio of the smallest singular value to the largest singular value)
 *     and 0 if self is all zeros. The SVD method calculate a pseudo-inverse matrix if self is singular.
 * * :svd_sym or :svd_symmetric
 *     SVD method for a symmetric positively-defined matrix.
 */
VALUE
rb_solve(int argc, VALUE *argv, VALUE self)
{
  VALUE mat, symbol;
  rb_scan_args(argc, argv, "11", &mat, &symbol);
  if (!rb_obj_is_kind_of(mat, rb_klass))
    rb_raise(rb_eTypeError, "argument 1 (right-hand part of the linear system) should be %s.)", rb_class2name(rb_klass));
  VALUE dest = new_object(cvGetSize(CVARR(self)), cvGetElemType(CVARR(self)));
  cvSolve(CVARR(self), CVARR(mat), CVARR(dest), CVMETHOD("INVERSION_METHOD", symbol, CV_LU));
  return dest;
}

/*
 * call-seq:
 *   svd(u = nil, v = nil</i>)
 *
 * not implementated.
 * Performs singular value decomposition of real floating-point matrix.
 */
VALUE
rb_svd(int argc, VALUE *argv, VALUE self)
{
  rb_raise(rb_eNotImpError, "");
  /*
    VALUE u = Qnil, v = Qnil;
    rb_scan_args(argc, argv, "02", &u, &v);
    CvMat
    *matU = NIL_P(u) ? NULL : CVARR(u),
    *matV = NIL_P(v) ? NULL : CVARR(v);
    cvSVD(CVARR(self), matU, matV);
    return dest;
  */
}

/*
 * call-seq:
 *   svbksb
 *
 * not yet.
 */
VALUE
rb_svbksb(int argc, VALUE *argv, VALUE self)
{
  rb_raise(rb_eNotImpError, "");
}

/*
 * call-seq:
 *   eigenvv(<i>[eps = 0.0]</i>) -> [eigen_vectors(cvmat), eigen_values(cvmat)]
 *
 * Computes eigenvalues and eigenvectors of symmetric matrix.
 * <i>self</i> should be symmetric square matrix.
 * 
 * see #eigenvv!
 */
VALUE
rb_eigenvv(int argc, VALUE *argv, VALUE self)
{
  return rb_eigenvv_bang(argc, argv, copy(self));
}

/*
 * call-seq:
 *   eigenvv!(<i>[eps = 0.0]</i>) -> [eigen_vectors(cvmat), eigen_values(cvmat)]
 *
 * Computes eigenvalues and eigenvectors of symmetric matrix.
 * <i>self</i> should be symmetric square matrix. <i>self</i> is modified during the processing.
 * 
 *   self * eigen_vectors(i,:)' = eigen_values(i) * eigen_vectors(i,:)'
 *
 * <b>The contents of <i>self</i> is destroyed by this method</b>.
 *
 * Currently the function is slower than #svd yet less accurate, so if <i>self</i> is known to be positively-defined
 * (e.g., it is a convariation matrix), it is recommanded to use #svd to find eigenvalues and eigenvectors of <i>self</i>,
 * especially if eigenvectors are not required.
 */
VALUE
rb_eigenvv_bang(int argc, VALUE *argv, VALUE self)
{
  VALUE epsilon;
  double eps = rb_scan_args(argc, argv, "01", &epsilon) < 1 ? 0.0 : NUM2DBL(epsilon);
  CvSize size = cvGetSize(CVARR(self));
  int type = cvGetElemType(CVARR(self));
  VALUE eigen_vectors = new_object(size, type), eigen_values = new_object(size.height, 1, type);      
  cvEigenVV(CVARR(self), CVARR(eigen_vectors), CVARR(eigen_values), eps);
  return rb_ary_new3(2, eigen_vectors, eigen_values);
}

/*
 * call-seq:
 *   calc_covar_matrix()
 *
 * not yet.
 *
 */
VALUE
rb_calc_covar_matrix(int argc, VALUE *argv, VALUE self)
{
  rb_raise(rb_eNotImpError, "");
}

/*
 * call-seq:
 *   mahalonobis(vec, mat) -> float
 *
 * not yet.
 */
VALUE
rb_mahalonobis(int argc, VALUE *argv, VALUE self)
{
  rb_raise(rb_eNotImpError, "");
}


/*
 * call-seq:
 *   dft(<i>anyflags...</i>) -> cvmat
 *
 * Performs forward or inverse Discrete Fourier Transform(DFT) of 1D or 2D floating-point array.
 * Argument should be following symbol or combination of these.
 *
 * * :forward or :inverse
 *     Do forward or inverse transform. The result is not scaled.
 * * :scale
 *     Scale the result: divide it by the number of array elements.
 * * :rows
 *     Do forward or inverse transform of every individual row of the self.
 *     This flag allow user to transofrm multiple vectors simulaneously and can be used to decrease the overhand
 *     (which sometimes several times larger then the processing itself), to do 3D and higher-dimensional transforms etc.
 *
 * e.g.
 *   mat.dft(:inverse)
 *   mat.dft(:forward, :scale)  etc...
 */
VALUE
rb_dft(int argc, VALUE *argv, VALUE self)
{
  int type = CV_DXT_FORWARD;
  if (argc > 0) {        
    for (int i = 0; i < argc; i++) {
      type |= CVMETHOD("DXT_FLAG", argv[i]);
    }        
  }
  VALUE dest = new_object(cvGetSize(CVARR(self)), cvGetElemType(CVARR(self)));
  cvDFT(CVARR(self), CVARR(dest), type);
  return dest;
}

/*
 * call-seq:
 *   dct(<i>anyflags...</i>) -> cvmat
 *
 * Performs forward or inverse Discrete Cosine Transform(DCT) of 1D or 2D floating-point array.
 * Argument should be following symbol or combination of these.
 *
 * * :forward or :inverse
 *     Do forward or inverse transform.
 * * :rows
 *     Do forward or inverse transform of every individual row of the self.
 *     This flag allow user to transofrm multiple vectors simulaneously and can be used to decrease the overhand
 *     (which sometimes several times larger then the processing itself), to do 3D and higher-dimensional transforms etc.
 */
VALUE
rb_dct(int argc, VALUE *argv, VALUE self)
{
  int type = CV_DXT_FORWARD;
  if (argc > 0) {        
    for (int i = 0; i < argc; i++) {
      type |= CVMETHOD("DXT_FLAG", argv[i]);
    }
  }
  VALUE dest = new_object(cvGetSize(CVARR(self)), cvGetElemType(CVARR(self)));
  cvDCT(CVARR(self), CVARR(dest), type);
  return dest;
}

/*
 * call-seq:
 *   line(<i>p1, p2[, drawing_option]</i>) -> mat
 *
 * Return image is drawn a line segment connecting two points.
 *
 * <i>drawing_option</i> should be Hash include these keys.
 *   :color
 *      Line color.
 *   :thickness
 *      Line Thickness.
 *   :line_type
 *      Type of the line:
 *        * 0 or 8 - 8-connected line(default).
 *        * 4 - 4-connected line.
 *        * negative-value - antialiased line.
 *   :shift
 *      Number of fractional bits in the point coordinates.
 *
 * note: <i>drawing_option</i>'s default value is CvMat::DRAWING_OPTION.
 *
 * for example
 *   mat = CvMat.new(100, 100)
 *   mat.line(CvPoint.new(10, 10), CvPoint.new(90, 90), :thickness => 3, :line_type => :aa)
 */
VALUE
rb_line(int argc, VALUE *argv, VALUE self)
{
  return rb_line_bang(argc, argv, rb_clone(self));
}

/*
 * call-seq:
 *   line!(<i>p1, p2[, drawing_option]</i>) -> self
 *
 * Draws a line segment connecting two points.
 * Same as CvMat#line, but modifies the receiver in place.
 * see CvMat#line
 */
VALUE
rb_line_bang(int argc, VALUE *argv, VALUE self)
{
  VALUE p1, p2, drawing_option;
  rb_scan_args(argc, argv, "21", &p1, &p2, &drawing_option);
  drawing_option = DRAWING_OPTION(drawing_option);
  cvLine(CVARR(self), VALUE_TO_CVPOINT(p1), VALUE_TO_CVPOINT(p2), 	 
	 DO_COLOR(drawing_option),	 
	 DO_THICKNESS(drawing_option),
	 DO_LINE_TYPE(drawing_option),
	 DO_SHIFT(drawing_option));
  return self;
}

/*
 * call-seq:
 *   rectangle(<i>p1, p2[, drawing_option]</i>) -> mat
 *
 * Return image is drawn a rectangle with two opposite corners <i>p1</i> and <i>p2</i>.
 *
 * <i>drawing_options</i> should be Hash include these keys. 
 *   :color
 *      Line color.
 *   :thickness
 *      Thickness of lines that make up the rectangle.
 *      Negative values make the function to draw a filled rectangle.
 *   :line_type
 *      Type of the line:
 *        * 0 or 8 - 8-connected line(default).
 *        * 4 - 4-connected line.
 *        * negative-value - antialiased line.
 *   :shift
 *      Number of fractional bits in the point coordinates.
 *
 * note: <i>drawing_option</i>'s default value is CvMat::DRAWING_OPTION.
 */
VALUE
rb_rectangle(int argc, VALUE *argv, VALUE self)
{
  return rb_rectangle_bang(argc, argv, rb_clone(self));
}

/*
 * call-seq:
 *   rectangle!(<i>p1, p2[, drawing_option]</i>) -> self
 * 
 * Draws simple, thick or filled rectangle.
 * Same as CvMat#rectangle, but modifies the receiver in place.
 * see CvMat#rectangle
 */
VALUE
rb_rectangle_bang(int argc, VALUE *argv, VALUE self)
{
  VALUE p1, p2, drawing_option;
  rb_scan_args(argc, argv, "21", &p1, &p2, &drawing_option);
  drawing_option = DRAWING_OPTION(drawing_option);
  cvRectangle(CVARR(self), VALUE_TO_CVPOINT(p1), VALUE_TO_CVPOINT(p2), 	 
	      DO_COLOR(drawing_option),	 
	      DO_THICKNESS(drawing_option),
	      DO_LINE_TYPE(drawing_option),
	      DO_SHIFT(drawing_option));
  return self;
}

/*
 * call-seq:
 *   circle(<i>center, radius[,drawing_option]</i>) -> cvmat
 *
 * Return image is drawn a simple or filled circle with given center and radius.
 *
 * <i>drawing_options</i> should be Hash include these keys.
 *   :color
 *      Circle color.
 *   :thickness
 *      Thickness of the circle outline if positive, otherwise that a filled circle has to be drawn.
 *   :line_type
 *      Type of the circle boundary:
 *        * 0 or 8 - 8-connected line(default).
 *        * 4 - 4-connected line.
 *        * negative-value - antialiased line.
 *   :shift
 *      Number of fractional bits in the center coordinates and radius value.
 *
 * note: <i>drawing_option</i>'s default value is CvMat::DRAWING_OPTION.
 */
VALUE
rb_circle(int argc, VALUE *argv, VALUE self)
{
  return rb_circle_bang(argc, argv, rb_clone(self));
}

/*
 * call-seq:
 *   circle!(<i>center, radius[,drawing_option]</i>) -> cvmat
 *
 * Draw a circle.
 * Same as CvMat#circle, but modifies the receiver in place.
 *
 * see CvMat#circle
 */
VALUE
rb_circle_bang(int argc, VALUE *argv, VALUE self)
{
  VALUE center, radius, drawing_option;
  rb_scan_args(argc, argv, "21", &center, &radius, &drawing_option);
  drawing_option = DRAWING_OPTION(drawing_option);
  cvCircle(CVARR(self), VALUE_TO_CVPOINT(center), NUM2INT(radius),
	   DO_COLOR(drawing_option),
	   DO_THICKNESS(drawing_option),
	   DO_LINE_TYPE(drawing_option),
	   DO_SHIFT(drawing_option));
  return self;
}

/*
 * call-seq:
 *   ellipse(<i>center, axis, angle, start_angle, end_angle[,drawing_option]</i>) -> mat
 *
 * Return image is drawn a simple or thick elliptic arc or fills an ellipse sector.
 *
 * <i>drawing_options</i> should be Hash include these keys.
 *   :color
 *      Ellipse color.
 *   :thickness
 *      Thickness of the ellipse arc.
 *   :line_type
 *      Type of the ellipse boundary:
 *        * 0 or 8 - 8-connected line(default).
 *        * 4 - 4-connected line.
 *        * negative-value - antialiased line.
 *   :shift
 *      Number of fractional bits in the center coordinates and axes' value.
 *
 * note: <i>drawing_option</i>'s default value is CvMat::DRAWING_OPTION.
 */
VALUE
rb_ellipse(int argc, VALUE *argv, VALUE self)
{
  return rb_ellipse_bang(argc, argv, rb_clone(self));
}

/*
 * call-seq:
 *   ellipse!(<i>center, axis, angle, start_angle, end_angle[,drawing_option]</i>) -> self
 *
 * Draws simple or thick elliptic arc or fills ellipse sector.
 * Same as CvMat#ellipse, but modifies the receiver in place.
 *
 * see CvMat#ellipse
 */
VALUE
rb_ellipse_bang(int argc, VALUE *argv, VALUE self)
{
  VALUE center, axis, angle, start_angle, end_angle, drawing_option;
  rb_scan_args(argc, argv, "51", &center, &axis, &angle, &start_angle, &end_angle, &drawing_option);
  drawing_option = DRAWING_OPTION(drawing_option);
  cvEllipse(CVARR(self), VALUE_TO_CVPOINT(center),
	    VALUE_TO_CVSIZE(axis),
	    NUM2DBL(angle), NUM2DBL(start_angle), NUM2DBL(end_angle),
	    DO_COLOR(drawing_option),	 
	    DO_THICKNESS(drawing_option),
	    DO_LINE_TYPE(drawing_option),
	    DO_SHIFT(drawing_option));
  return self;
}

/*
 * call-seq:
 *   ellipse_box(<i>box[, drawing_option]</i>) -> mat
 *
 * Return image is drawn a simple or thick ellipse outline, or fills an ellipse.
 * The method provides a convenient way to draw an ellipse approximating some shape.
 *
 * <i>drawing_options</i> should be Hash include these keys.
 *   :color
 *      Ellipse color.
 *   :thickness
 *      Thickness of the ellipse drawn.
 *   :line_type
 *      Type of the ellipse boundary:
 *        * 0 or 8 - 8-connected line(default).
 *        * 4 - 4-connected line.
 *        * negative-value - antialiased line.
 *   :shift
 *      Number of fractional bits in the box vertex coordinates.
 *
 * note: <i>drawing_option</i>'s default value is CvMat::DRAWING_OPTION.
 */
VALUE
rb_ellipse_box(int argc, VALUE *argv, VALUE self)
{
  return rb_ellipse_box_bang(argc, argv, rb_clone(self));
}

/*
 * call-seq:
 *   ellipse_box!(<i>box[, drawing_option]</i>) -> self
 *
 * Draws simple or thick elliptic arc or fills ellipse sector.
 * Same as CvMat#ellipse_box, but modifies the receiver in place.
 *
 * see CvMat#ellipse_box
 */
VALUE
rb_ellipse_box_bang(int argc, VALUE *argv, VALUE self)
{
  VALUE box, drawing_option;
  rb_scan_args(argc, argv, "11", &box, &drawing_option);
  drawing_option = DRAWING_OPTION(drawing_option);
  cvEllipseBox(CVARR(self), VALUE_TO_CVBOX2D(box),
	       DO_COLOR(drawing_option),	 
	       DO_THICKNESS(drawing_option),
	       DO_LINE_TYPE(drawing_option),
	       DO_SHIFT(drawing_option));
  return self;
}

/*
 * call-seq:
 *   fill_poly(<i>points[,drawing_option]</i>) -> mat
 *
 * Return image is filled an area bounded by several polygonal contours.
 * The method fills complex areas, for example, areas with holes, contour self-intersection, etc.
 */
VALUE
rb_fill_poly(int argc, VALUE *argv, VALUE self)
{
  return rb_fill_poly_bang(argc, argv, self);
}

/*
 * call-seq:
 *   fill_poly!(<i>points[,drawing_option]</i>) -> self
 *
 * Fills polygons interior.
 * Same as CvMat#fill_poly, but modifies the receiver in place.
 *
 * drawing_options should be Hash include these keys.
 *   :color
 *      Polygon color.
 *   :line_type
 *      Type of the polygon boundaries:
 *        * 0 or 8 - 8-connected line(default).
 *        * 4 - 4-connected line.
 *        * negative-value - antialiased line.
 *   :shift
 *      Number of fractional bits in the vertex coordinates.
 *
 * note: <i>drawing_option</i>'s default value is CvMat::DRAWING_OPTION. 
 */
VALUE
rb_fill_poly_bang(int argc, VALUE *argv, VALUE self)
{
  VALUE points, drawing_option;
  rb_scan_args(argc, argv, "11", &points, &drawing_option);
  drawing_option = DRAWING_OPTION(drawing_option);
  if (!POINT_SET_P(points))
    rb_raise(rb_eTypeError, "argument 1(points) should be %s.", cCvSeq::rb_class());
  /* // todo : draw multi-sequence polygon
  CvSeq *seq = CVSEQ(points);
  int contours = 1;
  while(seq = seq->h_next)
    contours++;
  int **nps = ALLOCA_N(int*, contours);
  CvPoint **ps = ALLOCA_N(CvPoint*, contours);  
  seq = CVSEQ(points);
  for (int i = 0; i < contours; i++) {    
  }
  */
  int np = CVSEQ(points)->total;
  VALUE tmp = cCvMat::new_object(1, np, CV_32SC2);
  CvPoint *p = (CvPoint*)cvCvtSeqToArray(CVSEQ(points), CVMAT(tmp)->data.ptr, CV_WHOLE_SEQ);
  cvFillPoly(CVARR(self),
	     &p,
	     &np,
	     1, //contours
	     DO_COLOR(drawing_option),	    
	     DO_LINE_TYPE(drawing_option),
	     DO_SHIFT(drawing_option));
  return self;
}

/*
 * call-seq:
 *   fill_convex_poly(<i>points[,drawing_option]</i>) -> mat
 *
 * Return image is filled convex polygon interior.
 * This method is much faster than The function CvMat#fill_poly 
 * and can fill not only the convex polygons but any monotonic polygon,
 * i.e. a polygon whose contour intersects every horizontal line (scan line) 
 * twice at the most.
 *
 * <i>drawing_options</i> should be Hash include these keys.
 *   :color
 *      Polygon color.
 *   :line_type
 *      Type of the polygon boundaries:
 *        * 0 or 8 - 8-connected line(default).
 *        * 4 - 4-connected line.
 *        * negative-value - antialiased line.
 *   :shift
 *      Number of fractional bits in the vertex coordinates.
 *
 * note: <i>drawing_option</i>'s default value is CvMat::DRAWING_OPTION.
 */
VALUE
rb_fill_convex_poly(int argc, VALUE *argv, VALUE self)
{
  return rb_fill_convex_poly_bang(argc, argv, rb_clone(self));
}

/*
 * call-seq:
 *   fill_convex_poly!(<i>points[,drawing_option]</i>) -> self
 *
 * Fills convex polygon.
 * Same as CvMat#fill_convex_poly, but modifies the receiver in place.
 *
 * see CvMat#fill_cnovex_poly
 */
VALUE
rb_fill_convex_poly_bang(int argc, VALUE *argv, VALUE self)
{
  VALUE points, drawing_option;
  rb_scan_args(argc, argv, "11", &points, &drawing_option);
  drawing_option = DRAWING_OPTION(drawing_option);
  if (!POINT_SET_P(points))
    rb_raise(rb_eTypeError, "argument 1(points) should be %s.", cCvSeq::rb_class());
  int np = CVSEQ(points)->total;
  VALUE tmp = cCvMat::new_object(1, np, CV_32SC2);
  CvPoint *p = (CvPoint*)cvCvtSeqToArray(CVSEQ(points), CVMAT(tmp)->data.ptr, CV_WHOLE_SEQ);
  cvFillConvexPoly(CVARR(self),
		   p,
		   np,
		   DO_COLOR(drawing_option),	    
		   DO_LINE_TYPE(drawing_option),
		   DO_SHIFT(drawing_option));
  return self;
}

/*
 * call-seq:
 *   poly_line(<i>points[,drawing_option]</i>) -> mat
 *
 * Return image drawed a single or multiple polygonal curves.
 *
 * <i>drawing_option</i> should be Hash include these keys.
 *   :is_closed
 *      Indicates whether the polylines must be drawn closed.
 *      If closed, the method draws the line from the last vertex
 *      of every contour to the first vertex.
 *   :color
 *      Polyline color.
 *   :thickness
 *      Thickness of the polyline edges
 *   :line_type
 *      Type of line segments:
 *        * 0 or 8 - 8-connected line(default).
 *        * 4 - 4-connected line.
 *        * negative-value - antialiased line.
 *   :shift
 *      Number of fractional bits in the vertex coordinates.
 *
 * note: <i>drawing_option</i>'s default value is CvMat::DRAWING_OPTION.
 */
VALUE
rb_poly_line(int argc, VALUE *argv, VALUE self)
{
  return rb_poly_line_bang(argc, argv, rb_clone(self));
}

/*
 * call-seq:
 *   poly_line!(<i>points[,drawing_option]</i>) -> self
 *
 * Draws simple or thick polygons.
 *
 * Same as CvMat#poly_line, but modifies the receiver in place.
 *
 * see CvMat#poly_line
 */
VALUE
rb_poly_line_bang(int argc, VALUE *argv, VALUE self)
{
  VALUE points, drawing_option;
  rb_scan_args(argc, argv, "11", &points, &drawing_option);
  drawing_option = DRAWING_OPTION(drawing_option);
  /*
  if (!POINT_SET_P(points))
    rb_raise(rb_eTypeError, "argument 1(points) should be %s.", cCvSeq::rb_class());
  int np = CVSEQ(points)->total;
  VALUE tmp = cCvMat::new_object(1, np, CV_32SC2);
  CvPoint *p = (CvPoint*)cvCvtSeqToArray(CVSEQ(points), CVMAT(tmp)->data.ptr, CV_WHOLE_SEQ);
  // todo: multi-sequence polygon
  cvPolyLine(CVARR(self),
	     &p,
	     &np,
	     1, //contour
	     DO_IS_CLOSED(drawing_option),
	     DO_COLOR(drawing_option),
	     DO_THICKNESS(drawing_option),
	     DO_LINE_TYPE(drawing_option),
	     DO_SHIFT(drawing_option));
  */
  CvPoint *pointset = 0;
  int length = CVPOINTS_FROM_POINT_SET(points, &pointset);
  cvPolyLine(CVARR(self),
	     &pointset,
	     &length,
	     1, //contour
	     DO_IS_CLOSED(drawing_option),
	     DO_COLOR(drawing_option),
	     DO_THICKNESS(drawing_option),
	     DO_LINE_TYPE(drawing_option),
	     DO_SHIFT(drawing_option));

  return self;
}

/*
 * call-seq:
 *   put_text(<i>str, point, font[,color]</i>) -> cvmat
 *  
 * Return image is drawn text string.
 * <i>font</i> should be CvFont object.
 */
VALUE
rb_put_text(int argc, VALUE *argv, VALUE self)
{
  return rb_put_text_bang(argc, argv, rb_clone(self));
}

/*
 * call-seq:
 *   put_text!(<i>str, point ,font[,color]</i>) -> self
 *
 * Draws text string. Return self.
 */
VALUE
rb_put_text_bang(int argc, VALUE *argv, VALUE self)
{
  VALUE text, point, font, color;
  rb_scan_args(argc, argv, "22", &text, &point, &font, &color);  
  cvPutText(CVARR(self), StringValueCStr(text), VALUE_TO_CVPOINT(point), CVFONT(font), *CVSCALAR(color));
  return self;
}

/*
 * call-seq:
 *   sobel(<i>xorder,yorder[,aperture_size=3]</i>) -> cvmat
 *
 * Calculates first, second, third or mixed image derivatives using extended Sobel operator.
 * <i>self</i> should be single-channel 8bit signed/unsigned or 32bit floating-point.
 *
 * link:../images/CvMat_sobel.png
 */
VALUE
rb_sobel(int argc, VALUE *argv, VALUE self)
{
  VALUE xorder, yorder, aperture_size, dest;
  if (rb_scan_args(argc, argv, "21", &xorder, &yorder, &aperture_size) < 3)
    aperture_size = INT2FIX(3);
  switch(CV_MAT_DEPTH(CVMAT(self)->type)) {
  case CV_8U:
  case CV_8S:
    dest = new_object(cvGetSize(CVARR(self)), CV_MAKETYPE(CV_16S, 1));
    break;
  case CV_32F:
    dest = new_object(cvGetSize(CVARR(self)), CV_MAKETYPE(CV_32F, 1));
    break;
  default:
    rb_raise(rb_eNotImpError, "source depth should be CV_8U or CV_8S or CV_32F.");
  }
  cvSobel(CVARR(self), CVARR(dest), NUM2INT(xorder), NUM2INT(yorder), NUM2INT(aperture_size));
  return dest;
}

/*
 * call-seq:
 *   laplace(<i>[aperture_size = 3]</i>) -> cvmat
 *
 * Calculates Laplacian of the image.
 * <i>self</i> should be single-channel 8bit signed/unsigned or 32bit floating-point.
 */
VALUE
rb_laplace(int argc, VALUE *argv, VALUE self)
{
  VALUE aperture_size, dest;
  if (rb_scan_args(argc, argv, "01", &aperture_size) < 1)
    aperture_size = INT2FIX(3);
  switch(CV_MAT_DEPTH(CVMAT(self)->type)) {
  case CV_8U:
  case CV_8S:
    dest = new_object(cvGetSize(CVARR(self)), CV_MAKETYPE(CV_16S, 1));
    break;
  case CV_32F:
    dest = new_object(cvGetSize(CVARR(self)), CV_MAKETYPE(CV_32F, 1));
    break;
  default:
    rb_raise(rb_eNotImpError, "source depth should be CV_8U or CV_8S or CV_32F.");
  }
  cvLaplace(CVARR(self), CVARR(dest), NUM2INT(aperture_size));
  return dest;
}

/*
 * call-seq:
 *   canny(<i>thresh1,thresh2[,aperture_size = 3]</i>) -> cvmat
 *
 * Canny algorithm for edge detection.
 */
VALUE
rb_canny(int argc, VALUE *argv, VALUE self)
{
  VALUE thresh1, thresh2, aperture_size;
  if (rb_scan_args(argc, argv, "21", &thresh1, &thresh2, &aperture_size) < 3)
    aperture_size = INT2FIX(3);
  VALUE dest = new_object(cvGetSize(CVARR(self)), cvGetElemType(CVARR(self)));
  cvCanny(CVARR(self), CVARR(dest), NUM2INT(thresh1), NUM2INT(thresh2), NUM2INT(aperture_size));
  return dest;
}

/*
 * call-seq:
 *   pre_corner_detect(<i>[aperture_size = 3]</i>) -> cvmat
 *
 * Calculates feature map for corner detection.
 * <i>aperture_size</i> is parameter for sobel operator(see #sobel).
 *
 * The corners can be found as local maximums of the function.
 */
VALUE
rb_pre_corner_detect(int argc, VALUE *argv, VALUE self)
{
  VALUE aperture_size, dest;
  if (rb_scan_args(argc, argv, "01", &aperture_size) < 1)
    aperture_size = INT2FIX(3);
  dest = new_object(cvGetSize(CVARR(self)), CV_MAKETYPE(CV_32F, 1));
  cvPreCornerDetect(CVARR(self), CVARR(dest), NUM2INT(aperture_size));
  return dest;
}

/*
 * call-seq:
 *   corner_eigenvv(<i>block_size[,aperture_size]</i>) -> cvmat
 *
 * For every pixel considers <i>block_size x block_size</i> neighborhood S(p).
 * It calculates convariation matrix of derivatives over the neighborhood.
 */
VALUE
rb_corner_eigenvv(int argc, VALUE *argv, VALUE self)
{
  VALUE block_size, aperture_size, dest;
  if (rb_scan_args(argc, argv, "11", &block_size, &aperture_size) < 1)
    aperture_size = INT2FIX(3);
  Check_Type(block_size, T_FIXNUM);
  CvSize size = cvGetSize(CVARR(self));
  dest = new_object(cvSize(size.width * 6, size.height), CV_MAKETYPE(CV_32F, 1));
  cvCornerEigenValsAndVecs(CVARR(self), CVARR(dest), NUM2INT(block_size), NUM2INT(aperture_size));
  return dest;
}

/*
 * call-seq:
 *   corner_min_eigen_val(<i>block_size[,aperture_size = 3]</i>) -> cvmat
 *
 * Calculates minimal eigenvalue of gradient matrices for corner detection.
 */
VALUE
rb_corner_min_eigen_val(int argc, VALUE *argv, VALUE self)
{
  VALUE block_size, aperture_size, dest;
  rb_scan_args(argc, argv, "11", &block_size, &aperture_size);
  dest = new_object(cvGetSize(CVARR(self)), CV_MAKETYPE(CV_32F, 1));
  cvCornerMinEigenVal(CVARR(self), CVARR(dest), FIX2INT(block_size), FIX2INT(aperture_size));
  return dest;
}

/*
 * call-seq:
 *   corner_harris(<i>block_size[,aperture_size = 3][,k = 0.04]</i>) -> cvmat
 *
 * Return image Applied Harris edge detector.
 */
VALUE
rb_corner_harris(int argc, VALUE *argv, VALUE self)
{
  SUPPORT_8UC1_ONLY(self);
  VALUE block_size, aperture_size, k, dest;
  rb_scan_args(argc, argv, "12", &block_size, &aperture_size, &k);
  dest = new_object(cvGetSize(CVARR(self)), CV_MAKETYPE(CV_32F, 1));
  cvCornerHarris(CVARR(self), CVARR(dest), FIX2INT(block_size), IF_INT(aperture_size, 3), IF_DBL(k, 0.04));
  return dest;
}

/*
 * call-seq:
 *   find_corner_sub_pix(<i></i>)
 *
 * Refines corner locations.
 * This method iterates to find the sub-pixel accurate location of corners,
 * or radial saddle points, as shown in on the picture below.
 */
VALUE
rbi_find_corner_sub_pix(int argc, VALUE *argv, VALUE self)
{
  /*
  VALUE corners, win, zero_zone, criteria;
  rb_scan_args(argc, argv, "13", &corners, &win, &zero_zone, &criteria);
  if (!rb_obj_is_kind_of(corners, mPointSet::rb_module()))
    rb_raise(rb_eTypeError, "argument 1 (corners) should be %s.", rb_class2name(mPointSet::rb_module()));
  int count = CVSEQ(corners)->total;
  VALUE storage = cCvMemStorage::new_object();
  CvPoint2D32f *pointset = POINTSET2D32f(corners);
  //cvFindCornerSubPix(CVARR(self), pointset, count, VALUE_TO_CVSIZE(win), VALUE_TO_CVSIZE(zero_zone), VALUE_TO_CVTERMCRITERIA(criteria));
  //return cCvSeq::new_sequence();
  */
  return Qnil;
}

VALUE
rb_good_features_to_track(int argc, VALUE *argv, VALUE self)
{  
  VALUE quality_level, min_distance, good_features_to_track_option, eigen, tmp, storage;
  rb_scan_args(argc, argv, "21", &quality_level, &min_distance, &good_features_to_track_option);
  good_features_to_track_option = GOOD_FEATURES_TO_TRACK_OPTION(good_features_to_track_option);
  CvMat *src = CVMAT(self);
  eigen = cCvMat::new_object(cvGetSize(src), CV_MAKETYPE(CV_32F, 1));
  tmp   = cCvMat::new_object(cvGetSize(src), CV_MAKETYPE(CV_32F, 1));
  int np = GF_MAX(good_features_to_track_option);
  if(!(np > 0))
    rb_raise(rb_eArgError, "option :max should be positive value.");
  CvPoint2D32f *p32 = (CvPoint2D32f*)cvAlloc(sizeof(CvPoint2D32f) * np);
  if(!p32)
    rb_raise(rb_eNoMemError, "failed allocate memory.");
  cvGoodFeaturesToTrack(src, CVARR(eigen), CVARR(tmp), p32, &np, NUM2DBL(quality_level), NUM2DBL(min_distance),
			GF_MASK(good_features_to_track_option),
			GF_BLOCK_SIZE(good_features_to_track_option),
			GF_USE_HARRIS(good_features_to_track_option),
			GF_K(good_features_to_track_option));
  storage = cCvMemStorage::new_object();
  CvSeq *pseq = cvCreateSeq(CV_SEQ_POINT_SET, sizeof(CvSeq), sizeof(CvPoint2D32f), CVMEMSTORAGE(storage));
  cvSeqPushMulti(pseq, p32, np);
  cvFree(&p32);
  return cCvSeq::new_sequence(cCvSeq::rb_class(), pseq, cCvPoint2D32f::rb_class(), storage);
}

/*
 * call-seq:
 *   sample_line(p1, p2[,connectivity = 8]) {|pixel| }
 *
 * not yet.
 */
VALUE
rb_sample_line(int argc, VALUE *argv, VALUE self)
{
  /*
  VALUE p1, p2, connectivity;
  if (rb_scan_args(argc, argv, "21", &p1, &p2, &connectivity) < 3)
    connectivity = INT2FIX(8);
  CvPoint point1 = VALUE_TO_CVPOINT(p1), point2 = VALUE_TO_CVPOINT(p2); 
  int size;
  switch(FIX2INT(connectivity)) {
  case 4:
    size = abs(point2.x - point1.x) + abs(point2.y - point1.y) + 1;
    break;
  case 8:
    size = maxint(abs(point2.x - point1.x) + 1, abs(point2.y - point1.y) + 1);
    break;
  default:
    rb_raise(rb_eArgError, "argument 3(connectivity) should be 4 or 8. 8 is default.");
  }
  VALUE buf = cCvMat::new_object(1, size, cvGetElemType(CVARR(self)));
  cvSampleLine(CVARR(self), point1, point2, CVMAT(buf)->data.ptr, FIX2INT(connectivity));
  if (rb_block_given_p()) {
    for(int i = 0; i < size; i++) {
      //Data_Wrap_Struct(cCvScalar::rb_class(), 0, 0, CVMAT(buf)->data.ptr[]);
      //rb_yield(cCvScalar::new_object);
    }
  }
  return buf;
  */
  return Qnil;
}

/*
 * call-seq:
 *   rect_sub_pix(<i>center,size</i>) -> cvmat
 *
 * Retrieves pixel rectangle from image with sub-pixel accuracy.
 * Extracts pixels from <i>self</i>.
 *   dst(x,y) = self(x + center.x - (size.width - 1) * 0.5, y + center.y - (size.height - 1) * 0.5)
 * where the values of pixels at non-integer coordinates are retrived using bilinear iterpolation.
 * Every channel of multiple-channel images is processed independently.
 * Whereas the rectangle center must be inside the image, the whole rectangle may be partially occludedl.
 * In this case, the replication border mode is used to get pixel values beyond the image boundaries.
 */
VALUE
rb_rect_sub_pix(VALUE self, VALUE center, VALUE size)
{
  VALUE dest = new_object(VALUE_TO_CVSIZE(size), cvGetElemType(CVARR(self)));
  cvGetRectSubPix(CVARR(self), CVARR(dest), VALUE_TO_CVPOINT2D32F(center));
  return dest;
}

/*
 * call-seq:
 *   quandrangle_sub_pix(<i>map_matrix</i>) -> cvmat
 *
 * Retrives pixel quadrangle from image with sub-pixel accuracy.
 * Extracts pixel from <i>self</i> at sub-pixel accuracy and store them:
 */
VALUE
rb_quadrangle_sub_pix(VALUE self, VALUE map_matrix, VALUE size)
{
  if (!rb_obj_is_kind_of(map_matrix, cCvMat::rb_class()))
    rb_raise(rb_eTypeError, "argument 1 (map matrix) should be %s (2x3).", rb_class2name(cCvMat::rb_class()));
  VALUE dest = new_object(VALUE_TO_CVSIZE(size), cvGetElemType(CVARR(self)));
  cvGetQuadrangleSubPix(CVARR(self), CVARR(dest), CVMAT(map_matrix));
  return dest;
}

/*
 * call-seq:
 *   resize(<i>size[,interpolation = :linear]</i>) -> cvmat
 *
 * Resize image.
 * <i>interpolation</i> is interpolation method:
 * * :nn
 *   nearest-neighbor interpolation.
 * * :linear
 *   bilinear interpolation (used by default)
 * * :area
 *   resampling using pixel area relation. It is preferred method for image decimation that give moire-free results.
 *   In case of zooming it is similar to NN method.
 * * :cubic
 *   bicubic interpolation.
 * Return <i>self</i> resized image that it fits exactly to <i>size</i>. If ROI is set, the method consideres the ROI as supported as usual.
 */
VALUE
rb_resize(int argc, VALUE *argv, VALUE self)
{
  VALUE size, interpolation;
  rb_scan_args(argc, argv, "11", &size, &interpolation);
  VALUE dest = new_object(VALUE_TO_CVSIZE(size), cvGetElemType(CVARR(self)));
  cvResize(CVARR(self), CVARR(dest), CVMETHOD("INTERPOLATION_METHOD", interpolation, CV_INTER_LINEAR));
  return self;
}

/*
 * call-seq:
 *   warp_affine(<i>map_matrix[,interpolation = :linear][,option = :fill_outliers][,fillval = 0]</i>) -> cvmat
 *
 * Applies affine transformation to the image.
 */
VALUE
rb_warp_affine(int argc, VALUE *argv, VALUE self)
{
  VALUE map_matrix, interpolation, option, fill_value;
  if (rb_scan_args(argc, argv, "13", &map_matrix, &interpolation, &option, &fill_value) < 4)
    fill_value = INT2FIX(0);
  if (!rb_obj_is_kind_of(map_matrix, cCvMat::rb_class()))
    rb_raise(rb_eTypeError, "argument 1 (map matrix) should be %s (2x3).", rb_class2name(cCvMat::rb_class()));
  VALUE dest = new_object(cvGetSize(CVARR(self)), cvGetElemType(CVARR(self)));
  cvWarpAffine(CVARR(self), CVARR(dest), CVMAT(map_matrix),
               CVMETHOD("INTERPOLATION_METHOD", interpolation, CV_INTER_LINEAR) | CVMETHOD("WARP_FLAG", option, CV_WARP_FILL_OUTLIERS), VALUE_TO_CVSCALAR(fill_value));
  return dest;
}

/*
 * call-seq:
 *   CvMat.rotation(<i>center,angle,scale</i>) -> cvmat
 *
 * Create new affine matrix of 2D rotation (2x3 32bit floating-point matrix).
 * <i>center</i> is center of rotation (x, y).
 * <i>angle</i> is rotation angle in degrees.
 * Positive values mean counter-clockwise rotation
 * (the coordinate origin is assumed at top-left corner).
 * <i>scale</i> is isotropic scale factor.
 *
 *  [ a b | (1 - a) * center.x - b * center.y  ]
 *  [-b a | (b * center.x + (1 + a) * center.y ]
 *  where a = scale * cos(angle), b = scale * sin(angle)
 */
VALUE
rb_rotation(VALUE self, VALUE center, VALUE angle, VALUE scale)
{
  VALUE map_matrix = new_object(cvSize(3,2), CV_MAKETYPE(CV_32F, 1));
  cv2DRotationMatrix(VALUE_TO_CVPOINT2D32F(center), NUM2DBL(angle), NUM2DBL(scale), CVMAT(map_matrix));
  return map_matrix;
}

/*
 * call-seq:
 *   warp_perspective(<i>map_matrix[,interpolation=:linear][,option =:fill_outliers][,fillval=0])</i>) -> cvmat
 *
 * Applies perspective transformation to the image.
 */
VALUE
rb_warp_perspective(int argc, VALUE *argv, VALUE self)
{
  VALUE map_matrix, interpolation, option, fillval;
  if (rb_scan_args(argc, argv, "13", &map_matrix, &interpolation, &option, &fillval) < 4)
    fillval = INT2FIX(0);
  if (!rb_obj_is_kind_of(map_matrix, cCvMat::rb_class()))
    rb_raise(rb_eTypeError, "argument 1 (map matrix) should be %s (3x3).", rb_class2name(cCvMat::rb_class()));      
  VALUE dest = new_object(cvGetSize(CVARR(self)), cvGetElemType(CVARR(self)));
  cvWarpPerspective(CVARR(self), CVARR(dest), CVMAT(map_matrix),
                    CVMETHOD("INTERPOLATION_METHOD", interpolation, CV_INTER_LINEAR) | CVMETHOD("WARP_FLAG", option, CV_WARP_FILL_OUTLIERS), VALUE_TO_CVSCALAR(fillval));
  return dest;
}

/*
 * call-seq:
 *   remap(<i>mapx,mapy[,interpolation=:linear][,option=:fill_outliers][,fillval=0]</i>) -> cvmat
 *
 * Applies generic geometrical transformation to the image.
 * Transforms source image using the specified map:
 *   dst(x,y)<-src(mapx(x,y),mapy(x,y))
 * Similar to other geometrical transformations, some interpolation method (specified by user) is used to
 * extract pixels with non-integer coordinates. 
 */
VALUE
rb_remap(int argc, VALUE *argv, VALUE self)
{
  VALUE mapx, mapy, interpolation, option, fillval;
  if (rb_scan_args(argc, argv, "23", &mapx, &mapy, &interpolation, &option, &fillval) < 5)
    fillval = INT2FIX(0);
  if (!rb_obj_is_kind_of(mapx, cCvMat::rb_class()))
    rb_raise(rb_eTypeError, "argument 1 (map of x-coordinates) should be %s(CV_32F and single-channel).", rb_class2name(cCvMat::rb_class()));
  if (!rb_obj_is_kind_of(mapy, cCvMat::rb_class()))    
    rb_raise(rb_eTypeError, "argument 2 (map of y-coordinates) should be %s(CV_32F and single-channel).", rb_class2name(cCvMat::rb_class()));
  VALUE dest = new_object(cvGetSize(CVARR(self)), cvGetElemType(CVARR(self)));
  cvRemap(CVARR(self), CVARR(dest), CVARR(mapx), CVARR(mapy),
          CVMETHOD("INTERPOLATION_METHOD", interpolation, CV_INTER_LINEAR) | CVMETHOD("WARP_FLAG", option, CV_WARP_FILL_OUTLIERS), VALUE_TO_CVSCALAR(fillval));
  return dest;
}

/*
 * call-seq:
 *   log_polar(<i>center, magnitude, </i>)
 *
 * Remaps image to log-polar space.
 */
VALUE
rb_log_polar(int argc, VALUE *argv, VALUE self)
{
  /*
  VALUE size, center, m, flags, fillval, dest;
  rb_scan_args(argc, argv, "3*", &size, &center, &m, &flags);
  dest = cCvMat::new_object();
  cvLogPolar(CVARR(self), CVARR(dest),
	     VALUE_TO_CVPOINT2D32F(center), NUM2DBL(m),
	     CVMETHOD("INTERPOLATION_METHOD", interpolation, CV_INTER_LINEAR) | CVMETHOD("WARP_FLAG", option, CV_WARP_FILL_OUTLIEARS), VALUE_TO_CVSCALAR(fillval));
  return dest;
  */
  return Qnil;
}

/*
 * call-seq:
 *   erode(<i>[element = nil, iteration = 1]</i>) -> cvmat
 *
 * Create erodes image by using arbitrary structuring element.
 * <i>element</i> is structuring element used for erosion.
 * <i>element</i> should be IplConvKernel. If it is nil, a 3x3 rectangular structuring element is used.
 * <i>iterations</i> is number of times erosion is applied.
 */
VALUE
rb_erode(int argc, VALUE *argv, VALUE self)
{
  return rb_erode_bang(argc, argv, rb_clone(self));
}

/*
 * call-seq:
 *   erode!(<i>[element = nil][,iteration = 1]</i>) -> self
 *
 * Erodes image by using arbitrary structuring element.
 * see also #erode.
 */
VALUE
rb_erode_bang(int argc, VALUE *argv, VALUE self)
{
  VALUE element, iteration;
  rb_scan_args(argc, argv, "02", &element, &iteration);
  cvErode(CVARR(self), CVARR(self), IPLCONVKERNEL(element), IF_INT(iteration, 1));
  return self;
}

/*
 * call-seq:
 *   dilate(<i>[element = nil][,iteration = 1]</i>) -> cvmat
 *
 * Create dilates image by using arbitrary structuring element.
 * <i>element</i> is structuring element used for erosion.
 * <i>element</i> should be IplConvKernel. If it is nil, a 3x3 rectangular structuring element is used.
 * <i>iterations</i> is number of times erosion is applied.
 */
VALUE
rb_dilate(int argc, VALUE *argv, VALUE self)
{  
  return rb_dilate_bang(argc, argv, rb_clone(self));
}

/*
 * call-seq:
 *   dilate!(<i>[element = nil][,iteration = 1]</i>) -> self
 *
 * Dilate image by using arbitrary structuring element.
 * see also #dilate.
 */
VALUE
rb_dilate_bang(int argc, VALUE *argv, VALUE self)
{
  VALUE element, iteration;
  rb_scan_args(argc, argv, "02", &element, &iteration);
  cvDilate(CVARR(self), CVARR(self), IPLCONVKERNEL(element), IF_INT(iteration, 1));
  return self;
}

/*
 * call-seq:
 *   morpholohy_open(<i>[element = nil][,iteration = 1]</i>) -> cvmat
 *
 * Performs advanced morphological transformations "Opening".
 *   dilate(erode(src,element),element)
 */
VALUE
rb_morphology_open(int argc, VALUE *argv, VALUE self)
{
  VALUE element, iteration, dest;
  rb_scan_args(argc, argv, "02", &element, &iteration);
  dest = new_object(cvGetSize(CVARR(self)), cvGetElemType(CVARR(self))); 
  cvMorphologyEx(CVARR(self), CVARR(dest), 0, IPLCONVKERNEL(element), CV_MOP_OPEN, IF_INT(iteration, 1));
  return dest;
}

/*
 * call-seq:
 *   morpholohy_close(<i>[element = nil][,iteration = 1]</i>) -> cvmat
 *
 * Performs advanced morphological transformations "Closing".
 *    erode(dilate(src,element),element)
 */
VALUE
rb_morphology_close(int argc, VALUE *argv, VALUE self)
{
  VALUE element, iteration, dest;
  rb_scan_args(argc, argv, "02", &element, &iteration);
  dest = new_object(cvGetSize(CVARR(self)), cvGetElemType(CVARR(self))); 
  cvMorphologyEx(CVARR(self), CVARR(dest), 0, IPLCONVKERNEL(element), CV_MOP_CLOSE, IF_INT(iteration, 1));
  return dest;
}

/*
 * call-seq:
 *   morpholohy_gradient(<i>[element = nil][,iteration = 1]</i>) -> cvmat
 *
 * Performs advanced morphological transformations "Morphological gradient".
 *    dilate(src,element)-erode(src,element)
 */
VALUE
rb_morphology_gradient(int argc, VALUE *argv, VALUE self)
{
  VALUE element, iteration, temp, dest;
  rb_scan_args(argc, argv, "02", &element, &iteration);
  temp = new_object(cvGetSize(CVARR(self)), cvGetElemType(CVARR(self))); 
  dest = new_object(cvGetSize(CVARR(self)), cvGetElemType(CVARR(self))); 
  cvMorphologyEx(CVARR(self), CVARR(dest), CVARR(temp), IPLCONVKERNEL(element), CV_MOP_GRADIENT, IF_INT(iteration, 1));
  return dest;
}

/*
 * call-seq:
 *   morpholohy_tophat(<i>[element = nil][,iteration = 1]</i>) -> cvmat
 *
 * Performs advanced morphological transformations "tophat".
 *    src-open(src,element)
 */
VALUE
rb_morphology_tophat(int argc, VALUE *argv, VALUE self)
{
  VALUE element, iteration, dest;
  rb_scan_args(argc, argv, "02", &element, &iteration);
  dest = new_object(cvGetSize(CVARR(self)), cvGetElemType(CVARR(self))); 
  cvMorphologyEx(CVARR(self), CVARR(dest), 0, IPLCONVKERNEL(element), CV_MOP_TOPHAT, IF_INT(iteration, 1));
  return dest;
}

/*
 * call-seq:
 *   morpholohy_blackhat(<i>[element = nil][,iteration = 1]</i>) -> cvmat
 *
 * Performs advanced morphological transformations "blackhat".
 *    close(src,element)-src
 */
VALUE
rb_morphology_blackhat(int argc, VALUE *argv, VALUE self)
{
  VALUE element, iteration, dest;
  rb_scan_args(argc, argv, "02", &element, &iteration);
  dest = new_object(cvGetSize(CVARR(self)), cvGetElemType(CVARR(self))); 
  cvMorphologyEx(CVARR(self), CVARR(dest), 0, IPLCONVKERNEL(element), CV_MOP_BLACKHAT, IF_INT(iteration, 1));
  return dest;
}

/*
 * call-seq:
 *   smooth_blur_no_scale(<i>[p1 = 3, p2 = 3]</i>) -> cvmat
 *
 * Smooths the image by simple blur with no scaling.
 * * 8bit unsigned -> return 16bit unsigned
 * * 8bit signed -> return 16bit signed
 * * 32bit floating point -> return 32bit floating point
 * <b>support single-channel image only.</b>
 */
VALUE
rb_smooth_blur_no_scale(int argc, VALUE *argv, VALUE self)
{
  SUPPORT_C1_ONLY(self);
  VALUE p1, p2, dest;
  rb_scan_args(argc, argv, "02", &p1, &p2);
  int type = cvGetElemType(CVARR(self)), dest_type;
  switch (CV_MAT_DEPTH(type)) {
  case CV_8U:
    dest_type = CV_16U;
    break;
  case CV_8S:
    dest_type = CV_16S;
    break;
  case CV_32F:
    dest_type = CV_32F;
    break;
  default:
    rb_raise(rb_eNotImpError, "unsupport format. (support 8bit unsigned/signed or 32bit floating point only)");
  }
  dest = new_object(cvGetSize(CVARR(self)), dest_type); 
  cvSmooth(CVARR(self), CVARR(dest), CV_BLUR_NO_SCALE, IF_INT(p1, 3), IF_INT(p2, 0));
  return dest;
}

/*
 * call-seq:
 *   smooth_blur(<i>[p1 = 3, p2 = 3]</i>) -> cvmat
 *
 * Smooths the image by simple blur.
 * Summation over a pixel <i>p1</i> x <i>p2</i> neighborhood with subsequent scaling by 1 / (p1*p2).
 */
VALUE
rb_smooth_blur(int argc, VALUE *argv, VALUE self)
{
  SUPPORT_C1C3_ONLY(self);
  VALUE p1, p2, dest;
  rb_scan_args(argc, argv, "02", &p1, &p2);
  dest = cCvMat::new_object(cvGetSize(CVARR(self)), cvGetElemType(CVARR(self)));
  cvSmooth(CVARR(self), CVARR(dest), CV_BLUR, IF_INT(p1, 3), IF_INT(p2, 0));
  return dest;
}

/*
 * call-seq:
 *   smooth_gaussian(<i>[p1 = 3, p2 = 3, p3 = 0.0, p4 = 0.0]</i>) -> cvmat
 *
 * Smooths the image by gaussian blur.
 * Convolving image with <i>p1</i> x <i>p2</i> Gaussian kernel.
 *
 * <i>p3</i> may specify Gaussian sigma (standard deviation).
 * If it is zero, it is calculated from the kernel size:
 *    sigma = (n/2 - 1)*0.3 + 0.8, where n = p1 for horizontal kernel,
 *                                       n = p2 for vertical kernel.
 *
 * <i>p4</i> is in case of non-square Gaussian kernel the parameter.
 * It may be used to specify a different (from p3) sigma in the vertical direction.
 */
VALUE
rb_smooth_gaussian(int argc, VALUE *argv, VALUE self)
{
  SUPPORT_C1C3_ONLY(self);
  VALUE p1, p2, p3, p4, dest;
  rb_scan_args(argc, argv, "04", &p1, &p2, &p3, p4);
  dest = cCvMat::new_object(cvGetSize(CVARR(self)), cvGetElemType(CVARR(self)));
  cvSmooth(CVARR(self), CVARR(dest), CV_GAUSSIAN, IF_INT(p1, 3), IF_INT(p2, 0), IF_DBL(p3, 0.0), IF_DBL(p4, 0.0));
  return dest;
}

/*
 * call-seq:
 *   smooth_median(<i>[p1 = 3]</i>) -> cvmat
 *
 * Smooths the image by median blur.
 * Finding median of <i>p1</i> x <i>p1</i> neighborhood (i.e. the neighborhood is square).
 */
VALUE
rb_smooth_median(int argc, VALUE *argv, VALUE self)
{
  SUPPORT_8U_ONLY(self);
  SUPPORT_C1C3_ONLY(self);
  VALUE p1, dest;
  rb_scan_args(argc, argv, "01", &p1);
  dest = cCvMat::new_object(cvGetSize(CVARR(self)), cvGetElemType(CVARR(self)));
  cvSmooth(CVARR(self), CVARR(dest), CV_MEDIAN, IF_INT(p1, 3));
  return dest;
}

/*
 * call-seq:
 *   smooth_bilateral(<i>[p1 = 3][p2 = 3]</i>) -> cvmat
 *
 * Smooths the image by bilateral filter.
 * Applying bilateral 3x3 filtering with color sigma=<i>p1</i> and space sigma=<i>p2</i>.
 */
VALUE
rb_smooth_bilateral(int argc, VALUE *argv, VALUE self)
{
  SUPPORT_8U_ONLY(self);
  SUPPORT_C1C3_ONLY(self);
  VALUE p1, p2, dest;
  rb_scan_args(argc, argv, "02", &p1, &p2);
  dest = cCvMat::new_object(cvGetSize(CVARR(self)), cvGetElemType(CVARR(self)));
  cvSmooth(CVARR(self), CVARR(dest), CV_BILATERAL, IF_INT(p1, 3), IF_INT(p2, 0));
  return dest;
}

/*
 * call-seq:
 *   filter2d(<i>kernel[,anchor]</i>) -> cvmat
 *
 * Convolves image with the kernel.
 * Convolution kernel, single-channel floating point matrix (or same depth of self's).
 * If you want to apply different kernels to different channels,
 * split the image using CvMat#split into separate color planes and process them individually.
 */
VALUE
rb_filter2d(int argc, VALUE *argv, VALUE self)
{
  VALUE kernel, anchor, dest;
  rb_scan_args(argc, argv, "11", &kernel, &anchor);
  if (!rb_obj_is_kind_of(kernel, cCvMat::rb_class()))
    rb_raise(rb_eTypeError, "argument 1 (kernel) should be %s.", rb_class2name(cCvMat::rb_class()));
  int type = cvGetElemType(CVARR(kernel));
  dest = cCvMat::new_object(cvGetSize(CVARR(self)), cvGetElemType(CVARR(self)));
  cvFilter2D(CVARR(self), CVARR(dest), CVMAT(kernel), NIL_P(anchor) ? cvPoint(-1,-1) : VALUE_TO_CVPOINT(anchor));
  return dest;
}

/*
 * call-seq:
 *   copy_make_border_constant(<i>size, offset[,value = CvScalar.new(0)]</i>)
 *
 * Copies image and makes border around it.
 * Border is filled with the fixed value, passed as last parameter of the function.
 */
VALUE
rb_copy_make_border_constant(int argc, VALUE *argv, VALUE self)
{
  VALUE size, offset, value, dest;
  rb_scan_args(argc, argv, "21", &size, &offset, &value);
  dest = cCvMat::new_object(VALUE_TO_CVSIZE(size), cvGetElemType(CVARR(self)));
  cvCopyMakeBorder(CVARR(self), CVARR(dest), VALUE_TO_CVPOINT(offset), IPL_BORDER_CONSTANT, NIL_P(value) ? cvScalar(0) : VALUE_TO_CVSCALAR(value));
  return dest;
}

/*
 * call-seq:
 *   copy_make_border_replicate(<i>size, offset</i>)
 *
 * Copies image and makes border around it.
 * The pixels from the top and bottom rows,
 * the left-most and right-most columns are replicated to fill the border.
 */
VALUE
rb_copy_make_border_replicate(int argc, VALUE *argv, VALUE self)
{
  VALUE size, offset, dest;
  rb_scan_args(argc, argv, "20", &size, &offset);
  dest = cCvMat::new_object(VALUE_TO_CVSIZE(size), cvGetElemType(CVARR(self)));
  cvCopyMakeBorder(CVARR(self), CVARR(dest), VALUE_TO_CVPOINT(offset), IPL_BORDER_REPLICATE);
  return dest;
}

/*
 * call-seq:
 *   integral(<i>need_sqsum = false, need_tilted_sum = false</i>) -> [cvmat, cvmat or nil, cvmat or nil]
 *
 * Calculates integral images.
 * If <i>need_sqsum</i> = true, calculate the integral image for squared pixel values.
 * If <i>need_tilted_sum</i> = true, calculate the integral for the image rotated by 45 degrees.
 *
 *   sum(X,Y)=sumx<X,y<Yimage(x,y)
 *   sqsum(X,Y)=sumx<X,y<Yimage(x,y)2
 *   tilted_sum(X,Y)=sumy<Y,abs(x-X)<yimage(x,y)
 *
 * Using these integral images, one may calculate sum, mean, standard deviation over arbitrary up-right or rotated rectangular region of the image in a constant time.
 */
VALUE
rb_integral(int argc, VALUE *argv, VALUE self)
{
  VALUE sum, sqsum, tilted_sum, dest;
  rb_scan_args(argc, argv, "02", &sqsum, &tilted_sum);
  CvSize size = cvSize(cvGetSize(CVARR(self)).width + 1, cvGetSize(CVARR(self)).height + 1);
  int cn = CV_MAT_CN(cvGetElemType(CVARR(self)));
  sum = cCvMat::new_object(size, CV_MAKETYPE(CV_64F, cn));
  sqsum = (sqsum == Qtrue ?  cCvMat::new_object(size, CV_MAKETYPE(CV_64F, cn)) : Qnil);
  tilted_sum = (tilted_sum == Qtrue ?  cCvMat::new_object(size, CV_MAKETYPE(CV_64F, cn)) : Qnil);
  cvIntegral(CVARR(self), CVARR(sum), NIL_P(sqsum) ? NULL : CVARR(sqsum), NIL_P(tilted_sum) ? NULL : CVARR(tilted_sum));
  dest = rb_ary_new3(1, sum);
  if(sqsum)
    rb_ary_push(dest, sqsum);
  if(tilted_sum)
    rb_ary_push(dest, tilted_sum);
  return dest;
}

/*
 * call-seq:
 *   threshold_binary(<i>threshold, max_value[,use_otsu = false]</i>)
 *
 * Applies fixed-level threshold to array elements.
 *
 *   dst(x,y) = max_value, if src(x,y)>threshold
 *              0, otherwise
 */
VALUE
rb_threshold_binary(int argc, VALUE *argv, VALUE self)
{
  VALUE threshold, max_value, use_otsu, dest;
  rb_scan_args(argc, argv, "21", &threshold, &max_value, &use_otsu);
  dest = cCvMat::new_object(cvGetSize(CVARR(self)), cvGetElemType(CVARR(self)));
  cvThreshold(CVARR(self), CVARR(dest), NUM2DBL(threshold), NUM2DBL(max_value), CV_THRESH_BINARY | (use_otsu == Qtrue ? CV_THRESH_OTSU : 0));
  return dest;
}

/*
 * call-seq:
 *   threshold_binary_inverse(<i>threshold, max_value[,use_otsu = false]</i>)
 *
 * Applies fixed-level threshold to array elements.
 *
 *   dst(x,y) = 0, if src(x,y)>threshold
 *              max_value, otherwise
 */
VALUE
rb_threshold_binary_inverse(int argc, VALUE *argv, VALUE self)
{
  VALUE threshold, max_value, use_otsu, dest;
  rb_scan_args(argc, argv, "21", &threshold, &max_value, &use_otsu);
  dest = cCvMat::new_object(cvGetSize(CVARR(self)), cvGetElemType(CVARR(self)));
  cvThreshold(CVARR(self), CVARR(dest), NUM2DBL(threshold), NUM2DBL(max_value), CV_THRESH_BINARY_INV | (use_otsu == Qtrue ? CV_THRESH_OTSU : 0));
  return dest;
}

/*
 * call-seq:
 *   threshold_trunc(<i>threshold[,use_otsu = false]</i>)
 *
 * Applies fixed-level threshold to array elements.
 *
 *   dst(x,y) = threshold, if src(x,y)>threshold
 *              src(x,y), otherwise
 */
VALUE
rb_threshold_trunc(int argc, VALUE *argv, VALUE self)
{
  VALUE threshold, use_otsu, dest;
  rb_scan_args(argc, argv, "11", &threshold, &use_otsu);
  dest = cCvMat::new_object(cvGetSize(CVARR(self)), cvGetElemType(CVARR(self)));
  cvThreshold(CVARR(self), CVARR(dest), NUM2DBL(threshold), 0, CV_THRESH_BINARY_INV | (use_otsu == Qtrue ? CV_THRESH_OTSU : 0));
  return dest;
}

/*
 * call-seq:
 *   threshold_to_zero(<i>threshold[,use_otsu = false]</i>)
 *
 * Applies fixed-level threshold to array elements.
 *
 *   dst(x,y) = src(x,y), if src(x,y)>threshold
 *              0, otherwise
 */
VALUE
rb_threshold_to_zero(int argc, VALUE *argv, VALUE self)
{
  VALUE threshold, use_otsu, dest;
  rb_scan_args(argc, argv, "11", &threshold, &use_otsu);
  dest = cCvMat::new_object(cvGetSize(CVARR(self)), cvGetElemType(CVARR(self)));
  cvThreshold(CVARR(self), CVARR(dest), NUM2DBL(threshold), 0, CV_THRESH_TOZERO | (use_otsu == Qtrue ? CV_THRESH_OTSU : 0));
  return dest;
}

/*
 * call-seq:
 *   threshold_to_zero_inverse(<i>threshold[,use_otsu = false]</i>)
 *
 * Applies fixed-level threshold to array elements.
 *
 *   dst(x,y) = 0, if src(x,y)>threshold
 *              src(x,y), otherwise
 */
VALUE
rb_threshold_to_zero_inverse(int argc, VALUE *argv, VALUE self)
{
  VALUE threshold, use_otsu, dest;
  rb_scan_args(argc, argv, "11", &threshold, &use_otsu);
  dest = cCvMat::new_object(cvGetSize(CVARR(self)), cvGetElemType(CVARR(self)));
  cvThreshold(CVARR(self), CVARR(dest), NUM2DBL(threshold), 0, CV_THRESH_TOZERO_INV | (use_otsu == Qtrue ? CV_THRESH_OTSU : 0));
  return dest;
}

/*
 * call-seq:
 *   pyr_down(<i>[filter = :gaussian_5x5]</i>) -> cvmat
 * 
 * Return downsamples image.
 *
 * This operation performs downsampling step of Gaussian pyramid decomposition.
 * First it convolves source image with the specified filter and then downsamples the image by rejecting even rows and columns.
 * 
 * note: filter - only :gaussian_5x5 is currently supported.
 */
VALUE
rb_pyr_down(int argc, VALUE *argv, VALUE self)
{
  VALUE filter_type, dest;
  rb_scan_args(argc, argv, "01", &filter_type);
  int filter = CV_GAUSSIAN_5x5;
  if (argc > 0) {
    switch (TYPE(filter_type)) {
    case T_SYMBOL:      
      // currently suport CV_GAUSSIAN_5x5 only.
      break;
    default:
      rb_raise(rb_eArgError, "argument 1 (filter_type) should be Symbol.");
    }
  }
  CvSize size = cvGetSize(CVARR(self));
  dest = cCvMat::new_object(size.height / 2, size.width / 2, cvGetElemType(CVARR(self)));
  cvPyrDown(CVARR(self), CVARR(dest), filter);
  return dest;
}

/*
 * call-seq:
 *   pyr_up(<i>[filter = :gaussian_5x5]</i>) -> cvmat
 *
 * Return upsamples image.
 *
 * This operation performs up-sampling step of Gaussian pyramid decomposition.
 * First it upsamples the source image by injecting even zero rows and columns and then convolves result with the specified filter multiplied by 4 for interpolation.
 * So the destination image is four times larger than the source image.
 *
 * note: filter - only :gaussian_5x5 is currently supported.
 */
VALUE
rb_pyr_up(int argc, VALUE *argv, VALUE self)
{
  VALUE filter_type, dest;
  rb_scan_args(argc, argv, "01", &filter_type);
  int filter = CV_GAUSSIAN_5x5;
  if (argc > 0) {
    switch (TYPE(filter_type)) {
    case T_SYMBOL:
      // currently suport CV_GAUSSIAN_5x5 only.
      break;
    default:
      rb_raise(rb_eArgError, "argument 1 (filter_type) should be Symbol.");
    }
  }
  CvSize size = cvGetSize(CVARR(self));
  dest = cCvMat::new_object(size.height * 2, size.width * 2, cvGetElemType(CVARR(self)));
  cvPyrUp(CVARR(self), CVARR(dest), filter);
  return dest;
}

/*
 * call-seq:
 *   flood_fill(<i>seed_point, new_val, lo_diff, up_diff[,flood_fill_option]</i>) -> [cvmat, cvconnectedcomp, iplimage(mask)]
 *
 * Return image filled a connnected compoment with given color.
 * This operation fills a connected component starting from the seed point with the specified color.
 * The connectivity is determined by the closeness of pixel values.
 * The pixel at (x, y) is considered to belong to the repainted domain if:
 *
 *   src(x',y')-lo_diff<=src(x,y)<=src(x',y')+up_diff,     grayscale image, floating range
 *   src(seed.x,seed.y)-lo<=src(x,y)<=src(seed.x,seed.y)+up_diff, grayscale image, fixed range
 *   src(x',y')r-lo_diffr<=src(x,y)r<=src(x',y')r+up_diffr and
 *   src(x',y')g-lo_diffg<=src(x,y)g<=src(x',y')g+up_diffg and
 *   src(x',y')b-lo_diffb<=src(x,y)b<=src(x',y')b+up_diffb, color image, floating range
 *   src(seed.x,seed.y)r-lo_diffr<=src(x,y)r<=src(seed.x,seed.y)r+up_diffr and
 *   src(seed.x,seed.y)g-lo_diffg<=src(x,y)g<=src(seed.x,seed.y)g+up_diffg and
 *   src(seed.x,seed.y)b-lo_diffb<=src(x,y)b<=src(seed.x,seed.y)b+up_diffb, color image, fixed range
 *
 * where src(x',y') is value of one of pixel neighbors.
 * That is, to be added to the connected component, a pixel's color/brightness should be close enough to:
 * * color/brightness of one of its neighbors that are already referred to the connected component in case of floating range
 * * color/brightness of the seed point in case of fixed range. 
 *
 * arguments
 * * seed_point -The starting point. 
 * * new_val - New value of repainted domain pixels.
 * * lo_diff - Maximal lower brightness/color difference between the currently observed pixel and one of its neighbor belong to the component or seed pixel to add the pixel to component. In case of 8-bit color images it is packed value. 
 * * up_diff - Maximal upper brightness/color difference between the currently observed pixel and one of its neighbor belong to the component or seed pixel to add the pixel to component. In case of 8-bit color images it is packed value.
 *
 * and flood_fill_option
 *   :connectivity => 4 or 8, 4 default
 *     Connectivity determines which neighbors of a pixel are considered.
 *   :fixed_range => true or false, false default
 *     If set the difference between the current pixel and seed pixel is considered, otherwise difference between neighbor pixels is considered (the range is floating).
 *   :mask_only => true or false, false default
 *     If set, the function does not fill the image(new_val is ignored), but the fills mask.
 *
 * note: <i>flood_fill_option</i>'s default value is CvMat::FLOOD_FILL_OPTION.
 */
VALUE
rb_flood_fill(int argc, VALUE *argv, VALUE self)
{
  return rb_flood_fill_bang(argc, argv, copy(self));
}

/*
 * call-seq:
 *   flood_fill!(<i>seed_point, new_val, lo_diff, up_diff[,flood_fill_option]</i>) -> [self, cvconnectedcomp, iplimage(mask)]
 *
 * Fills a connected component with given color.
 * see CvMat#flood_fill
 */
VALUE
rb_flood_fill_bang(int argc, VALUE *argv, VALUE self)
{
  VALUE seed_point, new_val, lo_diff, up_diff, flood_fill_option, mask, comp;
  rb_scan_args(argc, argv, "23", &seed_point, &new_val, &lo_diff, &up_diff, &flood_fill_option);
  flood_fill_option = FLOOD_FILL_OPTION(flood_fill_option);
  int flags = FF_CONNECTIVITY(flood_fill_option);
  if (FF_FIXED_RANGE(flood_fill_option)) {
    flags |= CV_FLOODFILL_FIXED_RANGE;
  }
  if (FF_MASK_ONLY(flood_fill_option)) {
    flags |= CV_FLOODFILL_MASK_ONLY;
  }
  CvSize size = cvGetSize(CVARR(self));
  mask = cIplImage::new_object(size.width + 2, size.height + 2, CV_MAKETYPE(CV_8U, 1));
  comp = cCvConnectedComp::new_object();
  cvFloodFill(CVARR(self),
	      VALUE_TO_CVPOINT(seed_point),
	      VALUE_TO_CVSCALAR(new_val),
	      NIL_P(lo_diff) ? cvScalar(0) : VALUE_TO_CVSCALAR(lo_diff),
	      NIL_P(lo_diff) ? cvScalar(0) : VALUE_TO_CVSCALAR(up_diff),
	      CVCONNECTEDCOMP(comp),
	      flags,
	      CVARR(mask));
  cvSetImageROI(IPLIMAGE(mask), cvRect(1, 1, size.width, size.height));
  return rb_ary_new3(3, self, comp, mask);
}

/*
 * call-seq:
 *   find_contours([find_contours_options]) -> cvchain or chcontour or nil
 *
 * Finds contours in binary image, and return contours as CvContour or CvChain.
 * If contours not found, return nil.
 * 
 * <i>flood_fill_option</i> should be Hash include these keys.
 *   :mode - Retrieval mode.
 *      :external - retrive only the extreme outer contours
 *      :list - retrieve all the contours and puts them in the list.(default)
 *      :ccomp - retrieve all the contours and organizes them into two-level hierarchy:
 *               top level are external boundaries of the components, second level are bounda boundaries of the holes
 *      :tree - retrieve all the contours and reconstructs the full hierarchy of nested contours 
 *              Connectivity determines which neighbors of a pixel are considered.
 *   :method - Approximation method.
 *      :code - output contours in the Freeman chain code. All other methods output polygons (sequences of vertices).
 *      :approx_none - translate all the points from the chain code into points;
 *      :approx_simple - compress horizontal, vertical, and diagonal segments, that is, the function leaves only their ending points;(default)
 *      :approx_tc89_l1
 *      :approx_tc89_kcos - apply one of the flavors of Teh-Chin chain approximation algorithm.
 *      If set the difference between the current pixel and seed pixel is considered,
 *      otherwise difference between neighbor pixels is considered (the range is floating).
 *   :offset - Offset, by which every contour point is shifted.
 *             This is useful if the contours are extracted from the image ROI
 *             and then they should be analyzed in the whole image context. Should be CvPoint.
 *
 * note: <i>find_contours_option</i>'s default value is CvMat::FIND_CONTOURS_OPTION.
 *
 * <b>support single-channel 8bit unsigned image only.</b>
 *
 * note: Non-zero pixels are treated as 1's, zero pixels remain 0's
 * that is image treated as binary. To get such a binary image from grayscale,
 * one may use threshold, adaptive_threshold or canny.
 */
VALUE
rb_find_contours(int argc, VALUE *argv, VALUE self)
{
  SUPPORT_8UC1_ONLY(self);
  return rb_find_contours_bang(argc, argv, copy(self));
}

/*
 * call-seq:
 *   find_contours!([find_contours_options]) -> cvchain or chcontour or nil
 *
 * Finds contours in binary image.
 * The function modifies the source image content.
 * (Because the copy is not made, it is slightly faster than find_contours.)
 * 
 * see find_contours
 *
 * <b>support single-channel 8bit unsigned image only.</b>
 */
VALUE
rb_find_contours_bang(int argc, VALUE *argv, VALUE self)
{
  SUPPORT_8UC1_ONLY(self);
  VALUE find_contours_option, klass, element_klass, storage;
  rb_scan_args(argc, argv, "01", &find_contours_option);
  CvSeq *contour = 0;
  find_contours_option = FIND_CONTOURS_OPTION(find_contours_option);
  int mode = FC_MODE(find_contours_option);
  int method = FC_METHOD(find_contours_option);
  int header, header_size, element_size;
  if (method == CV_CHAIN_CODE) {
    klass = cCvChain::rb_class();
    element_klass = cCvChainCode::rb_class();
    header = CV_SEQ_CHAIN_CONTOUR;
    header_size = sizeof(CvChain);
    element_size = sizeof(CvChainCode);
  } else {
    klass = cCvContour::rb_class();
    element_klass = cCvPoint::rb_class();
    header = CV_SEQ_CONTOUR;
    header_size = sizeof(CvContour);
    element_size = sizeof(CvPoint);
  }
  storage = cCvMemStorage::new_object();
  if(cvFindContours(CVARR(self), CVMEMSTORAGE(storage), &contour, header, mode, method, FC_OFFSET(find_contours_option)) == 0)
    return Qnil;
  if(!contour)
    contour = cvCreateSeq(header, header_size, element_size, CVMEMSTORAGE(storage));
  return cCvSeq::new_sequence(klass, contour, element_klass, storage);
}

/*
 * call-seq:
 *   pyr_segmentation(<i>level, threshold1, threshold2</i>) -> [cvmat, cvseq(include cvconnectedcomp)]
 *
 * Does image segmentation by pyramids.
 * The pyramid builds up to the level <i>level<i>.
 * The links between any pixel a on <i>level<i>i and
 * its candidate father pixel b on the adjacent level are established if
 *   p(c(a),c(b)) < threshold1. After the connected components are defined, they are joined into several clusters. Any two segments A and B belong to the same cluster, if
 *   p(c(A),c(B)) < threshold2. The input image has only one channel, then
 *   p(c^2,c^2)=|c^2-c^2|. If the input image has three channels (red, green and blue), then
 *   p(c^2,c^2)=0,3*(c^2 r-c^2 r)+0.59*(c^2 g-c^2 g)+0,11*(c^2 b-c^2 b) . There may be more than one connected component per a cluster.
 *
 * Return segmented image and sequence of connected components.
 * <b>support single-channel or 3-channel 8bit unsigned image only</b>
 */
VALUE
rb_pyr_segmentation(int argc, VALUE *argv, VALUE self)
{
  SUPPORT_8U_ONLY(self);
  SUPPORT_C1C3_ONLY(self);
  VALUE level, threshold1, threshold2, storage, dest;
  rb_scan_args(argc, argv, "30", &level, &threshold1, &threshold2);
  IplImage *src = IPLIMAGE(self);
  int l = FIX2INT(level);
  double t1 = NUM2DBL(threshold1), t2 = NUM2DBL(threshold2);
  if (!(l >0))
    rb_raise(rb_eArgError, "argument 1 (level) should be > 0.");
  if(((src->width | src->height) & ((1 << l) -1 )) != 0)
    rb_raise(rb_eArgError, "bad image size on level %d.", FIX2INT(level));
  if (t1 < 0)
    rb_raise(rb_eArgError, "argument 2 (threshold for establishing the link) should be >= 0.");
  if (t2 < 0)
    rb_raise(rb_eArgError, "argument 3 (threshold for the segments clustering) should be >= 0.");
  dest = cIplImage::new_object(cvGetSize(src), cvGetElemType(src));
  CvSeq *comp = 0;
  storage = cCvMemStorage::new_object();
  
  cvPyrSegmentation(src,
		    IPLIMAGE(dest),
		    CVMEMSTORAGE(storage),
		    &comp,
		    l, t1, t2);
  if(!comp)
    comp = cvCreateSeq(CV_SEQ_CONNECTED_COMP, sizeof(CvSeq), sizeof(CvConnectedComp), CVMEMSTORAGE(storage));
  return rb_ary_new3(2, dest, cCvSeq::new_sequence(cCvSeq::rb_class(), comp, cCvConnectedComp::rb_class(), storage));
}

/*
 * call-seq:
 *   pyr_mean_shift_filtering(<i>sp, sr[,max_level = 1][termcrit = CvTermCriteria.new(5,1)]</i>) -> cvmat
 *
 * Does meanshift image segmentation.
 *
 *   sp - The spatial window radius. 
 *   sr - The color window radius. 
 *   max_level - Maximum level of the pyramid for the segmentation. 
 *   termcrit - Termination criteria: when to stop meanshift iterations. 
 * 
 * This method is implements the filtering stage of meanshift segmentation,
 * that is, the output of the function is the filtered "posterized" image with color gradients and fine-grain texture flattened.
 * At every pixel (X,Y) of the input image (or down-sized input image, see below)
 * the function executes meanshift iterations, that is, the pixel (X,Y) neighborhood in the joint space-color hyperspace is considered:
 *   {(x,y): X-sp≤x≤X+sp && Y-sp≤y≤Y+sp && ||(R,G,B)-(r,g,b)|| ≤ sr},
 * where (R,G,B) and (r,g,b) are the vectors of color components at (X,Y) and (x,y),
 * respectively (though, the algorithm does not depend on the color space used,
 * so any 3-component color space can be used instead).
 * Over the neighborhood the average spatial value (X',Y')
 * and average color vector (R',G',B') are found and they act as the neighborhood center on the next iteration:
 *   (X,Y)~(X',Y'), (R,G,B)~(R',G',B').
 * After the iterations over, the color components of the initial pixel (that is, the pixel from where the iterations started)
 * are set to the final value (average color at the last iteration):
 *   I(X,Y) <- (R*,G*,B*).
 * Then max_level > 0, the gaussian pyramid of max_level+1 levels is built,
 * and the above procedure is run on the smallest layer.
 * After that, the results are propagated to the larger layer and the iterations are run again
 * only on those pixels where the layer colors differ much (>sr) from the lower-resolution layer,
 * that is, the boundaries of the color regions are clarified.
 *
 * Note, that the results will be actually different from the ones obtained by running the meanshift procedure on the whole original image (i.e. when max_level==0).
 */
VALUE
rb_pyr_mean_shift_filtering(int argc, VALUE *argv, VALUE self)
{
  VALUE spatial_window_radius, color_window_radius, max_level, termcrit, dest;
  rb_scan_args(argc, argv, "22", &spatial_window_radius, &color_window_radius, &max_level, &termcrit);
  dest = cCvMat::new_object(cvGetSize(CVARR(self)), cvGetElemType(CVARR(self)));
  cvPyrMeanShiftFiltering(CVARR(self), CVARR(dest),
			  NUM2DBL(spatial_window_radius),
			  NUM2DBL(color_window_radius),
			  IF_INT(max_level, 1),
			  NIL_P(termcrit) ? cvTermCriteria(CV_TERMCRIT_ITER + CV_TERMCRIT_EPS, 5, 1) : VALUE_TO_CVTERMCRITERIA(termcrit));
  return dest;
}

/*
 * call-seq:
 *   watershed -> cvmat(mean markers:cv32s)
 *
 * Does watershed segmentation.
 */
VALUE
rb_watershed(VALUE self)
{
  VALUE markers = cCvMat::new_object(cvGetSize(CVARR(self)), CV_32SC1);
  cvZero(CVARR(markers));
  cvWatershed(CVARR(self), CVARR(markers));
  return markers;
}

/*
 * call-seq:
 *   moments -> array(include CvMoments)
 *
 * Calculates moments.
 */
VALUE
rb_moments(int argc, VALUE *argv, VALUE self)
{
  VALUE is_binary, moments;
  rb_scan_args(argc, argv, "01", &is_binary);
  IplImage image = *IPLIMAGE(self);
  int cn = CV_MAT_CN(cvGetElemType(CVARR(self)));
  moments = rb_ary_new();
  for(int i = 1; i <= cn; i++) {
    cvSetImageCOI(&image, i);
    rb_ary_push(moments, cCvMoments::new_object(&image, TRUE_OR_FALSE(is_binary, 0)));
  }
  return moments;
}

/*
 * call-seq:
 *   hough_line_standard(<i>rho, theta, threshold</i>) -> cvseq(include CvLine)
 *
 * Finds lines in binary image using standard(classical) Hough transform.
 * * rho - Distance resolution in pixel-related units. 
 * * theta - Angle resolution measured in radians. 
 * * threshold - Threshold parameter. A line is returned by the function if the corresponding accumulator value is greater than threshold.
 */
VALUE
rb_hough_lines_standard(int argc, VALUE *argv, VALUE self)
{
  SUPPORT_8UC1_ONLY(self);
  VALUE rho, theta, threshold, storage;
  rb_scan_args(argc, argv, "30", &rho, &theta, &threshold);
  storage = cCvMemStorage::new_object();
  CvSeq *seq = cvHoughLines2(CVARR(copy(self)), CVMEMSTORAGE(storage),
			     CV_HOUGH_STANDARD, NUM2DBL(rho), NUM2DBL(theta), NUM2INT(threshold));
  return cCvSeq::new_sequence(cCvSeq::rb_class(), seq, cCvLine::rb_class(), storage);
}

/*
 * call-seq:
 *   hough_line_probabilistic(<i>rho, theta, threshold, min_length, max_gap</i>) -> cvseq(include CvTwoPoints)
 *
 * Finds lines in binary image using probabilistic Hough transform.
 * * rho - Distance resolution in pixel-related units. 
 * * theta - Angle resolution measured in radians. 
 * * threshold - Threshold parameter. A line is returned by the function if the corresponding accumulator value is greater than threshold.
 * * min_length - The minimum line length.
 * * max_gap - The maximum gap between line segments lieing on the same line to treat them as the single line segment (i.e. to join them).
 */
VALUE
rb_hough_lines_probabilistic(int argc, VALUE *argv, VALUE self)
{
  SUPPORT_8UC1_ONLY(self);
  VALUE rho, theta, threshold, p1, p2, storage;
  rb_scan_args(argc, argv, "50", &rho, &theta, &threshold, &p1, &p2);
  storage = cCvMemStorage::new_object();
  CvSeq *seq = cvHoughLines2(CVARR(copy(self)), CVMEMSTORAGE(storage),
			     CV_HOUGH_PROBABILISTIC, NUM2DBL(rho), NUM2DBL(theta), NUM2INT(threshold),
			     NUM2DBL(p1), NUM2DBL(p2));
  return cCvSeq::new_sequence(cCvSeq::rb_class(), seq, cCvTwoPoints::rb_class(), storage);
}

/*
 * call-seq:
 *   hough_line_multi_scale(<i>rho, theta, threshold, div_rho, div_theta</i>) -> cvseq(include CvLine)
 *
 * Finds lines in binary image using multi-scale variant of classical Hough transform.
 * * rho - Distance resolution in pixel-related units. 
 * * theta - Angle resolution measured in radians. 
 * * threshold - Threshold parameter. A line is returned by the function if the corresponding accumulator value is greater than threshold.
 * * div_rho = divisor for distance resolution rho.
 * * div_theta = divisor for angle resolution theta.
 */
VALUE
rb_hough_lines_multi_scale(int argc, VALUE *argv, VALUE self)
{
  SUPPORT_8UC1_ONLY(self);
  VALUE rho, theta, threshold, p1, p2, storage;
  rb_scan_args(argc, argv, "50", &rho, &theta, &threshold, &p1, &p2);
  storage = cCvMemStorage::new_object();
  CvSeq *seq = cvHoughLines2(CVARR(copy(self)), CVMEMSTORAGE(storage),
			     CV_HOUGH_MULTI_SCALE, NUM2DBL(rho), NUM2DBL(theta), NUM2INT(threshold),
			     NUM2DBL(p1), NUM2DBL(p2));
  return cCvSeq::new_sequence(cCvSeq::rb_class(), seq, cCvLine::rb_class(), storage);
}

/*
 * call-seq:
 *   hough_circles_gradient(<i>dp, min_dist, threshold_canny, threshold_accumulate, min_radius = 0, max_radius = max(width,height)</i>) -> cvseq(include CvCircle32f)
 *
 * Finds circles in grayscale image using Hough transform.
 */
VALUE
rb_hough_circles_gradient(int argc, VALUE *argv, VALUE self)
{
  SUPPORT_8UC1_ONLY(self);
  VALUE dp, min_dist, threshold_canny, threshold_accumulate, min_radius, max_radius, storage;
  rb_scan_args(argc, argv, "24", &dp, &min_dist, &threshold_canny, &threshold_accumulate, &min_radius, &max_radius);
  storage = cCvMemStorage::new_object();
  CvSeq *seq = cvHoughCircles(CVARR(self), CVMEMSTORAGE(storage),
			      CV_HOUGH_GRADIENT, NUM2DBL(dp), NUM2DBL(min_dist),
			      NUM2DBL(threshold_canny), NUM2DBL(threshold_accumulate),
			      IF_INT(min_radius, 0), IF_INT(max_radius, 0));
  return cCvSeq::new_sequence(cCvSeq::rb_class(), seq, cCvCircle32f::rb_class(), storage);
}

/*
 * call-seq:
 *   inpaint_ns(<i>mask, radius</i>) -> cvmat
 *
 * Inpaints the selected region in the image by Navier-Stokes based method.
 * The radius of circlular neighborhood of each point inpainted that is considered by the algorithm.
 */
VALUE
rb_inpaint_ns(VALUE self, VALUE mask, VALUE radius)
{
  SUPPORT_8U_ONLY(self);
  SUPPORT_C1C3_ONLY(self);
  VALUE dest;
  if (!(rb_obj_is_kind_of(mask, cCvMat::rb_class())) || cvGetElemType(CVARR(mask)) != CV_8UC1)
    rb_raise(rb_eTypeError, "argument 1 (mask) should be mask image.");
  dest = cCvMat::new_object(cvGetSize(CVARR(self)), cvGetElemType(CVARR(self)));  
  cvInpaint(CVARR(self), CVARR(mask), CVARR(dest), NUM2DBL(radius), CV_INPAINT_NS);
  return dest;
}

/*
 * call-seq:
 *   inpaint_telea(<i>mask, radius</i>) -> cvmat
 *
 * Inpaints the selected region in the image by The method by Alexandru Telea's method.
 * The radius of circlular neighborhood of each point inpainted that is considered by the algorithm.
 */
VALUE
rb_inpaint_telea(VALUE self, VALUE mask, VALUE radius)
{
  SUPPORT_8U_ONLY(self);
  SUPPORT_C1C3_ONLY(self);
  VALUE dest;
  if (!(rb_obj_is_kind_of(mask, cCvMat::rb_class())) || cvGetElemType(CVARR(mask)) != CV_8UC1)
    rb_raise(rb_eTypeError, "argument 1 (mask) should be mask image.");
  dest = cCvMat::new_object(cvGetSize(CVARR(self)), cvGetElemType(CVARR(self)));
  cvInpaint(CVARR(self), CVARR(mask), CVARR(dest), NUM2DBL(radius), CV_INPAINT_TELEA);
  return dest;
}

/*
 * call-seq:
 *   equalize_hist - cvmat
 *
 * Equalize histgram of grayscale of image.
 *
 * equalizes histogram of the input image using the following algorithm:
 * 1. calculate histogram H for src.
 * 2. normalize histogram, so that the sum of histogram bins is 255.
 * 3. compute integral of the histogram:
 *    H’(i) = sum0≤j≤iH(j)
 * 4. transform the image using H’ as a look-up table: dst(x,y)=H’(src(x,y))
 * The algorithm normalizes brightness and increases contrast of the image.
 *
 * <b>support single-channel 8bit image (grayscale) only.</b>
 */
VALUE
rb_equalize_hist(VALUE self)
{
  SUPPORT_8UC1_ONLY(self);
  VALUE dest = cCvMat::new_object(cvGetSize(CVARR(self)), cvGetElemType(CVARR(self)));
  cvEqualizeHist(CVARR(self), CVARR(dest));
  return dest;
}

/*
 * call-seq:
 *   match_template(<i>template[,method = :sqdiff]</i>) -> cvmat(result)
 *
 * Compares template against overlapped image regions.

 * <i>method</i> is specifies the way the template must be compared with image regions.
 * <i>method</i> should be following symbol. (see CvMat::MATCH_TEMPLATE_METHOD 's key and value.)
 *
 * * :sqdiff
 *   R(x,y)=sumx',y'[T(x',y')-I(x+x',y+y')]2
 * * :sqdiff_normed
 *   R(x,y)=sumx',y'[T(x',y')-I(x+x',y+y')]2/sqrt[sumx',y'T(x',y')2*sumx',y'I(x+x',y+y')2]
 * * :ccorr
 *   R(x,y)=sumx',y'[T(x',y')*I(x+x',y+y')]
 * * :ccorr_normed
 *   R(x,y)=sumx',y'[T(x',y')*I(x+x',y+y')]/sqrt[sumx',y'T(x',y')2*sumx',y'I(x+x',y+y')2]
 * * :ccoeff
 *   R(x,y)=sumx',y'[T'(x',y')*I'(x+x',y+y')],
 *     where T'(x',y')=T(x',y') - 1/(w*h)*sumx",y"T(x",y")
 *   I'(x+x',y+y')=I(x+x',y+y') - 1/(w*h)*sumx",y"I(x+x",y+y")
 * * :ccoeff_normed
 *   R(x,y)=sumx',y'[T'(x',y')*I'(x+x',y+y')]/sqrt[sumx',y'T'(x',y')2*sumx',y'I'(x+x',y+y')2]
 *
 * After the match_template finishes comparison, the best matches can be found as global 
 * minimums (:sqdiff*) or maximums(:ccorr* or :ccoeff*) using minmax function.
 * In case of color image and template summation in both numerator and each sum in denominator
 * is done over all the channels (and separate mean values are used for each channel).
 */
VALUE
rb_match_template(int argc, VALUE *argv, VALUE self)
{
  VALUE templ, method, result;
  rb_scan_args(argc, argv, "11", &templ, &method);
  if (!(rb_obj_is_kind_of(templ, cCvMat::rb_class())))
    rb_raise(rb_eTypeError, "argument 1 (template) should be %s.", rb_class2name(cCvMat::rb_class()));
  if (cvGetElemType(CVARR(self)) != cvGetElemType(CVARR(templ)))
    rb_raise(rb_eTypeError, "template should be same type of self.");
  CvSize src_size = cvGetSize(CVARR(self)), template_size = cvGetSize(CVARR(self));   
  result = cCvMat::new_object(cvSize(src_size.width - template_size.width + 1,
				     src_size.height - template_size.height + 1),
			      CV_32FC1);
  cvMatchTemplate(CVARR(self), CVARR(templ), CVARR(result), CVMETHOD("MATCH_TEMPLATE_METHOD", CV_TM_SQDIFF));
  return result;
}

/*
 * call-seq:
 *   match_shapes_i1(<i>object</i>) -> float
 *
 * Compares two shapes(self and object). <i>object</i> should be CvMat or CvContour.
 *
 * A ~ object1, B - object2):
 *   I1(A,B)=sumi=1..7abs(1/mAi - 1/mBi)
 */
VALUE
rb_match_shapes_i1(int argc, VALUE *argv, VALUE self)
{
  VALUE object;
  rb_scan_args(argc, argv, "10", &object);
  if ((!(rb_obj_is_kind_of(object, cCvMat::rb_class()))) && (!(rb_obj_is_kind_of(object, cCvContour::rb_class()))))
    rb_raise(rb_eTypeError, "argument 1 (shape) should be %s or %s", rb_class2name(cCvMat::rb_class()), rb_class2name(cCvContour::rb_class()));
  return rb_float_new(cvMatchShapes(CVARR(self), CVARR(object), CV_CONTOURS_MATCH_I1));
}

/*
 * call-seq:
 *   match_shapes_i2(<i>object</i>) -> float
 *
 * Compares two shapes(self and object). <i>object</i> should be CvMat or CvContour.
 *
 * A ~ object1, B - object2):
 *   I2(A,B)=sumi=1..7abs(mAi - mBi)
 */
VALUE
rb_match_shapes_i2(int argc, VALUE *argv, VALUE self)
{
  VALUE object;
  rb_scan_args(argc, argv, "10", &object);
  if ((!(rb_obj_is_kind_of(object, cCvMat::rb_class()))) && (!(rb_obj_is_kind_of(object, cCvContour::rb_class()))))
    rb_raise(rb_eTypeError, "argument 1 (shape) should be %s or %s", rb_class2name(cCvMat::rb_class()), rb_class2name(cCvContour::rb_class()));
  return rb_float_new(cvMatchShapes(CVARR(self), CVARR(object), CV_CONTOURS_MATCH_I2));
}

/*
 * call-seq:
 *   match_shapes_i3(<i>object</i>) -> float
 *
 * Compares two shapes(self and object). <i>object</i> should be CvMat or CvContour.
 *
 * A ~ object1, B - object2):
 *   I3(A,B)=sumi=1..7abs(mAi - mBi)/abs(mAi)
 */
VALUE
rb_match_shapes_i3(int argc, VALUE *argv, VALUE self)
{
  VALUE object;
  rb_scan_args(argc, argv, "10", &object);
  if ((!(rb_obj_is_kind_of(object, cCvMat::rb_class()))) && (!(rb_obj_is_kind_of(object, cCvContour::rb_class()))))
    rb_raise(rb_eTypeError, "argument 1 (shape) should be %s or %s", rb_class2name(cCvMat::rb_class()), rb_class2name(cCvContour::rb_class()));
  return rb_float_new(cvMatchShapes(CVARR(self), CVARR(object), CV_CONTOURS_MATCH_I3));
}


/*
 * call-seq:
 *   mean_shift(window, criteria) -> comp
 *
 * Implements CAMSHIFT object tracking algrorithm.
 * First, it finds an object center using mean_shift and, after that,
 * calculates the object size and orientation.
 */
VALUE
rb_mean_shift(VALUE self, VALUE window, VALUE criteria)
{
  VALUE comp = cCvConnectedComp::new_object();  
  cvMeanShift(CVARR(self), VALUE_TO_CVRECT(window), VALUE_TO_CVTERMCRITERIA(criteria), CVCONNECTEDCOMP(comp));
  return comp;
}

/*
 * call-seq:
 *   cam_shift(window, criteria) -> [comp, box]
 *
 * Implements CAMSHIFT object tracking algrorithm. First, it finds an object center using cvMeanShift and,
 * after that, calculates the object size and orientation. The function returns number of iterations made
 * within cvMeanShift.
 */
VALUE
rb_cam_shift(VALUE self, VALUE window, VALUE criteria)
{
  VALUE comp, box;
  comp = cCvConnectedComp::new_object();
  box = cCvBox2D::new_object();
  cvCamShift(CVARR(self), VALUE_TO_CVRECT(window), VALUE_TO_CVTERMCRITERIA(criteria), CVCONNECTEDCOMP(comp), CVBOX2D(box));
  return rb_ary_new3(2, comp, box);
}

/*
 * call-seq:
 *   snake_image(<i>points, alpha, beta, gamma, window, criteria[, calc_gradient = true]</i>) -> cvseq(pointset)
 *
 * Updates snake in order to minimize its total energy that is a sum of internal energy
 * that depends on contour shape (the smoother contour is, the smaller internal energy is)
 * and external energy that depends on the energy field and reaches minimum at the local energy
 * extremums that correspond to the image edges in case of image gradient.
 
 * The parameter criteria.epsilon is used to define the minimal number of points that must be moved
 * during any iteration to keep the iteration process running.
 *
 * If at some iteration the number of moved points is less than criteria.epsilon or
 * the function performed criteria.max_iter iterations, the function terminates.
 *
 * points
 *   Contour points (snake). 
 * alpha
 *   Weight[s] of continuity energy, single float or array of length floats, one per each contour point.
 * beta
 *   Weight[s] of curvature energy, similar to alpha. 
 * gamma
 *   Weight[s] of image energy, similar to alpha.
 * window
 *   Size of neighborhood of every point used to search the minimum, both win.width and win.height must be odd.
 * criteria
 *  Termination criteria.
 * calc_gradient
 *  Gradient flag. If not 0, the function calculates gradient magnitude for every image pixel and consideres
 *  it as the energy field, otherwise the input image itself is considered.
 */
VALUE
rb_snake_image(int argc, VALUE *argv, VALUE self)
{
  VALUE points, alpha, beta, gamma, window, criteria, calc_gradient, storage;
  rb_scan_args(argc, argv, "43", &points, &alpha, &beta, &gamma, &window, &criteria, &calc_gradient);
  CvPoint *pointset = 0;
  CvSeq *seq = 0;
  int length = CVPOINTS_FROM_POINT_SET(points, &pointset);
  int coeff = (TYPE(alpha) == T_ARRAY || TYPE(beta) == T_ARRAY || TYPE(gamma) == T_ARRAY) ? CV_ARRAY : CV_VALUE;
  float *a = 0, *b = 0, *c = 0;
  IplImage stub;  
  if(coeff == CV_VALUE){
    a = ALLOC(float);
    a[0] = (float)NUM2DBL(alpha);
    b = ALLOC(float);
    b[0] = (float)NUM2DBL(beta);
    c = ALLOC(float);
    c[0] = (float)NUM2DBL(gamma);
  }else{ // CV_ARRAY
    rb_raise(rb_eNotImpError, "");
    // todo
  }
  CvSize w = VALUE_TO_CVSIZE(window);
  CvTermCriteria tc = VALUE_TO_CVTERMCRITERIA(criteria);  
  cvSnakeImage(cvGetImage(CVARR(self), &stub), pointset, length,
	       a, b, c, coeff, w, tc, IF_BOOL(calc_gradient, 1, 0, 1));
  storage = cCvMemStorage::new_object();
  seq = cvCreateSeq(CV_SEQ_POINT_SET, sizeof(CvSeq), sizeof(CvPoint), CVMEMSTORAGE(storage));
  cvSeqPushMulti(seq, pointset, length);
  return cCvSeq::new_sequence(cCvSeq::rb_class(), seq, cCvPoint::rb_class(), storage);
}

/*
 * call-seq:
 *   optical_flow_hs(<i>prev[,velx = nil][,vely = nil][,options]</i>) -> [cvmat, cvmat]
 *
 * Calculates optical flow for two images (previous -> self) using Horn & Schunck algorithm.
 * Return horizontal component of the optical flow and vertical component of the optical flow.
 * <i>prev</i> is previous image
 * <i>velx</i> is previous velocity field of x-axis, and <i>vely</i> is previous velocity field of y-axis.
 *  
 * options
 * * :lambda -> should be Float (default is 0.0005)
 *     Lagrangian multiplier.
 * * :criteria -> should be CvTermCriteria object (default is CvTermCriteria(1, 0.001))
 *     Criteria of termination of velocity computing.
 * note: <i>option</i>'s default value is CvMat::OPTICAL_FLOW_HS_OPTION.
 *
 * sample code
 *   velx, vely = nil, nil
 *   while true
 *     current = capture.query
 *     velx, vely = current.optical_flow_hs(prev, velx, vely) if prev
 *     prev = current
 *   emd
 */
VALUE
rb_optical_flow_hs(int argc, VALUE *argv, VALUE self)
{
  SUPPORT_8UC1_ONLY(self);
  VALUE prev, velx, vely, options;
  int use_previous = 0;
  rb_scan_args(argc, argv, "13", &prev, &velx, &vely, &options);
  options = OPTICAL_FLOW_HS_OPTION(options);
  if (!rb_obj_is_kind_of(prev, cCvMat::rb_class()))
    rb_raise(rb_eTypeError, "argument 1 (previous image) should be %s", rb_class2name(cCvMat::rb_class()));
  if (NIL_P(velx) && NIL_P(vely)) {
    velx = cCvMat::new_object(cvGetSize(CVARR(self)), CV_MAKETYPE(CV_32F, 1));
    vely = cCvMat::new_object(cvGetSize(CVARR(self)), CV_MAKETYPE(CV_32F, 1));
  } else {
    if (rb_obj_is_kind_of(velx, cCvMat::rb_class()) && rb_obj_is_kind_of(vely, cCvMat::rb_class()))
      use_previous = 1;
    else
      rb_raise(rb_eArgError, "Necessary to give both argument 2(previous velocity field x) and argument 3(previous velocity field y)");
  }
  cvCalcOpticalFlowHS(CVARR(prev), CVARR(self), use_previous, CVARR(velx), CVARR(vely), HS_LAMBDA(options), HS_CRITERIA(options));
  return rb_ary_new3(2, velx, vely);
}

/*
 * call-seq:
 *   optical_flow_lk(<i>prev, win_size</i>) -> [cvmat, cvmat]
 *
 * Calculates optical flow for two images (previous -> self) using Lucas & Kanade algorithm
 * Return horizontal component of the optical flow and vertical component of the optical flow.
 *
 * <i>win_size</i> is size of the averaging window used for grouping pixels.
 */
VALUE
rb_optical_flow_lk(int argc, VALUE *argv, VALUE self)
{
  SUPPORT_8UC1_ONLY(self);
  VALUE prev, win_size, velx, vely;
  rb_scan_args(argc, argv, "20", &prev, &win_size);
  if (!rb_obj_is_kind_of(prev, cCvMat::rb_class()))
    rb_raise(rb_eTypeError, "argument 1 (previous image) should be %s", rb_class2name(cCvMat::rb_class()));
  velx = cCvMat::new_object(cvGetSize(CVARR(self)), CV_MAKETYPE(CV_32F, 1));
  vely = cCvMat::new_object(cvGetSize(CVARR(self)), CV_MAKETYPE(CV_32F, 1));
  cvCalcOpticalFlowLK(CVARR(prev), CVARR(self), VALUE_TO_CVSIZE(win_size), CVARR(velx), CVARR(vely));
  return rb_ary_new3(2, velx, vely);
}

/*
 * call-seq:
 *   optical_flow_bm(<i>prev[,velx = nil][,vely = nil][,option]</i>) -> [cvmat, cvmat]
 *
 * Calculates optical flow for two images (previous -> self) using block matching method.
 * Return horizontal component of the optical flow and vertical component of the optical flow.
 * <i>prev</i> is previous image.
 * <i>velx</i> is previous velocity field of x-axis, and <i>vely</i> is previous velocity field of y-axis.
 *
 * options
 * * :block_size -> should be CvSize (default is CvSize(4,4))
 *     Size of basic blocks that are compared.
 * * :shift_size -> should be CvSize (default is CvSize(1,1))
 *     Block coordinate increments.
 * * :max_range -> should be CvSize (default is CVSize(4,4))
 *     Size of the scanned neighborhood in pixels around block.
 * note: <i>option</i>'s default value is CvMat::OPTICAL_FLOW_BM_OPTION.
 *
 * Velocity is computed for every block, but not for every pixel, 
 * so velocity image pixels correspond to input image blocks.
 * input/output velocity field's size should be (self.width / block_size.width)x(self.height / block_size.height).
 * e.g. image.size is 320x240 and block_size is 4x4, velocity field's size is 80x60.
 * 
 */
VALUE
rb_optical_flow_bm(int argc, VALUE *argv, VALUE self)
{
  SUPPORT_8UC1_ONLY(self);
  VALUE prev, velx, vely, options;
  int use_previous = 0;
  rb_scan_args(argc, argv, "13", &prev, &velx, &vely, &options);
  options = OPTICAL_FLOW_BM_OPTION(options);
  CvSize
    image_size = cvGetSize(CVARR(self)),
    block_size = BM_BLOCK_SIZE(options),
    shift_size = BM_SHIFT_SIZE(options),
    max_range  = BM_MAX_RANGE(options),
    velocity_size = cvSize(image_size.width / block_size.width, image_size.height / block_size.height);
  if (NIL_P(velx) && NIL_P(vely)) {
    velx = cCvMat::new_object(velocity_size, CV_MAKETYPE(CV_32F, 1));
    vely = cCvMat::new_object(velocity_size, CV_MAKETYPE(CV_32F, 1));
  } else {
    if (rb_obj_is_kind_of(velx, cCvMat::rb_class()) && rb_obj_is_kind_of(vely, cCvMat::rb_class()))
      use_previous = 1;
    else
      rb_raise(rb_eArgError, "Necessary to give both argument 2(previous velocity field x) and argument 3(previous velocity field y)");
  }
  cvCalcOpticalFlowBM(CVARR(prev), CVARR(self),
		      block_size, shift_size, max_range, use_previous,
		      CVARR(velx), CVARR(vely));
  return rb_ary_new3(2, velx, vely);
}

/*
 * call-seq:
 *   CvMat.find_fundamental_mat_7point(<i>points1, points2[,options = {}]</i>) -> fundamental_matrix(cvmat) or nil
 *
 * Calculates fundamental matrix from corresponding points, use for 7-point algorism. Return fundamental matrix(9x3).
 * <i>points1</i> and <i>points2</i> should be 2x7 or 3x7 single-channel, or 1x7 multi-channel matrix.
 * <i>option</i> should be Hash include these keys.
 *   :with_status (true or false)
 *      If set true, return fundamental_matrix and status. [fundamental_matrix, status]
 *      Otherwise return fundamental matrix only(default).
 *
 * note: <i>option</i>'s default value is CvMat::FIND_FUNDAMENTAL_MAT_OPTION.
 * note: 9x3 fundamental matrix means 3x3 three fundamental matrices.
 */
VALUE
rb_find_fundamental_mat_7point(int argc, VALUE *argv, VALUE klass)
{
  VALUE points1, points2, option, fundamental_matrix, status;
  int num = 0;
  rb_scan_args(argc, argv, "21", &points1, &points2, &option);
  option = FIND_FUNDAMENTAL_MAT_OPTION(option);
  fundamental_matrix = cCvMat::new_object(9, 3, CV_32FC1);
  if(FFM_WITH_STATUS(option)){
    status = cCvMat::new_object(cvGetSize(CVARR(points1)), CV_8UC1);
    num = cvFindFundamentalMat(CVMAT(points1), CVMAT(points2), CVMAT(fundamental_matrix), CV_FM_7POINT, 0, 0, CVMAT(status));
    return rb_ary_new3(2, fundamental_matrix, status); 
  }else{
    num = cvFindFundamentalMat(CVMAT(points1), CVMAT(points2), CVMAT(fundamental_matrix), CV_FM_7POINT, 0, 0, NULL);
    return fundamental_matrix;
  }
}


/*
 * call-seq:
 *   CvMat.find_fundamental_mat_8point(<i>points1, points2[,options = {}]</i>) -> fundamental_matrix(cvmat) or nil
 *
 * Calculates fundamental matrix from corresponding points, use for 8-point algorism.
 * <i>points1</i> and <i>points2</i> should be 2x7 or 3x7 single-channel, or 1x7 multi-channel matrix.
 * <i>option</i> should be Hash include these keys.
 *   :with_status (true or false)
 *      If set true, return fundamental_matrix and status. [fundamental_matrix, status]
 *      Otherwise return fundamental matrix only(default).
 *
 * note: <i>option</i>'s default value is CvMat::FIND_FUNDAMENTAL_MAT_OPTION.
 */
VALUE
rb_find_fundamental_mat_8point(int argc, VALUE *argv, VALUE klass)
{
  VALUE points1, points2, option, fundamental_matrix, status;
  int num = 0;
  rb_scan_args(argc, argv, "21", &points1, &points2, &option);
  option = FIND_FUNDAMENTAL_MAT_OPTION(option);
  fundamental_matrix = cCvMat::new_object(3, 3, CV_32FC1);
  if(FFM_WITH_STATUS(option)){
    status = cCvMat::new_object(cvGetSize(CVARR(points1)), CV_8UC1);
    num = cvFindFundamentalMat(CVMAT(points1), CVMAT(points2), CVMAT(fundamental_matrix), CV_FM_8POINT, 0, 0, CVMAT(status));    
    return num == 0 ? Qnil : rb_ary_new3(2, fundamental_matrix, status);
  }else{
    num = cvFindFundamentalMat(CVMAT(points1), CVMAT(points2), CVMAT(fundamental_matrix), CV_FM_8POINT, 0, 0, NULL);
    return num == 0 ? Qnil : fundamental_matrix;
  }
}

/*
 * call-seq:
 *   CvMat.find_fundamental_mat_ransac(<i>points1, points2[,options = {}]</i>) -> fundamental_matrix(cvmat) or nil
 *
 * Calculates fundamental matrix from corresponding points, use for RANSAC algorism.
 * <i>points1</i> and <i>points2</i> should be 2x7 or 3x7 single-channel, or 1x7 multi-channel matrix.
 * <i>option</i> should be Hash include these keys.
 *   :with_status (true or false)
 *      If set true, return fundamental_matrix and status. [fundamental_matrix, status]
 *      Otherwise return fundamental matrix only(default).
 *   :maximum_distance
 *      The maximum distance from point to epipolar line in pixels, beyond which the point is considered an outlier
 *      and is not used for computing the final fundamental matrix. Usually it is set to 0.5 or 1.0.
 *   :desirable_level
 *      It denotes the desirable level of confidence that the matrix is correct.
 *
 * note: <i>option</i>'s default value is CvMat::FIND_FUNDAMENTAL_MAT_OPTION.
 */
VALUE
rb_find_fundamental_mat_ransac(int argc, VALUE *argv, VALUE klass)
{
  VALUE points1, points2, option, fundamental_matrix, status;
  int num = 0;
  rb_scan_args(argc, argv, "21", &points1, &points2, &option);
  option = FIND_FUNDAMENTAL_MAT_OPTION(option);
  fundamental_matrix = cCvMat::new_object(3, 3, CV_32FC1);
  if(FFM_WITH_STATUS(option)){
    status = cCvMat::new_object(cvGetSize(CVARR(points1)), CV_8UC1);
    num = cvFindFundamentalMat(CVMAT(points1), CVMAT(points2), CVMAT(fundamental_matrix), CV_FM_RANSAC, FFM_MAXIMUM_DISTANCE(option), FFM_DESIRABLE_LEVEL(option), CVMAT(status));    
    return num == 0 ? Qnil : rb_ary_new3(2, fundamental_matrix, status);
  }else{
    num = cvFindFundamentalMat(CVMAT(points1), CVMAT(points2), CVMAT(fundamental_matrix), CV_FM_RANSAC, FFM_MAXIMUM_DISTANCE(option), FFM_DESIRABLE_LEVEL(option), NULL);
    return num == 0 ? Qnil : fundamental_matrix;
  }
}

/*
 * call-seq:
 *   CvMat.find_fundamental_mat_lmeds(<i>points1, points2[,options = {}]</i>) -> fundamental_matrix(cvmat) or nil
 *
 * Calculates fundamental matrix from corresponding points, use for LMedS algorism.
 * <i>points1</i> and <i>points2</i> should be 2x7 or 3x7 single-channel, or 1x7 multi-channel matrix.
 * <i>option</i> should be Hash include these keys.
 *   :with_status (true or false)
 *      If set true, return fundamental_matrix and status. [fundamental_matrix, status]
 *      Otherwise return fundamental matrix only(default).
 *   :maximum_distance
 *      The maximum distance from point to epipolar line in pixels, beyond which the point is considered an outlier
 *      and is not used for computing the final fundamental matrix. Usually it is set to 0.5 or 1.0.
 *   :desirable_level
 *      It denotes the desirable level of confidence that the matrix is correct.
 *
 * note: <i>option</i>'s default value is CvMat::FIND_FUNDAMENTAL_MAT_OPTION.
 */
VALUE
rb_find_fundamental_mat_lmeds(int argc, VALUE *argv, VALUE klass)
{
  VALUE points1, points2, option, fundamental_matrix, status;
  int num = 0;
  rb_scan_args(argc, argv, "21", &points1, &points2, &option);
  option = FIND_FUNDAMENTAL_MAT_OPTION(option);
  fundamental_matrix = cCvMat::new_object(3, 3, CV_32FC1);
  if(FFM_WITH_STATUS(option)){
    status = cCvMat::new_object(cvGetSize(CVARR(points1)), CV_8UC1);
    num = cvFindFundamentalMat(CVMAT(points1), CVMAT(points2), CVMAT(fundamental_matrix), CV_FM_LMEDS, FFM_MAXIMUM_DISTANCE(option), FFM_DESIRABLE_LEVEL(option), CVMAT(status));    
    return num == 0 ? Qnil : rb_ary_new3(2, fundamental_matrix, status);
  }else{
    num = cvFindFundamentalMat(CVMAT(points1), CVMAT(points2), CVMAT(fundamental_matrix), CV_FM_LMEDS, FFM_MAXIMUM_DISTANCE(option), FFM_DESIRABLE_LEVEL(option), NULL);
    return num == 0 ? Qnil : fundamental_matrix;
  }
}

/*
 * call-seq:
 *   CvMat.compute_correspond_epilines(<i>points, which_image, fundamental_matrix</i>) -> correspondent_lines(cvmat)
 *
 * For points in one image of stereo pair computes the corresponding epilines in the other image.
 * Finds equation of a line that contains the corresponding point (i.e. projection of the same 3D point)
 * in the other image. Each line is encoded by a vector of 3 elements l=[a,b,c]T, so that:
 *   lT*[x, y, 1]T=0,
 * or
 *   a*x + b*y + c = 0
 * From the fundamental matrix definition (see cvFindFundamentalMatrix discussion), line l2 for a point p1 in the first image (which_image=1) can be computed as:
 *   l2=F*p1
 * and the line l1 for a point p2 in the second image (which_image=1) can be computed as:
 *   l1=FT*p2
 * Line coefficients are defined up to a scale. They are normalized (a2+b2=1) are stored into correspondent_lines.
 */
VALUE
rb_compute_correspond_epilines(VALUE klass, VALUE points, VALUE which_image, VALUE fundamental_matrix)
{
  VALUE correspondent_lines;
  CvSize size = cvGetSize(CVARR(points));
  int n;
  if(size.width <= 3 && size.height >= 7)
    n = size.height;
  else if(size.height <= 3 && size.width >= 7)
    n = size.width;
  else
    rb_raise(rb_eTypeError, "input points should 2xN, Nx2 or 3xN, Nx3 matrix(N >= 7).");
  correspondent_lines = cCvMat::new_object(n, 3, CV_32F);
  cvComputeCorrespondEpilines(CVMAT(points), FIX2INT(which_image), CVMAT(fundamental_matrix), CVMAT(correspondent_lines));
  return correspondent_lines;
}

VALUE
new_object(int rows, int cols, int type)
{
  return OPENCV_OBJECT(rb_klass, cvCreateMat(rows, cols, type));
}

VALUE
new_object(CvSize size, int type)
{
  return OPENCV_OBJECT(rb_klass, cvCreateMat(size.height, size.width, type));
}

__NAMESPACE_END_OPENCV
__NAMESPACE_END_CVMAT
