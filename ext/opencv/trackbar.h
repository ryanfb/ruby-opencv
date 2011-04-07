/************************************************************

   trackbar.h -

   $Author: lsxi $

   Copyright (C) 2005 Masakazu Yonekura

************************************************************/
#ifdef HAVE_CALLBACK_H

#ifndef RUBY_OPENCV_GUI_H
#include "gui.h"
#endif

#ifndef RUBY_OPENCV_GUI_TRACKBAR_H
#define RUBY_OPENCV_GUI_TRACKBAR_H

#include"opencv.h"

#define __NAMESPACE_BEGIN_TRACKBAR namespace cTrackbar{
#define __NAMESPACE_END_TRACKBAR }

__NAMESPACE_BEGIN_OPENCV
__NAMESPACE_BEGIN_GUI

typedef struct Trackbar{
  char *name;
  int maxval;
  int val;
  VALUE block;
}Trackbar;

__NAMESPACE_BEGIN_TRACKBAR

VALUE rb_class();

void define_ruby_class();
VALUE rb_allocate(VALUE klass);

void mark(void *ptr);
void free(void *ptr);

VALUE rb_initialize(int argc, VALUE *argv, VALUE self);
VALUE rb_name(VALUE self);
VALUE rb_max(VALUE self);
VALUE rb_value(VALUE self);
VALUE rb_set_value(VALUE self, VALUE val);

__NAMESPACE_END_TRACKBAR


inline Trackbar *TRACKBAR(VALUE object){
  Trackbar *ptr;
  Data_Get_Struct(object, Trackbar, ptr);
  return ptr;
}

__NAMESPACE_END_GUI

__NAMESPACE_END_OPENCV

#endif // RUBY_OPENCV_GUI_TRACKBAR_H

#endif // HAVE_CALLBACK_H
