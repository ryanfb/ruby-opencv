/************************************************************

   cvcircle32f.h -

   $Author: lsxi $

   Copyright (C) 2007 Masakazu Yonekura

************************************************************/
#ifndef RUBY_OPENCV_CVCIRCLE32F_H
#define RUBY_OPENCV_CVCIRCLE32F_H

#include "opencv.h"

#define __NAMESPACE_BEGIN_CVCIRCLE32F namespace cCvCircle32f{
#define __NAMESPACE_END_CVCIRCLE32F }

__NAMESPACE_BEGIN_OPENCV

typedef struct CvCircle32f{
  CvPoint2D32f center;
  float radius;
} CvCircle32f;

__NAMESPACE_BEGIN_CVCIRCLE32F

VALUE rb_class();

void define_ruby_class();

VALUE rb_allocate(VALUE klass);
VALUE rb_initialize(int argc, VALUE *argv, VALUE self);
VALUE rb_center(VALUE self);
VALUE rb_radius(VALUE self);
VALUE rb_aref(VALUE self, VALUE index);
VALUE rb_to_ary(VALUE self);

VALUE new_object(CvCircle32f circle32f);

__NAMESPACE_END_CVCIRCLE32F

inline CvCircle32f*
CVCIRCLE32F(VALUE object)
{
  CvCircle32f *ptr;
  Data_Get_Struct(object, CvCircle32f, ptr);
  return ptr;
}

__NAMESPACE_END_OPENCV

#endif // RUBY_OPENCV_CVCIRCLE32F_H
