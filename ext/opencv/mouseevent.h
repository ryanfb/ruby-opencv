/************************************************************

   mouseevent.h -

   $Author: lsxi $

   Copyright (C) 2005-2006 Masakazu Yonekura

************************************************************/
#ifdef HAVE_CALLBACK_H

#define __NAMESPACE_BEGIN_MOUSEEVENT namespace cMouseEvent{
#define __NAMESPACE_END_MOUSEEVENT }

#ifndef RUBY_OPENCV_GUI_MOUSEEVENT_H
#define RUBY_OPENCV_GUI_MOUSEEVENT_H

#include"opencv.h"

__NAMESPACE_BEGIN_OPENCV
__NAMESPACE_BEGIN_GUI
__NAMESPACE_BEGIN_MOUSEEVENT

typedef struct MouseEvent{
  CvPoint point;
  int event;
  int flag;
}MouseEvent;

VALUE rb_class();

void define_ruby_class();

VALUE rb_allocate(VALUE klass);

VALUE rb_event(VALUE self);

VALUE rb_left_button_q(VALUE self);
VALUE rb_right_button_q(VALUE self);
VALUE rb_middle_button_q(VALUE self);
VALUE rb_ctrl_key_q(VALUE self);
VALUE rb_shift_key_q(VALUE self);
VALUE rb_alt_key_q(VALUE self);

VALUE new_object(int event, int x, int y, int flag);

inline MouseEvent *MOUSEEVENT(VALUE object){
  MouseEvent *ptr;
  Data_Get_Struct(object, MouseEvent, ptr);
  return ptr;
}

__NAMESPACE_END_MOUSEEVENT
__NAMESPACE_END_GUI
__NAMESPACE_END_OPENCV

#endif // RUBY_OPENCV_GUI_MOUSEEVENT_H

#endif // HAVE_CALLBACK_H
