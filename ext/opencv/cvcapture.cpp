/************************************************************

   cvcapture.cpp -

   $Author: lsxi $

   Copyright (C) 2005-2006 Masakazu Yonekura

************************************************************/
#include"cvcapture.h"
/*
 * Document-class: OpenCV::CvCapture
 *
 * Capture image from video stream.
 *
 */
__NAMESPACE_BEGIN_OPENCV
__NAMESPACE_BEGIN_CVCAPTURE

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
  
  rb_klass = rb_define_class_under(opencv, "CvCapture", rb_cData);
  
  VALUE video_interface = rb_hash_new();
  /* {:any, :mil, :vfw, :v4l, :v4l2, :fireware, :ieee1394, :dc1394, :cmu1394, :stereo, :tyzx, :tyzx_left, :tyzx_right, :tyzx_color, :tyzx_z, :qt, :qtuicktime}: video source */
  rb_define_const(rb_klass, "INTERFACE", video_interface);
  rb_hash_aset(video_interface, ID2SYM(rb_intern("any")), INT2FIX(CV_CAP_ANY));
  rb_hash_aset(video_interface, ID2SYM(rb_intern("mil")), INT2FIX(CV_CAP_MIL));
  rb_hash_aset(video_interface, ID2SYM(rb_intern("vfw")), INT2FIX(CV_CAP_VFW));
  rb_hash_aset(video_interface, ID2SYM(rb_intern("v4l")), INT2FIX(CV_CAP_V4L));
  rb_hash_aset(video_interface, ID2SYM(rb_intern("v4l2")), INT2FIX(CV_CAP_V4L2));
  rb_hash_aset(video_interface, ID2SYM(rb_intern("fireware")), INT2FIX(CV_CAP_FIREWARE));
  rb_hash_aset(video_interface, ID2SYM(rb_intern("ieee1394")), INT2FIX(CV_CAP_IEEE1394));
  rb_hash_aset(video_interface, ID2SYM(rb_intern("dc1394")), INT2FIX(CV_CAP_DC1394));
  rb_hash_aset(video_interface, ID2SYM(rb_intern("cmu1394")), INT2FIX(CV_CAP_CMU1394));
  rb_hash_aset(video_interface, ID2SYM(rb_intern("stereo")), INT2FIX(CV_CAP_STEREO));
  rb_hash_aset(video_interface, ID2SYM(rb_intern("tyzx")), INT2FIX(CV_CAP_TYZX));
  rb_hash_aset(video_interface, ID2SYM(rb_intern("tyzx_left")), INT2FIX(CV_TYZX_LEFT));
  rb_hash_aset(video_interface, ID2SYM(rb_intern("tyzx_right")), INT2FIX(CV_TYZX_RIGHT));
  rb_hash_aset(video_interface, ID2SYM(rb_intern("tyzx_color")), INT2FIX(CV_TYZX_COLOR));
  rb_hash_aset(video_interface, ID2SYM(rb_intern("tyzx_z")), INT2FIX(CV_TYZX_Z));
  rb_hash_aset(video_interface, ID2SYM(rb_intern("qt")), INT2FIX(CV_CAP_QT));
  rb_hash_aset(video_interface, ID2SYM(rb_intern("quicktime")), INT2FIX(CV_CAP_QT));
  
  rb_define_singleton_method(rb_klass, "open", RUBY_METHOD_FUNC(rb_open), -1);
  
  rb_define_method(rb_klass, "grab", RUBY_METHOD_FUNC(rb_grab), 0);
  rb_define_method(rb_klass, "retrieve", RUBY_METHOD_FUNC(rb_retrieve), 0);
  rb_define_method(rb_klass, "query", RUBY_METHOD_FUNC(rb_query), 0);
  rb_define_method(rb_klass, "millisecond", RUBY_METHOD_FUNC(rb_millisecond), 0);
  rb_define_method(rb_klass, "frames", RUBY_METHOD_FUNC(rb_frames), 0);
  rb_define_method(rb_klass, "size", RUBY_METHOD_FUNC(rb_size), 0);
  rb_define_method(rb_klass, "width", RUBY_METHOD_FUNC(rb_width), 0);
  rb_define_method(rb_klass, "height", RUBY_METHOD_FUNC(rb_height), 0);
  rb_define_method(rb_klass, "fps", RUBY_METHOD_FUNC(rb_fps), 0);
  rb_define_method(rb_klass, "fourcc", RUBY_METHOD_FUNC(rb_fourcc), 0);
  rb_define_method(rb_klass, "frame_count", RUBY_METHOD_FUNC(rb_frame_count), 0);
}

void
free(void *ptr)
{ 
  if(ptr)
    cvReleaseCapture((CvCapture**)&ptr);
}

/*
 * call-seq:
 *   CvCapture.open(<i>[dev = -1]</i>)
 *
 * Reading video stream from the specified file or camera device.
 * If <i>dev</i> is string (i.e "stream.avi"), reading video stream from file.
 * If <i>dev</i> is number or symbol(include CvCapture::INTERFACE),
 * reading video stream from camera. 
 * Currently two camera interfaces can be used on Windows:
 * * Video for Windows(VFW)
 * * Matrox Imaging Library(MIL)
 * and two on Linux
 * * V4L
 * * FireWire(IEEE1394).
 * If there is only one camera or it does not matter what camera to use <i>nil</i> may be passed.
 */
VALUE
rb_open(int argc, VALUE *argv, VALUE self)
{
  VALUE device, i;
  rb_scan_args(argc, argv, "01", &device);
  CvCapture *capture = 0;
  switch (TYPE(device)) {
  case T_STRING:
    capture = cvCaptureFromFile(StringValueCStr(device));
    break;
  case T_FIXNUM:
    capture = cvCaptureFromCAM(FIX2INT(device));
    break;
  case T_SYMBOL:    
    i = rb_hash_aref(rb_const_get(rb_class(), rb_intern("INTERFACE")), device);
    if (NIL_P(i))
      rb_raise(rb_eArgError, "undefined interface.");
    capture = cvCaptureFromCAM(NUM2INT(i));
    break;
  case T_NIL:
    capture = cvCaptureFromCAM(CV_CAP_ANY);
    break;
  }
  if (!capture)
    rb_raise(rb_eStandardError, "Invalid capture format.");
  return Data_Wrap_Struct(rb_klass, 0, free, capture);
}


/*
 * call-seq:
 *   grab -> true or false
 *
 * Grabbed frame is stored internally. To grab frame
 * <i>fast</i> that is important for syncronization in case of reading from
 * several cameras simultaneously. The grabbed frames are not exposed because
 * they may be stored in compressed format (as defined by camera/driver).
 * To retrieve the grabbed frame, retrieve should be used.
 *
 * If grabbed frame was success, return true. Otherwise return false.
 */
VALUE
rb_grab(VALUE self)
{
  return cvGrabFrame(CVCAPTURE(self)) ? Qtrue : Qfalse;
}

/*
 * call-seq:
 *   retrieve -> CvMat or nil
 *
 * Gets the image grabbed with grab.
 */
VALUE
rb_retrieve(VALUE self)
{
  IplImage *frame = cvRetrieveFrame(CVCAPTURE(self));
  if(!frame)
    return Qnil;
  VALUE image = cIplImage::new_object(cvSize(frame->width, frame->height), CV_MAKETYPE(CV_8U, frame->nChannels));
  if (frame->origin == IPL_ORIGIN_TL) {
    cvCopy(frame, CVARR(image));
  } else {
    cvFlip(frame, CVARR(image));
  }
  return image;
}

/*
 * call-seq:
 *   query -> CvMat or nil
 *
 * Grabs and returns a frame camera or file. Just a combination of grab and retrieve in one call.
 */
VALUE
rb_query(VALUE self)
{
  IplImage *frame = cvQueryFrame(CVCAPTURE(self));
  if(!frame)
    return Qnil;
  VALUE image = cIplImage::new_object(cvSize(frame->width, frame->height), CV_MAKETYPE(CV_8U, frame->nChannels));
  if (frame->origin == IPL_ORIGIN_TL) {
    cvCopy(frame, CVARR(image));
  } else {
    cvFlip(frame, CVARR(image));
  }
  return image;
}

/*
 * Film current position in milliseconds or video capture timestamp.
 */
VALUE
rb_millisecond(VALUE self)
{
  return rb_dbl2big(cvGetCaptureProperty(CVCAPTURE(self), CV_CAP_PROP_POS_MSEC));
}

/*
 * 0-based index of the frame to be decoded/captured next
 */
VALUE
rb_frames(VALUE self)
{
  return rb_float_new(cvGetCaptureProperty(CVCAPTURE(self), CV_CAP_PROP_POS_FRAMES));
}

/*
 * Relative position of video file (0 - start of the film, 1 - end of the film)
 */
VALUE
rb_avi_ratio(VALUE self)
{
  return rb_float_new(cvGetCaptureProperty(CVCAPTURE(self), CV_CAP_PROP_POS_AVI_RATIO));  
}

/*
 * Size of frames in the video stream.
 */
VALUE
rb_size(VALUE self)
{
  return cCvSize::new_object(cvSize((int)cvGetCaptureProperty(CVCAPTURE(self), CV_CAP_PROP_FRAME_WIDTH), (int)cvGetCaptureProperty(CVCAPTURE(self), CV_CAP_PROP_FRAME_HEIGHT))); 
}

/*
 * Width of frames in the video stream.
 */
VALUE
rb_width(VALUE self)
{
  return rb_dbl2big(cvGetCaptureProperty(CVCAPTURE(self), CV_CAP_PROP_FRAME_WIDTH));
}

/*
 * Height of frames in the video stream.
 */
VALUE
rb_height(VALUE self)
{
  return rb_dbl2big(cvGetCaptureProperty(CVCAPTURE(self), CV_CAP_PROP_FRAME_HEIGHT));
}

/*
 * Frame rate
 */
VALUE
rb_fps(VALUE self)
{
  return rb_dbl2big(cvGetCaptureProperty(CVCAPTURE(self), CV_CAP_PROP_FPS));
}

/*
 * 4character code of codec. see http://www.fourcc.org/
 */
VALUE
rb_fourcc(VALUE self)
{
  char str[4];
  double fourcc = cvGetCaptureProperty(CVCAPTURE(self), CV_CAP_PROP_FOURCC);
  sprintf(str, "%s", (char*)&fourcc);
  return rb_str_new2(str);
}

/*
 * Number of frames in video file.
 */
VALUE
rb_frame_count(VALUE self)
{
  return rb_dbl2big(cvGetCaptureProperty(CVCAPTURE(self), CV_CAP_PROP_FRAME_COUNT));
}


__NAMESPACE_END_CVCAPTURE
__NAMESPACE_END_OPENCV
