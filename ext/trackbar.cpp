/************************************************************

   trackbar.cpp -

   $Author: lsxi $

   Copyright (C) 2005 Masakazu Yonekura

************************************************************/
#ifdef HAVE_CALLBACK_H

#include"trackbar.h"
/*
 * Document-class: OpenCV::GUI::Trackbar
 *
 * Simple Trackbar wedget. OpenCV::GUI::Window can treat trackbar.
 * Trackbar can treat only positive-integer value.
 */

__NAMESPACE_BEGIN_OPENCV
__NAMESPACE_BEGIN_GUI
__NAMESPACE_BEGIN_TRACKBAR

VALUE rb_klass;

VALUE rb_class(){
  return rb_klass;
}

void define_ruby_class(){
  if(rb_klass)
    return;
  /* 
   * opencv = rb_define_module("OpenCV");
   * GUI = rb_define_module_under(opencv, "GUI");         
   *
   * note: this comment is used by rdoc.
   */
  VALUE GUI = rb_module_GUI();
  rb_klass = rb_define_class_under(GUI, "Trackbar", rb_cObject);
  rb_define_alloc_func(rb_klass, rb_allocate);
  rb_define_method(rb_klass, "initialize", RUBY_METHOD_FUNC(rb_initialize), -1);
  rb_define_method(rb_klass, "name", RUBY_METHOD_FUNC(rb_name), 0);
  rb_define_method(rb_klass, "max", RUBY_METHOD_FUNC(rb_max), 0);
  rb_define_method(rb_klass, "value", RUBY_METHOD_FUNC(rb_value), 0);
  rb_define_method(rb_klass, "value=", RUBY_METHOD_FUNC(rb_set_value), 1);
}

VALUE rb_allocate(VALUE klass){
  Trackbar *ptr;
  return Data_Make_Struct(klass, Trackbar, mark, free, ptr);
}

void mark(void *ptr){
  rb_gc_mark(((Trackbar*)ptr)->block);
}

void free(void *ptr){
  //::free(((Trackbar*)ptr)->name);
  ::free(ptr);
}

/*
 * call-seq:
 *   new(<i>name,maxval[,val],&block</i>)
 *   new(<i>name,maxval[,val]</i>){|value| ... }
 *
 * Create new Trackbar.
 * <i>name</i> should be String.
 * <i>maxval</i> and <i>val</i> should be Fixnum.
 * When Trackbar adjuster changed, block will be called.
 */
VALUE rb_initialize(int argc, VALUE *argv, VALUE self){
  VALUE name, maxval, val, block;
  rb_scan_args(argc, argv, "21&", &name, &maxval, &val, &block);
  if(NIL_P(block)){rb_raise(rb_eArgError, "block not given.");}
  Check_Type(name, T_STRING);
  Trackbar *trackbar = TRACKBAR(self);
  trackbar->name = strcpy(ALLOC_N(char, RSTRING(name)->len), StringValueCStr(name));
  trackbar->maxval = NUM2INT(maxval);
  trackbar->val = IF_INT(val, 0);
  trackbar->block = block;
  return self;
}

/*
 * Return trackbar name.
 */
VALUE rb_name(VALUE self){
  return rb_str_new2(TRACKBAR(self)->name);
}

/*
 * Return the maximum value that can be taken this trackbar.
 */
VALUE rb_max(VALUE self){
  return INT2FIX(TRACKBAR(self)->maxval);
}

/*
 * Return the value of this trackbar.
 */
VALUE rb_value(VALUE self){
  return INT2FIX(TRACKBAR(self)->val);
}

/*
 * call-seq:
 *   value  = <i>val</i>
 *
 * Set trackbar value.
 */
VALUE rb_set_value(VALUE self, VALUE val){
  TRACKBAR(self)->val = NUM2INT(val);
  return self;
}
      
__NAMESPACE_END_TRACKBAR
__NAMESPACE_END_GUI
__NAMESPACE_END_OPENCV

#endif // HAVE_CALLBACK_H
