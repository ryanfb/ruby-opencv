/**********************************************************************

   cvavgcomp.h

   $Author: lsxi $

   Copyright (C) 2005-2006 Masakazu Yonekura

**********************************************************************/
#ifndef RUBY_OPENCV_AVGCOMP_H
#define RUBY_OPENCV_AVGCOMP_H

#define __NAMESPACE_BEGIN_AVGCOMP namespace cCvAvgComp{
#define __NAMESPACE_END_AVGCOMP }

#include <opencv.h>

__NAMESPACE_BEGIN_OPENCV
__NAMESPACE_BEGIN_AVGCOMP

VALUE rb_class();

void define_ruby_class();

VALUE rb_allocate(VALUE klass);
VALUE rb_neighbors(VALUE self);

__NAMESPACE_END_AVGCOMP

inline CvAvgComp *CVAVGCOMP(VALUE object){
  CvAvgComp *ptr;
  Data_Get_Struct(object, CvAvgComp, ptr);
  return ptr;
}

__NAMESPACE_END_OPENCV


#endif // RUBY_OPENCV_AVGCOMP_H
