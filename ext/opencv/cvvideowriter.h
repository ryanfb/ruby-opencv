/************************************************************

   cvvideowriter.h -

   $Author: lsxi $

   Copyright (C) 2005-2006 Masakazu Yonekura

************************************************************/
#ifndef RUBY_OPENCV_CVVIDEOWRITER_H
#define RUBY_OPENCV_CVVIDEOWRITER_H

#include "opencv.h"

#define __NAMESPACE_BEGIN_CVVIDEOWRITER namespace cCvVideoWriter{
#define __NAMESPACE_END_CVVIDEOWRITER }

__NAMESPACE_BEGIN_OPENCV
__NAMESPACE_BEGIN_CVVIDEOWRITER

VALUE rb_class();

void define_ruby_class();

VALUE rb_initialize(int argc, VALUE *argv, VALUE self);
VALUE rb_write(VALUE self, VALUE frame);
VALUE rb_close(VALUE self);

__NAMESPACE_END_CVVIDEOWRITER

inline CvVideoWriter*
CVVIDEOWRITER(VALUE object)
{
  CvVideoWriter *ptr;
  Data_Get_Struct(object, CvVideoWriter, ptr);
  return ptr;
}

__NAMESPACE_END_OPENCV

#endif // RUBY_OPENCV_CVVIDEOWRITER_H
