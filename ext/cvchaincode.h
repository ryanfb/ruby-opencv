/************************************************************

    opchaincode.h -

    $Author: lsxi $

    Copyright (C) 2008 Masakazu Yonekura

 ************************************************************/
#ifndef RUBY_OPENCV_CVCHAINCODE_H
#define RUBY_OPENCV_CVCHAINCODE_H

#include "opencv.h"

#define __NAMESPACE_BEGIN_CVCHAINCODE namespace cCvChainCode{
#define __NAMESPACE_END_CVCHAINCODE }

typedef struct CvChainCode{
  char code;
}CvChainCode;

__NAMESPACE_BEGIN_OPENCV
__NAMESPACE_BEGIN_CVCHAINCODE

VALUE rb_class();

void define_ruby_class();

VALUE rb_to_i(VALUE self);

__NAMESPACE_END_CVCHAINCODE

inline CvChainCode*
CVCHAINCODE(VALUE object){
  CvChainCode *ptr;
  Data_Get_Struct(object, CvChainCode, ptr);
  return ptr;
}


__NAMESPACE_END_OPENCV

#endif // RUBY_OPENCV_CVCHAINCODE_H
