/************************************************************

   cvmoments.cpp -

   $Author: lsxi $

   Copyright (C) 2007 Masakazu Yonekura

************************************************************/
#include "cvmoments.h"
/*
 * Document-class: OpenCV::CvMoments
 *
 * moments
 */
__NAMESPACE_BEGIN_OPENCV
__NAMESPACE_BEGIN_CVMOMENTS

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
  
  rb_klass = rb_define_class_under(opencv, "CvMoments", rb_cObject);
  rb_define_alloc_func(rb_klass, rb_allocate);
  rb_define_private_method(rb_klass, "initialize", RUBY_METHOD_FUNC(rb_initialize), -1);
  rb_define_method(rb_klass, "spatial", RUBY_METHOD_FUNC(rb_spatial), 2);
  rb_define_method(rb_klass, "central", RUBY_METHOD_FUNC(rb_central), 2);
  rb_define_method(rb_klass, "normalized_central", RUBY_METHOD_FUNC(rb_normalized_central), 2);
  rb_define_method(rb_klass, "hu", RUBY_METHOD_FUNC(rb_hu), 0);     
  rb_define_method(rb_klass, "gravity_center", RUBY_METHOD_FUNC(rb_gravity_center), 0);
  rb_define_alias(rb_klass, "center", "gravity_center");
  rb_define_method(rb_klass, "angle", RUBY_METHOD_FUNC(rb_angle), 0);
}

VALUE
rb_allocate(VALUE klass)
{
  CvMoments *ptr;
  return Data_Make_Struct(klass, CvMoments, 0, -1, ptr);
}
/*
 * call-seq:
 *   CvMoments.new(<i>src[,is_binary = nil]</i>)
 *
 * Calculates all moments up to third order of a polygon or rasterized shape.
 * <i>src</i> should be CvMat or CvPolygon.
 *
 * If is_binary = true, all the zero pixel values are treated as zeroes, all the others are treated as 1's.
 */
VALUE
rb_initialize(int argc, VALUE *argv, VALUE self)
{
  VALUE src, is_binary;
  rb_scan_args(argc, argv, "02", &src, &is_binary);
  if (NIL_P(src))
    ;
  else if (rb_obj_is_kind_of(src, cCvMat::rb_class()) || rb_obj_is_kind_of(src, cCvSeq::rb_class()))
    cvMoments(CVARR(src), CVMOMENTS(self), TRUE_OR_FALSE(is_binary, 0));
  else
    rb_raise(rb_eTypeError, "argument 1 (src) should be %s or %s.", rb_class2name(cCvMat::rb_class()), rb_class2name(cCvSeq::rb_class()));
  return self;      
}

/*
 * call-seq:
 *   spatial -> float
 *
 * Retrieves spatial moment.
 *
 * which in case of image moments is defined as:
 *   Mx_order,y_order=sumx,y(I(x,y)*xx_order*yy_order)
 *   where I(x,y) is the intensity of the pixel (x, y). 
 */
VALUE
rb_spatial(VALUE self, VALUE x_order, VALUE y_order)
{
  return rb_float_new(cvGetSpatialMoment(CVMOMENTS(self), NUM2INT(x_order), NUM2INT(y_order)));
}

/*
 * call-seq:
 *   central -> float
 *
 * Retrieves central moment.
 *
 * which in case of image moments is defined as:
 *   μx_order,y_order=sumx,y(I(x,y)*(x-xc)x_order*(y-yc)y_order),
 *   where xc=M10/M00, yc=M01/M00 - coordinates of the gravity center
 */
VALUE
rb_central(VALUE self, VALUE x_order, VALUE y_order)
{
  return rb_float_new(cvGetCentralMoment(CVMOMENTS(self), NUM2INT(x_order), NUM2INT(y_order)));
}

/*
 * call-seq:
 *   normalized_central -> float
 *
 * Retrieves normalized central moment.
 *
 *  ηx_order,y_order= μx_order,y_order/M00((y_order+x_order)/2+1)
 */
VALUE
rb_normalized_central(VALUE self, VALUE x_order, VALUE y_order)
{
  return rb_float_new(cvGetNormalizedCentralMoment(CVMOMENTS(self), NUM2INT(x_order), NUM2INT(y_order)));
}

/*
 * call-seq:
 *   hu -> [hu1, hu2, ... ,hu7]
 *
 * Calculates seven Hu invariants.
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
rb_hu(VALUE self)
{
  CvHuMoments hu_moments;
  cvGetHuMoments(CVMOMENTS(self), &hu_moments);
  return rb_ary_new3(7,
		     rb_float_new(hu_moments.hu1),
		     rb_float_new(hu_moments.hu2),
		     rb_float_new(hu_moments.hu3),
		     rb_float_new(hu_moments.hu4),
		     rb_float_new(hu_moments.hu5),
		     rb_float_new(hu_moments.hu6),
		     rb_float_new(hu_moments.hu7));
}

/*
 * call-seq:
 *   gravity_center -> cvpoint2d32f
 *
 * Return gravity center.
 */
VALUE
rb_gravity_center(VALUE self)
{
  CvMoments *moments = CVMOMENTS(self);
  double
    m00 = cvGetSpatialMoment(moments, 0, 0),
    m10 = cvGetSpatialMoment(moments, 1, 0),
    m01 = cvGetSpatialMoment(moments, 0, 1);
  return cCvPoint2D32f::new_object(cvPoint2D32f(m10 / m00, m01 / m00));
}

/*
 * call-seq:
 *   angle -> float
 *
 * Return angle.
 */
VALUE
rb_angle(VALUE self)
{
  CvMoments *moments = CVMOMENTS(self);
  double
    m11 = cvGetCentralMoment(moments, 1, 1),
    m20 = cvGetCentralMoment(moments, 2, 0),
    m02 = cvGetCentralMoment(moments, 0, 2),
    mangle = 0.5 * atan(2 * m11 / (m20 - m02));
  if(!cvIsNaN(mangle) && !cvIsInf(mangle))
    return rb_float_new(mangle);
  else
    return Qnil;
}

VALUE
new_object(CvArr *arr, int is_binary = 0)
{
  VALUE object = rb_allocate(rb_class());
  cvMoments(arr, CVMOMENTS(object), TRUE_OR_FALSE(is_binary, 0));
  return object;
}

__NAMESPACE_END_CVMOMENTS
__NAMESPACE_END_OPENCV
