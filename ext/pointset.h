/************************************************************

   pointset.h -

   $Author: lsxi $

   Copyright (C) 2005-2006 Masakazu Yonekura

************************************************************/
#ifndef RUBY_OPENCV_POINTSET_H
#define RUBY_OPENCV_POINTSET_H

#define __NAMESPACE_BEGIN_POINT_SET namespace mPointSet{
#define __NAMESPACE_END_POINT_SET }

#include"opencv.h"

__NAMESPACE_BEGIN_OPENCV
__NAMESPACE_BEGIN_POINT_SET

VALUE rb_module();

void define_ruby_module();
VALUE rb_contour_area(int argc, VALUE *argv, VALUE self);
VALUE rb_fit_ellipse(VALUE self);
VALUE rb_fit_line(int argc, VALUE *argv, VALUE self);
VALUE rb_convex_hull(int argc, VALUE *argv, VALUE self);
VALUE rb_check_contour_convexity(VALUE self);
VALUE rb_convexity_defects(int argc, VALUE *argv, VALUE self);
VALUE rb_min_area_rect(VALUE self);
VALUE rb_min_enclosing_circle(VALUE self);
VALUE rb_calc_pgh(VALUE self);

__NAMESPACE_END_POINT_SET

#define POINT_SET_P(object) rb_obj_is_kind_of(object, cCvSeq::rb_class()) && CV_IS_SEQ_POINT_SET(CVSEQ(object))

/*
inline CvPoint*
POINTSET(VALUE object)
{
  CvPoint *pointset = (CvPoint*)cvAlloc(CVSEQ(object)->total * sizeof(CvPoint));  
  cvCvtSeqToArray(CVSEQ(object), pointset, CV_WHOLE_SEQ);
  if (cCvSeq::seqblock_class(CVSEQ(object)) == cCvPoint2D32f::rb_class()) {
    for(int i =0; i < CVSEQ(object)->total; i++)
      pointset[i] = cvPointFrom32f(((CvPoint2D32f*)pointset)[i]);
  }
  return pointset;
}

inline CvPoint2D32f*
POINTSET2D32f(VALUE object)
{
  CvPoint2D32f *pointset = (CvPoint2D32f*)cvAlloc(CVSEQ(object)->total * sizeof(CvPoint2D32f));
  cvCvtSeqToArray(CVSEQ(object), pointset, CV_WHOLE_SEQ);
  if (cCvSeq::seqblock_class(CVSEQ(object)) == cCvPoint::rb_class()) {
    for(int i = 0; i < CVSEQ(object)->total; i++)
      pointset[i] = cvPointTo32f(((CvPoint*)pointset)[i]);
  }
  return pointset;
}
*/

int CVPOINTS_FROM_POINT_SET(VALUE object, CvPoint **pointset);
CvSeq* VALUE_TO_POINT_SET(VALUE object);

__NAMESPACE_END_OPENCV

#endif // RUBY_OPENCV_POINTSET_H
