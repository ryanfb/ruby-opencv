/************************************************************

   cvhistogram.h -

   $Author: lsxi $

   Copyright (C) 2005-2008 Masakazu Yonekura

************************************************************/
#ifndef RUBY_OPENCV_CVHISTOGRAM_H
#define RUBY_OPENCV_CVHISTOGRAM_H

#include "opencv.h"

#define __NAMESPACE_BEGIN_CVHISTOGRAM namespace cCvHistogram{
#define __NAMESPACE_END_CVHISTOGRAM }

__NAMESPACE_BEGIN_OPENCV
__NAMESPACE_BEGIN_CVHISTOGRAM

VALUE rb_class();

void define_ruby_class();

VALUE rb_is_uniform(VALUE self);
VALUE rb_is_sparse(VALUE self);
VALUE rb_has_range(VALUE self);
VALUE rb_dims(VALUE self);
VALUE rb_bins(VALUE self);

VALUE rb_clear(VALUE self);
VALUE rb_clear_bang(VALUE self);

VALUE rb_normalize(VALUE self, VALUE factor);
VALUE rb_normalize_bang(VALUE self, VALUE factor);
VALUE rb_thresh(VALUE self, VALUE factor);
VALUE rb_thresh_bang(VALUE self, VALUE factor);

__NAMESPACE_END_CVHISTOGRAM

inline CvHistogram
*CVHISTOGRAM(VALUE object)
{
  CvHistogram *ptr;
  Data_Get_Struct(object, CvHistogram, ptr);
  return ptr;
}

__NAMESPACE_END_OPENCV

#endif // RUBY_OPENCV_CVHISTOGRAM_H
