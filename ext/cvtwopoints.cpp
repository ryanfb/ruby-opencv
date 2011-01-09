/************************************************************

   cvtwopoints.cpp -

   $Author: lsxi $

   Copyright (C) 2005-2006 Masakazu Yonekura

************************************************************/
#include "cvtwopoints.h"
/*
 * Document-class: OpenCV::CvTwopoints
 *
 * This class means one twopoints on X axis Y axis. 
 * X and Y takes the value of the Fixnum. see also CvTwopoints2D32F
 * 
 * C structure is here, very simple.
 *   typdef struct CvTwopoints{
 *     int x;
 *     int y;
 *   }
 */
__NAMESPACE_BEGIN_OPENCV
__NAMESPACE_BEGIN_CVTWOPOINTS

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
  
  rb_klass = rb_define_class_under(opencv, "CvTwoPoints", rb_cObject);
  rb_define_alloc_func(rb_klass, rb_allocate);
  rb_define_method(rb_klass, "point1", RUBY_METHOD_FUNC(rb_point1), 0);
  rb_define_method(rb_klass, "point2", RUBY_METHOD_FUNC(rb_point2), 0);
  rb_define_method(rb_klass, "to_ary", RUBY_METHOD_FUNC(rb_to_ary), 0);
  rb_define_alias(rb_klass, "to_a", "to_ary");
}

VALUE
rb_allocate(VALUE klass)
{
  CvTwoPoints *ptr;
  return Data_Make_Struct(klass, CvTwoPoints, 0, -1, ptr);
}

VALUE
rb_cvseqblock(VALUE self)
{
  return Qnil;
}


/*
 * Return point 1.
 */
VALUE
rb_point1(VALUE self)
{
  return cCvPoint::new_object(CVTWOPOINTS(self)->p1);
}

/*
 * Return point2.
 */
VALUE
rb_point2(VALUE self)
{
  return cCvPoint::new_object(CVTWOPOINTS(self)->p2);
}

/*
 * call-seq:
 *   to_ary -> [self.point1, self.point2]
 *
 * Return 2 point by Array.
 */
VALUE
rb_to_ary(VALUE self)
{
  return rb_ary_new3(2, rb_point1(self), rb_point2(self));
}

__NAMESPACE_END_CVTWOPOINTS
__NAMESPACE_END_OPENCV
