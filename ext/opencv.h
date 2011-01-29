/************************************************************

    opencv.h -

    $Author: lsxi $

    Copyright (C) 2005-2007 Masakazu Yonekura

************************************************************/
#ifndef RUBY_OPENCV_H
#define RUBY_OPENCV_H

#define __NAMESPACE_BEGIN_OPENCV namespace mOpenCV{
#define __NAMESPACE_END_OPENCV }

/* include headers */
#include <ruby.h>
#ifdef HAVE_RUBY_VERSION_H
#include <ruby/version.h>
#else
#include <version.h>
#endif

#ifdef RUBY_WIN32_H
#ifdef write
#undef write
#endif // write
#endif // RUBY_WIN32_H

#ifndef ANYARGS
#define ANYARGS ()
#endif

extern "C"{
#ifdef HAVE_RUBY_ST_H
#include <ruby/st.h>
#else
#include <st.h>
#endif

#ifdef HAVE_CALLBACK_H
#include <callback.h> // callhack.h is ffcall header
#endif

#ifdef HAVE_STDARG_H
#include <stdarg.h>
#define va_init_list(a,b) va_start(a,b)
#else
#include <varargs.h>
#define va_init_list(a,b) va_start(a)
#endif
}

// standard c headers
#include <math.h>
#include <limits.h>
#include <float.h>
#include <assert.h>

// OpenCV headers
#include "opencv2/core/core_c.h"
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc_c.h"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/video/tracking.hpp"
#include "opencv2/video/background_segm.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/flann/flann.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/legacy/compat.hpp"
#include "opencv2/legacy/legacy.hpp"
#include "opencv2/legacy/blobtrack.hpp"
#include "opencv2/contrib/contrib.hpp"
#include "opencv2/highgui/highgui_c.h"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/core/internal.hpp"

#ifdef HAVE_ML_H
#include "opencv2/ml/ml.hpp"
#endif

// Ruby/OpenCV headers
#include "cverror.h"
#include "cvpoint.h"
#include "cvpoint2d32f.h"
#include "cvsize.h"
#include "cvsize2d32f.h"
#include "cvrect.h"
#include "cvscalar.h"
#include "cvslice.h"
#include "cvtermcriteria.h"
#include "cvbox2d.h"
#include "cvfont.h"
#include "iplconvkernel.h"
#include "cvmoments.h"
#include "cvconvexitydefect.h"
#include "cvpoint3d32f.h"

#include "cvmemstorage.h"

#include "cvseq.h"
#include "curve.h"
#include "pointset.h"
#include "point3dset.h"
#include "cvset.h"
#include "cvchain.h"
#include "cvchaincode.h"
#include "cvcontour.h"
#include "cvcontourtree.h"

#include "cvmat.h"
#include "iplimage.h"
#include "cvmatnd.h"
#include "cvsparsemat.h"
#include "cvhistogram.h"
#include "cvcapture.h"

#include "cvindex.h"
#include "cvline.h"
#include "cvtwopoints.h"
#include "cvcircle32f.h"

#include "cvcondensation.h"

#include "cvconnectedcomp.h"
#include "cvavgcomp.h"
#include "cvhaarclassifiercascade.h"

// GUI
#include "gui.h"
#include "window.h"
#include "trackbar.h"
#include "mouseevent.h"

// memory management wrapper
#define CVALLOC(type) (type*)cvAlloc(sizeof(type))

// useful macros
#define IF_INT(val, ifnone) NIL_P(val) ? ifnone : NUM2INT(val)
#define IF_DBL(val, ifnone) NIL_P(val) ? ifnone : NUM2DBL(val)
#define IF_STRING(str) NIL_P(str) ? NULL : TYPE(str) == T_STRING ? rb
#define IF_BOOL(val, t, f, ifnone) val == Qtrue ? t : val == Qfalse ? f : ifnone

#define IF_DEPTH(val, ifnone) NIL_P(val) ? ifnone : FIX2INT(val)

#define RESIST_CVMETHOD(hash, str, value) rb_hash_aset(hash, ID2SYM(rb_intern(str)), INT2FIX(value))

#define maxint(a,b) ({int _a = (a), _b = (b); _a > _b ? _a : _b; })

// wrapper for <= 1.8
#ifndef RARRAY_LEN
#define RARRAY_LEN(arg) (RARRAY(arg)->len)
#endif

#ifndef RARRAY_PTR
#define RARRAY_PTR(arg) (RARRAY(arg)->ptr)
#endif

#ifndef RSTRING_LEN
#define RSTRING_LEN(arg) (RSTRING(arg)->len)
#endif

#ifndef RSTRING_PTR
#define RSTRING_PTR(arg) (RSTRING(arg)->ptr)
#endif


// OpenCV module
__NAMESPACE_BEGIN_OPENCV

void mark_root_object(void *ptr);
VALUE lookup_root_object(void *ptr);
void resist_root_object(void *ptr, VALUE root);
void unresist_object(void *ptr);
void free_object(void *ptr);
void release_object(void *ptr);
void release_iplconvkernel_object(void *ptr);

VALUE rb_module_opencv();
void define_ruby_module();

// Ruby/OpenCV inline functions  
inline CvArr*
CVARR(VALUE object)
{
  CvArr *ptr;
  Data_Get_Struct(object, CvArr, ptr);
  return ptr;
}  

inline VALUE
OPENCV_OBJECT(VALUE klass, void *ptr)
{
  return Data_Wrap_Struct(klass, 0, release_object, ptr);
}

inline VALUE
IPLCONVKERNEL_OBJECT(VALUE klass, void *ptr)
{
  return Data_Wrap_Struct(klass, 0, release_iplconvkernel_object, ptr);
}

inline VALUE
GENERIC_OBJECT(VALUE klass, void *ptr)
{
  return Data_Wrap_Struct(klass, 0, 0, ptr);
}

inline VALUE
DEPEND_OBJECT(VALUE klass, void *ptr, VALUE root)
{
  resist_root_object(ptr, root);
  return Data_Wrap_Struct(klass, mark_root_object, free_object, ptr);
}

inline VALUE
REFER_OBJECT(VALUE klass, void *ptr, VALUE root)
{
  resist_root_object(ptr, root);
  return Data_Wrap_Struct(klass, mark_root_object, unresist_object, ptr);
}

inline VALUE
CONVERT(VALUE object, VALUE klass)
{
  VALUE method_name = rb_str_concat(rb_str_new2("from_"), rb_str_new2(rb_obj_classname(object)));
  VALUE result = rb_funcall(klass, rb_intern(StringValueCStr(method_name)), 1, object);
  if(CLASS_OF(result) != klass)
    rb_raise(rb_eTypeError, "require %s, but return %s.", rb_class2name(klass), rb_class2name(CLASS_OF(result)));
  return result;
}

inline int
CVMETHOD(const char *name, VALUE method, int ifnone = 0)
{
  VALUE value;
  switch(TYPE(method)){
  case T_NIL:
    return ifnone;
  case T_FIXNUM:
    return FIX2INT(method);
  case T_STRING:
    method = rb_str_intern(method);
  case T_SYMBOL:
    value = rb_hash_aref(rb_const_get(rb_module_opencv(), rb_intern(name)), method);
    if(NIL_P(value)){
      rb_warn("invalid opencv method type (see OpenCV::%s)", name);
      return ifnone;
    }else{
      return FIX2INT(value);
    }
  default:
    rb_raise(rb_eTypeError, "");
  }
  return ifnone;
}

inline int
TRUE_OR_FALSE(VALUE object, int ifnone)
{
  int value = ifnone;
  switch (TYPE(object)) {
  case T_TRUE:
    value = 1;
    break;
  case T_FALSE:
    value = 0;
    break;
  case T_NIL:
    break;
  default:
    rb_warn("argument should be true or false.");
  }
  return value;
}

VALUE rb_BGR2BGRA(VALUE klass, VALUE image);
VALUE rb_RGB2RGBA(VALUE klass, VALUE image);
VALUE rb_BGRA2BGR(VALUE klass, VALUE image);
VALUE rb_RGBA2RGB(VALUE klass, VALUE image);
VALUE rb_BGR2RGBA(VALUE klass, VALUE image);
VALUE rb_RGB2BGRA(VALUE klass, VALUE image);
VALUE rb_RGBA2BGR(VALUE klass, VALUE image);
VALUE rb_BGRA2RGB(VALUE klass, VALUE image);
VALUE rb_BGR2RGB(VALUE klass, VALUE image);
VALUE rb_RGB2BGR(VALUE klass, VALUE image);
VALUE rb_BGRA2RGBA(VALUE klass, VALUE image);
VALUE rb_RGBA2BGRA(VALUE klass, VALUE image);
VALUE rb_BGR2GRAY(VALUE klass, VALUE image);
VALUE rb_RGB2GRAY(VALUE klass, VALUE image);
VALUE rb_GRAY2BGR(VALUE klass, VALUE image);
VALUE rb_GRAY2RGB(VALUE klass, VALUE image);
VALUE rb_GRAY2BGRA(VALUE klass, VALUE image);
VALUE rb_GRAY2RGBA(VALUE klass, VALUE image);
VALUE rb_BGRA2GRAY(VALUE klass, VALUE image);
VALUE rb_RGBA2GRAY(VALUE klass, VALUE image);
VALUE rb_BGR2BGR565(VALUE klass, VALUE image);
VALUE rb_RGB2BGR565(VALUE klass, VALUE image);
VALUE rb_BGR5652BGR(VALUE klass, VALUE image);
VALUE rb_BGR5652RGB(VALUE klass, VALUE image);
VALUE rb_BGRA2BGR565(VALUE klass, VALUE image);
VALUE rb_RGBA2BGR565(VALUE klass, VALUE image);
VALUE rb_BGR5652BGRA(VALUE klass, VALUE image);
VALUE rb_BGR5652RGBA(VALUE klass, VALUE image);
VALUE rb_GRAY2BGR565(VALUE klass, VALUE image);
VALUE rb_BGR5652GRAY(VALUE klass, VALUE image);
VALUE rb_BGR2BGR555(VALUE klass, VALUE image);
VALUE rb_RGB2BGR555(VALUE klass, VALUE image);
VALUE rb_BGR5552BGR(VALUE klass, VALUE image);
VALUE rb_BGR5552RGB(VALUE klass, VALUE image);
VALUE rb_BGRA2BGR555(VALUE klass, VALUE image);
VALUE rb_RGBA2BGR555(VALUE klass, VALUE image);
VALUE rb_BGR5552BGRA(VALUE klass, VALUE image);
VALUE rb_BGR5552RGBA(VALUE klass, VALUE image);
VALUE rb_GRAY2BGR555(VALUE klass, VALUE image);
VALUE rb_BGR5552GRAY(VALUE klass, VALUE image);
VALUE rb_BGR2XYZ(VALUE klass, VALUE image);
VALUE rb_RGB2XYZ(VALUE klass, VALUE image);
VALUE rb_XYZ2BGR(VALUE klass, VALUE image);
VALUE rb_XYZ2RGB(VALUE klass, VALUE image);
VALUE rb_BGR2YCrCb(VALUE klass, VALUE image);
VALUE rb_RGB2YCrCb(VALUE klass, VALUE image);
VALUE rb_YCrCb2BGR(VALUE klass, VALUE image);
VALUE rb_YCrCb2RGB(VALUE klass, VALUE image);
VALUE rb_BGR2HSV(VALUE klass, VALUE image);
VALUE rb_RGB2HSV(VALUE klass, VALUE image);
VALUE rb_BGR2Lab(VALUE klass, VALUE image);
VALUE rb_RGB2Lab(VALUE klass, VALUE image);
VALUE rb_BayerBG2BGR(VALUE klass, VALUE image);
VALUE rb_BayerGB2BGR(VALUE klass, VALUE image);
VALUE rb_BayerRG2BGR(VALUE klass, VALUE image);
VALUE rb_BayerGR2BGR(VALUE klass, VALUE image);
VALUE rb_BayerBG2RGB(VALUE klass, VALUE image);
VALUE rb_BayerGB2RGB(VALUE klass, VALUE image);
VALUE rb_BayerRG2RGB(VALUE klass, VALUE image);
VALUE rb_BayerGR2RGB(VALUE klass, VALUE image);
VALUE rb_BGR2Luv(VALUE klass, VALUE image);
VALUE rb_RGB2Luv(VALUE klass, VALUE image);
VALUE rb_BGR2HLS(VALUE klass, VALUE image);
VALUE rb_RGB2HLS(VALUE klass, VALUE image);
VALUE rb_HSV2BGR(VALUE klass, VALUE image);
VALUE rb_HSV2RGB(VALUE klass, VALUE image);
VALUE rb_Lab2BGR(VALUE klass, VALUE image);
VALUE rb_Lab2RGB(VALUE klass, VALUE image);
VALUE rb_Luv2BGR(VALUE klass, VALUE image);
VALUE rb_Luv2RGB(VALUE klass, VALUE image);
VALUE rb_HLS2BGR(VALUE klass, VALUE image);
VALUE rb_HLS2RGB(VALUE klass, VALUE image);

__NAMESPACE_END_OPENCV

inline VALUE
extract_options_from_args_bang(VALUE ary)
{
  return (RARRAY_LEN(ary) > 0 && rb_obj_is_kind_of(RARRAY_PTR(ary)[RARRAY_LEN(ary) -1], rb_cHash)) ? rb_ary_pop(ary) : rb_hash_new();
}

/*
  inline VALUE
  assert_valid_keys(VALUE keys, VALUE valid_keys)
  {
  VALUE unknown_keys = rb_funcall(keys, rb_intern("-"), 1, rb_funcall(valid_keys, rb_intern("flatten"), 0));
  if (NUM2INT(rb_funcall(unknown_keys, rb_intern("empty?"), 0)) != 0){
  rb_raise(rb_eArgError, "Unknown key(s): %s",
  RSTRING_PTR(rb_funcall(unknown_keys, rb_intern("join"), 1, rb_str_new2(", "))));
  }
  return Qnil;  
  }
*/
/*
  inline void
  assert_valid_keys(VALUE options, int n, ...){
  va_list valid_keys;  
  if (!(n > 0)) {return;}
  VALUE unknown_keys = rb_funcall(options, rb_intern("keys"), 0);
  va_start(valid_keys, n);
  for (int i = 0; i < n; i++)
  rb_ary_delete(unknown_keys, ID2SYM(rb_intern(va_arg(valid_keys, char*))));
  if (RARRAY_LEN(unknown_keys) > 0)
  rb_raise(rb_eArgError, "Unknown key(s): %s",
  RSTRING_PTR(rb_funcall(unknown_keys, rb_intern("join"), 1, rb_str_new2(", "))));
  va_end(valid_keys);
  }

  inline VALUE
  validate_option(VALUE options, const *char key, char *ifnone, int n, ...){
  va_list valid_values;
  VALUE value = rb_hash_aref(options, ID2SYM(rb_intern(key)));
  if (!value || !(n > 0)) {return ifnone;}
  va_start(valid_values, n); 
  for (int i = 0; i < n; i++){    
  if (!strcmp(StringValueCStr(value), va_arg(valid_values, char*))){
  rb_warn("Option :%s value :%s does not defined. Default value :%s is used.", StringValueCStr(value), );
  return ifnone;
  }
  }
  va_end(valid_values);
  return value;
  }

  #define OPTIONS(value, hash, key, default) value = ((value = rb_hash_aref(hash, ID2SYM(rb_intern(key)))) ? value : default)
*/
#endif // RUBY_OPENCV_H
