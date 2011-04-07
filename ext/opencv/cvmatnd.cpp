/***********************************************************

   cvmatnd.cpp -

   $Author: lsxi $

   Copyright (C) 2007 Masakazu Yonekura

************************************************************/
#include "cvmatnd.h"
/*
 * Document-class: OpenCV::CvMatND
 *
 */
__NAMESPACE_BEGIN_OPENCV
__NAMESPACE_BEGIN_CVMATND

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
   * cvmat = rb_define_class_under(opencv, "CvMat", rb_cObject);
   *
   * note: this comment is used by rdoc.
   */
  VALUE opencv = rb_module_opencv(), cvmat = cCvMat::rb_class();
  
  rb_klass = rb_define_class_under(opencv, "CvMatND", cvmat);
}

__NAMESPACE_END_CVMATND
__NAMESPACE_END_OPENCV

