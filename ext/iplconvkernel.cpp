/************************************************************

   iplconvkernel.cpp -

   $Author: lsxi $

   Copyright (C) 2005-2006 Masakazu Yonekura

************************************************************/
#include"iplconvkernel.h"
/*
 * Document-class: OpenCV::IplConvKernel
 *
 * Structuring element can be used in the morphological operations.
 *
 * CvMat#erode, CvMat#dilate, CvMat#morphology_open, CvMat#morphology_close,
 * CvMat#morphology_gradient, CvMat#morphology_tophat, CvMat#morphology_blackhat
 *
 * Create by IplConvKernel.new or CvMat#to_IplConvKernel
 */
__NAMESPACE_BEGIN_OPENCV
__NAMESPACE_BEGIN_IPLCONVKERNEL

VALUE rb_klass;

VALUE
rb_class()
{
  return rb_klass;
}

void
define_ruby_class()
{
  if (rb_klass)
    return;
  /* 
   * opencv = rb_define_module("OpenCV");
   * 
   * note: this comment is used by rdoc.
   */
  VALUE opencv = rb_module_opencv();  
  rb_klass = rb_define_class_under(opencv, "IplConvKernel", rb_cObject);
  rb_define_alloc_func(rb_klass, rb_allocate);

  rb_define_private_method(rb_klass, "initialize", RUBY_METHOD_FUNC(rb_initialize), -1);
  rb_define_method(rb_klass, "size", RUBY_METHOD_FUNC(rb_size), 0);
  rb_define_method(rb_klass, "cols", RUBY_METHOD_FUNC(rb_cols), 0);
  rb_define_alias(rb_klass, "columns", "cols");
  rb_define_method(rb_klass, "rows", RUBY_METHOD_FUNC(rb_rows), 0);
  rb_define_method(rb_klass, "anchor", RUBY_METHOD_FUNC(rb_anchor), 0);
  rb_define_method(rb_klass, "anchor_x", RUBY_METHOD_FUNC(rb_anchor_x), 0);
  rb_define_method(rb_klass, "anchor_y", RUBY_METHOD_FUNC(rb_anchor_y), 0);
}

VALUE
rb_allocate(VALUE klass)
{
  return IPLCONVKERNEL_OBJECT(klass, 0);
}

/*
 * call-seq:
 *   IplConvKernel.new(cols, rows, anchor_x, anchor_y, shape [,values = nil])
 *
 * Creates structuring element.
 *  cols
 *    Number of columns in the structuring element. 
 *  rows
 *    Number of rows in the structuring element. 
 *  anchor_x
 *    Relative horizontal offset of the anchor point. 
 *  anchor_y
 *    Relative vertical offset of the anchor point.
 *  shape
 *    Shape of the structuring element; may have the following values:
 *     :rect
 *     :cross
 *     :ellipse
 *     :custom
 *
 */
VALUE
rb_initialize(int argc, VALUE *argv, VALUE self)
{
  VALUE shape, rows, cols, anchor_x, anchor_y, values;
  rb_scan_args(argc, argv, "51", &cols, &rows, &anchor_x, &anchor_y, &shape, &values);
  Check_Type(shape, T_SYMBOL);
  const char *shape_name = rb_id2name(SYM2ID(shape));
  int shape_type = 0;
  int _cols = NUM2INT(cols);
  int _rows = NUM2INT(rows);
  int num_values;
  int *_values;
  if (!strcmp(shape_name, "rect"))
    shape_type = CV_SHAPE_RECT;
  else if (!strcmp(shape_name, "cross"))
    shape_type = CV_SHAPE_CROSS;
  else if (!strcmp(shape_name, "ellipse"))
    shape_type = CV_SHAPE_ELLIPSE;
  else if (!strcmp(shape_name, "custom")) {
    if (NIL_P(values))
      rb_raise(rb_eArgError, "argument 6 (values) should not be nil when the shape is :custom.");
    shape_type = CV_SHAPE_CUSTOM;
    num_values = RARRAY_LEN(values);
    _values = ALLOCA_N(int, num_values);
    VALUE *values_ptr = RARRAY_PTR(values);
    for (int i = 0; i < num_values; i++)
      _values[i] = NUM2INT(values_ptr[i]);
  }
  else
    rb_raise(rb_eTypeError, "argument 1 (shape) should be :rect or :cross or :ellipse or :custom.");
  DATA_PTR(self) = cvCreateStructuringElementEx(_cols, _rows, NUM2INT(anchor_x), NUM2INT(anchor_y), shape_type, _values);
  return self;
}

/*
 * call-seq:
 *   size -> cvsize
 *
 * Return the structuring element's size.
 */
VALUE
rb_size(VALUE self)
{
  IplConvKernel *kernel = IPLCONVKERNEL(self);
  return cCvSize::new_object(cvSize(kernel->nCols, kernel->nRows));
}

/*
 * call-seq:
 *   cols -> int
 *
 * Return number of columns in the structuring element.
 */
VALUE
rb_cols(VALUE self)
{
  return INT2NUM(IPLCONVKERNEL(self)->nCols);
}

/*
 * call-seq:
 *   rows -> int
 *
 * Return number of rows in the structuring element.
 */
VALUE
rb_rows(VALUE self)
{
  return INT2NUM(IPLCONVKERNEL(self)->nRows);
}

/*
 * call-seq:
 *   anchor -> cvpoint
 *
 * Return anchor of the structuring element.
 */
VALUE
rb_anchor(VALUE self)
{
  IplConvKernel *kernel = IPLCONVKERNEL(self);
  return cCvPoint::new_object(cvPoint(kernel->anchorX, kernel->anchorY));
}

/*
 * call-seq:
 *   anchor_x -> int
 *
 * Return relative horizontal offset of the anchor point.
 */
VALUE
rb_anchor_x(VALUE self)
{
  return INT2NUM(IPLCONVKERNEL(self)->anchorX);
}

/*
 * call-seq:
 *   anchor_y -> int
 *
 * Return relative vertical offset of the anchor point.
 */
VALUE
rb_anchor_y(VALUE self)
{
  return INT2NUM(IPLCONVKERNEL(self)->anchorY);
}

__NAMESPACE_END_IPLCONVKERNEL
__NAMESPACE_END_OPENCV
