/************************************************************

   cvrect.h -

   $Author: lsxi $

   Copyright (C) 2005-2006 Masakazu Yonekura

************************************************************/
#ifndef RUBY_OPENCV_CVRECT_H
#define RUBY_OPENCV_CVRECT_H

#include"opencv.h"

#define __NAMESPACE_BEGIN_CVRECT namespace cCvRect{
#define __NAMESPACE_END_CVRECT }

__NAMESPACE_BEGIN_OPENCV
__NAMESPACE_BEGIN_CVRECT

VALUE rb_class();

void define_ruby_class();

VALUE rb_compatible_q(VALUE klass, VALUE object);
VALUE rb_max_rect(VALUE klass, VALUE rect1, VALUE rect2);
VALUE rb_bounding(VALUE klass, VALUE points);

VALUE rb_allocate(VALUE klass);
VALUE rb_initialize(int argc, VALUE *argv, VALUE self);
VALUE rb_x(VALUE self);
VALUE rb_set_x(VALUE self, VALUE x);
VALUE rb_y(VALUE self);
VALUE rb_set_y(VALUE self, VALUE y);
VALUE rb_width(VALUE self);
VALUE rb_set_width(VALUE self, VALUE width);
VALUE rb_height(VALUE self);
VALUE rb_set_height(VALUE self, VALUE height);

VALUE rb_center(VALUE self);
VALUE rb_points(VALUE self);
VALUE rb_top_left(VALUE self);
VALUE rb_set_top_left(VALUE self, VALUE point);
VALUE rb_top_right(VALUE self);
VALUE rb_set_top_right(VALUE self, VALUE point);
VALUE rb_bottom_left(VALUE self);
VALUE rb_set_bottom_left(VALUE self, VALUE point);
VALUE rb_bottom_right(VALUE self);
VALUE rb_set_bottom_right(VALUE self, VALUE point);

VALUE new_object(CvRect rect);

__NAMESPACE_END_CVRECT

inline CvRect*
CVRECT(VALUE object)
{
  CvRect *ptr;
  Data_Get_Struct(object, CvRect, ptr);
  return ptr;
}

inline CvRect
VALUE_TO_CVRECT(VALUE object)
{
  if(cCvRect::rb_compatible_q(cCvRect::rb_class(), object)){
    return cvRect(NUM2INT(rb_funcall(object, rb_intern("x"), 0)),
                  NUM2INT(rb_funcall(object, rb_intern("y"), 0)),
                  NUM2INT(rb_funcall(object, rb_intern("width"), 0)),
                  NUM2INT(rb_funcall(object, rb_intern("height"), 0)));
  }else{
      rb_raise(rb_eTypeError, "require %s or compatible object.", rb_class2name(cCvRect::rb_class()));
  }
}

__NAMESPACE_END_OPENCV

#endif // RUBY_OPENCV_CVRECT_H
