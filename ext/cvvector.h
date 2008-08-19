/************************************************************

   cvvector.h -

   $Author: lsxi $

   Copyright (C) 2007 Masakazu Yonekura

************************************************************/
#ifndef RUBY_OPENCV_CVVECTOR_H
#define RUBY_OPENCV_CVVECTOR_H

#include "opencv.h"

#define __NAMESPACE_BEGIN_CVVECTOR namespace cCvVector{
#define __NAMESPACE_END_CVVECTOR }

typedef struct CvVector{
  float x;
  float y;
} CvVector;

__NAMESPACE_BEGIN_OPENCV
__NAMESPACE_BEGIN_CVVECTOR

VALUE rb_class();

void define_ruby_class();

VALUE rb_compatible_q(VALUE klass, VALUE object);

VALUE rb_allocate(VALUE klass);
VALUE rb_initialize(int argc, VALUE *argv, VALUE self);
VALUE rb_x(VALUE self);
VALUE rb_set_x(VALUE self, VALUE x);
VALUE rb_y(VALUE self);
VALUE rb_set_y(VALUE self, VALUE y);

VALUE rb_to_s(VALUE self);
VALUE rb_to_ary(VALUE self);

VALUE new_object(CvVector vector);

__NAMESPACE_END_CVVECTOR

inline CvVector *CVVECTOR(VALUE object){
  CvVector *ptr;
  Data_Get_Struct(object, CvVector, ptr);
  return ptr;
}

__NAMESPACE_END_OPENCV

#endif // RUBY_OPENCV_CVVECTOR_H
