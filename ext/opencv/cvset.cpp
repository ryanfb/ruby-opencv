/************************************************************

   cvseq.cpp -

   $Author: lsxi $

   Copyright (C) 2005-2006 Masakazu Yonekura

************************************************************/
#include "cvset.h"
/*
 * Document-class: OpenCV::CvSet
 *
 * Collection of nodes.
 */
__NAMESPACE_BEGIN_OPENCV
__NAMESPACE_BEGIN_CVSET

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
   * cvseq = rb_define_class_under(opencv, "CvSeq", rb_cObject); 
   *
   * note: this comment is used by rdoc.
   */
  VALUE opencv = rb_module_opencv(), cvseq = cCvSeq::rb_class();
  rb_klass = rb_define_class_under(opencv, "CvSet", cvseq);
  rb_define_method(rb_klass, "active_count", RUBY_METHOD_FUNC(rb_active_count), 0);
  rb_define_method(rb_klass, "free?", RUBY_METHOD_FUNC(rb_free_q), 0);
}

/*
 * ?
 */
VALUE
rb_active_count(VALUE self)
{
  return INT2FIX(CVSET(self)->active_count);
}

/*
 * ?
 */
VALUE
rb_free_q(VALUE self)
{
  return CVSET(self)->free_elems->flags < 0 ? Qtrue : Qfalse;
}

__NAMESPACE_END_CVSET
__NAMESPACE_END_OPENCV
