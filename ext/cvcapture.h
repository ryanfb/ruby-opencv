/************************************************************

   cvcapture.h -

   $Author: lsxi $

   Copyright (C) 2005-2006 Masakazu Yonekura

************************************************************/
#ifndef RUBY_OPENCV_CVCAPTURE_H
#define RUBY_OPENCV_CVCAPTURE_H

#include "opencv.h"

#define __NAMESPACE_BEGIN_CVCAPTURE namespace cCvCapture{
#define __NAMESPACE_END_CVCAPTURE }

__NAMESPACE_BEGIN_OPENCV
__NAMESPACE_BEGIN_CVCAPTURE


VALUE rb_class();

void define_ruby_class();

void free(void *ptr);
VALUE rb_open(int argc, VALUE *argv, VALUE klass);

VALUE rb_grab(VALUE self);
VALUE rb_retrieve(VALUE self);
VALUE rb_query(VALUE self);

VALUE rb_millisecond(VALUE self);
VALUE rb_frames(VALUE self);
VALUE rb_avi_ratio(VALUE self);
VALUE rb_size(VALUE self);
VALUE rb_width(VALUE self);
VALUE rb_height(VALUE self);
VALUE rb_fps(VALUE self);
VALUE rb_fourcc(VALUE self);
VALUE rb_frame_count(VALUE self);

__NAMESPACE_END_CVCAPTURE


inline CvCapture *CVCAPTURE(VALUE object){
  CvCapture *ptr;
  Data_Get_Struct(object, CvCapture, ptr);
  return ptr;
}

__NAMESPACE_END_OPENCV

#endif // RUBY_OPENCV_CVCAPTURE_H
