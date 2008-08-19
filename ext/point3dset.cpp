/************************************************************

   point3dset.cpp -

   $Author: lsxi $

   Copyright (C) 2008 Masakazu Yonekura

************************************************************/
#include"point3dset.h"
/*
 * Document-class: OpenCV::Point3DSet
 */

__NAMESPACE_BEGIN_OPENCV
__NAMESPACE_BEGIN_POINT3D_SET

VALUE module;

VALUE
rb_module()
{
  return module;
}

void
define_ruby_module()
{
  if(module)
    return;
  /* 
   * opencv = rb_define_module("OpenCV");
   *
   * note: this comment is used by rdoc.
   */
  VALUE opencv = rb_module_opencv();
  module = rb_define_module_under(opencv, "Point3DSet");
}

__NAMESPACE_END_POINT3D_SET
__NAMESPACE_END_OPENCV
