/************************************************************

   cvindex.h -

   $Author: lsxi $

   Copyright (C) 2005 Masakazu Yonekura

************************************************************/
#ifndef RUBY_OPENCV_CVINDEX_H
#define RUBY_OPENCV_CVINDEX_H

#include"opencv.h"

namespace mOpenCV{

  typedef struct CvIndex{
    int value;
  }CvIndex;

  namespace cCvIndex{    
    VALUE rb_class();

    void define_ruby_class();

    VALUE rb_allocate(VALUE klass);
    VALUE rb_initialize(int argc, VALUE *argv, VALUE self);
    VALUE rb_cv_new(int argc, VALUE *argv, VALUE klass);
    VALUE rb_value(VALUE self);
    VALUE rb_to_s(VALUE self);
  }

  inline CvIndex *CVINDEX(VALUE object){    
    CvIndex *ptr;
    Data_Get_Struct(object, CvIndex, ptr);
    return ptr;
  }
}

#endif // RUBY_OPENCV_CVINDEX_H
