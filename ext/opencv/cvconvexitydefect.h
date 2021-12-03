/************************************************************

   cvconvexitydefect.h -

   $Author: lsxi $

   Copyright (C) 2007 Masakazu Yonekura

************************************************************/
#ifndef RUBY_OPENCV_CVCONVEXITYDEFECT_H
#define RUBY_OPENCV_CVCONVEXITYDEFECT_H

#include "opencv.h"

#define __NAMESPACE_BEGIN_CVCONVEXITYDEFECT namespace cCvConvexityDefect{
#define __NAMESPACE_END_CVCONVEXITYDEFECT }

__NAMESPACE_BEGIN_OPENCV
__NAMESPACE_BEGIN_CVCONVEXITYDEFECT

VALUE rb_class();

void define_ruby_class();

VALUE rb_start(VALUE self);
VALUE rb_end(VALUE self);
VALUE rb_depth_point(VALUE self);
VALUE rb_depth(VALUE self);

__NAMESPACE_END_CVCONVEXITYDEFECT

inline CvConvexityDefect*
CVCONVEXITYDEFECT(VALUE object)
{
  CvConvexityDefect *ptr;
  Data_Get_Struct(object, CvConvexityDefect, ptr);
  return ptr;
}

__NAMESPACE_END_OPENCV

#endif // RUBY_OPENCV_CVCONVEXITYDEFECT_H
