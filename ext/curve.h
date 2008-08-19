/************************************************************

   curve.h -

   $Author: lsxi $

   Copyright (C) 2005-2006 Masakazu Yonekura

************************************************************/
#ifndef RUBY_OPENCV_CVSEQ_CURVE_H
#define RUBY_OPENCV_CVSEQ_CURVE_H

#include"opencv.h"

#define __NAMESPACE_BEGIN_CURVE namespace mCurve{
#define __NAMESPACE_END_CURVE }

__NAMESPACE_BEGIN_OPENCV
__NAMESPACE_BEGIN_CURVE

VALUE rb_module();

void define_ruby_module();

VALUE rb_closed_q(VALUE self);
VALUE rb_convex_q(VALUE self);
VALUE rb_hole_q(VALUE self);
VALUE rb_simple_q(VALUE self);
VALUE rb_arc_length(int argc, VALUE *argv, VALUE self);

__NAMESPACE_END_CURVE
__NAMESPACE_END_OPENCV

#endif // RUBY_OPENCV_CVSEQ_CURVE_H
