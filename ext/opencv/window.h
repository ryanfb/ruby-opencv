/************************************************************

   window.h -

   $Author: lsxi $

   Copyright (C) 2005-2006 Masakazu Yonekura

************************************************************/
#ifdef HAVE_CALLBACK_H

#ifndef RUBY_OPENCV_GUI_H
#include "gui.h"
#endif

#ifndef RUBY_OPENCV_GUI_WINDOW_H
#define RUBY_OPENCV_GUI_WINDOW_H

#include "opencv.h"
#define __NAMESPACE_BEGIN_WINDOW namespace cWindow{
#define __NAMESPACE_END_WINDOW }

__NAMESPACE_BEGIN_OPENCV
__NAMESPACE_BEGIN_GUI
__NAMESPACE_BEGIN_WINDOW

void define_ruby_class();

VALUE rb_allocate(VALUE klass);

void mark(void *ptr);
VALUE each_protect(VALUE key, VALUE value);
//VALUE each_protect(void *key, VALUE value);
void free(void *ptr);

VALUE rb_aref(VALUE klass, VALUE name);
VALUE rb_initialize(int argc, VALUE *argv, VALUE self);
VALUE rb_alive_q(VALUE self);
VALUE rb_destroy(VALUE self);
VALUE rb_destroy_all(VALUE klass);
VALUE rb_resize(int argc, VALUE *argv, VALUE self);
VALUE rb_move(int argc, VALUE *argv, VALUE self);
VALUE rb_show_image(VALUE self, VALUE image);
VALUE rb_set_trackbar(int argc, VALUE *argv, VALUE self);
VALUE rb_set_mouse_callback(VALUE self);

void trackbar_callback(VALUE block, va_alist ap);
//void mouse_callback(VALUE block, va_alist ap);
void on_mouse( int event, int x, int y, int flags, void* param );

__NAMESPACE_END_WINDOW
__NAMESPACE_END_GUI
__NAMESPACE_END_OPENCV

#endif // RUBY_OPENCV_GUI_WINDOW_H

#endif // HAVE_CALLBACK_H
