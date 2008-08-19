/************************************************************

   cvset.h -

   $Author: lsxi $

   Copyright (C) 2005-2006 Masakazu Yonekura

************************************************************/
#ifndef RUBY_OPENCV_CVSET_H
#define RUBY_OPENCV_CVSET_H

#include "opencv.h"

#define __NAMESPACE_BEGIN_CVSET namespace cCvSet{
#define __NAMESPACE_END_CVSET }

__NAMESPACE_BEGIN_OPENCV
__NAMESPACE_BEGIN_CVSET

VALUE rb_class();
void define_ruby_class();

VALUE rb_active_count(VALUE self);
VALUE rb_free_q(VALUE self);

__NAMESPACE_END_CVSET

inline CvSet*
CVSET(VALUE object)
{
  CvSet *ptr;
  Data_Get_Struct(object, CvSet, ptr);
  return ptr;
}

__NAMESPACE_END_OPENCV

#endif // RUBY_OPENCV_CVSET_H
