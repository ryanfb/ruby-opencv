/************************************************************

   cvsize.cpp -

   $Author: lsxi $

   Copyright (C) 2005 Masakazu Yonekura

************************************************************/
#include"cvsize.h"
/*
 * Document-class: OpenCV::CvSize
 *
 * This class means one size on X axis Y axis. 
 * X and Y takes the value of the Fixnum.
 * 
 * C structure is here, very simple.
 *   typdef struct CvSize{
 *     int width;
 *     int height;
 *   }
 */
__NAMESPACE_BEGIN_OPENCV
__NAMESPACE_BEGIN_CVSIZE

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
  
  rb_klass = rb_define_class_under(opencv, "CvSize", rb_cObject);
  rb_define_alloc_func(rb_klass, rb_allocate);
  rb_define_singleton_method(rb_klass, "compatible?", RUBY_METHOD_FUNC(rb_compatible_q), 1);
  rb_define_private_method(rb_klass, "initialize", RUBY_METHOD_FUNC(rb_initialize), -1);
  rb_define_method(rb_klass, "width", RUBY_METHOD_FUNC(rb_width), 0);
  rb_define_method(rb_klass, "width=", RUBY_METHOD_FUNC(rb_set_width), 1);
  rb_define_method(rb_klass, "height", RUBY_METHOD_FUNC(rb_height), 0);
  rb_define_method(rb_klass, "height=", RUBY_METHOD_FUNC(rb_set_height), 1);
  
  rb_define_method(rb_klass, "to_s", RUBY_METHOD_FUNC(rb_to_s), 0);
  rb_define_method(rb_klass, "to_ary", RUBY_METHOD_FUNC(rb_to_ary), 0);
  rb_define_alias(rb_klass, "to_a", "to_ary");
}

/*
 * call-seq:
 *   compatible?(obj)
 *
 * Return compatibility to CvSize. Return true if object have method #width and #height.
 *
 * For example.
 *   class MySize
 *     def width
 *       10
 *     end
 *     def height
 *       20
 *     end
 *   end
 *   mp = MySize.new
 *   CvSize.compatible?(mp)  #=> true
 *   CvSize.new(mp)          #=> same as CvSize(10, 20)
 */
VALUE
rb_compatible_q(VALUE klass, VALUE object)
{
  return (rb_respond_to(object, rb_intern("width")) && rb_respond_to(object, rb_intern("height"))) ? Qtrue : Qfalse;
}

VALUE
rb_allocate(VALUE klass)
{
  CvSize *ptr;
  return Data_Make_Struct(klass, CvSize, 0, -1, ptr);
}

/*
 * call-seq:
 *   new
 *   new(obj)
 *   new(width, height)
 *
 * Create new size of 2D, (width, height). It is dropped below the decimal point.
 *
 * new() is same as new(0, 0)
 *
 * new(obj) is same as new(obj.x.to_i, obj.y.to_i)
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
    if(rb_compatible_q(rb_klass, obj)) {
      CVSIZE(self)->width = NUM2INT(rb_funcall(rb_funcall(obj, rb_intern("width"), 0), rb_intern("to_i"), 0));
      CVSIZE(self)->height = NUM2INT(rb_funcall(rb_funcall(obj, rb_intern("height"), 0), rb_intern("to_i"), 0));
    }else{
      rb_raise(rb_eArgError, "object is not compatible %s.", rb_class2name(rb_klass));
    }
    break;
  case 2:
    x = argv[0], y = argv[1];
    CVSIZE(self)->width = NUM2INT(x);
    CVSIZE(self)->height = NUM2INT(y);
    break;
  default:
    rb_raise(rb_eArgError, "wrong number of arguments (%d for 0..2)", argc);
  }
  return Qnil;      
}

/*
 * Return size of x-axis.
 */
VALUE
rb_width(VALUE self)
{
  return INT2FIX(CVSIZE(self)->width);
}

/*
 * call-seq:
 *   width = val
 *
 * Set x-axis size, return self.
 * It is dropped below the decimal point.
 */
VALUE
rb_set_width(VALUE self, VALUE x)
{
  CVSIZE(self)->width = NUM2INT(x);
  return self;
}

/*
 * Return size of yaxis.
 */
VALUE
rb_height(VALUE self)
{
  return INT2FIX(CVSIZE(self)->height);
}

/*
 * call-seq:
 *   height = val
 *
 * Set y-axis size, return self.
 * It is dropped below the decimal point.
 */
VALUE
rb_set_height(VALUE self, VALUE y)
{
  CVSIZE(self)->height = NUM2INT(y);
  return self;
}

/*
 * call-seq:
 *   to_s -> "<OpenCV::CvSize:widthxheight>"
 *
 * Return width and height by String.
 */
VALUE
rb_to_s(VALUE self)
{
  const int i = 4;
  VALUE str[i];
  str[0] = rb_str_new2("<%s:%dx%d>");
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
new_object()
{
  VALUE object = rb_allocate(rb_klass);
  *CVSIZE(object) = cvSize(0, 0);
  return object;
}

VALUE
new_object(CvSize size)
{
  VALUE object = rb_allocate(rb_klass);
  *CVSIZE(object) = size;
  return object;
}

__NAMESPACE_END_CVSIZE
__NAMESPACE_END_OPENCV
