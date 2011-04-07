/************************************************************

   cvseq.h -

   $Author: lsxi $

   Copyright (C) 2005-2006 Masakazu Yonekura

************************************************************/
#ifndef RUBY_OPENCV_CVSEQ_H
#define RUBY_OPENCV_CVSEQ_H

#include "opencv.h"

#define __NAMESPACE_BEGIN_CVSEQ namespace cCvSeq{
#define __NAMESPACE_END_CVSEQ }

__NAMESPACE_BEGIN_OPENCV
__NAMESPACE_BEGIN_CVSEQ

VALUE rb_class();
void define_ruby_class();

VALUE seqblock_class(void *ptr);

VALUE rb_allocate(VALUE klass);
void free(void *ptr);
void resist_class_information_of_sequence(CvSeq *seq, VALUE klass);

VALUE rb_initialize(int argc, VALUE *argv, VALUE self);
VALUE rb_total(VALUE self);
VALUE rb_empty_q(VALUE self);
VALUE rb_aref(VALUE self, VALUE index);
VALUE rb_first(VALUE self);
VALUE rb_last(VALUE self);
VALUE rb_h_prev(VALUE self);
VALUE rb_h_next(VALUE self);
VALUE rb_v_prev(VALUE self);
VALUE rb_v_next(VALUE self);
VALUE rb_push(VALUE self, VALUE args);
VALUE rb_pop(VALUE self);
VALUE rb_unshift(VALUE self, VALUE args);
VALUE rb_shift(VALUE self);
VALUE rb_each(VALUE self);
VALUE rb_each_index(VALUE self);
VALUE rb_insert(VALUE self, VALUE index, VALUE object);
VALUE rb_remove(VALUE self, VALUE index);
VALUE rb_clear(VALUE self);

VALUE new_object(CvSeq *seq, VALUE klass);
VALUE new_object(CvSeq *seq, VALUE klass, VALUE storage);
VALUE new_sequence(VALUE klass, CvSeq *seq, VALUE element_klass, VALUE storage);
VALUE auto_extend(VALUE object);

__NAMESPACE_END_CVSEQ

inline CvSeq*
CVSEQ(VALUE object)
{
  CvSeq *ptr;
  Data_Get_Struct(object, CvSeq, ptr);
  return ptr;
}

__NAMESPACE_END_OPENCV

#endif // RUBY_OPENCV_CVSEQ_H
