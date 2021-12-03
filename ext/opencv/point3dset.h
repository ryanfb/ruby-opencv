/************************************************************

   point3dset.h -

   $Author: lsxi $

   Copyright (C) 2008 Masakazu Yonekura

************************************************************/
#ifndef RUBY_OPENCV_POINT3DSET_H
#define RUBY_OPENCV_POINT3DSET_H

#define __NAMESPACE_BEGIN_POINT3D_SET namespace mPoint3DSet{
#define __NAMESPACE_END_POINT3D_SET }

#include"opencv.h"

__NAMESPACE_BEGIN_OPENCV
__NAMESPACE_BEGIN_POINT3D_SET

VALUE rb_module();

void define_ruby_module();

__NAMESPACE_END_POINT3D_SET

#define POINT3D_SET_P(object) rb_obj_is_kind_of(object, cCvSeq::rb_class()) && CV_IS_SEQ_POINT3D_SET(CVSEQ(object))

__NAMESPACE_END_OPENCV

#endif // RUBY_OPENCV_POINT3DSET_H
