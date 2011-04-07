/************************************************************

   cvline.cpp -

   $Author: lsxi $

   Copyright (C) 2005-2006 Masakazu Yonekura

************************************************************/
#include "cvline.h"
/*
 * Document-class: OpenCV::CvLine
 *
 */
__NAMESPACE_BEGIN_OPENCV
__NAMESPACE_BEGIN_CVLINE

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
  
  rb_klass = rb_define_class_under(opencv, "CvLine", rb_cObject);
  rb_define_alloc_func(rb_klass, rb_allocate);
  rb_define_method(rb_klass, "rho", RUBY_METHOD_FUNC(rb_rho), 0);
  rb_define_method(rb_klass, "rho=", RUBY_METHOD_FUNC(rb_set_rho), 1);
  rb_define_method(rb_klass, "theta", RUBY_METHOD_FUNC(rb_theta), 0);
  rb_define_method(rb_klass, "theta=", RUBY_METHOD_FUNC(rb_set_theta), 1);
  rb_define_method(rb_klass, "[]", RUBY_METHOD_FUNC(rb_aref), 1);
  rb_define_method(rb_klass, "[]=", RUBY_METHOD_FUNC(rb_aset), 2);
}

VALUE
rb_allocate(VALUE klass)
{
  CvLine *ptr;
  return Data_Make_Struct(klass, CvLine, 0, -1, ptr);
}

/*
 * Return parameter on rho.
 */
VALUE
rb_rho(VALUE self)
{
  return rb_float_new(CVLINE(self)->rho);
}

/*
 * call-seq:
 *   rho = val
 *
 * Set rho parameter, return self.
 */
VALUE
rb_set_rho(VALUE self, VALUE rho)
{
  CVLINE(self)->rho = NUM2DBL(rho);
  return self;
}

/*
 * Return parameter on theta.
 */
VALUE
rb_theta(VALUE self)
{
  return rb_float_new(CVLINE(self)->theta);
}

/*
 * call-seq:
 *   y = val
 *
 * Set theta parameter, return self.
 */
VALUE
rb_set_theta(VALUE self, VALUE theta)
{
  CVLINE(self)->theta = NUM2DBL(theta);
  return self;
}

/*
 * call-seq:
 *   [<i>index</i>]
 *
 * Return value of <i>index</i> dimension.
 */
VALUE
rb_aref(VALUE self, VALUE index)
{
  switch (NUM2INT(index)) {
  case 0:
    return DBL2NUM(CVLINE(self)->rho);
    break;
  case 1:
    return DBL2NUM(CVLINE(self)->theta);
    break;
  default:
    rb_raise(rb_eIndexError, "index should be 0...2");
    break;
  }
  return Qnil;
}

/*
 * call-seq:
 *   [<i>index</i>] = <i>value</i>
 *
 * Set value of <i>index</i> dimension to <i>value</i>
 */
VALUE
rb_aset(VALUE self, VALUE index, VALUE value)
{
  switch (NUM2INT(index)) {
  case 0:
    CVLINE(self)->rho = NUM2DBL(value);
    break;
  case 1:
    CVLINE(self)->theta = NUM2DBL(value);
    break;
  default:
    rb_raise(rb_eIndexError, "index should be 0...2");
    break;
  }
  return value;
}

VALUE
new_object(CvLine line)
{
  VALUE object = rb_allocate(rb_klass);
  *CVLINE(object) = line;
  return object;
}

__NAMESPACE_END_CVLINE
__NAMESPACE_END_OPENCV
