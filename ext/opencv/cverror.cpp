/************************************************************

    cverror.cpp -

    $Author: lsxi $

    Copyright (C) 2005-2006 Masakazu Yonekura

************************************************************/
#include "cverror.h"
/*
 * Document-module: OpenCV::CvError
 * 
 * =Internal OpenCV errors
 *
 * This module collect OpenCV internal error wrapper classes.
 * * CvStatusBackTrace
 * * CvStatusError
 * * CvStatusInternal
 * * CvStatusNoMemory
 * * CvStatusBadArgument
 * * CvStatusNoConverge
 * * CvStatusAutoTrace
 *
 * * CvHeaderIsNull
 * * CvBadImageSize
 * * CvBadOffset
 * * CvBadDataPointer
 * * CvBadStep
 * * CvBadModelOrChannelSequence
 * * CvBadNumChannels
 * * CvBadAlphaChannel
 * * CvBadOrder
 * * CvBadOrigin
 * * CvBadAlign
 * * CvBadCallback
 * * CvBadTileSize
 * * CvBadCOI
 * * CvBadROISize
 * 
 * * CvMaskIsTiled
 *
 * * CvStatusNullPointer
 * * CvStatusVectorLengthError
 * * CvStatusFilterStructContentError
 * * CvStatusKernelStructContentError
 * * CvStatusFilterOffsetError
 * 
 * * CvStatusBadSize
 * * CvStatusDivByZero
 * * CvStatusInplaceNotSupported
 * * CvStatusObjectNotFound
 * * CvStatusUnmatchedFormant
 * * CvStatusUnsupportedFormats
 * * CvStatusOutOfRange
 * * CvStatusParseError
 * * CvStatusNotImplemented
 * * CvStsBadMemoryBlock
 */
#define RESIST_CVERROR(object_name, error_code, parent) st_insert(cv_error, (st_data_t)error_code, (st_data_t)rb_define_class_under(rb_module_opencv(), object_name, parent))

__NAMESPACE_BEGIN_OPENCV
__NAMESPACE_BEGIN_CVERROR

st_table *cv_error = st_init_numtable();
    
VALUE module;

void
define_ruby_module()
{
  /* 
   * opencv = rb_define_module("OpenCV");
   * 
   * note: this comment is used by rdoc.
   */
  VALUE opencv = rb_module_opencv();
  
  module = rb_define_module_under(opencv, "CvError");
  RESIST_CVERROR("CvStatusBackTrace",   CV_StsBackTrace, rb_eStandardError);
  RESIST_CVERROR("CvStatusError",       CV_StsError,     rb_eStandardError);
  RESIST_CVERROR("CvStatusInternal",    CV_StsInternal,  rb_eStandardError);
  RESIST_CVERROR("CvStatusNoMemory",    CV_StsNoMem,     rb_eNoMemError);
  RESIST_CVERROR("CvStatusBadArgument", CV_StsBadArg,    rb_eArgError);
  RESIST_CVERROR("CvStatusBadFunction", CV_StsBadFunc,   rb_eStandardError);
  RESIST_CVERROR("CvStatusNoConverge",  CV_StsNoConv,    rb_eStandardError);
  RESIST_CVERROR("CvStatusAutoTrace",   CV_StsAutoTrace, rb_eStandardError);
  
  RESIST_CVERROR("CvHeaderIsNull",      CV_HeaderIsNull, rb_eStandardError);
  RESIST_CVERROR("CvBadImageSize",      CV_BadImageSize, rb_eRangeError);
  RESIST_CVERROR("CvBadOffset",         CV_BadOffset,    rb_eStandardError);
  RESIST_CVERROR("CvBadDataPointer",    CV_BadDataPtr,   rb_eStandardError);
  RESIST_CVERROR("CvBadStep",           CV_BadStep,      rb_eStandardError);
  RESIST_CVERROR("CvBadModelOrChannelSequence", CV_BadModelOrChSeq, rb_eStandardError);
  RESIST_CVERROR("CvBadNumChannels",    CV_BadNumChannels, rb_eStandardError);
  RESIST_CVERROR("CvBadNumChannel1U",   CV_BadNumChannel1U, rb_eStandardError);
  RESIST_CVERROR("CvNBadDepth",         CV_BadDepth,     rb_eStandardError);
  RESIST_CVERROR("CvBadAlphaChannel",   CV_BadAlphaChannel, rb_eStandardError);
  RESIST_CVERROR("CvBadOrder",          CV_BadOrder,     rb_eStandardError);
  RESIST_CVERROR("CvBadOrigin",         CV_BadOrigin,    rb_eStandardError);
  RESIST_CVERROR("CvBadAlign",          CV_BadAlign,     rb_eStandardError);
  RESIST_CVERROR("CvBadCallBack",       CV_BadCallBack,  rb_eStandardError);
  RESIST_CVERROR("CvBadTileSize",       CV_BadTileSize,  rb_eStandardError);
  RESIST_CVERROR("CvBadCOI",            CV_BadCOI,       rb_eStandardError);
  RESIST_CVERROR("CvBadROISize",        CV_BadROISize,   rb_eStandardError);
  
  RESIST_CVERROR("CvMaskIsTiled",       CV_MaskIsTiled,   rb_eStandardError);
  
  RESIST_CVERROR("CvStatusNullPointer", CV_StsNullPtr,    rb_eStandardError);
  RESIST_CVERROR("CvStatusVectorLengthError", CV_StsVecLengthErr, rb_eStandardError);
  RESIST_CVERROR("CvStatusFilterStructContentError", CV_StsFilterStructContentErr, rb_eStandardError);
  RESIST_CVERROR("CvStatusKernelStructContentError", CV_StsKernelStructContentErr, rb_eStandardError);
  RESIST_CVERROR("CvStatusFilterOffsetError",        CV_StsFilterOffsetErr,        rb_eStandardError);
      
  RESIST_CVERROR("CvStatusBadSize",     CV_StsBadSize,    rb_eStandardError);
  RESIST_CVERROR("CvStatusDivByZero",   CV_StsDivByZero,  rb_eStandardError);
  RESIST_CVERROR("CvStatusInplaceNotSupported", CV_StsInplaceNotSupported, rb_eStandardError);
  RESIST_CVERROR("CvStatusObjectNotFound", CV_StsObjectNotFound, rb_eStandardError);
  RESIST_CVERROR("CvStatusUnmatchedFormats", CV_StsUnmatchedFormats, rb_eStandardError);
  RESIST_CVERROR("CvStatusBadFlag",     CV_StsBadFlag,    rb_eStandardError);
  RESIST_CVERROR("CvStatusBadPoint",    CV_StsBadPoint,   rb_eStandardError);
  RESIST_CVERROR("CvStatusBadMask",     CV_StsBadMask,    rb_eStandardError);
  RESIST_CVERROR("CvStatusUnmatchedSizes", CV_StsUnmatchedSizes, rb_eStandardError);
  RESIST_CVERROR("CvStatusUnsupportedFormat", CV_StsUnsupportedFormat, rb_eStandardError);
  RESIST_CVERROR("CvStatusOutOfRange",  CV_StsOutOfRange, rb_eStandardError);
  RESIST_CVERROR("CvStatusParseError",  CV_StsParseError, rb_eStandardError);
  RESIST_CVERROR("CvStatusNotImplemented", CV_StsNotImplemented, rb_eNotImpError);
  RESIST_CVERROR("CvStsBadMemoryBlock", CV_StsBadMemBlock,rb_eStandardError);
}
    
VALUE
by_code(int error_code)
{
  VALUE klass = 0;
  st_lookup(cv_error, (st_data_t)error_code, (st_data_t*)&klass);
  return klass ? klass : rb_eStandardError;
}

__NAMESPACE_END_CVERROR
__NAMESPACE_END_OPENCV
