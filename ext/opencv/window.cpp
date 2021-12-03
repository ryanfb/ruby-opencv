/************************************************************

   window.cpp -

   $Author: lsxi $

   Copyright (C) 2005-2006 Masakazu Yonekura

************************************************************/
#ifdef HAVE_CALLBACK_H

#include "window.h"
/*
 * Document-class: OpenCV::GUI::Window
 *
 * Simple Window wedget. Window can show image(CvMat/IplImage).
 *
 * view image sample:
 *   image = OpenCV::IplImage::load("opencv.bmp")     #=> load image
 *   window = OpenCV::GUI::Window.new("simple viewer")#=> create new window named "simaple viewer"
 *   window.show(image)                               #=> show image
 */
__NAMESPACE_BEGIN_OPENCV
__NAMESPACE_BEGIN_GUI
__NAMESPACE_BEGIN_WINDOW

const char*
GET_WINDOW_NAME(VALUE object)
{
  void *handle = DATA_PTR(object);
  if (!handle)
    rb_raise(rb_eStandardError, "window handle error");
  const char *window_name = cvGetWindowName(handle);
  return window_name;
}

st_table *windows = st_init_numtable();

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
   * GUI = rb_define_module_under(opencv, "GUI");
   *
   * note: this comment is used by rdoc.
   */
  VALUE GUI = rb_module_GUI();
  rb_klass = rb_define_class_under(GUI, "Window", rb_cObject);
  rb_define_singleton_method(rb_klass, "[]", RUBY_METHOD_FUNC(rb_aref), 1);
  rb_define_method(rb_klass, "initialize", RUBY_METHOD_FUNC(rb_initialize), -1);
  rb_define_method(rb_klass, "alive?", RUBY_METHOD_FUNC(rb_alive_q), 0);
  rb_define_method(rb_klass, "destroy", RUBY_METHOD_FUNC(rb_destroy), 0);
  rb_define_singleton_method(rb_klass, "destroy_all", RUBY_METHOD_FUNC(rb_destroy_all), 0);
  rb_define_method(rb_klass, "resize", RUBY_METHOD_FUNC(rb_resize), -1);
  rb_define_method(rb_klass, "move", RUBY_METHOD_FUNC(rb_move), -1);
  rb_define_method(rb_klass, "show_image", RUBY_METHOD_FUNC(rb_show_image), 1);
  rb_define_alias(rb_klass, "show", "show_image");
  rb_define_method(rb_klass, "set_trackbar", RUBY_METHOD_FUNC(rb_set_trackbar), -1);
  rb_define_method(rb_klass, "set_mouse_callback", RUBY_METHOD_FUNC(rb_set_mouse_callback), 0);
  rb_define_alias(rb_klass, "on_mouse", "set_mouse_callback");
}

VALUE
rb_allocate(VALUE klass)
{
  return Data_Wrap_Struct(klass, mark, free, 0);
}

void
mark(void *ptr)
{
  st_table *holder;
  if (st_lookup(windows, (st_data_t)ptr, (st_data_t*)&holder)) {
    st_foreach(holder, (int (*)(ANYARGS))each_protect, 0);
  }
}

VALUE
each_protect(VALUE key, VALUE value)
{
  rb_gc_mark(value);
  return ST_CONTINUE;
}

void
free(void *ptr)
{
  st_table *holder;
  if (st_delete(windows, (st_data_t*)ptr, (st_data_t*)&holder)) { st_free_table(holder); }
  cvFree(&ptr);
}

/*
 * call-seq:
 *   [<i>name</i>]
 *
 * Return window named <i>name</i> if exist, otherwise nil.
 */
VALUE
rb_aref(VALUE klass, VALUE name)
{
  VALUE window;
  Check_Type(name, T_STRING);
  void *handle = cvGetWindowHandle(StringValueCStr(name));
  st_table *holder;
  if (st_lookup(windows, (st_data_t)handle, (st_data_t*)&holder) &&
      st_lookup(holder, 0, (st_data_t*)&window)) {
    return window;
  }
  return Qnil;
}

/*
 * call-seq:
 *   new(<i>name[,autosize]</i>)
 *
 * Create new window named <i>name</i>.
 * If <i>autoresize</i> is true(default), window size automatically resize when image given.
 */
VALUE
rb_initialize(int argc, VALUE *argv, VALUE self)
{
  VALUE name, autosize;
  rb_scan_args(argc, argv, "11", &name, &autosize);
  Check_Type(name, T_STRING);
  int mode;
  if (argc < 2)
    mode = CV_WINDOW_AUTOSIZE;
  else{
    switch (TYPE(autosize)) {
    case T_TRUE:
      mode = CV_WINDOW_AUTOSIZE;
      break;
    case T_FALSE:
      mode = 0;
      break;
    default:
      rb_raise(rb_eTypeError, "argument 2 (auto-size) should be true or false.");
    }
  }
  cvNamedWindow(StringValueCStr(name), mode);
  void *handle = cvGetWindowHandle(StringValueCStr(name));
  if (st_lookup(windows, (st_data_t)handle, 0)) {
    rb_raise(rb_eStandardError, "window name should be unique.");
  }
  DATA_PTR(self) = handle;
  st_table *holder = st_init_numtable();
  st_insert(holder, (st_data_t)0, (st_data_t)self);
  st_insert(windows, (st_data_t)handle, (st_data_t)holder);
  return self;
}

/*
 * Return alive status of window. Return true if alive, otherwise return false.
 */
VALUE
rb_alive_q(VALUE self)
{
  if (st_lookup(windows, (st_data_t)DATA_PTR(self), 0)) {
    return Qtrue;
  }
  return Qfalse;
}

/*
 * Destroys a window. alive status of window be false.
 */
VALUE
rb_destroy(VALUE self)
{
  void *handle = DATA_PTR(self);
  st_table *holder;
  if (st_delete(windows, (st_data_t*)&handle, (st_data_t*)&holder)) {
    st_free_table(holder);
  }
  cvDestroyWindow(GET_WINDOW_NAME(self));
  return self;
}

/*
 * Destorys all the windows.
 */
VALUE
rb_destroy_all(VALUE klass)
{
  st_free_table(windows);
  windows = st_init_numtable();
  cvDestroyAllWindows();
  return Qnil;
}

/*
 * call-seq:
 *   resize(<i>size</i>)
 *   resize(<i>width, height</i>)
 *
 * Set window size.
 */
VALUE
rb_resize(int argc, VALUE *argv, VALUE self)
{
  CvSize size;
  switch (argc) {
  case 1:
    size = VALUE_TO_CVSIZE(argv[0]);
    break;
  case 2:
    size = cvSize(FIX2INT(argv[0]), FIX2INT(argv[1]));
    break;
  default:
    rb_raise(rb_eArgError, "wrong number of arguments (1 or 2)");
  }
  cvResizeWindow(GET_WINDOW_NAME(self), size.width, size.height);
  return self;
}

/*
 * call-seq:
 *   move(<i>point</i>)
 *   move(<i>x, y</i>)
 *
 * Set window position.
 */
VALUE
rb_move(int argc, VALUE *argv, VALUE self)
{
  CvPoint point;
  switch (argc) {
  case 1:
    point = VALUE_TO_CVPOINT(argv[0]);
    break;
  case 2:
    point = cvPoint(FIX2INT(argv[0]), FIX2INT(argv[1]));
    break;
  default:
    rb_raise(rb_eArgError, "wrong number of arguments (1 or 2)");
  }
  cvMoveWindow(GET_WINDOW_NAME(self), point.x, point.y);
  return self;
}

/*
 * call-seq:
 *   show_image(<i>image</i>)
 *
 * Show the image. If the window was created with <i>autosize</i> = true then the image is shown
 * with its original size, otherwize the image is scaled to fit the window.
 */
VALUE
rb_show_image(VALUE self, VALUE image)
{
  if (!rb_obj_is_kind_of(image, cCvMat::rb_class())) {
    rb_raise(rb_eTypeError, "argument should be %s.", rb_class2name(cCvMat::rb_class()));
  }
  cvShowImage(GET_WINDOW_NAME(self), CVARR(image));
  st_table *holder;
  if (st_lookup(windows, (st_data_t)DATA_PTR(self), (st_data_t*)&holder)) {
    st_insert(holder, cCvMat::rb_class(), image);
    return self;
  }else
    rb_raise(rb_eFatal, "invalid window operation.");
  return self;
}

/*
 * call-seq:
 *   set_trackbar(<i>trackbar</i>)
 *   set_trackbar(<i>name,maxval[,val],&block</i>)
 *   set_trackbar(<i>name,maxval[,val]</i>){|value| ... }
 *
 * Create Trackbar on this window. Return new Trackbar.
 * see Trackbar.new
 */
VALUE
rb_set_trackbar(int argc, VALUE *argv, VALUE self)
{
  VALUE instance;
  if (argc == 1 && rb_obj_is_kind_of(argv[0], cTrackbar::rb_class())) {
    instance = argv[0];
  }else{
    instance = cTrackbar::rb_initialize(argc, argv, cTrackbar::rb_allocate(cTrackbar::rb_class()));
  }
  Trackbar *trackbar = TRACKBAR(instance);
  void *callback = (void *)alloc_callback(&trackbar_callback, trackbar->block);
  cvCreateTrackbar(trackbar->name, GET_WINDOW_NAME(self), &trackbar->val, trackbar->maxval, (CvTrackbarCallback)callback);
  st_table *holder;
  if (st_lookup(windows, (st_data_t)DATA_PTR(self), (st_data_t*)&holder)) {
    st_insert(holder, (st_data_t)&trackbar->name, (st_data_t)instance);
  }
  return instance;
}

/*
 * call-seq:
 *   set_mouse_callback(&block)
 *   set_mouse_callback{|| ... }
 *
 * Set mouse callback.
 * When the mouse is operated on the window, block will be called.
 * Return Proc object.
 * block given mouse event object, see GUI::Window::MouseEvent
 *
 * e.g. display mouse event on console.
 *   window = OpenCV::GUI::Window.new "sample window"
 *   image = OpenCV::IplImage::load "sample.png"
 *   window.show(image)
 *   window.set_mouse_callback{|mouse|
 *     e = "#{mouse.x}, #{mouse.y} : #{mouse.event} : "
 *     e << "<L>" if mouse.left_button?
 *     e << "<R>" if mouse.right_button?
 *     e << "<M>" if mouse.middle_button?
 *     e << "[CTRL]" if mouse.ctrl_key?
 *     e << "[SHIFT]" if mouse.shift_key?
 *     e << "[ALT]" if mouse.alt_key?
 *     puts e
 *   }
 *   OpenCV::GUI::wait_key
 */
/*
VALUE
rb_set_mouse_callback(VALUE self)
{
  VALUE block = rb_block_given_p() ? rb_block_proc() : 0;
  if (!block) {rb_raise(rb_eArgError, "block not given.");}
  void *callback = (void *)alloc_callback(&mouse_callback, block);
  cvSetMouseCallback(GET_WINDOW_NAME(self), (CvMouseCallback)callback, 0);
  st_table *holder;
  if (st_lookup(windows, (st_data_t)DATA_PTR(self), (st_data_t*)&holder)) {
    st_insert(holder, rb_cProc, block);
  }else{
    rb_raise(rb_eStandardError, "window is destroyed.");
  }
  return block;
}
*/
VALUE
rb_set_mouse_callback(VALUE self)
{
  VALUE block = rb_block_given_p() ? rb_block_proc() : 0;
  if (!block) {rb_raise(rb_eArgError, "block not given.");}
  cvSetMouseCallback(GET_WINDOW_NAME(self), on_mouse, (void*)block);
  st_table *holder;
  if (st_lookup(windows, (st_data_t)DATA_PTR(self), (st_data_t*)&holder)){
	  st_insert(holder, self, block);
  }
  return block;
}

void
trackbar_callback(VALUE block, va_alist ap)
{
  va_start_void(ap);
  rb_funcall(block, rb_intern("call"), 1, INT2FIX(va_arg_int(ap)));
  va_return_void(ap);
}

/*
void
mouse_callback(VALUE block, va_alist ap)
{
  va_start_void(ap);
  rb_funcall(block, rb_intern("call"), 1, cMouseEvent::new_object(va_arg_int(ap),va_arg_int(ap),va_arg_int(ap),va_arg_int(ap)));
  va_return_void(ap);
}
*/

void on_mouse( int event, int x, int y, int flags, void* param ) {
  rb_funcall((VALUE)param, rb_intern("call"), 1, cMouseEvent::new_object(event, x, y, flags));
}

__NAMESPACE_END_WINDOW
__NAMESPACE_END_GUI
__NAMESPACE_END_OPENCV


#endif // HAVE_CALLBACK_H
