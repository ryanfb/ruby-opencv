/************************************************************

  cvcontour.h -

  $Author: lsxi $

  Copyright (C) 2007 Masakazu Yonekura

************************************************************/
#ifndef RUBY_OPENCV_CVCONTOUR_H
#define RUBY_OPENCV_CVCONTOUR_H
#include "opencv.h"

#define __NAMESPACE_BEGIN_CVCONTOUR namespace cCvContour{
#define __NAMESPACE_END_CVCONTOUR }

__NAMESPACE_BEGIN_OPENCV
__NAMESPACE_BEGIN_CVCONTOUR

VALUE rb_class();

void define_ruby_class();

VALUE rb_initialize(int argc, VALUE *argv, VALUE self);
VALUE rb_rect(VALUE self);
VALUE rb_color(VALUE self);
VALUE rb_set_color(VALUE self, VALUE color);
VALUE rb_reserved(VALUE self);
VALUE rb_approx_poly(int argc, VALUE *argv, VALUE self);
VALUE rb_bounding_rect(VALUE self);
VALUE rb_create_tree(int argc, VALUE *argv, VALUE self);
VALUE rb_in_q(VALUE self, VALUE point);
VALUE rb_measure_distance(VALUE self, VALUE point);
VALUE rb_point_polygon_test(VALUE self, VALUE point, VALUE measure_dist);

VALUE new_object();
__NAMESPACE_END_CVCONTOUR

inline CvContour*
CVCONTOUR(VALUE object){
  CvContour *ptr;
  Data_Get_Struct(object, CvContour, ptr);
  return ptr;
}

__NAMESPACE_END_OPENCV

#endif // RUBY_OPENCV_CVCONTOUR_H
