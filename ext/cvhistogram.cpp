/************************************************************

   cvhistogram.cpp -

   $Author: lsxi $

   Copyright (C) 2005-2008 Masakazu Yonekura

************************************************************/
#include "cvhistogram.h"
/*
 * Document-class: OpenCV::CvHistogram
 *
 * Muti-dimensional histogram.
 */
__NAMESPACE_BEGIN_OPENCV
__NAMESPACE_BEGIN_CVHISTOGRAM

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
  
  rb_klass = rb_define_class_under(opencv, "CvHistogram", rb_cObject);
  
  rb_define_method(rb_klass, "is_uniform?", RUBY_METHOD_FUNC(rb_is_uniform), 0);
  rb_define_method(rb_klass, "is_sparse?", RUBY_METHOD_FUNC(rb_is_sparse), 0);
  rb_define_method(rb_klass, "has_range?", RUBY_METHOD_FUNC(rb_has_range), 0);
  rb_define_method(rb_klass, "dims", RUBY_METHOD_FUNC(rb_dims), 0);

  rb_define_method(rb_klass, "normalize", RUBY_METHOD_FUNC(rb_normalize), 1);
  rb_define_method(rb_klass, "normalize!", RUBY_METHOD_FUNC(rb_normalize_bang), 1);
  rb_define_method(rb_klass, "thresh", RUBY_METHOD_FUNC(rb_thresh), 1);
  rb_define_alias(rb_klass, "threshold", "thresh");
  rb_define_method(rb_klass, "thresh!", RUBY_METHOD_FUNC(rb_thresh_bang), 1);
  rb_define_alias(rb_klass, "threshold!", "thresh!");
}

VALUE
rb_allocate(VALUE klass)
{
  // not yet
  return Qnil;
}

/*
 * call-seq:
 *   is_uniform? -> true or false
 *
 */
VALUE
rb_is_uniform(VALUE self)
{
  return CV_IS_UNIFORM_HIST(CVHISTOGRAM(self)) ? Qtrue : Qfalse;
}

/*
 * call-seq:
 *   is_sparse? -> true or false
 *
 */
VALUE
rb_is_sparse(VALUE self)
{
  return CV_IS_SPARSE_HIST(CVHISTOGRAM(self)) ? Qtrue : Qfalse;
}

/*
 * call-seq:
 *   has_range? -> true or false
 */
VALUE
rb_has_range(VALUE self)
{
  return CV_HIST_HAS_RANGES(CVHISTOGRAM(self)) ? Qtrue : Qfalse;
}

/*
 * call-seq:
 *   dims -> [int[,int...]]
 */
VALUE
rb_dims(VALUE self)
{
  int size[CV_MAX_DIM];
  int dims = cvGetDims(CVHISTOGRAM(self)->bins, size);
  VALUE result = rb_ary_new2(dims);
  for(int i = 0; i < dims; i++){
    rb_ary_store(result, i, INT2FIX(size[i]));
  }
  return result;
}

/*
 * call-seq:
 *   bins -> cvmatnd or cvsparsemat
 */
VALUE
rb_bins(VALUE self)
{
  CvHistogram *hist = CVHISTOGRAM(self);
  return REFER_OBJECT(CV_IS_SPARSE_HIST(hist) ? cCvSparseMat::rb_class() : cCvMatND::rb_class(), hist->bins, self);
}

/*
 * call-seq:
 *   copy -> cvhist
 *
 * Clone histogram.
 */
VALUE
rb_copy(VALUE self)
{
  VALUE dest = 0;
  CvHistogram *hist = CVHISTOGRAM(dest);
  cvCopyHist(CVHISTOGRAM(self), &hist);
  return dest;
}

/*
 * call-seq:
 *   clear!
 *
 * Sets all histogram bins to 0 in case of dense histogram and removes all histogram bins in case of sparse array.
 */
VALUE
rb_clear_bang(VALUE self)
{
  cvClearHist(CVHISTOGRAM(self));
  return self;
}

/*
 * call-seq:
 *   normalize(<i>factor</i>) -> cvhist
 *
 * Return normalized the histogram bins by scaling them, such that the sum of the bins becomes equal to <i>factor</i>.
 */
VALUE
rb_normalize(VALUE self, VALUE factor)
{
  return rb_normalize_bang(rb_copy(self), factor);
}

/*
 * call-seq:
 *   normalize!(<i>factor</i>) -> self
 *
 * normalizes the histogram bins by scaling them, such that the sum of the bins becomes equal to <i>factor</i>.
 */
VALUE
rb_normalize_bang(VALUE self, VALUE factor)
{
  cvNormalizeHist(CVHISTOGRAM(self), NUM2DBL(factor));
  return self;
}

/*
 * call-seq:
 *   thresh(<i>factor</i>) -> cvhist
 *
 * Return cleared histogram bins that are below the specified threshold.
 */
VALUE
rb_thresh(VALUE self, VALUE factor)
{
  return rb_thresh_bang(rb_copy(self), factor);
}

/*
 * call-seq:
 *   thresh!(<i>factor</i>) -> self
 *
 * Cleares histogram bins that are below the specified threshold.
 */
VALUE
rb_thresh_bang(VALUE self, VALUE factor)
{
  cvThreshHist(CVHISTOGRAM(self), NUM2DBL(factor));
  return self;
}


__NAMESPACE_END_CVHISTOGRAM
__NAMESPACE_END_OPENCV
