/************************************************************

   curve.cpp -

   $Author: lsxi $

   Copyright (C) 2005 Masakazu Yonekura

************************************************************/
#include"curve.h"
/*
 * Document-class: OpenCV::Curve
 *
 * Curve sequence.
 */
__NAMESPACE_BEGIN_OPENCV
__NAMESPACE_BEGIN_CURVE

VALUE module;
    
VALUE
rb_module()
{
  return module;
}

void
define_ruby_module()
{
  if(module)
    return;
  /* 
   * opencv = rb_define_module("OpenCV");
   *
   * note: this comment is used by rdoc.
   */
  VALUE opencv = rb_module_opencv();
  module = rb_define_module_under(opencv, "Curve");
  rb_define_method(module, "close?", RUBY_METHOD_FUNC(rb_closed_q), 0);
  rb_define_method(module, "convex?", RUBY_METHOD_FUNC(rb_convex_q), 0);
  rb_define_method(module, "hole?", RUBY_METHOD_FUNC(rb_hole_q), 0);
  rb_define_method(module, "simple?", RUBY_METHOD_FUNC(rb_simple_q), 0);
  rb_define_method(module, "arc_length", RUBY_METHOD_FUNC(rb_arc_length), -1);
}

/*
 * If curve is closed, return true. Otherwise return false.
 */
VALUE
rb_closed_q(VALUE self)
{
  return CV_IS_SEQ_CLOSED(CVSEQ(self)) ? Qtrue : Qfalse;
}

/*
 * If curve is convex, return true. Otherwise return false.
 */
VALUE
rb_convex_q(VALUE self)
{
  return CV_IS_SEQ_CONVEX(CVSEQ(self)) ? Qtrue : Qfalse;
}

/*
 * If curve is hole(inner contour), return true. Otherwise return false.
 */
VALUE
rb_hole_q(VALUE self)
{  
  return CV_IS_SEQ_HOLE(CVSEQ(self)) ? Qtrue : Qfalse;
}

/*
 * no idia.
 */
VALUE
rb_simple_q(VALUE self)
{
  return CV_IS_SEQ_SIMPLE(CVSEQ(self)) ? Qtrue : Qfalse;
}

/*
 * call-seq:
 *   arc_length(<i>[slice = nil][,is_closed = nil]</i>) -> float
 *
 * Calculates contour perimeter or curve length.
 * <i>slice</i> is starting and ending points of the curve.
 * <i>is_closed</i> is indicates whether the curve is closed or not. There are 3 cases:
 * * is_closed = true  - the curve is assumed to be unclosed.
 * * is_closed = false - the curve is assumed to be closed.
 * * is_closed = nil (default) use self#close?
 */
VALUE
rb_arc_length(int argc, VALUE *argv, VALUE self)
{
  VALUE slice, is_closed;
  rb_scan_args(argc, argv, "02", &slice, &is_closed);
  return rb_float_new(cvArcLength(CVARR(self), NIL_P(slice) ? CV_WHOLE_SEQ : VALUE_TO_CVSLICE(slice), TRUE_OR_FALSE(is_closed, -1)));
}

__NAMESPACE_END_CURVE
__NAMESPACE_END_OPENCV

