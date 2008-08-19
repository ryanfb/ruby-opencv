/************************************************************

   cvmoments.h -

   $Author: lsxi $

   Copyright (C) 2007 Masakazu Yonekura

************************************************************/
#ifndef RUBY_OPENCV_CVMOMENTS_H
#define RUBY_OPENCV_CVMOMENTS_H

#include "opencv.h"

#define __NAMESPACE_BEGIN_CVMOMENTS namespace cCvMoments{
#define __NAMESPACE_END_CVMOMENTS }

__NAMESPACE_BEGIN_OPENCV
__NAMESPACE_BEGIN_CVMOMENTS

VALUE rb_class();

void define_ruby_class();

VALUE rb_allocate(VALUE klass);
VALUE rb_initialize(int argc, VALUE *argv, VALUE self);
VALUE rb_spatial(VALUE self, VALUE x_order, VALUE y_order);
VALUE rb_central(VALUE self, VALUE x_order, VALUE y_order);
VALUE rb_normalized_central(VALUE self, VALUE x_order, VALUE y_order);
VALUE rb_hu(VALUE self);
VALUE rb_gravity_center(VALUE self);
VALUE rb_angle(VALUE self);

VALUE new_object(CvArr *arr, int is_binary);

__NAMESPACE_END_CVMOMENTS

inline CvMoments*
CVMOMENTS(VALUE object)
{
  CvMoments *ptr;
  Data_Get_Struct(object, CvMoments, ptr);
  return ptr;
}

__NAMESPACE_END_OPENCV

#endif // RUBY_OPENCV_CVMOMENTS_H
