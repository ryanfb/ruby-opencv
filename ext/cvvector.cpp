/************************************************************

   cvvector.cpp -

   $Author: lsxi $

   Copyright (C) 2005-2006 Masakazu Yonekura

************************************************************/
#include "cvvector.h"
/*
 * Document-class: OpenCV::CvVector
 *
 * 2D vector (X axis and Y axis) 
 */
__NAMESPACE_BEGIN_OPENCV
__NAMESPACE_BEGIN_CVVECTOR

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
  
  rb_klass = rb_define_class_under(opencv, "CvVector", rb_cObject);
  rb_define_alloc_func(rb_klass, rb_allocate);
  rb_define_singleton_method(rb_klass, "compatible?", RUBY_METHOD_FUNC(rb_compatible_q), 1);
  rb_define_private_method(rb_klass, "initialize", RUBY_METHOD_FUNC(rb_initialize), -1);
  rb_define_method(rb_klass, "x", RUBY_METHOD_FUNC(rb_x), 0);
  rb_define_method(rb_klass, "x=", RUBY_METHOD_FUNC(rb_set_x), 1);
  rb_define_method(rb_klass, "y", RUBY_METHOD_FUNC(rb_y), 0);
  rb_define_method(rb_klass, "y=", RUBY_METHOD_FUNC(rb_set_y), 1);

  rb_define_method(rb_klass, "to_s", RUBY_METHOD_FUNC(rb_to_s), 0);
  rb_define_method(rb_klass, "to_ary", RUBY_METHOD_FUNC(rb_to_ary), 0);
}

/*
 * call-seq:
 *   combatible?(obj)
 *
 * Return compatibility to CvVector. Return true if object have method #x and #y.
 *
 * For example.
 *   class MyVector
 *     def x
 *       1.0
 *     end
 *     def y
 *       2.0
 *     end
 *   end
 *   mv = MyVector.new
 *   CvVector.compatible?(mv)  #=> true
 *   CvVector.new(mv)          #=> same as CvVector(1.0, 2.0)
 */
VALUE
rb_compatible_q(VALUE klass, VALUE object)
{
  return (rb_respond_to(object, rb_intern("x")) && rb_respond_to(object, rb_intern("y"))) ? Qtrue : Qfalse;
}

VALUE
rb_allocate(VALUE klass)
{
  CvVector *ptr;
  return Data_Make_Struct(klass, CvVector, 0, -1, ptr);
}

/*
 * call-seq:
 *   new -> CvVector.new(0.0, 0.0)
 *   new(obj) -> CvVector.new(obj.x.to_f, obj.y.to_f)
 *   new(x, y)
 *
 * Create new 2D-vector, (x, y).
 *
 * new() is same as new(0, 0)
 *
 * new(obj) is same as new(obj.x.to_f, obj.y.to_f)
 */
VALUE
rb_initialize(int argc, VALUE *argv, VALUE self)
{
  VALUE obj, x, y;
  switch (argc) {
  case 0:
    break;
  case 1:
    obj = argv[0];
    if (rb_compatible_q(rb_klass, obj)) {
      CVVECTOR(self)->x = NUM2INT(rb_funcall(rb_funcall(obj, rb_intern("x"), 0), rb_intern("to_i"), 0));
      CVVECTOR(self)->y = NUM2INT(rb_funcall(rb_funcall(obj, rb_intern("y"), 0), rb_intern("to_i"), 0));
    }else{
      rb_raise(rb_eArgError, "object is not compatible %s.", rb_class2name(rb_klass));
    }
    break;
  case 2:
    x = argv[0], y = argv[1];
    CVVECTOR(self)->x = NUM2INT(x);
    CVVECTOR(self)->y = NUM2INT(y);
    break;
  default:
    rb_raise(rb_eArgError, "wrong number of arguments (%d for 0..2)", argc);
  }
  return Qnil;    
}

/*
 * Return parameter on vector of x-axis.
 */
VALUE
rb_x(VALUE self)
{
  return INT2FIX(CVVECTOR(self)->x);
}

/*
 * call-seq:
 *   x = val
 *
 * Set vector of x-axis, return self.
 */
VALUE
rb_set_x(VALUE self, VALUE x)
{
  CVVECTOR(self)->x = NUM2INT(x);
  return self;
}

/*
 * Return parameter on vector of y-axis.
 */
VALUE
rb_y(VALUE self)
{
  return INT2FIX(CVVECTOR(self)->y);
}

/*
 * call-seq:
 *   y = val
 *
 * Set vector of y-axis, return self.
 */
VALUE
rb_set_y(VALUE self, VALUE y)
{
  CVVECTOR(self)->y = NUM2INT(y);
  return self;
}

/*
 * call-seq:
 *   to_s -> "<OpenCV::CvVector:(self.x,self.y)>"
 *
 * Return x and y by String.
 */
VALUE
rb_to_s(VALUE self)
{
  const int i = 4;
  VALUE str[i];
  str[0] = rb_str_new2("<%s:(%d,%d)>");
  str[1] = rb_str_new2(rb_class2name(CLASS_OF(self)));
  str[2] = rb_x(self);
  str[3] = rb_y(self);
  return rb_f_sprintf(i, str);
}

/*
 * call-seq:
 *   to_ary -> [x, y]
 *
 * Return x and y by Array.
 */
VALUE
rb_to_ary(VALUE self)
{
  return rb_ary_new3(2, rb_x(self), rb_y(self));
}

VALUE
new_object(CvVector point)
{
  VALUE object = rb_allocate(rb_klass);
  *CVVECTOR(object) = point;
  return object;
}

__NAMESPACE_END_CVVECTOR
__NAMESPACE_END_OPENCV
