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
  rb_define_method(rb_klass, "center", RUBY_METHOD_FUNC(rb_center), 0);
  rb_define_method(rb_klass, "size", RUBY_METHOD_FUNC(rb_size), 0);
  rb_define_method(rb_klass, "angle", RUBY_METHOD_FUNC(rb_angle), 0);
  rb_define_method(rb_klass, "points", RUBY_METHOD_FUNC(rb_points), 0);
}

VALUE
rb_allocate(VALUE klass)
{
  CvBox2D *ptr;
  return Data_Make_Struct(klass, CvBox2D, 0, -1, ptr);
}

/*
 * Return center point of box as CvPoint2D32f.
 */
VALUE
rb_center(VALUE self)
{
  return REFER_OBJECT(cCvPoint2D32f::rb_class(), &CVBOX2D(self)->center, self);
}

/*
 * Return size of box as CvSize2D32f.
 */
VALUE
rb_size(VALUE self)
{
  return REFER_OBJECT(cCvSize2D32f::rb_class(), &CVBOX2D(self)->size, self);
}

/*
 * Return angle of box as Float.
 */
VALUE
rb_angle(VALUE self)
{
  return rb_float_new(CVBOX2D(self)->angle);
}

/*
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
