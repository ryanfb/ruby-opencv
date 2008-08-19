/************************************************************

   cvcondensation.h -

   $Author: lsxi $

   Copyright (C) 2005-2006 Masakazu Yonekura

************************************************************/
#ifndef RUBY_OPENCV_CVCONDENSATION_H
#define RUBY_OPENCV_CVCONDENSATION_H

#include "opencv.h"

#define __NAMESPACE_BEGIN_CVCONDENSATION namespace cCvConDensation{
#define __NAMESPACE_END_CVCONDENSATION }

__NAMESPACE_BEGIN_OPENCV
__NAMESPACE_BEGIN_CVCONDENSATION

VALUE rb_class();
void define_ruby_class();

VALUE rb_dp(VALUE self);
VALUE rb_mp(VALUE self);
VALUE rb_dynamic_matrix(VALUE self);
VALUE rb_state(VALUE self);
VALUE rb_confidence(VALUE self);
VALUE rb_cumulative(VALUE self);
VALUE rb_samples_num(VALUE self);

VALUE rb_init_sample_set(VALUE self, VALUE lower, VALUE upper);
VALUE rb_update_by_time(VALUE self);
VALUE rb_each_sample(VALUE self);
VALUE rb_calculate_confidence(VALUE self);

__NAMESPACE_END_CVCONDENSATION

inline CvConDensation*
CVCONDENSATION(VALUE object)
{
  CvConDensation *ptr;
  Data_Get_Struct(object, CvConDensation, ptr);
  return ptr;
}

__NAMESPACE_END_OPENCV

#endif // RUBY_OPENCV_CVCONDENSATION_H
