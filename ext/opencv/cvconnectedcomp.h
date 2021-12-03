/************************************************************

   cvconnectedcomp.h -

   $Author: lsxi $

   Copyright (C) 2005-2007 Masakazu Yonekura

************************************************************/
#ifndef RUBY_OPENCV_CVCONNECTEDCOMP_H
#define RUBY_OPENCV_CVCONNECTEDCOMP_H

#include "opencv.h"

#define __NAMESPACE_BEGIN_CVCONNECTEDCOMP namespace cCvConnectedComp{
#define __NAMESPACE_END_CVCONNECTEDCOMP }

__NAMESPACE_BEGIN_OPENCV
__NAMESPACE_BEGIN_CVCONNECTEDCOMP

VALUE rb_class();

void define_ruby_class();

VALUE rb_allocate(VALUE klass);

VALUE rb_area(VALUE self);
VALUE rb_value(VALUE self);
VALUE rb_rect(VALUE self);
VALUE rb_set_rect(VALUE self, VALUE rect);
VALUE rb_contour(VALUE self);

VALUE new_object();
VALUE new_object(CvConnectedComp comp);

__NAMESPACE_END_CVCONNECTEDCOMP

inline CvConnectedComp *CVCONNECTEDCOMP(VALUE object){
  CvConnectedComp *ptr;
  Data_Get_Struct(object, CvConnectedComp, ptr);
  return ptr;
}

__NAMESPACE_END_OPENCV

#endif // RUBY_OPENCV_CVCONNECTEDCOMP_H
