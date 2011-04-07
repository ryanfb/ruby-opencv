/************************************************************

   cvindex.cpp -

   $Author: lsxi $

   Copyright (C) 2005 Masakazu Yonekura

************************************************************/
#include"cvindex.h"
/*
 * Document-class: OpenCV::CvIndex
 *
 * 
 */

namespace mOpenCV{
  
  namespace cCvIndex{
    VALUE rb_klass;

    VALUE rb_class(){
      return rb_klass;
    }
    
    void define_ruby_class(){
      if(rb_klass)
        return;
      /* 
       * opencv = rb_define_module("OpenCV");
       * 
       * note: this comment is used by rdoc.
       */
      VALUE opencv = rb_module_opencv();
      rb_klass = rb_define_class_under(opencv, "CvIndex", rb_cObject);
      rb_define_alloc_func(rb_klass, rb_allocate);
      rb_define_private_method(rb_klass, "initialize", RUBY_METHOD_FUNC(rb_initialize), -1);
      rb_define_singleton_method(rb_klass, "cv_new", RUBY_METHOD_FUNC(rb_cv_new), -1);
      rb_define_method(rb_klass, "value", RUBY_METHOD_FUNC(rb_value), 0);
      rb_define_method(rb_klass, "to_s", RUBY_METHOD_FUNC(rb_to_s), 0);
    }

    VALUE rb_allocate(VALUE klass){
      CvIndex *ptr;
      return Data_Make_Struct(rb_klass, CvIndex, 0, free, ptr);
    }

    VALUE rb_initialize(int argc, VALUE *argv, VALUE self){
      VALUE value;
      if(rb_scan_args(argc, argv, "01", &value) > 0){
        Check_Type(value, T_FIXNUM);
        CVINDEX(self)->value = FIX2INT(value);
      }
      return self;
    }

    VALUE rb_cv_new(int argc, VALUE *argv, VALUE klass){
      VALUE object = rb_allocate(klass);
      return rb_initialize(argc, argv, object);
    }

    VALUE rb_value(VALUE self){
      return INT2FIX(CVINDEX(self)->value);
    }
    
    VALUE rb_to_s(VALUE self){
      return rb_funcall(INT2FIX(CVINDEX(self)->value), rb_intern("to_s"), 0);
    }
    
  }
  
  
}
