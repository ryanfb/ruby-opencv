/************************************************************

  opencv.cpp -

  $Author: lsxi $

  Copyright (C) 2005-2008 Masakazu Yonekura

************************************************************/
/*
 * Document-module: OpenCV
 *
 * =What is OpenCV?
 *
 * OpenCV is "Open Source Computer Vision Library".
 * OpenCV is developed by Intel and many opensource developers.
 * This library include many useful function for computer vision, such as object-detection.
 *
 * OpenCV is developed at
 * sourceforge.net[http://sourceforge.net/projects/opencvlibrary]
 *
 * =What is Ruby/OpenCV?
 * Ruby/OpenCV is manual Wrapper of OpenCV (not use SWIG).
 * This library seem to be *Ruby*.
 * * object-oriented
 * * support Garbage Collection by Ruby
 * Ruby/OpenCV is developed
 * http://rubyforge.org/projects/opencv  (Official)
 * and
 * http://blueruby.mydns.jp/pages/opencv (Japanese)
 *
 * =How to install
 * 
 * Show INSTALL
 *
 * =How to generate this documentation?
 * This document created by rdoc.
 * If you have Ruby 1.8 or later, you might use rdoc command.
 * for example
 *   > cd opencv
 *   > rdoc
 * and show "./doc/index.html"
 */
#include "opencv.h"

__NAMESPACE_BEGIN_OPENCV


/*
 * Hashtable for protect from GC
 */
st_table *root_table = st_init_numtable();

/*
 * Mark root object. (protect from GC)
 */
void
mark_root_object(void *ptr)
{
  VALUE value;
  if (ptr && st_lookup(root_table, (st_data_t)ptr, (st_data_t*)&value)) {
    rb_gc_mark(value);
  }
}

/*
 * Look-up Root root object.
 */
VALUE
lookup_root_object(void *ptr)
{
  VALUE value = 0;
  if (ptr)
    st_lookup(root_table, (st_data_t)ptr, (st_data_t*)&value);
  return value;
}

/*
 * Resist root object.
 */
void
resist_root_object(void *ptr, VALUE root)
{
  st_insert(root_table, (st_data_t)ptr, (st_data_t)root);
}

/*
 * Delete mark symbol from hashtable only, not free memory.
 */
void
unresist_object(void *ptr)
{
  st_delete(root_table, (st_data_t*)&ptr, 0);
}

/*
 * Delete mark symbol from hash table, then free memory.
 */
void
free_object(void *ptr)
{
  if (ptr) {
    unresist_object(ptr);
    cvFree(&ptr);
  }
}

/*
 * Release OpenCV specific structure(i.e CvMat, IplImage..) from memory and delete from hashtable.
 */
void
release_object(void *ptr)
{
  if (ptr) {
    unresist_object(ptr);
    cvRelease(&ptr);
  }
}

/*
 * Release IplConvKernel object from memory and delete from hashtable.
 */
void
release_iplconvkernel_object(void *ptr)
{
  if (ptr) {
    unresist_object(ptr);
    cvReleaseStructuringElement((IplConvKernel**)(&ptr));
  }
}

VALUE rb_module;
VALUE rb_opencv_constants;

VALUE
rb_module_opencv()
{
  return rb_module;
}

/*
 * convert OpenCV internal error to Ruby exception.
 */
int
error_callback(int status,
               const char *function_name,
               const char *error_message,
               const char *file_name,
               int line,
               void *user_data)
{
  int error_code = (CvStatus)cvGetErrStatus();
  if (error_code) {
    OPENCV_RSTERR(); // = CV_StsOk
    rb_warn("OpenCV error code (%d) : %s (%d in %s)", error_code, function_name, line, file_name);
    rb_raise(mCvError::by_code(error_code), "%s", error_message);
  }
  return 0;
}

void
define_ruby_module()
{
  if (rb_module)
    return;
  rb_module = rb_define_module("OpenCV");
  
  /* 0: 8bit unsigned */
  rb_define_const(rb_module, "CV_8U", INT2FIX(CV_8U));
  /* 1: 8bit signed */
  rb_define_const(rb_module, "CV_8S", INT2FIX(CV_8S));
  /* 2: 16bit unsigned */
  rb_define_const(rb_module, "CV_16U", INT2FIX(CV_16U));
  /* 3: 16bit signed */
  rb_define_const(rb_module, "CV_16S", INT2FIX(CV_16S));
  /* 4: 32bit signed */
  rb_define_const(rb_module, "CV_32S", INT2FIX(CV_32S));
  /* 5: 32bit floating-point */
  rb_define_const(rb_module, "CV_32F", INT2FIX(CV_32F));
  /* 6: 64bit floating-point */
  rb_define_const(rb_module, "CV_64F", INT2FIX(CV_64F));
  
  /* Color types of loaded images */
  rb_define_const(rb_module, "CV_LOAD_IMAGE_UNCHANGED", INT2FIX(CV_LOAD_IMAGE_UNCHANGED));
  rb_define_const(rb_module, "CV_LOAD_IMAGE_GRAYSCALE", INT2FIX(CV_LOAD_IMAGE_GRAYSCALE));
  rb_define_const(rb_module, "CV_LOAD_IMAGE_COLOR", INT2FIX(CV_LOAD_IMAGE_COLOR));
  rb_define_const(rb_module, "CV_LOAD_IMAGE_ANYDEPTH", INT2FIX(CV_LOAD_IMAGE_ANYDEPTH));
  rb_define_const(rb_module, "CV_LOAD_IMAGE_ANYCOLOR", INT2FIX(CV_LOAD_IMAGE_ANYCOLOR));

  /* Types of morphological operations */
  rb_define_const(rb_module, "CV_MOP_OPEN", INT2FIX(CV_MOP_OPEN));
  rb_define_const(rb_module, "CV_MOP_CLOSE", INT2FIX(CV_MOP_CLOSE));
  rb_define_const(rb_module, "CV_MOP_GRADIENT", INT2FIX(CV_MOP_GRADIENT));
  rb_define_const(rb_module, "CV_MOP_TOPHAT", INT2FIX(CV_MOP_TOPHAT));
  rb_define_const(rb_module, "CV_MOP_BLACKHAT", INT2FIX(CV_MOP_BLACKHAT));

  /* Shape of the structuring elements */
  rb_define_const(rb_module, "CV_SHAPE_RECT", INT2FIX(CV_SHAPE_RECT));
  rb_define_const(rb_module, "CV_SHAPE_CROSS", INT2FIX(CV_SHAPE_CROSS));
  rb_define_const(rb_module, "CV_SHAPE_ELLIPSE", INT2FIX(CV_SHAPE_ELLIPSE));
  rb_define_const(rb_module, "CV_SHAPE_CUSTOM", INT2FIX(CV_SHAPE_CUSTOM));

  /* Types of the smoothing */
  rb_define_const(rb_module, "CV_BLUR_NO_SCALE", INT2FIX(CV_BLUR_NO_SCALE));
  rb_define_const(rb_module, "CV_BLUR", INT2FIX(CV_BLUR));
  rb_define_const(rb_module, "CV_GAUSSIAN", INT2FIX(CV_GAUSSIAN));
  rb_define_const(rb_module, "CV_MEDIAN", INT2FIX(CV_MEDIAN));
  rb_define_const(rb_module, "CV_BILATERAL", INT2FIX(CV_BILATERAL));

  /* Thresholding types */
  rb_define_const(rb_module, "CV_THRESH_BINARY", INT2FIX(CV_THRESH_BINARY));
  rb_define_const(rb_module, "CV_THRESH_BINARY_INV", INT2FIX(CV_THRESH_BINARY_INV));
  rb_define_const(rb_module, "CV_THRESH_TRUNC", INT2FIX(CV_THRESH_TRUNC));
  rb_define_const(rb_module, "CV_THRESH_TOZERO", INT2FIX(CV_THRESH_TOZERO));
  rb_define_const(rb_module, "CV_THRESH_TOZERO_INV", INT2FIX(CV_THRESH_TOZERO_INV));
  rb_define_const(rb_module, "CV_THRESH_OTSU", INT2FIX(CV_THRESH_OTSU));

  /* Retrieval mode */
  rb_define_const(rb_module, "CV_RETR_EXTERNAL", INT2FIX(CV_RETR_EXTERNAL));
  rb_define_const(rb_module, "CV_RETR_LIST", INT2FIX(CV_RETR_LIST));
  rb_define_const(rb_module, "CV_RETR_CCOMP", INT2FIX(CV_RETR_CCOMP));
  rb_define_const(rb_module, "CV_RETR_TREE", INT2FIX(CV_RETR_TREE));
  
  /* Approximation method */
  rb_define_const(rb_module, "CV_CHAIN_CODE", INT2FIX(CV_CHAIN_CODE));
  rb_define_const(rb_module, "CV_CHAIN_APPROX_NONE", INT2FIX(CV_CHAIN_APPROX_NONE));
  rb_define_const(rb_module, "CV_CHAIN_APPROX_SIMPLE", INT2FIX(CV_CHAIN_APPROX_SIMPLE));
  rb_define_const(rb_module, "CV_CHAIN_APPROX_TC89_L1", INT2FIX(CV_CHAIN_APPROX_TC89_L1));
  rb_define_const(rb_module, "CV_CHAIN_APPROX_TC89_KCOS", INT2FIX(CV_CHAIN_APPROX_TC89_KCOS));
  rb_define_const(rb_module, "CV_LINK_RUNS", INT2FIX(CV_LINK_RUNS));

  VALUE inversion_method = rb_hash_new();
  /* {:lu, :svd, :svd_sym(:svd_symmetric)}: Inversion method */
  rb_define_const(rb_module, "INVERSION_METHOD", inversion_method);
  RESIST_CVMETHOD(inversion_method, "lu", CV_LU);
  RESIST_CVMETHOD(inversion_method, "svd", CV_SVD);
  RESIST_CVMETHOD(inversion_method, "svd_sym", CV_SVD_SYM);
  RESIST_CVMETHOD(inversion_method, "svd_symmetric", CV_SVD_SYM);
    
  VALUE dxt_flag = rb_hash_new();
  /* {:forward, :inverse, :scale, :rows}: DFT and DCT flags */
  rb_define_const(rb_module, "DXT_FLAG", dxt_flag);
  RESIST_CVMETHOD(dxt_flag, "forward", CV_DXT_FORWARD);
  RESIST_CVMETHOD(dxt_flag, "inverse", CV_DXT_INVERSE);
  RESIST_CVMETHOD(dxt_flag, "scale", CV_DXT_SCALE);
  RESIST_CVMETHOD(dxt_flag, "rows", CV_DXT_ROWS);
    
  VALUE interpolation_method = rb_hash_new();
  /* {:nn, :linear, :area, :cubic}: Interpolation method */
  rb_define_const(rb_module, "INTERPOLATION_METHOD", interpolation_method);
  RESIST_CVMETHOD(interpolation_method, "nn", CV_INTER_NN);
  RESIST_CVMETHOD(interpolation_method, "linear", CV_INTER_LINEAR);
  RESIST_CVMETHOD(interpolation_method, "area", CV_INTER_AREA);
  RESIST_CVMETHOD(interpolation_method, "cubic", CV_INTER_CUBIC);
  
  VALUE warp_flag = rb_hash_new();
  /* {:fill_outliers, :inverse_map}: Warp affine optional flags */
  rb_define_const(rb_module, "WARP_FLAG", warp_flag);
  RESIST_CVMETHOD(warp_flag, "fill_outliers", CV_WARP_FILL_OUTLIERS);
  RESIST_CVMETHOD(warp_flag, "inverse_map", CV_WARP_INVERSE_MAP);

  VALUE homography_calc_method = rb_hash_new();
  /* {:all, :ransac, :lmeds}: Methods used to computed homography matrix */
  rb_define_const(rb_module, "HOMOGRAPHY_CALC_METHOD", homography_calc_method);
  RESIST_CVMETHOD(homography_calc_method, "all", 0);
  RESIST_CVMETHOD(homography_calc_method, "ransac", CV_RANSAC);
  RESIST_CVMETHOD(homography_calc_method, "lmeds", CV_LMEDS);

  VALUE depth = rb_hash_new();
  /* {:cv8u, :cv8s, :cv16u, :cv16s, :cv32s, :cv32f, :cv64f}: Depth of each pixel. */
  rb_define_const(rb_module, "DEPTH", depth);
  RESIST_CVMETHOD(depth, "cv8u", CV_8U);
  RESIST_CVMETHOD(depth, "cv8s", CV_8S);
  RESIST_CVMETHOD(depth, "cv16u", CV_16U);
  RESIST_CVMETHOD(depth, "cv16s", CV_16S);
  RESIST_CVMETHOD(depth, "cv32s", CV_32S);
  RESIST_CVMETHOD(depth, "cv32f", CV_32F);
  RESIST_CVMETHOD(depth, "cv64f", CV_64F);
  
  VALUE connectivity = rb_hash_new();
  /* {:aa(:anti_alias)}: Determined by the closeness of pixel values */
  rb_define_const(rb_module, "CONNECTIVITY", connectivity);
  RESIST_CVMETHOD(connectivity, "aa", CV_AA);
  RESIST_CVMETHOD(connectivity, "anti_alias", CV_AA);

  VALUE structuring_element_shape = rb_hash_new();
  /* {:rect, :cross, :ellipse, :custom}: Shape of the structuring elements */
  rb_define_const(rb_module, "STRUCTURING_ELEMENT_SHAPE", structuring_element_shape);
  RESIST_CVMETHOD(structuring_element_shape, "rect", CV_SHAPE_RECT);
  RESIST_CVMETHOD(structuring_element_shape, "cross", CV_SHAPE_CROSS);
  RESIST_CVMETHOD(structuring_element_shape, "ellipse", CV_SHAPE_ELLIPSE);
  RESIST_CVMETHOD(structuring_element_shape, "custom", CV_SHAPE_CUSTOM);

  VALUE retrieval_mode = rb_hash_new();
  /* {:external, :list, :ccomp, :tree}: Retrieval mode */
  rb_define_const(rb_module, "RETRIEVAL_MODE", retrieval_mode);
  RESIST_CVMETHOD(retrieval_mode, "external", CV_RETR_EXTERNAL);
  RESIST_CVMETHOD(retrieval_mode, "list", CV_RETR_LIST);
  RESIST_CVMETHOD(retrieval_mode, "ccomp", CV_RETR_CCOMP);
  RESIST_CVMETHOD(retrieval_mode, "tree", CV_RETR_TREE);

  VALUE approx_chain_method = rb_hash_new();
  /* {:code, :approx_none, :approx_simple, :apporx_tc89_11, :approx_tc89_kcos}: Approximation method */
  rb_define_const(rb_module, "APPROX_CHAIN_METHOD", approx_chain_method);
  RESIST_CVMETHOD(approx_chain_method, "code", CV_CHAIN_CODE);
  RESIST_CVMETHOD(approx_chain_method, "approx_none", CV_CHAIN_APPROX_NONE);
  RESIST_CVMETHOD(approx_chain_method, "approx_simple", CV_CHAIN_APPROX_SIMPLE);
  RESIST_CVMETHOD(approx_chain_method, "approx_tc89_l1", CV_CHAIN_APPROX_TC89_L1);
  RESIST_CVMETHOD(approx_chain_method, "approx_tc89_kcos", CV_CHAIN_APPROX_TC89_KCOS);
  
  VALUE approx_poly_method = rb_hash_new();
  /* {:dp}: Approximation method (polygon) */
  rb_define_const(rb_module, "APPROX_POLY_METHOD", approx_poly_method);
  RESIST_CVMETHOD(approx_poly_method, "dp", CV_POLY_APPROX_DP);

  VALUE match_template_method = rb_hash_new();
  /* {:sqdiff, :sqdiff_normed, :ccorr, :ccorr_normed, :ccoeff, :ccoeff_normed}: Match template method */
  rb_define_const(rb_module, "MATCH_TEMPLATE_METHOD", match_template_method);
  RESIST_CVMETHOD(match_template_method, "sqdiff", CV_TM_SQDIFF);
  RESIST_CVMETHOD(match_template_method, "sqdiff_normed", CV_TM_SQDIFF_NORMED);
  RESIST_CVMETHOD(match_template_method, "ccorr", CV_TM_CCORR);
  RESIST_CVMETHOD(match_template_method, "ccorr_normed", CV_TM_CCORR_NORMED);
  RESIST_CVMETHOD(match_template_method, "ccoeff", CV_TM_CCOEFF);
  RESIST_CVMETHOD(match_template_method, "ccoeff_normed", CV_TM_CCOEFF_NORMED);

  VALUE morphological_operation = rb_hash_new();
  /* {:open, :close, :gradient, :tophat, :blackhat}: Types of morphological operations */
  rb_define_const(rb_module, "MORPHOLOGICAL_OPERATION", morphological_operation);
  RESIST_CVMETHOD(morphological_operation, "open", CV_MOP_OPEN);
  RESIST_CVMETHOD(morphological_operation, "close", CV_MOP_CLOSE);
  RESIST_CVMETHOD(morphological_operation, "gradient", CV_MOP_GRADIENT);
  RESIST_CVMETHOD(morphological_operation, "tophat", CV_MOP_TOPHAT);
  RESIST_CVMETHOD(morphological_operation, "blackhat", CV_MOP_BLACKHAT);

  VALUE smoothing_type = rb_hash_new();
  /* {:blur_no_scale, :blur, :gaussian, :median, :bilateral}: Types of smoothing */
  rb_define_const(rb_module, "SMOOTHING_TYPE", smoothing_type);
  RESIST_CVMETHOD(smoothing_type, "blur_no_scale", CV_BLUR_NO_SCALE);
  RESIST_CVMETHOD(smoothing_type, "blur", CV_BLUR);
  RESIST_CVMETHOD(smoothing_type, "gaussian", CV_GAUSSIAN);
  RESIST_CVMETHOD(smoothing_type, "median", CV_MEDIAN);
  RESIST_CVMETHOD(smoothing_type, "bilateral", CV_BILATERAL);

  VALUE threshold_type = rb_hash_new();
  /* {:binary, :binary_inv, :trunc, :tozero, :tozero_inv, :otsu} : Thresholding types */
  rb_define_const(rb_module, "THRESHOLD_TYPE", threshold_type);
  RESIST_CVMETHOD(threshold_type, "binary", CV_THRESH_BINARY);
  RESIST_CVMETHOD(threshold_type, "binary_inv", CV_THRESH_BINARY_INV);
  RESIST_CVMETHOD(threshold_type, "trunc", CV_THRESH_TRUNC);
  RESIST_CVMETHOD(threshold_type, "tozero", CV_THRESH_TOZERO);
  RESIST_CVMETHOD(threshold_type, "tozero_inv", CV_THRESH_TOZERO_INV);
  RESIST_CVMETHOD(threshold_type, "otsu", CV_THRESH_OTSU);

  /* color convert methods */
  rb_define_module_function(rb_module, "BGR2BGRA", RUBY_METHOD_FUNC(rb_BGR2BGRA), 1);
  rb_define_module_function(rb_module, "RGB2RGBA", RUBY_METHOD_FUNC(rb_RGB2RGBA), 1);
  rb_define_module_function(rb_module, "BGRA2BGR", RUBY_METHOD_FUNC(rb_BGRA2BGR), 1);
  rb_define_module_function(rb_module, "RGBA2RGB", RUBY_METHOD_FUNC(rb_RGBA2RGB), 1);
  rb_define_module_function(rb_module, "BGR2RGBA", RUBY_METHOD_FUNC(rb_BGR2RGBA), 1);
  rb_define_module_function(rb_module, "RGB2BGRA", RUBY_METHOD_FUNC(rb_RGB2BGRA), 1);
  rb_define_module_function(rb_module, "RGBA2BGR", RUBY_METHOD_FUNC(rb_RGBA2BGR), 1);
  rb_define_module_function(rb_module, "BGRA2RGB", RUBY_METHOD_FUNC(rb_BGRA2RGB), 1);
  rb_define_module_function(rb_module, "BGR2RGB", RUBY_METHOD_FUNC(rb_BGR2RGB), 1);
  rb_define_module_function(rb_module, "RGB2BGR", RUBY_METHOD_FUNC(rb_RGB2BGR), 1);
  rb_define_module_function(rb_module, "BGRA2RGBA", RUBY_METHOD_FUNC(rb_BGRA2RGBA), 1);
  rb_define_module_function(rb_module, "RGBA2BGRA", RUBY_METHOD_FUNC(rb_RGBA2BGRA), 1);
  rb_define_module_function(rb_module, "BGR2GRAY", RUBY_METHOD_FUNC(rb_BGR2GRAY), 1);
  rb_define_module_function(rb_module, "RGB2GRAY", RUBY_METHOD_FUNC(rb_RGB2GRAY), 1);
  rb_define_module_function(rb_module, "GRAY2BGR", RUBY_METHOD_FUNC(rb_GRAY2BGR), 1);
  rb_define_module_function(rb_module, "GRAY2RGB", RUBY_METHOD_FUNC(rb_GRAY2RGB), 1);
  rb_define_module_function(rb_module, "GRAY2BGRA", RUBY_METHOD_FUNC(rb_GRAY2BGRA), 1);
  rb_define_module_function(rb_module, "GRAY2RGBA", RUBY_METHOD_FUNC(rb_GRAY2RGBA), 1);
  rb_define_module_function(rb_module, "BGRA2GRAY", RUBY_METHOD_FUNC(rb_BGRA2GRAY), 1);
  rb_define_module_function(rb_module, "RGBA2GRAY", RUBY_METHOD_FUNC(rb_RGBA2GRAY), 1);
  rb_define_module_function(rb_module, "BGR2BGR565", RUBY_METHOD_FUNC(rb_BGR2BGR565), 1);
  rb_define_module_function(rb_module, "RGB2BGR565", RUBY_METHOD_FUNC(rb_RGB2BGR565), 1);
  rb_define_module_function(rb_module, "BGR5652BGR", RUBY_METHOD_FUNC(rb_BGR5652BGR), 1);
  rb_define_module_function(rb_module, "BGR5652RGB", RUBY_METHOD_FUNC(rb_BGR5652RGB), 1);
  rb_define_module_function(rb_module, "BGRA2BGR565", RUBY_METHOD_FUNC(rb_BGRA2BGR565), 1);
  rb_define_module_function(rb_module, "RGBA2BGR565", RUBY_METHOD_FUNC(rb_RGBA2BGR565), 1);
  rb_define_module_function(rb_module, "BGR5652BGRA", RUBY_METHOD_FUNC(rb_BGR5652BGRA), 1);
  rb_define_module_function(rb_module, "BGR5652RGBA", RUBY_METHOD_FUNC(rb_BGR5652RGBA), 1);
  rb_define_module_function(rb_module, "GRAY2BGR565", RUBY_METHOD_FUNC(rb_GRAY2BGR565), 1);
  rb_define_module_function(rb_module, "BGR5652GRAY", RUBY_METHOD_FUNC(rb_BGR5652GRAY), 1);
  rb_define_module_function(rb_module, "BGR2BGR555", RUBY_METHOD_FUNC(rb_BGR2BGR555), 1);
  rb_define_module_function(rb_module, "RGB2BGR555", RUBY_METHOD_FUNC(rb_RGB2BGR555), 1);
  rb_define_module_function(rb_module, "BGR5552BGR", RUBY_METHOD_FUNC(rb_BGR5552BGR), 1);
  rb_define_module_function(rb_module, "BGR5552RGB", RUBY_METHOD_FUNC(rb_BGR5552RGB), 1);
  rb_define_module_function(rb_module, "BGRA2BGR555", RUBY_METHOD_FUNC(rb_BGRA2BGR555), 1);
  rb_define_module_function(rb_module, "RGBA2BGR555", RUBY_METHOD_FUNC(rb_RGBA2BGR555), 1);
  rb_define_module_function(rb_module, "BGR5552BGRA", RUBY_METHOD_FUNC(rb_BGR5552BGRA), 1);
  rb_define_module_function(rb_module, "BGR5552RGBA", RUBY_METHOD_FUNC(rb_BGR5552RGBA), 1);
  rb_define_module_function(rb_module, "GRAY2BGR555", RUBY_METHOD_FUNC(rb_GRAY2BGR555), 1);
  rb_define_module_function(rb_module, "BGR5552GRAY", RUBY_METHOD_FUNC(rb_BGR5552GRAY), 1);
  rb_define_module_function(rb_module, "BGR2XYZ", RUBY_METHOD_FUNC(rb_BGR2XYZ), 1);
  rb_define_module_function(rb_module, "RGB2XYZ", RUBY_METHOD_FUNC(rb_RGB2XYZ), 1);
  rb_define_module_function(rb_module, "XYZ2BGR", RUBY_METHOD_FUNC(rb_XYZ2BGR), 1);
  rb_define_module_function(rb_module, "XYZ2RGB", RUBY_METHOD_FUNC(rb_XYZ2RGB), 1);
  rb_define_module_function(rb_module, "BGR2YCrCb", RUBY_METHOD_FUNC(rb_BGR2YCrCb), 1);
  rb_define_module_function(rb_module, "RGB2YCrCb", RUBY_METHOD_FUNC(rb_RGB2YCrCb), 1);
  rb_define_module_function(rb_module, "YCrCb2BGR", RUBY_METHOD_FUNC(rb_YCrCb2BGR), 1);
  rb_define_module_function(rb_module, "YCrCb2RGB", RUBY_METHOD_FUNC(rb_YCrCb2RGB), 1);
  rb_define_module_function(rb_module, "BGR2HSV", RUBY_METHOD_FUNC(rb_BGR2HSV), 1);
  rb_define_module_function(rb_module, "RGB2HSV", RUBY_METHOD_FUNC(rb_RGB2HSV), 1);
  rb_define_module_function(rb_module, "BGR2Lab", RUBY_METHOD_FUNC(rb_BGR2Lab), 1);
  rb_define_module_function(rb_module, "RGB2Lab", RUBY_METHOD_FUNC(rb_RGB2Lab), 1);
  rb_define_module_function(rb_module, "BayerBG2BGR", RUBY_METHOD_FUNC(rb_BayerBG2BGR), 1);
  rb_define_module_function(rb_module, "BayerGB2BGR", RUBY_METHOD_FUNC(rb_BayerGB2BGR), 1);
  rb_define_module_function(rb_module, "BayerRG2BGR", RUBY_METHOD_FUNC(rb_BayerRG2BGR), 1);
  rb_define_module_function(rb_module, "BayerGR2BGR", RUBY_METHOD_FUNC(rb_BayerGR2BGR), 1);
  rb_define_module_function(rb_module, "BayerBG2RGB", RUBY_METHOD_FUNC(rb_BayerBG2RGB), 1);
  rb_define_module_function(rb_module, "BayerGB2RGB", RUBY_METHOD_FUNC(rb_BayerGB2RGB), 1);
  rb_define_module_function(rb_module, "BayerRG2RGB", RUBY_METHOD_FUNC(rb_BayerRG2RGB), 1);
  rb_define_module_function(rb_module, "BayerGR2RGB", RUBY_METHOD_FUNC(rb_BayerGR2RGB), 1);
  rb_define_module_function(rb_module, "BGR2Luv", RUBY_METHOD_FUNC(rb_BGR2Luv), 1);
  rb_define_module_function(rb_module, "RGB2Luv", RUBY_METHOD_FUNC(rb_RGB2Luv), 1);
  rb_define_module_function(rb_module, "BGR2HLS", RUBY_METHOD_FUNC(rb_BGR2HLS), 1);
  rb_define_module_function(rb_module, "RGB2HLS", RUBY_METHOD_FUNC(rb_RGB2HLS), 1);
  rb_define_module_function(rb_module, "HSV2BGR", RUBY_METHOD_FUNC(rb_HSV2BGR), 1);
  rb_define_module_function(rb_module, "HSV2RGB", RUBY_METHOD_FUNC(rb_HSV2RGB), 1);
  rb_define_module_function(rb_module, "Lab2BGR", RUBY_METHOD_FUNC(rb_Lab2BGR), 1);
  rb_define_module_function(rb_module, "Lab2RGB", RUBY_METHOD_FUNC(rb_Lab2RGB), 1);
  rb_define_module_function(rb_module, "Luv2BGR", RUBY_METHOD_FUNC(rb_Luv2BGR), 1);
  rb_define_module_function(rb_module, "Luv2RGB", RUBY_METHOD_FUNC(rb_Luv2RGB), 1);
  rb_define_module_function(rb_module, "HLS2BGR", RUBY_METHOD_FUNC(rb_HLS2BGR), 1);
  rb_define_module_function(rb_module, "HLS2RGB", RUBY_METHOD_FUNC(rb_HLS2RGB), 1);
}

#define CREATE_CVTCOLOR_FUNC(rb_func_name, c_const_name, src_cn, dest_cn) \
  VALUE rb_func_name(VALUE klass, VALUE image)				\
  {									\
    VALUE dest;								\
    if (!rb_obj_is_kind_of(image, cCvMat::rb_class()))			\
      rb_raise(rb_eTypeError, "argument 1 should be %s.", rb_class2name(cCvMat::rb_class())); \
    int type = cvGetElemType(CVARR(image));				\
    if (CV_MAT_CN(type) != src_cn)					\
      rb_raise(rb_eTypeError, "argument 1 should be %d-channel.", src_cn); \
    dest = cIplImage::new_object(cvGetSize(CVARR(image)), CV_MAKETYPE(CV_MAT_DEPTH(type), dest_cn)); \
    cvCvtColor(CVARR(image), CVARR(dest), c_const_name);		\
    return dest;							\
  }

CREATE_CVTCOLOR_FUNC(rb_BGR2BGRA, CV_BGR2BGRA, 3, 4);
CREATE_CVTCOLOR_FUNC(rb_RGB2RGBA, CV_RGB2RGBA, 3, 4);
CREATE_CVTCOLOR_FUNC(rb_BGRA2BGR, CV_BGRA2BGR, 4, 3);
CREATE_CVTCOLOR_FUNC(rb_RGBA2RGB, CV_RGBA2RGB, 4, 3);
CREATE_CVTCOLOR_FUNC(rb_BGR2RGBA, CV_BGR2RGBA, 3, 4);
CREATE_CVTCOLOR_FUNC(rb_RGB2BGRA, CV_RGB2BGRA, 3, 4);
CREATE_CVTCOLOR_FUNC(rb_RGBA2BGR, CV_RGBA2BGR, 4, 3);
CREATE_CVTCOLOR_FUNC(rb_BGRA2RGB, CV_BGRA2RGB, 4, 3);
CREATE_CVTCOLOR_FUNC(rb_BGR2RGB, CV_BGR2RGB, 3, 3);
CREATE_CVTCOLOR_FUNC(rb_RGB2BGR, CV_RGB2BGR, 3, 3);
CREATE_CVTCOLOR_FUNC(rb_BGRA2RGBA, CV_BGRA2RGBA, 4, 4);
CREATE_CVTCOLOR_FUNC(rb_RGBA2BGRA, CV_RGBA2BGRA, 4, 4);
CREATE_CVTCOLOR_FUNC(rb_BGR2GRAY, CV_BGR2GRAY, 3, 1);
CREATE_CVTCOLOR_FUNC(rb_RGB2GRAY, CV_RGB2GRAY, 3, 1);
CREATE_CVTCOLOR_FUNC(rb_GRAY2BGR, CV_GRAY2BGR, 1, 3);
CREATE_CVTCOLOR_FUNC(rb_GRAY2RGB, CV_GRAY2RGB, 1, 3);
CREATE_CVTCOLOR_FUNC(rb_GRAY2BGRA, CV_GRAY2BGRA, 1, 4);
CREATE_CVTCOLOR_FUNC(rb_GRAY2RGBA, CV_GRAY2RGBA, 1, 4);
CREATE_CVTCOLOR_FUNC(rb_BGRA2GRAY, CV_BGRA2GRAY, 4, 1);
CREATE_CVTCOLOR_FUNC(rb_RGBA2GRAY, CV_RGBA2GRAY, 4, 1);
CREATE_CVTCOLOR_FUNC(rb_BGR2BGR565, CV_BGR2BGR565, 3, 3);
CREATE_CVTCOLOR_FUNC(rb_RGB2BGR565, CV_RGB2BGR565, 3, 3);
CREATE_CVTCOLOR_FUNC(rb_BGR5652BGR, CV_BGR5652BGR, 3, 3);
CREATE_CVTCOLOR_FUNC(rb_BGR5652RGB, CV_BGR5652RGB, 3, 3);
CREATE_CVTCOLOR_FUNC(rb_BGRA2BGR565, CV_BGRA2BGR565, 4, 3);
CREATE_CVTCOLOR_FUNC(rb_RGBA2BGR565, CV_RGBA2BGR565, 4, 3);
CREATE_CVTCOLOR_FUNC(rb_BGR5652BGRA, CV_BGR5652BGRA, 3, 4);
CREATE_CVTCOLOR_FUNC(rb_BGR5652RGBA, CV_BGR5652RGBA, 3, 4);
CREATE_CVTCOLOR_FUNC(rb_GRAY2BGR565, CV_GRAY2BGR565, 1, 3);
CREATE_CVTCOLOR_FUNC(rb_BGR5652GRAY, CV_BGR5652GRAY, 3, 1);
CREATE_CVTCOLOR_FUNC(rb_BGR2BGR555, CV_BGR2BGR555, 3, 3);
CREATE_CVTCOLOR_FUNC(rb_RGB2BGR555, CV_RGB2BGR555, 3, 3);
CREATE_CVTCOLOR_FUNC(rb_BGR5552BGR, CV_BGR5552BGR, 3, 3);
CREATE_CVTCOLOR_FUNC(rb_BGR5552RGB, CV_BGR5552RGB, 3, 3);
CREATE_CVTCOLOR_FUNC(rb_BGRA2BGR555, CV_BGRA2BGR555, 4, 3);
CREATE_CVTCOLOR_FUNC(rb_RGBA2BGR555, CV_RGBA2BGR555, 4, 3);
CREATE_CVTCOLOR_FUNC(rb_BGR5552BGRA, CV_BGR5552BGRA, 3, 4);
CREATE_CVTCOLOR_FUNC(rb_BGR5552RGBA, CV_BGR5552RGBA, 3, 4);
CREATE_CVTCOLOR_FUNC(rb_GRAY2BGR555, CV_GRAY2BGR555, 1, 3);
CREATE_CVTCOLOR_FUNC(rb_BGR5552GRAY, CV_BGR5552GRAY, 3, 1);
CREATE_CVTCOLOR_FUNC(rb_BGR2XYZ, CV_BGR2XYZ, 3, 3);
CREATE_CVTCOLOR_FUNC(rb_RGB2XYZ, CV_RGB2XYZ, 3, 3);
CREATE_CVTCOLOR_FUNC(rb_XYZ2BGR, CV_XYZ2BGR, 3, 3);
CREATE_CVTCOLOR_FUNC(rb_XYZ2RGB, CV_XYZ2RGB, 3, 3);
CREATE_CVTCOLOR_FUNC(rb_BGR2YCrCb, CV_BGR2YCrCb, 3, 3);
CREATE_CVTCOLOR_FUNC(rb_RGB2YCrCb, CV_RGB2YCrCb, 3, 3);
CREATE_CVTCOLOR_FUNC(rb_YCrCb2BGR, CV_YCrCb2BGR, 3, 3);
CREATE_CVTCOLOR_FUNC(rb_YCrCb2RGB, CV_YCrCb2RGB, 0, 3);
CREATE_CVTCOLOR_FUNC(rb_BGR2HSV, CV_BGR2HSV, 3, 3);
CREATE_CVTCOLOR_FUNC(rb_RGB2HSV, CV_RGB2HSV, 3, 3);
CREATE_CVTCOLOR_FUNC(rb_BGR2Lab, CV_BGR2Lab, 3, 3);
CREATE_CVTCOLOR_FUNC(rb_RGB2Lab, CV_RGB2Lab, 3, 3);
CREATE_CVTCOLOR_FUNC(rb_BayerBG2BGR, CV_BayerBG2BGR, 3, 3);
CREATE_CVTCOLOR_FUNC(rb_BayerGB2BGR, CV_BayerGB2BGR, 3, 3);
CREATE_CVTCOLOR_FUNC(rb_BayerRG2BGR, CV_BayerRG2BGR, 3, 3);
CREATE_CVTCOLOR_FUNC(rb_BayerGR2BGR, CV_BayerGR2BGR, 3, 3);
CREATE_CVTCOLOR_FUNC(rb_BayerBG2RGB, CV_BayerBG2RGB, 3, 3);
CREATE_CVTCOLOR_FUNC(rb_BayerGB2RGB, CV_BayerGB2RGB, 3, 3);
CREATE_CVTCOLOR_FUNC(rb_BayerRG2RGB, CV_BayerRG2RGB, 3, 3);
CREATE_CVTCOLOR_FUNC(rb_BayerGR2RGB, CV_BayerGR2RGB, 3, 3);
CREATE_CVTCOLOR_FUNC(rb_BGR2Luv, CV_BGR2Luv, 3, 3);
CREATE_CVTCOLOR_FUNC(rb_RGB2Luv, CV_RGB2Luv, 3, 3);
CREATE_CVTCOLOR_FUNC(rb_BGR2HLS, CV_BGR2HLS, 3, 3);
CREATE_CVTCOLOR_FUNC(rb_RGB2HLS, CV_RGB2HLS, 3, 3);
CREATE_CVTCOLOR_FUNC(rb_HSV2BGR, CV_HSV2BGR, 3, 3);
CREATE_CVTCOLOR_FUNC(rb_HSV2RGB, CV_HSV2RGB, 3, 3);
CREATE_CVTCOLOR_FUNC(rb_Lab2BGR, CV_Lab2BGR, 3, 3);
CREATE_CVTCOLOR_FUNC(rb_Lab2RGB, CV_Lab2RGB, 3, 3);
CREATE_CVTCOLOR_FUNC(rb_Luv2BGR, CV_Luv2BGR, 3, 3);
CREATE_CVTCOLOR_FUNC(rb_Luv2RGB, CV_Luv2RGB, 3, 3);
CREATE_CVTCOLOR_FUNC(rb_HLS2BGR, CV_HLS2BGR, 3, 3);
CREATE_CVTCOLOR_FUNC(rb_HLS2RGB, CV_HLS2RGB, 3, 3);

__NAMESPACE_END_OPENCV

extern "C"{
  void
  Init_opencv()
  {
    cvRedirectError((CvErrorCallback)mOpenCV::error_callback);
    
    mOpenCV::define_ruby_module();
    
    mOpenCV::mCvError::define_ruby_module();
    mOpenCV::cCvPoint::define_ruby_class();
    mOpenCV::cCvPoint2D32f::define_ruby_class();
    mOpenCV::cCvPoint3D32f::define_ruby_class();
    mOpenCV::cCvSize::define_ruby_class();
    mOpenCV::cCvSize2D32f::define_ruby_class();
    mOpenCV::cCvRect::define_ruby_class();
    mOpenCV::cCvScalar::define_ruby_class();
    mOpenCV::cCvSlice::define_ruby_class();
    mOpenCV::cCvTermCriteria::define_ruby_class();
    mOpenCV::cCvBox2D::define_ruby_class();
    mOpenCV::cCvFont::define_ruby_class();
    mOpenCV::cIplConvKernel::define_ruby_class();
    mOpenCV::cCvMoments::define_ruby_class();
    mOpenCV::cCvConvexityDefect::define_ruby_class();

    mOpenCV::cCvMemStorage::define_ruby_class();

    mOpenCV::cCvSeq::define_ruby_class();
    mOpenCV::mCurve::define_ruby_module();
    mOpenCV::mPointSet::define_ruby_module();
    mOpenCV::mPoint3DSet::define_ruby_module();
    mOpenCV::cCvSet::define_ruby_class();
    mOpenCV::cCvChain::define_ruby_class();
    mOpenCV::cCvChainCode::define_ruby_class();
    mOpenCV::cCvContour::define_ruby_class();
    mOpenCV::cCvContourTree::define_ruby_class();

    mOpenCV::cCvMat::define_ruby_class();
    mOpenCV::cIplImage::define_ruby_class();
    mOpenCV::cCvMatND::define_ruby_class();
    mOpenCV::cCvSparseMat::define_ruby_class();
    mOpenCV::cCvHistogram::define_ruby_class();
    mOpenCV::cCvCapture::define_ruby_class();

    mOpenCV::cCvIndex::define_ruby_class();
    mOpenCV::cCvLine::define_ruby_class();
    mOpenCV::cCvTwoPoints::define_ruby_class();
    mOpenCV::cCvCircle32f::define_ruby_class();

    mOpenCV::cCvConDensation::define_ruby_class();

    mOpenCV::cCvConnectedComp::define_ruby_class();
    mOpenCV::cCvAvgComp::define_ruby_class();
    mOpenCV::cCvHaarClassifierCascade::define_ruby_class();
#ifdef HAVE_CALLBACK_H
    mOpenCV::mGUI::define_ruby_module();
    mOpenCV::mGUI::cWindow::define_ruby_class();
    mOpenCV::mGUI::cTrackbar::define_ruby_class();
    mOpenCV::mGUI::cMouseEvent::define_ruby_class();
#endif

#ifdef HAVE_ML_H
    /* feature support.
       mOpenCV::mMachineLearning::define_ruby_module();
    */
#endif
  }
}
