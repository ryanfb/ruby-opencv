/************************************************************

   cvslice.cpp -

   $Author: lsxi $

   Copyright (C) 2005-2006 Masakazu Yonekura

************************************************************/
#include "cvslice.h"
/*
 * Document-class: OpenCV::CvClice
 *
 * C structure is here, very simple.
 *   typdef struct CvSlice{
 *     int start_index;
 *     int end_index;
 *   }CvSlice;
 */
__NAMESPACE_BEGIN_OPENCV
__NAMESPACE_BEGIN_CVSLICE

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
  rb_klass = rb_define_class_under(opencv, "CvSlice", rb_cObject);
  rb_define_alloc_func(rb_klass, rb_allocate);
  rb_define_singleton_method(rb_klass, "compatible?", RUBY_METHOD_FUNC(rb_compatible_q), 1);
  rb_define_method(rb_klass, "initialize", RUBY_METHOD_FUNC(rb_initialize), 2);
}

/*
 * call-seq:
 *   compatible?(<i>obj</i>)
 *
 * same Object#kind_of(Range)
 */
VALUE
rb_compatible_q(VALUE klass, VALUE object)
{
  return rb_obj_is_kind_of(object, rb_cRange);
}

VALUE
rb_allocate(VALUE klass)
{
  CvSlice *ptr;
  return Data_Make_Struct(klass, CvSlice, 0, -1, ptr);
}

/*
 * call-seq:
 *   new(<i>start, end</i>)
 *
 * Create new slice object.
 */
VALUE
rb_initialize(VALUE self, VALUE start, VALUE end)
{
  CVSLICE(self)->start_index = NUM2INT(start);
  CVSLICE(self)->end_index = NUM2INT(end);
  return self;
}

__NAMESPACE_END_CVSLICE
__NAMESPACE_END_OPENCV
