/************************************************************

   cvtermcriteria.h -

   $Author: lsxi $

   Copyright (C) 2005-2006 Masakazu Yonekura

************************************************************/
#ifndef RUBY_OPENCV_CVTERMCRITERIA_H
#define RUBY_OPENCV_CVTERMCRITERIA_H

#include "opencv.h"

#define __NAMESPACE_BEGIN_CVTERMCRITERIA namespace cCvTermCriteria{
#define __NAMESPACE_END_CVTERMCRITERIA }

__NAMESPACE_BEGIN_OPENCV
__NAMESPACE_BEGIN_CVTERMCRITERIA

VALUE rb_class();

void define_ruby_class();    

VALUE rb_allocate(VALUE klass);
VALUE rb_initialize(int argc, VALUE *argv, VALUE self);

VALUE rb_max(VALUE self);
VALUE rb_set_max(VALUE self, VALUE max_value);
VALUE rb_eps(VALUE self);
VALUE rb_set_eps(VALUE self, VALUE eps_value);
VALUE rb_to_ary(VALUE self);

VALUE new_object(CvTermCriteria criteria);

__NAMESPACE_END_CVTERMCRITERIA

inline CvTermCriteria*
CVTERMCRITERIA(VALUE object)
{
  CvTermCriteria *ptr;
  Data_Get_Struct(object, CvTermCriteria, ptr);
  return ptr;
}

inline CvTermCriteria
VALUE_TO_CVTERMCRITERIA(VALUE object)
{
  if (rb_obj_is_kind_of(object, cCvTermCriteria::rb_class())) {
    return *CVTERMCRITERIA(object);
  }
  switch (TYPE(object)) {
  case T_NIL:
    return cvTermCriteria(CV_TERMCRIT_ITER, 0, 0);
  case T_FIXNUM:
    return cvTermCriteria(CV_TERMCRIT_ITER, NUM2INT(object), 0);
  case T_FLOAT:
    return cvTermCriteria(CV_TERMCRIT_EPS, 0, NUM2DBL(object));
  case T_ARRAY:
    if (RARRAY_LEN(object) == 2) {
      return cvTermCriteria(CV_TERMCRIT_ITER|CV_TERMCRIT_EPS,
                            NUM2INT(rb_ary_entry(object, 0)),
                            NUM2DBL(rb_ary_entry(object, 1)));
    }
  }
  rb_raise(rb_eTypeError, "");
}

__NAMESPACE_END_OPENCV

#endif // RUBY_OPENCV_CVTERMCRITERIA_H
