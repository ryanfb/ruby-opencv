/************************************************************

   cvfont.h -

   $Author: lsxi $

   Copyright (C) 2005-2006 Masakazu Yonekura

************************************************************/
#ifndef RUBY_OPENCV_CVFONT_H
#define RUBY_OPENCV_CVFONT_H

#include "opencv.h"

#define __NAMESPACE_BEGIN_CVFONT namespace cCvFont{
#define __NAMESPACE_END_CVFONT }

__NAMESPACE_BEGIN_OPENCV
__NAMESPACE_BEGIN_CVFONT

#define FONT_OPTION(op) NIL_P(op) ? rb_const_get(rb_class(), rb_intern("FONT_OPTION")) : rb_funcall(rb_const_get(rb_class(), rb_intern("FONT_OPTION")), rb_intern("merge"), 1, font_option)
#define FO_ITALIC(op) ({VALUE _italic = rb_hash_aref(op, ID2SYM(rb_intern("italic"))); NIL_P(_italic) ? 0 : _italic == Qfalse ? 0 : CV_FONT_ITALIC;})
#define FO_HSCALE(op) NUM2DBL(rb_hash_aref(op, ID2SYM(rb_intern("hscale"))))
#define FO_VSCALE(op) NUM2DBL(rb_hash_aref(op, ID2SYM(rb_intern("vscale"))))
#define FO_SHEAR(op) NUM2DBL(rb_hash_aref(op, ID2SYM(rb_intern("shear"))))
#define FO_THICKNESS(op) FIX2INT(rb_hash_aref(op, ID2SYM(rb_intern("thickness"))))
#define FO_LINE_TYPE(op) FIX2INT(rb_hash_aref(op, ID2SYM(rb_intern("line_type"))) == ID2SYM("aa") ? INT2FIX(CV_AA) : rb_hash_aref(op, ID2SYM(rb_intern("line_type"))))


VALUE rb_class();

void define_ruby_class();

VALUE rb_allocate(VALUE klass);
VALUE rb_initialize(int argc, VALUE *argv, VALUE self);

VALUE rb_face(VALUE self);
VALUE rb_hscale(VALUE self);
VALUE rb_vscale(VALUE self);
VALUE rb_shear(VALUE self);
VALUE rb_thickness(VALUE self);
VALUE rb_line_type(VALUE self);
VALUE rb_italic(VALUE self);

__NAMESPACE_END_CVFONT

inline CvFont*
CVFONT(VALUE object)
{
  CvFont *ptr;
  Data_Get_Struct(object, CvFont, ptr);
  return ptr;
}

__NAMESPACE_END_OPENCV

#endif // RUBY_OPENCV_CVFONT_H
