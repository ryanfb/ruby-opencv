/************************************************************

   iplimage.cpp -

   $Author: lsxi $

   Copyright (C) 2005-2006 Masakazu Yonekura

************************************************************/
#include "iplimage.h"
/*
 * Document-class: OpenCV::IplImage
 *
 * IPL(Intel Image Processing Library) Image class. 
 *
 * IplImage is subclass of CvMat. IplImage support ROI(region of interest) and COI(color of interest).
 * Most of CvMat method support ROI, and some of CvMat method support COI.
 *
 * =What is ROI?
 * region of interest.
 *
 * =What is COI?
 * color of interest.
 */
__NAMESPACE_BEGIN_OPENCV
__NAMESPACE_BEGIN_IPLIMAGE

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
   * cvmat = rb_define_class_under(opencv, "CvMat", rb_cObject);
   *
   * note: this comment is used by rdoc.
   */
  VALUE opencv = rb_module_opencv();
  VALUE cvmat = cCvMat::rb_class();
  rb_klass = rb_define_class_under(opencv, "IplImage", cvmat);
  rb_define_alloc_func(rb_klass, rb_allocate);
  rb_define_singleton_method(rb_klass, "load", RUBY_METHOD_FUNC(rb_load_image), -1);
  rb_define_private_method(rb_klass, "initialize", RUBY_METHOD_FUNC(rb_initialize), -1); 
  rb_define_method(rb_klass, "get_roi", RUBY_METHOD_FUNC(rb_get_roi), 0);
  rb_define_alias(rb_klass, "roi", "get_roi");
  rb_define_method(rb_klass, "set_roi", RUBY_METHOD_FUNC(rb_set_roi), 1);
  rb_define_alias(rb_klass, "roi=", "set_roi");
  rb_define_method(rb_klass, "reset_roi", RUBY_METHOD_FUNC(rb_reset_roi), 0);
  rb_define_method(rb_klass, "get_coi", RUBY_METHOD_FUNC(rb_get_coi), 0);
  rb_define_alias(rb_klass, "coi", "get_coi");
  rb_define_method(rb_klass, "set_coi", RUBY_METHOD_FUNC(rb_set_coi), 1);
  rb_define_alias(rb_klass, "coi=", "set_coi");
  rb_define_method(rb_klass, "reset_coi", RUBY_METHOD_FUNC(rb_reset_coi), 0); 
}

VALUE
rb_allocate(VALUE klass)
{
  return OPENCV_OBJECT(rb_klass, 0);
}

/*
 * call-seq:
 *   new(<i>width, height[, depth = CV_8U][, channel = 3]</i>)
 *
 * Create width * height image. Each element-value set 0.
 *
 * Each element possigle range is set by <i>depth</i>. Default is unsigned 8bit.
 *
 * Number of channel is set by <i>channel</i>. <i>channel</i> should be 1..4.
 *
 * note: width = col, height = row, on CvMat. It is noted not to make a mistake
 * because the order of arguument is differenct to CvMat.
 */
VALUE
rb_initialize(int argc, VALUE *argv, VALUE self)
{
  VALUE width, height, depth, channel;
  rb_scan_args(argc, argv, "22", &width, &height, &depth, &channel);
  int _depth = argc < 3 ? CV_8U : FIX2INT(depth);
  int _channel = argc < 4 ? 3 : FIX2INT(channel);
  DATA_PTR(self) = cvCreateImage(cvSize(FIX2INT(width), FIX2INT(height)), cvIplDepth(_depth), _channel);
  return self;
}

/*
 * call-seq:
 *   IplImage::load(<i>filename[,iscolor = nil]</i>)
 *
 * Load an image from file.
 *  iscolor = true, the loaded image is forced to be color 3-channel image.
 *  iscolor = false, the loaded image is forced to be grayscale.
 *  iscolor = nil, the loaded image will be loaded as is (depend on the file).
 * Currently the following file format are supported.
 * * Windows bitmaps - BMP,DIB
 * * JPEG files - JPEG,JPG,JPE
 * * Portable Network Graphics - PNG
 * * Portable image format - PBM,PGM,PPM
 * * Sun rasters - SR,RAS     
 * * TIFF files - TIFF,TIF
 */
VALUE
rb_load_image(int argc, VALUE *argv, VALUE self)
{
  VALUE filename, iscolor;
  rb_scan_args(argc, argv, "11", &filename, &iscolor);
  Check_Type(filename, T_STRING);
  int _iscolor;
  switch (TYPE(iscolor)) {
  case T_FALSE:
    _iscolor = 0;
    break;
  case T_TRUE:
    _iscolor = 1;
    break;
  case T_NIL:
    _iscolor = -1;
    break;
  default:
    rb_warn("argument 2 should be true(color)/false(non-color) or nil(auto).");
    _iscolor = -1;        
  }
  IplImage *image;
  if ((image = cvLoadImage(StringValueCStr(filename), _iscolor)) == NULL) {
    rb_raise(rb_eStandardError, "file does not exist or invalid format image.");
  }
  return OPENCV_OBJECT(rb_klass, image);
}

/*
 * Get ROI as CvRect.
 */
VALUE
rb_get_roi(VALUE self)
{
  return cCvRect::new_object(cvGetImageROI(IPLIMAGE(self)));
}

/*
 * call-seq:
 *   set_roi(<i>rect</i>)
 *   set_roi(<i>rect</i>){|image| ...}
 *
 * Set ROI. <i>rect</i> should be CvRect or compatible object.
 * Return self.
 */
VALUE
rb_set_roi(VALUE self, VALUE roi)
{
  VALUE block = rb_block_given_p() ? rb_block_proc() : 0;
  if (block) {
    CvRect prev_roi = cvGetImageROI(IPLIMAGE(self));
    cvSetImageROI(IPLIMAGE(self), VALUE_TO_CVRECT(roi));
    rb_yield_values(1, self);
    cvSetImageROI(IPLIMAGE(self), prev_roi);
  } else {
    cvSetImageROI(IPLIMAGE(self), VALUE_TO_CVRECT(roi));
  }
  return self;
}


/*
 * Reset ROI setting. Same as IplImage#roi = nil. Return self.
 */
VALUE
rb_reset_roi(VALUE self)
{
  cvResetImageROI(IPLIMAGE(self));
  return self;
}

/*
 * Return COI as Fixnum.
 */
VALUE
rb_get_coi(VALUE self)
{
  return INT2FIX(cvGetImageCOI(IPLIMAGE(self)));
}

/*
 * call-seq:
 *   set_coi(<i>coi</i>)
 *   set_coi(<i>coi</i>){|image| ...}
 *
 * Set COI. <i>coi</i> should be Fixnum.
 * Return self.
 */
VALUE
rb_set_coi(VALUE self, VALUE coi)
{
  VALUE block = rb_block_given_p() ? rb_block_proc() : 0;
  if (block) {
    int prev_coi = cvGetImageCOI(IPLIMAGE(self));
    cvSetImageCOI(IPLIMAGE(self), FIX2INT(coi));
    rb_yield_values(1, self);
    cvSetImageCOI(IPLIMAGE(self), prev_coi);
  } else {
    cvSetImageCOI(IPLIMAGE(self), FIX2INT(coi));
  }
  return self;
}

/*
 * Reset COI setting. Same as IplImage#coi = 0. Return self.
 */
VALUE
rb_reset_coi(VALUE self)
{
  cvSetImageCOI(IPLIMAGE(self), 0);
  return self;
}


VALUE
new_object(int width, int height, int type)
{
  return OPENCV_OBJECT(rb_klass, cvCreateImage(cvSize(width, height), cvIplDepth(type), CV_MAT_CN(type)));
}

VALUE
new_object(CvSize size, int type)
{
  return OPENCV_OBJECT(rb_klass, cvCreateImage(size, cvIplDepth(type), CV_MAT_CN(type)));
}

__NAMESPACE_END_IPLIMAGE
__NAMESPACE_END_OPENCV
