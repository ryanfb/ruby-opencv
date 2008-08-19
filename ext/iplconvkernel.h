/************************************************************

   iplconvkernel.h -

   $Author: lsxi $

   Copyright (C) 2005-2006 Masakazu Yonekura

************************************************************/
#ifndef RUBY_OPENCV_IPLCONVKERNEL_H
#define RUBY_OPENCV_IPLCONVKERNEL_H

#include "opencv.h"

#define __NAMESPACE_BEGIN_IPLCONVKERNEL namespace cIplConvKernel{
#define __NAMESPACE_END_IPLCONVKERNEL }

__NAMESPACE_BEGIN_OPENCV
__NAMESPACE_BEGIN_IPLCONVKERNEL

VALUE rb_class();
VALUE rb_allocate(VALUE klass);

void define_ruby_class();
VALUE rb_initialize(int argc, VALUE *argv, VALUE self);
VALUE rb_size(VALUE self);
VALUE rb_cols(VALUE self);
VALUE rb_rows(VALUE self);
VALUE rb_anchor(VALUE self);
VALUE rb_anchor_x(VALUE self);
VALUE rb_anchor_y(VALUE self);

__NAMESPACE_END_IPLCONVKERNEL

inline IplConvKernel*
IPLCONVKERNEL(VALUE object)
{
  IplConvKernel *ptr;
  if (NIL_P(object))
    return NULL;
  else if (rb_obj_is_kind_of(object, cIplConvKernel::rb_class())) {
    Data_Get_Struct(object, IplConvKernel, ptr);
    return ptr;
  } else {
    rb_warn("invalid kernel. use default kernel (3x3 rectangle).");
    return NULL;
  }
}

__NAMESPACE_END_OPENCV

#endif // RUBY_OPENCV_IPLCONVKERNEL_H
