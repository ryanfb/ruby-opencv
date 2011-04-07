/************************************************************

   cvcondensation.cpp -

   $Author: lsxi $

   Copyright (C) 2005-2006 Masakazu Yonekura

************************************************************/
#include "cvcondensation.h"
/*
 * Document-class: OpenCV::CvConDensation
 *
 */
__NAMESPACE_BEGIN_OPENCV
__NAMESPACE_BEGIN_CVCONDENSATION

VALUE rb_klass;

VALUE
rb_class()
{
  return rb_klass;
}

void
define_ruby_class()
{
  if(rb_klass)
    return;
  /* 
   * opencv = rb_define_module("OpenCV");
   * 
   * note: this comment is used by rdoc.
   */
  VALUE opencv = rb_module_opencv();
  rb_klass = rb_define_class_under(opencv, "CvConDensation", rb_cObject);
  rb_define_method(rb_klass, "dp", RUBY_METHOD_FUNC(rb_dp), 0);
  rb_define_method(rb_klass, "mp", RUBY_METHOD_FUNC(rb_mp), 0);
  rb_define_method(rb_klass, "dynamic_matrix", RUBY_METHOD_FUNC(rb_dynamic_matrix), 0);
  rb_define_method(rb_klass, "confidence", RUBY_METHOD_FUNC(rb_confidence), 0);
  rb_define_method(rb_klass, "cumulative", RUBY_METHOD_FUNC(rb_cumulative), 0);
  rb_define_method(rb_klass, "state", RUBY_METHOD_FUNC(rb_state), 0);
  rb_define_method(rb_klass, "samples_num", RUBY_METHOD_FUNC(rb_samples_num), 0);
  rb_define_method(rb_klass, "init_sample_set", RUBY_METHOD_FUNC(rb_init_sample_set), 2);
  rb_define_method(rb_klass, "update_by_time", RUBY_METHOD_FUNC(rb_update_by_time), 0);
  rb_define_alias(rb_klass, "update", "update_by_time");
  rb_define_method(rb_klass, "each_sample", RUBY_METHOD_FUNC(rb_each_sample), 0);
  rb_define_method(rb_klass, "calculate_confidence", RUBY_METHOD_FUNC(rb_calculate_confidence), 0);
}

/*
 * call-seq:
 *   dp -> int
 *
 * Return dimension of state vector
 */
VALUE
rb_dp(VALUE self)
{
  return INT2FIX(CVCONDENSATION(self)->DP);
}

/*
 * call-seq:
 *   mp -> int
 *
 * Return demension of measurement vector.
 */
VALUE
rb_mp(VALUE self)
{
  return INT2FIX(CVCONDENSATION(self)->MP);
}

/*
 * call-seq:
 *   dynamic_matrix -> mat
 *
 * Return matrix of the linear Dynamics system.
 */
VALUE
rb_dynamic_matrix(VALUE self)
{
  CvConDensation *cd = CVCONDENSATION(self);
  return DEPEND_OBJECT(cCvMat::rb_class(), cvInitMatHeader(ALLOC(CvMat), cd->DP, cd->DP, CV_MAKETYPE(CV_32F, 1), cd->DynamMatr), self);
}

/*
 * call-seq:
 *   confidence -> mat
 *
 * Return confidence for each sample.
 */
VALUE
rb_confidence(VALUE self)
{
  CvConDensation *cd = CVCONDENSATION(self);
  return DEPEND_OBJECT(cCvMat::rb_class(), cvInitMatHeader(ALLOC(CvMat), cd->SamplesNum, 1, CV_MAKETYPE(CV_32F, 1), cd->flConfidence), self);
}

/*
 * call-seq:
 *   cumulative -> mat
 *
 * Return cumulative confidence.
 */
VALUE
rb_cumulative(VALUE self)
{
  CvConDensation *cd = CVCONDENSATION(self);
  return DEPEND_OBJECT(cCvMat::rb_class(), cvInitMatHeader(ALLOC(CvMat), cd->SamplesNum, 1, CV_MAKETYPE(CV_32F, 1), cd->flCumulative), self);
}

/*
 * call-seq:
 *   state -> mat
 *
 * Return vector of state
 */
VALUE
rb_state(VALUE self)
{
  CvConDensation *cd = CVCONDENSATION(self);
  return DEPEND_OBJECT(cCvMat::rb_class(), cvInitMatHeader(ALLOC(CvMat), cd->DP, 1, CV_MAKETYPE(CV_32F, 1), cd->State), self);  
}

/*
 * call-seq:
 *   samples_num -> int
 *
 * Return number of the samples
 */
VALUE
rb_samples_num(VALUE self)
{
  return INT2FIX(CVCONDENSATION(self)->SamplesNum);
}

/*
 * call-seq:
 *   init_sample_set(upper, lower)
 *
 * Initializes sample set for ConDensation algorithm.
 * Fills the samples with values within specified(lower to upper) ranges.
 */
VALUE
rb_init_sample_set(VALUE self, VALUE lower, VALUE upper)
{
  CvConDensation *cd = CVCONDENSATION(self);
  CvMat *lower_bound = CVMAT(lower), *upper_bound = CVMAT(upper), lb_stub, ub_stub;
  int lower_type = lower_bound->type, upper_type = lower_bound->type;
  if (lower_type != CV_32FC1 || lower_bound->cols != 1) {
    if (CV_MAT_DEPTH(lower_type) == CV_32F) {
      lower_bound = cvReshape(lower_bound, &lb_stub, 1, lower_bound->rows * lower_bound->cols);
    } else {
      lower = cCvMat::new_object(cvSize(lower_bound->rows * lower_bound->cols, 1), CV_MAKETYPE(CV_32S, 1));
      cvConvertScale(lower_bound, CVARR(lower));
      lower_bound = CVMAT(lower);      
    }
  }
  if (upper_type != CV_32FC1 || upper_bound->cols != 1) {
    if (CV_MAT_DEPTH(upper_type) == CV_32F) {
      upper_bound = cvReshape(upper_bound, &ub_stub, 1, upper_bound->rows * upper_bound->cols);
    } else {
      upper = cCvMat::new_object(cvSize(upper_bound->rows * upper_bound->cols, 1), CV_MAKETYPE(CV_32F, 1));
      cvConvertScale(upper_bound, CVARR(upper));
      upper_bound = CVMAT(upper);      
    }
  }
  if (lower_bound->rows != cd->DP || upper_bound->rows != cd->DP) {
    rb_raise(rb_eTypeError, "sample matrix step unmatch.");
  }
  cvConDensInitSampleSet(cd, lower_bound, upper_bound);
  return self;
}

/*
 * call-seq:
 *   update_by_time
 *
 * Estimates subsequent model state.
 */
VALUE
rb_update_by_time(VALUE self)
{
  cvConDensUpdateByTime(CVCONDENSATION(self));
  return self;
}

/*
 * call-seq:
 *   each_sample{|mat| ... }
 *
 * Evaluate each sample by given block.
 */
VALUE
rb_each_sample(VALUE self)
{
  CvConDensation *cd = CVCONDENSATION(self);
  if (rb_block_given_p()) {
    for (int i = 0; i < cd->SamplesNum; i++) {
      rb_yield(DEPEND_OBJECT(cCvMat::rb_class(), cvInitMatHeader(ALLOC(CvMat), cd->DP, 1, CV_MAKETYPE(CV_32F, 1), cd->flSamples[i]), self));
      }
  }
  return self;
}

/*
 * call-seq:
 *   calculate_confidence{|value| ... }
 *
 * Evalute each sample by given block, then return value set to confidence.
 */
VALUE
rb_calculate_confidence(VALUE self)
{
  VALUE value;
  CvConDensation *cd = CVCONDENSATION(self);
  if (rb_block_given_p()) {
    for (int i = 0; i < cd->SamplesNum; i++) {
      value = rb_yield(DEPEND_OBJECT(cCvMat::rb_class(), cvInitMatHeader(ALLOC(CvMat), cd->DP, 1, CV_MAKETYPE(CV_32F, 1), cd->flSamples[i]), self));
      cd->flConfidence[i] = NUM2DBL(value);
    }
  }
  return self;
}

__NAMESPACE_END_CVCONDENSATION
__NAMESPACE_END_OPENCV
