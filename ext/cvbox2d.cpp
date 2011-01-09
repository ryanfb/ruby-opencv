/************************************************************

   cvbox2d.cpp -

   $Author: lsxi $

   Copyright (C) 2005-2006 Masakazu Yonekura

************************************************************/
#include "cvbox2d.h"
/*
 * Document-class: OpenCV::CvBox2D
 *
 * C structure is here.
 *   typdef struct CvBox2D{
 *     CvPoint2D32f center; // center of the box.
 *     CvSize2D32f size;    // box width and length
 *     float angle;         // angle between the horizonal axis and the first side (i.e length) in radians
 *   }CvBox2D;
 */
__NAMESPACE_BEGIN_OPENCV
__NAMESPACE_BEGIN_CVBOX2D

VALUE rb_klass;

VALUE
rb_class()
{
  return rb_klass;
}

void
define_ruby_class()
{
  if(rb_klass)
    return;
  /* 
   * opencv = rb_define_module("OpenCV");
   * 
   * note: this comment is used by rdoc.
   */
  VALUE opencv = rb_module_opencv();
  rb_klass = rb_define_class_under(opencv, "CvBox2D", rb_cObject);
  rb_define_alloc_func(rb_klass, rb_allocate);
  rb_define_method(rb_klass, "initialize", RUBY_METHOD_FUNC(rb_initialize), -1);
  rb_define_method(rb_klass, "center", RUBY_METHOD_FUNC(rb_center), 0);
  rb_define_method(rb_klass, "center=", RUBY_METHOD_FUNC(rb_set_center), 1);
  rb_define_method(rb_klass, "size", RUBY_METHOD_FUNC(rb_size), 0);
  rb_define_method(rb_klass, "size=", RUBY_METHOD_FUNC(rb_set_size), 1);
  rb_define_method(rb_klass, "angle", RUBY_METHOD_FUNC(rb_angle), 0);
  rb_define_method(rb_klass, "angle=", RUBY_METHOD_FUNC(rb_set_angle), 1);
  rb_define_method(rb_klass, "points", RUBY_METHOD_FUNC(rb_points), 0);
}

VALUE
rb_allocate(VALUE klass)
{
  CvBox2D *ptr;
  return Data_Make_Struct(klass, CvBox2D, 0, -1, ptr);
}

/* 
 * call-seq:
 *   CvBox2D.new(<i>[center][, size][, angle]</i>) -> cvbox2d
 *
 * Create a box
 */
VALUE
rb_initialize(int argc, VALUE *argv, VALUE self)
{
  VALUE center, size, angle;
  rb_scan_args(argc, argv, "03", &center, &size, &angle);
  if (!NIL_P(center))
    CVBOX2D(self)->center = VALUE_TO_CVPOINT2D32F(center);

  if (!NIL_P(size))
    CVBOX2D(self)->size = VALUE_TO_CVSIZE2D32F(size);
  
  if (!NIL_P(size))
    CVBOX2D(self)->angle = NUM2DBL(angle);

  return self;
}

/*
 * call-seq:
 *   center -> cvpoint2d32f
 * Return center point of box as CvPoint2D32f.
 */
VALUE
rb_center(VALUE self)
{
  return REFER_OBJECT(cCvPoint2D32f::rb_class(), &CVBOX2D(self)->center, self);
}

/*
 * call-seq:
 *   center = <i>value</i>
 *
 * Set center to <i>value</i>
 */
VALUE
rb_set_center(VALUE self, VALUE value)
{
  if (!cCvPoint2D32f::rb_compatible_q(rb_klass, value))
    rb_raise(rb_eArgError, "object is not compatible %s.", rb_class2name(cCvPoint2D32f::rb_class()));
  CVBOX2D(self)->center = VALUE_TO_CVPOINT2D32F(value);
  return self;
}

/*
 * call-seq:
 *   size -> cvsize2d32f
 * Return size of box as CvSize2D32f.
 */
VALUE
rb_size(VALUE self)
{
  return REFER_OBJECT(cCvSize2D32f::rb_class(), &CVBOX2D(self)->size, self);
}

/*
 * call-seq:
 *   size = <i>value</i>
 *
 * Set size to <i>value</i>
 */
VALUE
rb_set_size(VALUE self, VALUE value)
{
  if (!cCvSize2D32f::rb_compatible_q(rb_klass, value))
    rb_raise(rb_eArgError, "object is not compatible %s.", rb_class2name(cCvSize2D32f::rb_class()));
  CVBOX2D(self)->size = VALUE_TO_CVSIZE2D32F(value);
  return self;
}

/*
 * call-seq:
 *   angle -> float
 *
 * Return angle of box as Float.
 */
VALUE
rb_angle(VALUE self)
{
  return rb_float_new(CVBOX2D(self)->angle);
}

/*
 * call-seq:
 *   angle = <i>value</i>
 *
 * Set angle to <i>value</i>
 */
VALUE
rb_set_angle(VALUE self, VALUE value)
{
  CVBOX2D(self)->angle = NUM2DBL(value);
  return self;
}

/*
 * call-seq:
 *   points -> array(include cvpoint2d32f)
 * Find box vertices. Return Array contain 4 CvPoint2D32f.
 */
VALUE
rb_points(VALUE self)
{
  const int n = 4;
  CvPoint2D32f p[n];
  cvBoxPoints(*CVBOX2D(self), p);
  VALUE points = rb_ary_new2(n);
  for(int i = 0; i < n; i++)
    rb_ary_store(points, i, cCvPoint2D32f::new_object(p[i]));        
  return points;
}

VALUE
new_object()
{
  return rb_allocate(cCvBox2D::rb_class());
}

VALUE
new_object(CvBox2D box)
{
  VALUE object = rb_allocate(rb_klass);
  *CVBOX2D(object) = box;
  return object;
}

__NAMESPACE_END_CVBOX2D
__NAMESPACE_END_OPENCV
