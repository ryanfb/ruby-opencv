/************************************************************

   cvmemstorage.h -

   $Author: lsxi $

   Copyright (C) 2005 Masakazu Yonekura

************************************************************/
#ifndef RUBY_OPENCV_CVMEMSTORAGE_H
#define RUBY_OPENCV_CVMEMSTORAGE_H

#include "opencv.h"

#define __NAMESPACE_BEGIN_CVMEMSTORAGE namespace cCvMemStorage{
#define __NAMESPACE_END_CVMEMSTORAGE }

__NAMESPACE_BEGIN_OPENCV
__NAMESPACE_BEGIN_CVMEMSTORAGE
    
void define_ruby_class();

VALUE rb_class();
VALUE rb_allocate(VALUE klass);
void cvmemstorage_free(void *ptr);

VALUE new_object(int blocksize = 0);

__NAMESPACE_END_CVMEMSTORAGE

inline CvMemStorage*
CVMEMSTORAGE(VALUE object)
{
  CvMemStorage *ptr;
  Data_Get_Struct(object, CvMemStorage, ptr);
  return ptr;
}

inline VALUE
CHECK_CVMEMSTORAGE(VALUE object)
{
  if(rb_obj_is_kind_of(object, cCvMemStorage::rb_class()))
    return object;
  else{
    if(!NIL_P(object))
      rb_warn("invalid CvMemStorage object given. allocate new memory storage automatically.");
    return cCvMemStorage::new_object();
  }
}

__NAMESPACE_END_OPENCV

#endif // RUBY_OPENCV_CVMEMSTORAGE_H
