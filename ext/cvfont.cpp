/************************************************************

   cvfont.cpp -

   $Author: lsxi $

   Copyright (C) 2005-2006 Masakazu Yonekura

************************************************************/
#include "cvfont.h"
/*
 * Document-class: OpenCV::CvFont
 *
 * Font structure that can be passed to text rendering functions. 
 * see CvMat#put_text, CvMat#put_text!
 */

__NAMESPACE_BEGIN_OPENCV
__NAMESPACE_BEGIN_CVFONT

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
  
  rb_klass = rb_define_class_under(opencv, "CvFont", rb_cObject);
  rb_define_alloc_func(rb_klass, rb_allocate);
  VALUE face = rb_hash_new();
  rb_define_const(rb_klass, "FACE", face);
  rb_hash_aset(face, ID2SYM(rb_intern("simplex")), INT2FIX(CV_FONT_HERSHEY_SIMPLEX));
  rb_hash_aset(face, ID2SYM(rb_intern("plain")), INT2FIX(CV_FONT_HERSHEY_PLAIN));
  rb_hash_aset(face, ID2SYM(rb_intern("duplex")), INT2FIX(CV_FONT_HERSHEY_DUPLEX));
  rb_hash_aset(face, ID2SYM(rb_intern("triplex")), INT2FIX(CV_FONT_HERSHEY_TRIPLEX));
  rb_hash_aset(face, ID2SYM(rb_intern("complex_small")), INT2FIX(CV_FONT_HERSHEY_COMPLEX_SMALL));
  rb_hash_aset(face, ID2SYM(rb_intern("script_simplex")), INT2FIX(CV_FONT_HERSHEY_SCRIPT_SIMPLEX));
  rb_hash_aset(face, ID2SYM(rb_intern("script_complex")), INT2FIX(CV_FONT_HERSHEY_SCRIPT_COMPLEX));

  VALUE default_option = rb_hash_new();
  rb_define_const(rb_klass, "FONT_OPTION", default_option);
  rb_hash_aset(default_option, ID2SYM(rb_intern("hscale")), rb_float_new(1.0));
  rb_hash_aset(default_option, ID2SYM(rb_intern("vscale")), rb_float_new(1.0));
  rb_hash_aset(default_option, ID2SYM(rb_intern("shear")), INT2FIX(0));
  rb_hash_aset(default_option, ID2SYM(rb_intern("thickness")), INT2FIX(1));
  rb_hash_aset(default_option, ID2SYM(rb_intern("line_type")), INT2FIX(8));
  
  rb_define_private_method(rb_klass, "initialize", RUBY_METHOD_FUNC(rb_initialize), -1);
}

VALUE
rb_allocate(VALUE klass)
{
  CvFont *ptr;
  return Data_Make_Struct(klass, CvFont, 0, -1, ptr);
}


/*
 * call-seq:
 *   CvFont.new(<i>face[,font_option]</i>) -> font
 *
 * Create font object.
 * <i>face</i> is font name identifier. 
 * 
 * Only a subset of Hershey fonts (http://sources.isc.org/utils/misc/hershey-font.txt) are supported now:
 * * :simplex - normal size sans-serif font
 * * :plain - small size sans-serif font
 * * :duplex - normal size sans-serif font (more complex than :simplex)
 * * :complex - normal size serif font
 * * :triplex - normal size serif font (more complex than :complex)
 * * :complex_small - smaller version of :complex
 * * :script_simplex - hand-writing style font
 * * :script_complex - more complex variant of :script_simplex
 *
 * <i>font_option</i> should be Hash include these keys.
 *   :hscale
 *     Horizontal scale. If equal to 1.0, the characters have the original width depending on the font type.
 *     If equal to 0.5, the characters are of half the original width.
 *   :vscale
 *     Vertical scale. If equal to 1.0, the characters have the original height depending on the font type.
 *     If equal to 0.5, the characters are of half the original height.
 *   :shear
 *     Approximate tangent of the character slope relative to the vertical line.
 *     Zero value means a non-italic font, 1.0f means ~45degree slope, etc.
 *   :thickness
 *     Thickness of the text strokes.
 *   :line_type
 *     Type of the strokes, see CvMat#Line description.
 *   :italic
 *     If value is not nil or false that means italic or oblique font.
 *
 * note: <i>font_option</i>'s default value is CvFont::FONT_OPTION.
 *
 * e.g. Create Font
 *   OpenCV::CvFont.new(:simplex, :hscale => 2, :vslace => 2, :italic => true)
 *   # create 2x bigger than normal, italic type font.
 */
VALUE
rb_initialize(int argc, VALUE *argv, VALUE self)
{
  VALUE face, font_option;
  rb_scan_args(argc, argv, "11", &face, &font_option);
  Check_Type(face, T_SYMBOL);
  face = rb_hash_aref(rb_const_get(cCvFont::rb_class(), rb_intern("FACE")), face);
  if (NIL_P(face)) {
    rb_raise(rb_eArgError, "undefined face.");
  }
  font_option = FONT_OPTION(font_option);
  /*
  cvInitFont(CVFONT(self),
	     (FIX2INT(face) | FO_ITALIC(font_option)),
	     FO_HSCALE(font_option),
	     FO_VSCALE(font_option),
	     FO_SHEAR(font_option),
	     FO_THICKNESS(font_option),
	     FO_LINE_TYPE(font_option));
  */
  return self;
}


VALUE
rb_face(VALUE self)
{
  return FIX2INT(CVFONT(self)->font_face);
}

VALUE
rb_hscale(VALUE self)
{
  return rb_float_new(CVFONT(self)->hscale);
}

VALUE
rb_vscale(VALUE self)
{
  return rb_float_new(CVFONT(self)->vscale);
}

VALUE
rb_shear(VALUE self)
{
  return rb_float_new(CVFONT(self)->shear);
}

VALUE
rb_thickness(VALUE self)
{
  return FIX2INT(CVFONT(self)->thickness);
}

VALUE
rb_line_type(VALUE self)
{
  return FIX2INT(CVFONT(self)->line_type);
}

__NAMESPACE_END_CVFONT
__NAMESPACE_END_OPENCV
