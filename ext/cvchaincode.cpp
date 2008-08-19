/************************************************************

   cvchaincode.cpp -

   $Author: lsxi $

   Copyright (C) 2007 Masakazu Yonekura

************************************************************/
#include "cvchaincode.h"
/*
 * Document-class: OpenCV::CvChainCode
 *
 * Freeman chain code.
 */
__NAMESPACE_BEGIN_OPENCV
__NAMESPACE_BEGIN_CVCHAINCODE

VALUE rb_klass;

VALUE
rb_class()
{
  return rb_klass;
}

void
define_ruby_class()
{
  if (rb_klass)
    return;
  /* 
   * opencv = rb_define_module("OpenCV");
   * 
   * note: this comment is used by rdoc.
   */
  VALUE opencv = rb_module_opencv();
  
  rb_klass = rb_define_class_under(opencv, "CvChainCode", rb_cObject);
}

VALUE
rb_to_i(VALUE self)
{
  return CHR2FIX(CVCHAINCODE(self)->code);
}

__NAMESPACE_END_CVCHAINCODE
__NAMESPACE_END_OPENCV
