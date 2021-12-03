/************************************************************

   cvbox2d.h -

   $Author: lsxi $

   Copyright (C) 2005-2006 Masakazu Yonekura

************************************************************/
#ifndef RUBY_OPENCV_CVBOX2D_H
#define RUBY_OPENCV_CVBOX2D_H

#include"opencv.h"

#define __NAMESPACE_BEGIN_CVBOX2D namespace cCvBox2D{
#define __NAMESPACE_END_CVBOX2D }

__NAMESPACE_BEGIN_OPENCV
__NAMESPACE_BEGIN_CVBOX2D

VALUE rb_class();

void define_ruby_class();

VALUE rb_allocate(VALUE klass);

VALUE rb_center(VALUE self);
VALUE rb_size(VALUE self);
VALUE rb_angle(VALUE self);
VALUE rb_points(VALUE self);

VALUE new_object();
VALUE new_object(CvBox2D box);

__NAMESPACE_END_CVBOX2D

inline CvBox2D *CVBOX2D(VALUE object){
  CvBox2D *ptr;
  Data_Get_Struct(object, CvBox2D, ptr);
  return ptr;
}

inline CvBox2D VALUE_TO_CVBOX2D(VALUE object){
  if(rb_obj_is_kind_of(object, cCvBox2D::rb_class())) {
    return *CVBOX2D(object);
  }else{
    rb_raise(rb_eTypeError, "require %s or compatible object.", rb_class2name(cCvBox2D::rb_class()));
  }
}

__NAMESPACE_END_OPENCV

#endif // RUBY_OPENCV_CVBOX2D_H
