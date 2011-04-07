/************************************************************

   cverror.h -

   $Author: lsxi $

   Copyright (C) 2005-2006 Masakazu Yonekura

************************************************************/
#ifndef RUBY_OPENCV_CVERROR_H
#define RUBY_OPENCV_CVERROR_H

#include "opencv.h"

#define __NAMESPACE_BEGIN_CVERROR namespace mCvError{
#define __NAMESPACE_END_CVERROR }

__NAMESPACE_BEGIN_OPENCV
__NAMESPACE_BEGIN_CVERROR

/*
  VALUE
  status_back_trace,
  status_error,
  status_internal,
  status_no_memory,      
  status_bad_argument,
  status_bad_function,
  status_no_converge,
  status_auto_trace,
  
  header_is_null,
  bad_image_size,
  bad_offset,
  bad_data_pointer,
  bad_step,
  bad_model_or_channel_seqence,
  bad_num_channels,
  bad_num_channel1U,
  bad_depth,
  bad_alpha_channel,
  bad_order,
  bad_origin,
  bad_align,
  bad_callback,
  bad_tile_size,
  bad_COI,
  bad_ROI_size,
  
  mask_is_tiled,
  
  status_null_pointer,
  status_vector_length_error,
  status_filter_struct_content_error,
  status_kernel_struct_content_error,
  status_filter_offset_error,
  
  status_bad_size,
  status_div_by_zero,
  status_inplace_not_supported,
  status_object_not_found,
  status_unmatched_formats,
  status_bad_flags,
  status_bad_point,
  status_bad_mask,
  status_unmatched_sizes,
  status_unsupported_format,
  status_out_of_range,
  status_parse_error,
  status_not_implemented,
  status_bad_memory_block;
*/
void define_ruby_module();  
VALUE by_code(int error_code);

__NAMESPACE_END_CVERROR
__NAMESPACE_END_OPENCV

#endif // RUBY_OPENCV_CVERROR_H
