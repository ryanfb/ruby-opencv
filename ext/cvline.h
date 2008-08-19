/************************************************************

   cvline.h -

   $Author: lsxi $

   Copyright (C) 2007 Masakazu Yonekura

************************************************************/
#ifndef RUBY_OPENCV_CVLINE_H
#define RUBY_OPENCV_CVLINE_H

#include "opencv.h"

#define __NAMESPACE_BEGIN_CVLINE namespace cCvLine{
#define __NAMESPACE_END_CVLINE }

__NAMESPACE_BEGIN_OPENCV

typedef struct CvLine{
  float rho;
  float theta;
} CvLine;

__NAMESPACE_BEGIN_CVLINE

VALUE rb_class();

void define_ruby_class();

VALUE rb_allocate(VALUE klass);
VALUE rb_initialize(int argc, VALUE *argv, VALUE self);
VALUE rb_rho(VALUE self);
VALUE rb_set_rho(VALUE self, VALUE rho);
VALUE rb_theta(VALUE self);
VALUE rb_set_theta(VALUE self, VALUE theta);

VALUE new_object(CvLine line);

__NAMESPACE_END_CVLINE

inline CvLine*
CVLINE(VALUE object)
{
  CvLine *ptr;
  Data_Get_Struct(object, CvLine, ptr);
  return ptr;
}

__NAMESPACE_END_OPENCV

#endif // RUBY_OPENCV_CVLINE_H
