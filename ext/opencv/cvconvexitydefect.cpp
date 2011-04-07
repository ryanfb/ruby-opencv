/************************************************************

   cvconvexitydefect.cpp -

   $Author: lsxi $

   Copyright (C) 2007 Masakazu Yonekura

************************************************************/
#include "cvconvexitydefect.h"
/*
 * Document-class: OpenCV::CvConvexityDefect
 *
 * Convexity.
 * C structure is here.
 *   typedef struct CvConvexityDefect {
 *     CvPoint* start;
 *     CvPoint* end;
 *     CvPoint* depth_point;
 *     float depth;
 *   }CvConvexityDefect;
 *
 */
__NAMESPACE_BEGIN_OPENCV
__NAMESPACE_BEGIN_CVCONVEXITYDEFECT

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
  
  rb_klass = rb_define_class_under(opencv, "CvConvexityDefect", rb_cObject);
  rb_define_method(rb_klass, "start", RUBY_METHOD_FUNC(rb_start), 0);
  rb_define_method(rb_klass, "end", RUBY_METHOD_FUNC(rb_end), 0);
  rb_define_method(rb_klass, "depth_point", RUBY_METHOD_FUNC(rb_depth_point), 0);
  rb_define_method(rb_klass, "depth", RUBY_METHOD_FUNC(rb_depth), 0);
}

/*
 * call-seq:
 *   start -> cvpoint
 *
 * Return start point as CvPoint.
 */
VALUE
rb_start(VALUE self)
{
  return cCvPoint::new_object(*CVCONVEXITYDEFECT(self)->start);
}

/*
 * call-seq:
 *   end -> cvpoint
 *
 * Return end point as CvPoint.
 */
VALUE
rb_end(VALUE self)
{
  return cCvPoint::new_object(*CVCONVEXITYDEFECT(self)->end);
}

/*
 * call-seq:
 *   depth_point -> cvpoint
 *
 * Return depth point as CvPoint.
 */
VALUE
rb_depth_point(VALUE self)
{
  return cCvPoint::new_object(*CVCONVEXITYDEFECT(self)->depth_point);
}

/*
 * call-seq:
 *   depth -> float
 *
 * Return depth.
 */
VALUE
rb_depth(VALUE self)
{
  return rb_float_new(CVCONVEXITYDEFECT(self)->depth);
}

__NAMESPACE_END_CVCONVEXITYDEFECT
__NAMESPACE_END_OPENCV
