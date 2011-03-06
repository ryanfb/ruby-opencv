/************************************************************

   cvhumoments.cpp -

   $Author: ser1zw $

   Copyright (C) 2011 ser1zw

************************************************************/
#include "cvhumoments.h"

/*
 * Document-class: OpenCV::CvHuMoments
 *
 * moments
 */
__NAMESPACE_BEGIN_OPENCV
__NAMESPACE_BEGIN_CVHUMOMENTS

#define DEFINE_CVHUMOMENTS_GETTER(elem)					\
  rb_define_method(rb_klass, #elem, RUBY_METHOD_FUNC(rb_##elem), 0)

#define CVHUMOMENTS_GETTER(elem)					\
  VALUE rb_##elem(VALUE self) { return DBL2NUM(CVHUMOMENTS(self)->elem); }

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
  
  rb_klass = rb_define_class_under(opencv, "CvHuMoments", rb_cObject);
  rb_define_alloc_func(rb_klass, rb_allocate);
  rb_define_private_method(rb_klass, "initialize", RUBY_METHOD_FUNC(rb_initialize), 1);
  rb_define_method(rb_klass, "to_ary", RUBY_METHOD_FUNC(rb_to_ary), 0);
  rb_define_alias(rb_klass, "to_a", "to_ary");
  
  DEFINE_CVHUMOMENTS_GETTER(hu1);
  DEFINE_CVHUMOMENTS_GETTER(hu2);
  DEFINE_CVHUMOMENTS_GETTER(hu3);
  DEFINE_CVHUMOMENTS_GETTER(hu4);
  DEFINE_CVHUMOMENTS_GETTER(hu5);
  DEFINE_CVHUMOMENTS_GETTER(hu6);
  DEFINE_CVHUMOMENTS_GETTER(hu7);
}

VALUE
rb_allocate(VALUE klass)
{
  CvHuMoments *ptr;
  return Data_Make_Struct(klass, CvHuMoments, 0, -1, ptr);
}

/*
 * call-seq:
 *   CvHuMoments.new(<i>src_moments</i>)
 *
 * Calculates the seven Hu invariants.
 * <i>src_moments</i> The input moments
 *
 * seven Hu invariants that are defined as:
 *   h1=η20+η02
 *   h2=(η20-η02)²+4η11²
 *   h3=(η30-3η12)²+ (3η21-η03)²
 *   h4=(η30+η12)²+ (η21+η03)²
 *   h5=(η30-3η12)(η30+η12)[(η30+η12)²-3(η21+η03)²]+(3η21-η03)(η21+η03)[3(η30+η12)²-(η21+η03)²]
 *   h6=(η20-η02)[(η30+η12)²- (η21+η03)²]+4η11(η30+η12)(η21+η03)
 *   h7=(3η21-η03)(η21+η03)[3(η30+η12)²-(η21+η03)²]-(η30-3η12)(η21+η03)[3(η30+η12)²-(η21+η03)²]
 * where ηi,j are normalized central moments of 2-nd and 3-rd orders. The computed values are proved to be invariant to the image scaling, rotation, and reflection except the seventh one, whose sign is changed by reflection. 
 */
VALUE
rb_initialize(VALUE self, VALUE src_moments)
{
  if (rb_obj_is_kind_of(src_moments, cCvMoments::rb_class()))
    cvGetHuMoments(CVMOMENTS(src_moments), CVHUMOMENTS(self));
  else
    rb_raise(rb_eTypeError, "argument 1 (src_moments) should be %s.",
	     rb_class2name(cCvMoments::rb_class()));
  return self;      
}

CVHUMOMENTS_GETTER(hu1);
CVHUMOMENTS_GETTER(hu2);
CVHUMOMENTS_GETTER(hu3);
CVHUMOMENTS_GETTER(hu4);
CVHUMOMENTS_GETTER(hu5);
CVHUMOMENTS_GETTER(hu6);
CVHUMOMENTS_GETTER(hu7);

/*
 * call-seq:
 *   to_ary -> [hu1, hu2, ... ,hu7]
 *
 * Return values by Array.
 */
VALUE
rb_to_ary(VALUE self)
{
  CvHuMoments *hu_moments = CVHUMOMENTS(self);
  return rb_ary_new3(7,
		     rb_float_new(hu_moments->hu1),
		     rb_float_new(hu_moments->hu2),
		     rb_float_new(hu_moments->hu3),
		     rb_float_new(hu_moments->hu4),
		     rb_float_new(hu_moments->hu5),
		     rb_float_new(hu_moments->hu6),
		     rb_float_new(hu_moments->hu7));
}

VALUE
new_object(CvMoments *src_moments)
{
  VALUE object = rb_allocate(rb_class());
  cvGetHuMoments(src_moments, CVHUMOMENTS(object));
  return object;
}

__NAMESPACE_END_CVHUMOMENTS
__NAMESPACE_END_OPENCV
