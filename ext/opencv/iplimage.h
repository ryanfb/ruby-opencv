/************************************************************

   iplimage.h -

   $Author: lsxi $

   Copyright (C) 2005-2006 Masakazu Yonekura

************************************************************/
#ifndef RUBY_OPENCV_IPLIMAGE_H
#define RUBY_OPENCV_IPLIMAGE_H

#include "opencv.h"

#define __NAMESPACE_BEGIN_IPLIMAGE namespace cIplImage{
#define __NAMESPACE_END_IPLIMAGE }

__NAMESPACE_BEGIN_OPENCV
__NAMESPACE_BEGIN_IPLIMAGE

VALUE rb_class();

void define_ruby_class();    

VALUE rb_allocate(VALUE klass);
VALUE rb_initialize(int argc, VALUE *argv, VALUE self);

VALUE rb_load_image(int argc, VALUE *argv, VALUE self);

VALUE rb_color_model(VALUE self);

VALUE rb_get_roi(VALUE self);
VALUE rb_set_roi(VALUE self, VALUE roi);
VALUE rb_reset_roi(VALUE self);

VALUE rb_get_coi(VALUE self);
VALUE rb_set_coi(VALUE self, VALUE coi);
VALUE rb_reset_coi(VALUE self);

VALUE rb_smoothness(int argc, VALUE *argv, VALUE self);
typedef enum { SMOOTH = 1, BLANK = 2, MESSY = 3 } Smoothness;
Smoothness compute_smoothness(const IplImage *pFourierImage, const double lowFreqRatio, const double blankDensity, const double messyDensity, const double highFreqRatio, double &outLowDensity, double &outHighDensity);
void cvShiftDFT(CvArr *src_arr, CvArr *dst_arr );
IplImage* create_fourier_image(const IplImage *im);
IplImage* create_frequency_filtered_image(const IplImage *pImage, int low, int high);
void high_pass_range(const IplImage *pImage, float lostPercentage, int &outLow, int &outHigh);

VALUE new_object(int width, int height, int type);
VALUE new_object(CvSize size, int type);
__NAMESPACE_END_IPLIMAGE

inline IplImage*
IPLIMAGE(VALUE object)
{
  IplImage *ptr, stub;
  Data_Get_Struct(object, IplImage, ptr);
  return cvGetImage(ptr, &stub);
}

__NAMESPACE_END_OPENCV

#endif // RUBY_OPENCV_IPLIMAGE_H
