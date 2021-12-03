/************************************************************

   cvchain.cpp -

   $Author: lsxi $

   Copyright (C) 2007 Masakazu Yonekura

************************************************************/
#include "cvchain.h"
/*
 * Document-class: OpenCV::CvChain
 *
 * Freeman chain code.
 * CvMat#find_contours(:method => :code)
 */
__NAMESPACE_BEGIN_OPENCV
__NAMESPACE_BEGIN_CVCHAIN

#define APPROX_CHAIN_OPTION(op) NIL_P(op) ? rb_const_get(rb_class(), rb_intern("APPROX_CHAIN_OPTION")) : rb_funcall(rb_const_get(rb_class(), rb_intern("APPROX_CHAIN_OPTION")), rb_intern("merge"), 1, op)
#define APPROX_CHAIN_METHOD(op) CVMETHOD("APPROX_CHAIN_METHOD", rb_hash_aref(op, ID2SYM(rb_intern("method"))), CV_CHAIN_APPROX_SIMPLE)
#define APPROX_CHAIN_PARAMETER(op) NUM2INT(rb_hash_aref(op, ID2SYM(rb_intern("parameter"))))
#define APPROX_CHAIN_MINIMAL_PARAMETER(op) NUM2INT(rb_hash_aref(op, ID2SYM(rb_intern("minimal_parameter"))))
#define APPROX_CHAIN_RECURSIVE(op) RTEST(rb_hash_aref(op, ID2SYM(rb_intern("recursive")))) ? 0 : 1
//#define APPROX_CHAIN_RECURSIVE(op) ({VALUE _recursive = rb_hash_aref(op, ID2SYM(rb_intern("recursive"))); NIL_P(_recursive) ? 0 : _recursive == Qfalse ? 0 : 1;})

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
   * note: this comment is used by rdoc.
   */
  VALUE opencv = rb_module_opencv();
  VALUE cvseq = cCvSeq::rb_class();
  VALUE curve = mCurve::rb_module();

  rb_klass = rb_define_class_under(opencv, "CvChain", cvseq);
  rb_include_module(rb_klass, curve);
  VALUE approx_chain_option = rb_hash_new();
  rb_define_const(rb_klass, "APPROX_CHAIN_OPTION", approx_chain_option); 
  rb_hash_aset(approx_chain_option, ID2SYM(rb_intern("method")), ID2SYM(rb_intern("approx_simple")));
  rb_hash_aset(approx_chain_option, ID2SYM(rb_intern("parameter")), rb_float_new(0));
  rb_hash_aset(approx_chain_option, ID2SYM(rb_intern("minimal_parameter")), INT2FIX(0));
  rb_hash_aset(approx_chain_option, ID2SYM(rb_intern("recursive")), Qfalse);

  rb_define_method(rb_klass, "origin", RUBY_METHOD_FUNC(rb_origin), 0);
  rb_define_method(rb_klass, "origin=", RUBY_METHOD_FUNC(rb_set_origin), 0);
  rb_define_method(rb_klass, "codes", RUBY_METHOD_FUNC(rb_codes), 0);
  rb_define_method(rb_klass, "points", RUBY_METHOD_FUNC(rb_points), 0);
  rb_define_method(rb_klass, "approx_chain", RUBY_METHOD_FUNC(rb_approx_chain), -1);
  rb_define_alias(rb_klass, "approx", "approx_chain");
}

/*
 * call-seq:
 *   origin -> cvpoint
 *
 * Return Freeman chain code origin.
 */
VALUE
rb_origin(VALUE self)
{
  return cCvPoint::new_object(CVCHAIN(self)->origin);
}

/*
 * call-seq:
 *   origin = point -> self
 *
 * Set Freeman chain code origin.
 */
VALUE
rb_set_origin(VALUE self, VALUE origin)
{
  CVCHAIN(self)->origin = VALUE_TO_CVPOINT(origin);  
  return self;
}

/*
 * call-seq:
 *   codes -> array(contain fixnum)
 *
 * Return Freeman chain codes.
 */
VALUE
rb_codes(VALUE self)
{
  CvChain *chain = CVCHAIN(self);
  CvChainPtReader reader;
  CvPoint p = chain->origin;
  VALUE ary = rb_ary_new2(chain->total);
  cvStartReadChainPoints(chain, &reader);
  for (int i = 0; i < chain->total; i++) {
    CV_READ_SEQ_ELEM(reader.code, (*((CvSeqReader*)&(reader))));
    rb_ary_store(ary, i, CHR2FIX(reader.code));
  }
  return ary;
}

/*
 * call-seq:
 *   points -> array(contain cvpoint)
 *
 * Return points that represent by Freeman chain code.
 */
VALUE
rb_points(VALUE self)
{
  CvChain *chain = CVCHAIN(self);
  CvChainPtReader reader;
  CvPoint p = chain->origin;
  VALUE ary = rb_ary_new2(chain->total);
  cvStartReadChainPoints(chain, &reader);
  for (int i = 0; i < chain->total; i++) {
    CV_READ_CHAIN_POINT(p, reader);
    rb_ary_store(ary, i, cCvPoint::new_object(p));
  }
  return ary;
}

/*
 * call-seq:
 *   approx_chain(<i>[approx_chain_option]</i>) -> cvcontour
 *
 * Approximates Freeman chain(s) with polygonal curve.
 * <i>approx_chain_option</i> should be Hash include these keys.
 *   :method - Approximation method.
 *      :approx_none - translate all the points from the chain code into points;
 *      :approx_simple(default) - compress horizontal, vertical, and diagonal segments, that is,
 *                                the function leaves only their ending points.
 *      :approx_tc89_l1
 *      :approx_tc89_kcos - apply one of the flavors of Teh-Chin chain approximation algorithm.
 *      If set the difference between the current pixel and seed pixel is considered,
 *      otherwise difference between neighbor pixels is considered (the range is floating).
 *   :parameter - Method parameter (not used now).
 *   :minimal_perimeter (default 0)
 *      Approximates only those contours whose perimeters are not less than minimal_perimeter. Other chains are removed from the resulting structure.
 *   :recursive (default false)
 *      If not nil or false, the function approximates all chains that access can be obtained to
 *      from self by h_next or v_next links. If 0, the single chain is approximated.
 *
 */
VALUE
rb_approx_chain(int argc, VALUE *argv, VALUE self)
{
  VALUE approx_chain_option, storage;
  rb_scan_args(argc, argv, "01", &approx_chain_option);
  approx_chain_option = APPROX_CHAIN_OPTION(approx_chain_option);
  storage = cCvMemStorage::new_object();
  CvSeq *seq = cvApproxChains(CVSEQ(self), CVMEMSTORAGE(storage),			      
			      APPROX_CHAIN_METHOD(approx_chain_option),
			      APPROX_CHAIN_PARAMETER(approx_chain_option),			    
			      APPROX_CHAIN_MINIMAL_PARAMETER(approx_chain_option),			      
			      APPROX_CHAIN_RECURSIVE(approx_chain_option));  
  return cCvSeq::new_sequence(cCvContour::rb_class(), seq, cCvPoint::rb_class(), storage);
}

VALUE
new_object()
{
  VALUE storage = cCvMemStorage::new_object();
  CvSeq *seq = cvCreateSeq(CV_SEQ_CHAIN_CONTOUR, sizeof(CvChain), sizeof(CvChainCode), CVMEMSTORAGE(storage));
  VALUE object = cCvSeq::new_sequence(cCvChain::rb_class(), seq, cCvChainCode::rb_class(), storage);
  return object;
}

__NAMESPACE_END_CVCHAIN
__NAMESPACE_END_OPENCV
