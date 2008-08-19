/************************************************************

   cvsize2d32f.h -

   $Author: lsxi $

   Copyright (C) 2005-2006 Masakazu Yonekura

************************************************************/
#ifndef RUBY_OPENCV_CVSIZE2D32F_H
#define RUBY_OPENCV_CVSIZE2D32F_H

#include "opencv.h"

#define __NAMESPACE_BEGIN_CVSIZE2D32F namespace cCvSize2D32f{
#define __NAMESPACE_END_CVSIZE2D32F }

__NAMESPACE_BEGIN_OPENCV
__NAMESPACE_BEGIN_CVSIZE2D32F

VALUE rb_class();

void define_ruby_class();

VALUE rb_compatible_q(VALUE klass, VALUE object);

VALUE rb_allocate(VALUE klass);
VALUE rb_initialize(int argc, VALUE *argv, VALUE self);
VALUE rb_width(VALUE self);
VALUE rb_set_width(VALUE self, VALUE width);
VALUE rb_height(VALUE self);
VALUE rb_set_height(VALUE self, VALUE height);

VALUE new_object(CvSize2D32f size);

__NAMESPACE_END_CVSIZE2D32F

inline CvSize2D32f*
CVSIZE2D32F(VALUE object)
{
  CvSize2D32f *ptr;
  Data_Get_Struct(object, CvSize2D32f, ptr);
  return ptr;
}

inline CvSize2D32f
VALUE_TO_CVSIZE2D32F(VALUE object)
{
  if(cCvSize2D32f::rb_compatible_q(cCvSize2D32f::rb_class(), object)){
    return cvSize2D32f(NUM2DBL(rb_funcall(object, rb_intern("width"), 0)),
                       NUM2DBL(rb_funcall(object, rb_intern("height"), 0)));
  }else{
    rb_raise(rb_eTypeError, "require %s or compatible object.", rb_class2name(cCvSize2D32f::rb_class()));
  }
}

__NAMESPACE_END_OPENCV

#endif // RUBY_OPENCV_CVSIZE2D32F_H
