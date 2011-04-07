/************************************************************

   pointset.cpp -

   $Author: lsxi $

   Copyright (C) 2005-2006 Masakazu Yonekura

************************************************************/
#include"pointset.h"
/*
 * Document-class: OpenCV::PointSet
 */

__NAMESPACE_BEGIN_OPENCV
__NAMESPACE_BEGIN_POINT_SET

VALUE module;

VALUE
rb_module()
{
  return module;
}

void
define_ruby_module()
{
  if(module)
    return;
  /* 
   * opencv = rb_define_module("OpenCV");
   *
   * note: this comment is used by rdoc.
   */
  VALUE opencv = rb_module_opencv();
  module = rb_define_module_under(opencv, "PointSet");
  rb_define_method(module, "contour_area", RUBY_METHOD_FUNC(rb_contour_area), -1);
  rb_define_method(module, "fit_ellipse", RUBY_METHOD_FUNC(rb_fit_ellipse), 0);
  
  rb_define_method(module, "convex_hull", RUBY_METHOD_FUNC(rb_convex_hull), -1);
  rb_define_method(module, "check_contour_convexity", RUBY_METHOD_FUNC(rb_check_contour_convexity), 0);
  rb_define_alias(module, "convexity?", "check_contour_convexity");
  rb_define_method(module, "convexity_defects", RUBY_METHOD_FUNC(rb_convexity_defects), -1);
  rb_define_method(module, "min_area_rect", RUBY_METHOD_FUNC(rb_min_area_rect), 0);
  rb_define_method(module, "min_enclosing_circle", RUBY_METHOD_FUNC(rb_min_enclosing_circle), 0);
}

VALUE
rb_extend_object(VALUE self, VALUE object)
{
  CvSeq *seq = 0;
  if(!rb_obj_is_kind_of(object, cCvSeq::rb_class()))
    rb_raise(rb_eTypeError, "object is not %s.\n", rb_class2name(cCvSeq::rb_class()));
  
  if(!CV_IS_SEQ(seq))
    rb_raise(rb_eTypeError, "object is not sequence.");  
  return rb_call_super(1, &object);
}

/*
 * call-seq:
 *   contour_area -> float
 *
 * Calculates area of the whole contour or contour section.
 * 
 * note: Orientation of the contour affects the area sign, thus the method may return negative result.
 */
VALUE
rb_contour_area(int argc, VALUE *argv, VALUE self)
{
  VALUE slice;
  rb_scan_args(argc, argv, "01", &slice);
  return rb_float_new(cvContourArea(CVARR(self), NIL_P(slice) ? CV_WHOLE_SEQ : VALUE_TO_CVSLICE(slice)));
}

/*
 * call-seq:
 *   fit_ellipse -> cvbox2d
 *
 * Return fits ellipse to set of 2D points.
 */
VALUE
rb_fit_ellipse(VALUE self)
{
  return cCvBox2D::new_object(cvFitEllipse2(CVARR(self)));
}

/*
 * call-seq:
 *   convex_hull(<i>[reverse = fasle]</i>) -> cvcontour
 *
 * Finds convex hull of 2D point set using Sklansky's algorithm.
 * 
 * <i>reverse</i> is desired orientation of convex hull.
 * If reverse is false mean clockwise, otherwise counter clockwise.
 */
VALUE
rb_convex_hull(int argc, VALUE *argv, VALUE self)
{
  VALUE reverse, storage;
  rb_scan_args(argc, argv, "01", &reverse);
  storage = cCvMemStorage::new_object();
  CvSeq *hull = cvConvexHull2(CVSEQ(self), CVMEMSTORAGE(storage), TRUE_OR_FALSE(reverse, 0) ? CV_COUNTER_CLOCKWISE : CV_CLOCKWISE, 1);
  if(CV_IS_SEQ_HOLE(CVSEQ(self)))
    hull->flags |= CV_SEQ_FLAG_HOLE;
  return cCvSeq::new_sequence(cCvContour::rb_class(), hull, cCvPoint::rb_class(), storage);
}

/*
 * call-seq:
 *   check_contour_convexity -> true or false
 *
 * Tests whether the input contour is convex or not. The contour must be simple, i.e. without self-intersections.
 */
VALUE
rb_check_contour_convexity(VALUE self)
{
  return cvCheckContourConvexity(CVARR(self)) ? Qtrue : Qfalse;
}

/*
 * call-seq:
 *   convexity_defects(<i>[reverse = false]</i>) -> cvseq(include CvConvexityDefect)
 *
 * Finds convexity defects of contour.
 */
VALUE
rb_convexity_defects(int argc, VALUE *argv, VALUE self)
{
  VALUE reverse, storage;
  rb_scan_args(argc, argv, "01", &reverse);
  storage = cCvMemStorage::new_object();
  CvSeq *hull, *convex;
  hull = cvConvexHull2(CVSEQ(self), CVMEMSTORAGE(storage), TRUE_OR_FALSE(reverse, 0) ? CV_COUNTER_CLOCKWISE : CV_CLOCKWISE, 0);
  convex = cvConvexityDefects(CVSEQ(self), hull, CVMEMSTORAGE(storage));
  return cCvSeq::new_sequence(cCvSeq::rb_class(), convex, cCvConvexityDefect::rb_class(), storage);
}

/*
 * call-seq:
 *   min_area_rect -> cvbox2d
 *
 * Finds circumscribed rectangle of minimal area for given 2D point set.
 */
VALUE
rb_min_area_rect(VALUE self)
{
  VALUE storage = cCvMemStorage::new_object();
  return cCvBox2D::new_object(cvMinAreaRect2(CVARR(self), CVMEMSTORAGE(storage)));
}

/*
 * call-seq:
 *   min_enclosing_circle -> cvcircle32f
 *
 * Finds circumscribed circle of minimal area for given 2D point set.
 */
VALUE
rb_min_enclosing_circle(VALUE self)
{
  VALUE circle = cCvCircle32f::rb_allocate(cCvCircle32f::rb_class());
  cvMinEnclosingCircle(CVARR(self), &CVCIRCLE32F(circle)->center, &CVCIRCLE32F(circle)->radius);
  return circle;
}

VALUE
rb_calc_pgh(VALUE self)
{
  /* not yet */
  return Qnil;
}

__NAMESPACE_END_POINT_SET

int
CVPOINTS_FROM_POINT_SET(VALUE object, CvPoint **pointset)
{
  VALUE storage;
  CvSeq *seq = 0;
  CvPoint2D32f p32;
  if(rb_obj_is_kind_of(object, cCvSeq::rb_class())){
    if(CV_IS_SEQ_POINT_SET(CVSEQ(object))){ 
      *pointset = (CvPoint*)cvCvtSeqToArray(CVSEQ(object), cvAlloc(CVSEQ(object)->total * CVSEQ(object)->elem_size));
      return CVSEQ(object)->total;
    }else{
      rb_raise(rb_eTypeError, "sequence is not contain %s or %s.", rb_class2name(cCvPoint::rb_class()), rb_class2name(cCvPoint2D32f::rb_class()));
    }
  }else if(rb_obj_is_kind_of(object, cCvMat::rb_class())){
    /* to do */
    rb_raise(rb_eNotImpError, "CvMat to CvSeq conversion not implemented.");
  }else if(rb_obj_is_kind_of(object, rb_cArray)){
    *pointset = (CvPoint*)cvAlloc(RARRAY_LEN(object) * sizeof(CvPoint));
    for(int i = 0; i < RARRAY_LEN(object); i++){
      (*pointset)[i].x = NUM2INT(rb_funcall(rb_ary_entry(object, i), rb_intern("x"), 0));
      (*pointset)[i].y = NUM2INT(rb_funcall(rb_ary_entry(object, i), rb_intern("y"), 0));
    }
    return RARRAY_LEN(object);
  }else{
    rb_raise(rb_eTypeError, "Can't convert CvSeq(PointSet).");
  }  
}

CvSeq*
VALUE_TO_POINT_SET(VALUE object)
{
  CvSeq *seq = 0;
  VALUE tmp, storage;
  int length;
  CvPoint2D32f p32;
  if(rb_obj_is_kind_of(object, cCvSeq::rb_class())){
    seq = CVSEQ(object);
    if(CV_IS_SEQ_POINT_SET(seq)){
      return seq;
    }else{
      rb_raise(rb_eTypeError, "sequence is not contain %s or %s.", rb_class2name(cCvPoint::rb_class()), rb_class2name(cCvPoint2D32f::rb_class()));
    }
  }else if(rb_obj_is_kind_of(object, cCvMat::rb_class())){
    /* to do */
    rb_raise(rb_eNotImpError, "CvMat to CvSeq conversion not implemented.");
  }else if(rb_obj_is_kind_of(object, rb_cArray)){
    //pointset = cCvSeq::new_sequence(cCvSeq::rb_class(), )
    length = RARRAY_LEN(object);
    storage = cCvMemStorage::new_object();
    seq = cvCreateSeq(CV_SEQ_POINT_SET, sizeof(CvSeq), sizeof(CvPoint), CVMEMSTORAGE(storage));    
    for(int i = 0; i < RARRAY_LEN(object); i++){
      p32.x = NUM2DBL(rb_funcall(rb_ary_entry(object, i), rb_intern("x"), 0));
      p32.y = NUM2DBL(rb_funcall(rb_ary_entry(object, i), rb_intern("y"), 0));
      cvSeqPush(seq, &p32);
    }
    tmp = cCvSeq::new_sequence(cCvSeq::rb_class(), seq, cCvPoint2D32f::rb_class(), storage);
    return seq;
  }else{
    rb_raise(rb_eTypeError, "Can't convert CvSeq(PointSet).");
  }  
}

__NAMESPACE_END_OPENCV
