/************************************************************

   cvvideowriter.cpp -

   $Author: lsxi $

   Copyright (C) 2005-2006 Masakazu Yonekura

************************************************************/
#include "cvvideowriter.h"
/*
 * Document-class: OpenCV::CvVideoWriter
 *
 * Create video stream from images.
 *
 * C structure is "black box".
 */
__NAMESPACE_BEGIN_OPENCV
__NAMESPACE_BEGIN_CVVIDEOWRITER

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
  rb_klass = rb_define_class_under(opencv, "CvVideoWriter", rb_cObject);
  rb_define_private_method(rb_klass, "initialize", RUBY_METHOD_FUNC(rb_initialize), -1);
  rb_define_method(rb_klass, "write", RUBY_METHOD_FUNC(rb_write), 1);
  rb_define_method(rb_klass, "close", RUBY_METHOD_FUNC(rb_close), 0);
}

/*
 * call-seq:
 *   CvVideoWriter.new(<i>filanem, fourcc, fps, size</i>) -> cvvideowriter
 *   CvVideoWriter.new(<i>filanem, fourcc, fps, size</i>){|vw| ... } -> nil
 *
 * Open new video writer. If block given, writer is closed automatically when end of block.
 * 
 * note: if <i>fourcc</i> is nil, popup codec select dialog (Windows only).
 */
VALUE
rb_initialize(int argc, VALUE *argv, VALUE self)
{
  VALUE filename, fourcc, fps, size;
  rb_scan_args(argc, argv, "40", &filename, &fourcc, &fps, &size);
  char codec[4] = {' ', ' ', ' ', ' '};
  int codec_number;
  Check_Type(filename, T_STRING);
  if (RSTRING_LEN(filename) == 0)
    rb_raise(rb_eArgError, "argument 1 (file name) dose not given");
  if (NIL_P(fourcc))
    codec_number = -1;
  else {
    Check_Type(fourcc, T_STRING);
    if (RSTRING_LEN(fourcc) > 4)
      rb_raise(rb_eStandardError, "argument 2 (fourcc) should be specific 4-character. (i.e \"PIM1\",\"MJPG\")");
    else {
      for (int i = 0; i < RSTRING_LEN(fourcc); i++)
        codec[i] = RSTRING_PTR(fourcc)[i];
      codec_number = CV_FOURCC(codec[0], codec[1], codec[2], codec[3]);
    }
  }
  DATA_PTR(self) = cvCreateVideoWriter(StringValueCStr(filename), codec_number, FIX2INT(fps), VALUE_TO_CVSIZE(size));
  if (rb_block_given_p()) {
    rb_yield(self);
    rb_close(self);
    return Qnil;
  } else {
    return self;
  }
}

/*
 * call-seq:
 *   write(<i>frame</i>)
 *
 * Write image as frame of video stream.
 * <i>frame</i> should be CvMat or subclass.
 */
VALUE
rb_write(VALUE self, VALUE frame)
{
  if (!rb_obj_is_kind_of(frame, cCvMat::rb_class()))
    rb_raise(rb_eTypeError, "argument 1 (frame) should be %s or subclass", rb_class2name(cCvMat::rb_class()));
  cvWriteFrame(CVVIDEOWRITER(self), IPLIMAGE(frame));
  return self;
}

/*
 * Close vidoe writer.
 */
VALUE
rb_close(VALUE self)
{
  CvVideoWriter *writer = CVVIDEOWRITER(self);
  cvReleaseVideoWriter(&writer);
  return Qnil;
}


__NAMESPACE_END_CVVIDEOWRITER
__NAMESPACE_END_OPENCV
