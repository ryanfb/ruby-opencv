/************************************************************

  cvchain.h -

  $Author: lsxi $

  Copyright (C) 2007 Masakazu Yonekura

************************************************************/
#ifndef RUBY_OPENCV_CVCHAIN_H
#define RUBY_OPENCV_CVCHAIN_H
#include "opencv.h"

#define __NAMESPACE_BEGIN_CVCHAIN namespace cCvChain{
#define __NAMESPACE_END_CVCHAIN }

__NAMESPACE_BEGIN_OPENCV
__NAMESPACE_BEGIN_CVCHAIN

VALUE rb_class();

void define_ruby_class();

VALUE rb_allocate(VALUE klass);
void cvchain_free(void *ptr);

VALUE rb_initialize(int argc, VALUE *argv, VALUE self);
VALUE rb_origin(VALUE self);
VALUE rb_set_origin(VALUE self, VALUE origin);
VALUE rb_codes(VALUE self);
VALUE rb_points(VALUE self);
VALUE rb_approx_chain(int argc, VALUE *argv, VALUE self);

VALUE new_object();

__NAMESPACE_END_CVCHAIN

inline CvChain*
CVCHAIN(VALUE object){
  CvChain *ptr;
  Data_Get_Struct(object, CvChain, ptr);
  return ptr;
}

__NAMESPACE_END_OPENCV

#endif // RUBY_OPENCV_CVCHAIN_H
