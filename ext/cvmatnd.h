/************************************************************

   cvmatnd.h -

   $Author: lsxi $

   Copyright (C) 2007 Masakazu Yonekura

************************************************************/
#ifndef RUBY_OPENCV_CVMATND_H
#define RUBY_OPENCV_CVMATND_H

#include "opencv.h"

#define __NAMESPACE_BEGIN_CVMATND namespace cCvMatND{
#define __NAMESPACE_END_CVMATND }

__NAMESPACE_BEGIN_OPENCV
__NAMESPACE_BEGIN_CVMATND

VALUE rb_class();

void define_ruby_class();

__NAMESPACE_END_CVMATND
__NAMESPACE_END_OPENCV

#endif // RUBY_OPENCV_CVMATND_H
