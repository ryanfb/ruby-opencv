/************************************************************

   cvcontourtree.cpp -

   $Author: lsxi $

   Copyright (C) 2007 Masakazu Yonekura

************************************************************/
#include "cvcontour.h"
/*
 * Document-class: OpenCV::CvContourTree
 *
 * Contour tree. CvContour#create_tree
 *
 * C structure is here.
 *  typedef struct CvContourTree{
 *    CV_SEQUENCE_FIELDS()
 *    CvPoint p1;
 *    CvPoint p2;
 *  }CvContourTree;
 * 
 */
__NAMESPACE_BEGIN_OPENCV
__NAMESPACE_BEGIN_CVCONTOURTREE

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
   *
   * note: this comment is used by rdoc.
   */
  VALUE opencv = rb_module_opencv();
  VALUE cvseq = cCvSeq::rb_class();
  
  rb_klass = rb_define_class_under(opencv, "CvContourTree", cvseq);
  rb_define_method(rb_klass, "p1", RUBY_METHOD_FUNC(rb_p1), 0);
  rb_define_method(rb_klass, "p2", RUBY_METHOD_FUNC(rb_p2), 0);
  rb_define_method(rb_klass, "contour", RUBY_METHOD_FUNC(rb_contour), 0);

}

VALUE
rb_p1(VALUE self)
{
  return REFER_OBJECT(cCvPoint::rb_class(), &CVCONTOURTREE(self)->p1, self);
}

VALUE
rb_p2(VALUE self)
{
  return REFER_OBJECT(cCvPoint::rb_class(), &CVCONTOURTREE(self)->p2, self);
}

/*
 * call-seq:
 *   contour(<i>[criteria = 0]</i>) -> cvcontour
 *
 * Restores the contour from its binary tree representation.
 * The parameter criteria determines the accuracy and/or the number of tree levels
 * used for reconstruction, so it is possible to build approximated contour.
 */
VALUE
rb_contour(int argc, VALUE *argv, VALUE self)
{
  VALUE criteria, storage;
  rb_scan_args(argc, argv, "01", &criteria);
  CvSeq *contour = cvContourFromContourTree(CVCONTOURTREE(self), CVMEMSTORAGE(storage), VALUE_TO_CVTERMCRITERIA(criteria));
  return cCvSeq::new_sequence(cCvContour::rb_class(), contour, cCvPoint::rb_class(), storage);
}

__NAMESPACE_END_CVCONTOURTREE
__NAMESPACE_END_OPENCV
