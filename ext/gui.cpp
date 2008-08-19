/************************************************************

  gui.cpp -

  $Author: lsxi $

  Copyright (C) 2005-2006 Masakazu Yonekura

************************************************************/
#ifdef HAVE_CALLBACK_H

#include "gui.h"
/*
 * Document-module: OpenCV::GUI
 *
 * OpenCV simple GUI interface. Provides Window, Trackbar.
 * This GUI work on GTK or Microsoft Windows native GUI.
 */
__NAMESPACE_BEGIN_OPENCV
__NAMESPACE_BEGIN_GUI
    
VALUE rb_module;

VALUE
rb_module_GUI()
{
  return rb_module;
}

void
define_ruby_module()
{
  if(rb_module)
    return;
  /* 
   * opencv = rb_define_module("OpenCV");      
   *
   * note: this comment is used by rdoc.
   */
  VALUE opencv = rb_module_opencv();
  rb_module = rb_define_module_under(opencv, "GUI");
  rb_define_singleton_method(rb_module, "wait_key", RUBY_METHOD_FUNC(rb_wait_key), -1);
}

/*
 * call-seq:
 *   wait_key(<i>[delay]</i>) -> int or nil
 *
 * Waits for a pressed key each GUI wedget.
 * Return the code of the pressed key or nil if no key were pressed until the specified timeout has elapsed.
 * <i>delay</i> should be Fixnum. Wait <i>delay</i> millisecond.
 */
VALUE
rb_wait_key(int argc, VALUE *argv, VALUE self)
{
  VALUE delay;
  rb_scan_args(argc, argv, "01", &delay);
  int keycode = 0;
  return ((keycode = cvWaitKey(IF_INT(delay, 0))) < 0) ? Qnil : INT2FIX(keycode);
}

__NAMESPACE_END_GUI
__NAMESPACE_END_OPENCV

#endif // HAVE_CALLBACK_H
