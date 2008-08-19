/************************************************************

   cvslice.h -

   $Author: lsxi $

   Copyright (C) 2005-2006 Masakazu Yonekura

************************************************************/
#ifndef RUBY_OPENCV_CVSLICE_H
#define RUBY_OPENCV_CVSLICE_H

#include "opencv.h"

#define __NAMESPACE_BEGIN_CVSLICE namespace cCvSlice{
#define __NAMESPACE_END_CVSLICE }

__NAMESPACE_BEGIN_OPENCV
__NAMESPACE_BEGIN_CVSLICE

VALUE rb_class();

void define_ruby_class();

VALUE rb_compatible_q(VALUE klass, VALUE object);

VALUE rb_allocate(VALUE klass);
VALUE rb_initialize(VALUE self, VALUE start, VALUE end);

__NAMESPACE_END_CVSLICE

inline CvSlice*
CVSLICE(VALUE object)
{
  CvSlice *ptr;
  Data_Get_Struct(object, CvSlice, ptr);
  return ptr;
}

inline CvSlice
VALUE_TO_CVSLICE(VALUE object)
{
  if(cCvSlice::rb_compatible_q(cCvSlice::rb_class(), object)){
    return cvSlice(NUM2INT(rb_funcall(object, rb_intern("begin"), 0)),
                   rb_funcall(object, rb_intern("exclude_end?"), 0) ? NUM2INT(rb_funcall(object, rb_intern("end"), 0)) : NUM2INT(rb_funcall(object, rb_intern("end"), 0)) - 1);
  }else{
    rb_raise(rb_eTypeError, "require %s or compatible object.", rb_class2name(cCvSlice::rb_class()));
  }
}

__NAMESPACE_END_OPENCV

#endif // RUBY_OPENCV_CVSLICE_H
