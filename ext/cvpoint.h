/************************************************************

   cvpoint.h -

   $Author: lsxi $

   Copyright (C) 2005-2006 Masakazu Yonekura

************************************************************/
#ifndef RUBY_OPENCV_CVPOINT_H
#define RUBY_OPENCV_CVPOINT_H

#include "opencv.h"

#define __NAMESPACE_BEGIN_CVPOINT namespace cCvPoint{
#define __NAMESPACE_END_CVPOINT }

__NAMESPACE_BEGIN_OPENCV
__NAMESPACE_BEGIN_CVPOINT

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

VALUE new_object();
VALUE new_object(CvPoint point);

__NAMESPACE_END_CVPOINT

inline CvPoint *CVPOINT(VALUE object){
  CvPoint *ptr;
  Data_Get_Struct(object, CvPoint, ptr);
  return ptr;
}

inline CvPoint VALUE_TO_CVPOINT(VALUE object){
  if(cCvPoint::rb_compatible_q(cCvPoint::rb_class(), object)){
    return cvPoint(NUM2INT(rb_funcall(object, rb_intern("x"), 0)),
                   NUM2INT(rb_funcall(object, rb_intern("y"), 0)));
  }else{
    rb_raise(rb_eTypeError, "require %s or compatible object.", rb_class2name(cCvPoint::rb_class()));
  }
}

__NAMESPACE_END_OPENCV

#endif // RUBY_OPENCV_CVPOINT_H
