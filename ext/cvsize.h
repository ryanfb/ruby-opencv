/************************************************************

   cvsize.h -

   $Author: lsxi $

   Copyright (C) 2005-2006 Masakazu Yonekura

************************************************************/
#ifndef RUBY_OPENCV_CVSIZE_H
#define RUBY_OPENCV_CVSIZE_H

#include"opencv.h"

#define __NAMESPACE_BEGIN_CVSIZE namespace cCvSize{
#define __NAMESPACE_END_CVSIZE }

__NAMESPACE_BEGIN_OPENCV
__NAMESPACE_BEGIN_CVSIZE

VALUE rb_class();

void define_ruby_class();

VALUE rb_compatible_q(VALUE klass, VALUE object);

VALUE rb_allocate(VALUE klass);
VALUE rb_initialize(int argc, VALUE *argv, VALUE self);
VALUE rb_width(VALUE self);
VALUE rb_set_width(VALUE self, VALUE width);
VALUE rb_height(VALUE self);
VALUE rb_set_height(VALUE self, VALUE height);

VALUE rb_to_s(VALUE self);
VALUE rb_to_ary(VALUE self);

VALUE new_object();
VALUE new_object(CvSize size);

__NAMESPACE_END_CVSIZE

inline CvSize*
CVSIZE(VALUE object)
{
  CvSize *ptr;
  Data_Get_Struct(object, CvSize, ptr);
  return ptr;
}

inline CvSize
VALUE_TO_CVSIZE(VALUE object)
{
  if(cCvSize::rb_compatible_q(cCvSize::rb_class(), object)){
    return cvSize(NUM2INT(rb_funcall(object, rb_intern("width"), 0)),
                  NUM2INT(rb_funcall(object, rb_intern("height"), 0)));
  }else{
    rb_raise(rb_eTypeError, "require %s or compatible object.", rb_class2name(cCvSize::rb_class()));
  }
}

__NAMESPACE_END_OPENCV

#endif // RUBY_OPENCV_CVSIZE_H
