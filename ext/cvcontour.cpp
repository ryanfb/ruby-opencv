/************************************************************

   cvcontour.cpp -

   $Author: lsxi $

   Copyright (C) 2007 Masakazu Yonekura

************************************************************/
#include "cvcontour.h"
/*
 * Document-class: OpenCV::CvContour
 *
 * Contour.
 * CvMat#find_contours
 */
__NAMESPACE_BEGIN_OPENCV
__NAMESPACE_BEGIN_CVCONTOUR

#define APPROX_POLY_OPTION(op) NIL_P(op) ? rb_const_get(rb_class(), rb_intern("APPROX_OPTION")) : rb_funcall(rb_const_get(rb_class(), rb_intern("APPROX_OPTION")), rb_intern("merge"), 1, op)
#define APPROX_POLY_METHOD(op) CVMETHOD("APPROX_POLY_METHOD", rb_hash_aref(op, ID2SYM(rb_intern("method"))), CV_POLY_APPROX_DP)
#define APPROX_POLY_ACCURACY(op) NUM2DBL(rb_hash_aref(op, ID2SYM(rb_intern("accuracy"))))
#define APPROX_POLY_RECURSIVE(op) ({VALUE _recursive = rb_hash_aref(op, ID2SYM(rb_intern("recursive"))); NIL_P(_recursive) ? 0 : _recursive == Qfalse ? 0 : 1;})

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
   * cvseq = rb_define_class_under(opencv, "CvSeq");
   * curve = rb_define_module_under(opencv, "Curve");
   * pointset = rb_define_module_under(opencv, "PointSet");
   *
   * note: this comment is used by rdoc.
   */
  VALUE opencv = rb_module_opencv();
  VALUE cvseq = cCvSeq::rb_class();
  VALUE curve = mCurve::rb_module();
  VALUE pointset = mPointSet::rb_module();

  rb_klass = rb_define_class_under(opencv, "CvContour", cvseq);
  rb_include_module(rb_klass, curve);
  rb_include_module(rb_klass, pointset);
  
  VALUE approx_option = rb_hash_new();
  rb_define_const(rb_klass, "APPROX_OPTION", approx_option);
  rb_hash_aset(approx_option, ID2SYM(rb_intern("method")), INT2FIX(CV_POLY_APPROX_DP));
  rb_hash_aset(approx_option, ID2SYM(rb_intern("accuracy")), rb_float_new(1.0));
  rb_hash_aset(approx_option, ID2SYM(rb_intern("recursive")), Qfalse);
  
  rb_define_private_method(rb_klass, "initialize", RUBY_METHOD_FUNC(rb_initialize), -1);
  rb_define_method(rb_klass, "rect", RUBY_METHOD_FUNC(rb_rect), 0);
  rb_define_method(rb_klass, "color", RUBY_METHOD_FUNC(rb_color), 0);
  rb_define_method(rb_klass, "color=", RUBY_METHOD_FUNC(rb_set_color), 1);
  rb_define_method(rb_klass, "reserved", RUBY_METHOD_FUNC(rb_reserved), 0);
  rb_define_method(rb_klass, "approx_poly", RUBY_METHOD_FUNC(rb_approx_poly), -1);
  rb_define_alias(rb_klass, "approx", "approx_poly");
  rb_define_method(rb_klass, "bounding_rect", RUBY_METHOD_FUNC(rb_bounding_rect), 0);
  rb_define_method(rb_klass, "create_tree", RUBY_METHOD_FUNC(rb_create_tree), -1);
  rb_define_method(rb_klass, "in?", RUBY_METHOD_FUNC(rb_in_q), 1);
  rb_define_method(rb_klass, "measure_distance", RUBY_METHOD_FUNC(rb_measure_distance), 1);
}

VALUE
rb_initialize(int argc, VALUE *argv, VALUE self)
{
  /*
  VALUE storage;
  CvSeq *seq = 0;
  rb_scan_args(argc, argv, "01", &storage);
  
  storage = CHECK_CVMEMSTORAGE(storage);
  seq = cvCreateSeq(CV_SEQ_ELTYPE_POINT, sizeof(CvSeq), sizeof(CvPoint), CVMEMSTORAGE(storage));
  DATA_PTR(self) = seq;
  resist_root_object(seq, storage);
  st_insert(cCvSeq::seqblock_klass, (st_data_t)seq, (st_data_t)klass);
  */
  return self;
}

VALUE
rb_rect(VALUE self)
{
  return cCvRect::new_object(CVCONTOUR(self)->rect);
}

VALUE
rb_color(VALUE self)
{
  return INT2NUM(CVCONTOUR(self)->color);
}

VALUE
rb_set_color(VALUE self, VALUE color)
{
  CVCONTOUR(self)->color = NUM2INT(color);
  return self;
}

VALUE
rb_reserved(VALUE self)
{
  return rb_ary_new3(3,
		     INT2NUM(CVCONTOUR(self)->reserved[0]),
		     INT2NUM(CVCONTOUR(self)->reserved[1]),
		     INT2NUM(CVCONTOUR(self)->reserved[2]));
}

/*
 * call-seq:
 *   approx_poly(<i>approx_poly_option</i>) -> cvcontour
 *
 * Approximates polygonal curve(s) with desired precision.
 * <i>approx_poly_option</i> should be Hash include these keys.
 *   :method - Approximation method.
 *     :dp - corresponds to Douglas-Peucker algorithm.
 *   :accuracy - approximation accuracy. (high-accuracy will create more simple contours)
 *   :recursive - (default false)
 *      If not nil or false, the function approximates all chains that access can be obtained to
 *      from self by h_next or v_next links. If 0, approximated this one.
 */

VALUE
rb_approx_poly(int argc, VALUE *argv, VALUE self)
{
  VALUE approx_poly_option, storage;
  rb_scan_args(argc, argv, "01", &approx_poly_option);  
  approx_poly_option = APPROX_POLY_OPTION(approx_poly_option);
  storage = cCvMemStorage::new_object();
  /*
  CvSeq *contour = cvApproxPoly(CVCONTOUR(self), sizeof(CvContour), CVMEMSTORAGE(storage),
				APPROX_POLY_METHOD(approx_poly_option),
				APPROX_POLY_ACCURACY(approx_poly_option),
				APPROX_POLY_RECURSIVE(approx_poly_option));
  return cCvSeq::new_sequence(cCvContour::rb_class(), contour, cCvPoint::rb_class(), storage);
  */
  return Qnil;
}

/*
 * call-seq:
 *   bounding_rect -> rect
 *
 * Calculates up-right bounding rectangle of point set.
 * 
 */
VALUE
rb_bounding_rect(VALUE self)
{
  return cCvRect::new_object(cvBoundingRect(CVCONTOUR(self), 1));
}

/*
 * call-seq:
 *   create_tree([threshold = 0.0]) -> cvcontourtree
 *
 * Creates hierarchical representation of contour.
 * If the parameter <i>threshold</i> is less than or equal to 0,
 * the method creates full binary tree representation.
 * If the threshold is greater than 0, the function creates
 * representation with the precision threshold: 
 */
VALUE
rb_create_tree(int argc, VALUE *argv, VALUE self)
{
  VALUE threshold, storage;
  rb_scan_args(argc, argv, "01", &threshold);
  storage = cCvMemStorage::new_object();
  CvContourTree *tree = cvCreateContourTree(CVSEQ(self), CVMEMSTORAGE(storage), IF_DBL(threshold, 0.0));
  return cCvSeq::new_sequence(cCvContourTree::rb_class(), (CvSeq*)tree, cCvPoint::rb_class(), storage);
}

/*
 * call-seq:
 *    in?(<i>point</i>) -> true or nil or false
 *
 * Determines whether the <i>point</i> is inside contour(true), outside(false), or lies on an edge(nil).
 */
VALUE
rb_in_q(VALUE self, VALUE point)
{
  double n = cvPointPolygonTest(CVARR(self), VALUE_TO_CVPOINT2D32F(point), 0);
  return n == 0 ? Qnil : n > 0 ? Qtrue : Qfalse;
}

/*
 * call-seq:
 *    measure_distance(<i>point</i>) -> float
 *
 * Return distance between the point and the nearest contour edge.
 */
VALUE
rb_measure_distance(VALUE self, VALUE point)
{
  return rb_float_new(cvPointPolygonTest(CVARR(self), VALUE_TO_CVPOINT2D32F(point), 1));
}


VALUE new_object()
{  
  VALUE storage = cCvMemStorage::new_object();
  CvSeq *seq = cvCreateSeq(CV_SEQ_CONTOUR, sizeof(CvContour), sizeof(CvPoint), CVMEMSTORAGE(storage));
  VALUE object = cCvSeq::new_sequence(cCvContour::rb_class(), seq, cCvPoint::rb_class(), storage);
  return object;
}

__NAMESPACE_END_CVCONTOUR
__NAMESPACE_END_OPENCV

