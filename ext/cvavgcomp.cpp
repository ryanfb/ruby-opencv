/************************************************************

   cvavgcomp.cpp -

   $Author: lsxi $

   Copyright (C) 2005-2006 Masakazu Yonekura

************************************************************/
#include "cvavgcomp.h"
/*
 * Document-class: OpenCV::CvAvgComp
 *
 * CvRect with parameter "neighbors".
 * CvHaarClassifierCascade#detect_object. 
 *
 *   typedef struct CvAvgComp{
 *     CvRect rect;
 *     int neighbors;
 *   }
 */
__NAMESPACE_BEGIN_OPENCV
__NAMESPACE_BEGIN_AVGCOMP

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
   * cvrect = rb_define_class_under(opencv, "CvRect", rb_cObject);
   *
   * note: this comment is used by rdoc.
   */
  VALUE opencv = rb_module_opencv(), cvrect = cCvRect::rb_class();
  rb_klass = rb_define_class_under(opencv, "CvAvgComp", cvrect);
  rb_define_alloc_func(rb_klass, rb_allocate);
  rb_define_method(rb_klass, "neighbors", RUBY_METHOD_FUNC(rb_neighbors), 0);
}

VALUE
rb_allocate(VALUE klass)
{
  CvAvgComp *ptr;
  return Data_Make_Struct(klass, CvAvgComp, 0, -1, ptr);
}

/*
 * Return neighbors.
 */
VALUE
rb_neighbors(VALUE self)
{
  return INT2FIX(CVAVGCOMP(self)->neighbors);
}

__NAMESPACE_END_AVGCOMP
__NAMESPACE_END_OPENCV
