/************************************************************

   cvhaarclassifiercascade.h -

   $Author: lsxi $

   Copyright (C) 2005-2006 Masakazu Yonekura

************************************************************/
#ifndef RUBY_OPENCV_CVHAARCLASSIFIERCASCADE_H
#define RUBY_OPENCV_CVHAARCLASSIFIERCASCADE_H

#define __NAMESPACE_BEGIN_CVHAARCLASSIFERCASCADE namespace cCvHaarClassifierCascade{
#define __NAMESPACE_END_CVHAARCLASSIFERCASCADE }

#include"opencv.h"

__NAMESPACE_BEGIN_OPENCV
__NAMESPACE_BEGIN_CVHAARCLASSIFERCASCADE

VALUE rb_class();

void define_ruby_class();

VALUE rb_allocate(VALUE klass);
VALUE rb_initialize(int argc, VALUE *argv, VALUE self);

VALUE rb_load(VALUE klass, VALUE path);
VALUE rb_save(VALUE self, VALUE name);
VALUE rb_detect_objects(int argc, VALUE *argv, VALUE self);
VALUE rb_detect_objects_with_pruning(int argc, VALUE *argv, VALUE self);

__NAMESPACE_END_CVHAARCLASSIFERCASCADE
inline CvHaarClassifierCascade *CVHAARCLASSIFIERCASCADE(VALUE object){
  CvHaarClassifierCascade *ptr;
  Data_Get_Struct(object, CvHaarClassifierCascade, ptr);
  return ptr;
}
__NAMESPACE_END_OPENCV

#endif // RUBY_OPENCV_CVHAARCLASSIFIERCASCADE_H
