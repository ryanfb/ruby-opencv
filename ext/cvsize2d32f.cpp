/************************************************************

   cvsize2d32f.cpp -

   $Author: lsxi $

   Copyright (C) 2005-2006 Masakazu Yonekura

************************************************************/
#include "cvsize2d32f.h"
/*
 * Document-class: OpenCV::CvSize2D32f
 *
 * This class means one size on X axis Y axis. 
 * X and Y takes the value of the Float.
 * 
 * C structure is here, very simple.
 *   typdef struct CvSize2D32f{
 *     float width;
 *     float height;
 *   }CvSize2D32f;
 */
__NAMESPACE_BEGIN_OPENCV
__NAMESPACE_BEGIN_CVSIZE2D32F

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
  
  rb_klass = rb_define_class_under(opencv, "CvSize2D32f", rb_cObject);
  rb_define_alloc_func(rb_klass, rb_allocate);
  rb_define_singleton_method(rb_klass, "compatible?", RUBY_METHOD_FUNC(rb_compatible_q), 1);
  rb_define_method(rb_klass, "initialize", RUBY_METHOD_FUNC(rb_initialize), -1);
  rb_define_method(rb_klass, "width", RUBY_METHOD_FUNC(rb_width), 0);
  rb_define_method(rb_klass, "width=", RUBY_METHOD_FUNC(rb_set_width), 1);
  rb_define_method(rb_klass, "height", RUBY_METHOD_FUNC(rb_height), 0);
  rb_define_method(rb_klass, "height=", RUBY_METHOD_FUNC(rb_set_height), 1);
  rb_define_method(rb_klass, "to_s", RUBY_METHOD_FUNC(rb_to_s), 0);
  rb_define_method(rb_klass, "to_ary", RUBY_METHOD_FUNC(rb_to_ary), 0);
}

/*
 * call-seq:
 *   compatible?(obj)
 *
 * Return compatibility to CvSize2D32f. Return true if object have method #width and #height.
 *
 * For example.
 *   class MySize
 *     def width
 *       10.1
 *     end
 *     def height
 *       20.2
 *     end
 *   end
 *   mp = MySize.new
 *   CvSize2D32f.compatible?(mp)  #=> true
 *   CvSize2D32f.new(mp)          #=> same as CvSize2D32f.new(10.1, 20.2)
 */
VALUE
rb_compatible_q(VALUE klass, VALUE object)
{
  return (rb_respond_to(object, rb_intern("width")) && rb_respond_to(object, rb_intern("height"))) ? Qtrue : Qfalse;
}

VALUE
rb_allocate(VALUE klass)
{
  CvSize2D32f *ptr;
  return Data_Make_Struct(klass, CvSize2D32f, 0, -1, ptr);
}

/*
 * call-seq:
 *   new
 *   new(obj)
 *   new(width, height)
 *
 * Create new size of 2D, (width, height).
 *
 * new() is same as new(0.0, 0.0)
 *
 * new(obj) is same as new(obj.x.to_f, obj.y.to_f)
 */
VALUE
rb_initialize(int argc, VALUE *argv, VALUE self)
{
  VALUE obj, x, y;
  switch(argc){
  case 0:
    break;
  case 1:
    obj = argv[0];
    if(rb_compatible_q(rb_klass, obj)){
      CVSIZE2D32F(self)->width = NUM2DBL(rb_funcall(rb_funcall(obj, rb_intern("width"), 0), rb_intern("to_f"), 0));
      CVSIZE2D32F(self)->height = NUM2DBL(rb_funcall(rb_funcall(obj, rb_intern("height"), 0), rb_intern("to_f"), 0));
    }else{
      rb_raise(rb_eArgError, "object is not compatible %s.", rb_class2name(rb_klass));
    }
    break;
  case 2:
    x = argv[0], y = argv[1];
    CVSIZE2D32F(self)->width = NUM2DBL(x);
    CVSIZE2D32F(self)->height = NUM2DBL(y);
    break;
  default:
    rb_raise(rb_eArgError, "wrong number of arguments (%d for 0..2)", argc);
  }
  return self;      
}

/*
 * Return size of x-axis.
 */
VALUE
rb_width(VALUE self)
{
  return rb_float_new(CVSIZE2D32F(self)->width);
}

/*
 * call-seq:
 *   width = val
 *
 * Set x-axis size, return self.     
 */
VALUE
rb_set_width(VALUE self, VALUE x)
{
  CVSIZE2D32F(self)->width = NUM2DBL(x);
  return self;
}

/*
 * Return size of yaxis.
 */
VALUE
rb_height(VALUE self)
{
  return rb_float_new(CVSIZE2D32F(self)->height);
}

/*
 * call-seq:
 *   height = val
 *
 * Set y-axis size, return self.
 */
VALUE
rb_set_height(VALUE self, VALUE y)
{
  CVSIZE2D32F(self)->height = NUM2DBL(y);
  return self;
}

/*
 * call-seq:
 *   to_s -> "<OpenCV::CvSize2D32f:widthxheight>"
 *
 * Return width and height by String.
 */
VALUE
rb_to_s(VALUE self)
{
  const int i = 4;
  VALUE str[i];
  str[0] = rb_str_new2("<%s:%gx%g>");
  str[1] = rb_str_new2(rb_class2name(CLASS_OF(self)));
  str[2] = rb_width(self);
  str[3] = rb_height(self);
  return rb_f_sprintf(i, str);
}

/*
 * call-seq:
 *   to_ary -> [width, height]
 *
 * Return width and height by Array.
 */
VALUE
rb_to_ary(VALUE self)
{
  return rb_ary_new3(2, rb_width(self), rb_height(self));
}

VALUE
new_object(CvSize2D32f size)
{
  VALUE object = rb_allocate(rb_klass);
  *CVSIZE2D32F(object) = size;
  return object;
}

__NAMESPACE_END_CVSIZE2D32F
__NAMESPACE_END_OPENCV
