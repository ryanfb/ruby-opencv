/************************************************************

   cvtwopoints.h -

   $Author: lsxi $

   Copyright (C) 2005-2006 Masakazu Yonekura

************************************************************/
#ifndef RUBY_OPENCV_CVTWOPOINTS_H
#define RUBY_OPENCV_CVTWOPOINTS_H

#include "opencv.h"

#define __NAMESPACE_BEGIN_CVTWOPOINTS namespace cCvTwoPoints{
#define __NAMESPACE_END_CVTWOPOINTS }

__NAMESPACE_BEGIN_OPENCV

typedef struct CvTwoPoints{
  CvPoint p1;
  CvPoint p2;
} CvTwoPoints;

__NAMESPACE_BEGIN_CVTWOPOINTS

VALUE rb_class();

void define_ruby_class();

VALUE rb_allocate(VALUE klass);

VALUE rb_point1(VALUE self);
VALUE rb_point2(VALUE self);
VALUE rb_aref(VALUE self, VALUE index);
VALUE rb_to_ary(VALUE self);

VALUE new_object(CvTwoPoints twopoints);

__NAMESPACE_END_CVTWOPOINTS

inline CvTwoPoints*
CVTWOPOINTS(VALUE object){
  CvTwoPoints *ptr;
  Data_Get_Struct(object, CvTwoPoints, ptr);
  return ptr;
}

__NAMESPACE_END_OPENCV

#endif // RUBY_OPENCV_CVTWOPOINTS_H
