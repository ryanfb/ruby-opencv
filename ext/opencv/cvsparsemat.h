/************************************************************

   cvsparsemat.h -

   $Author: lsxi $

   Copyright (C) 2005-2006 Masakazu Yonekura

************************************************************/
#ifndef RUBY_OPENCV_CVSPARSEMAT_H
#define RUBY_OPENCV_CVSPARSEMAT_H

#include "opencv.h"

#define __NAMESPACE_BEGIN_CVSPARSEMAT namespace cCvSparseMat{
#define __NAMESPACE_END_CVSPARSEMAT }

__NAMESPACE_BEGIN_OPENCV
__NAMESPACE_BEGIN_CVSPARSEMAT

VALUE rb_class();

void define_ruby_class();

__NAMESPACE_END_CVSPARSEMAT
__NAMESPACE_END_OPENCV

#endif // RUBY_OPENCV_CVSPARSEMAT_H
