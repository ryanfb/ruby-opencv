#!/usr/bin/env ruby
# -*- mode: ruby; coding: utf-8-unix -*- 
require 'test/unit'
require 'opencv'
require File.expand_path(File.dirname(__FILE__)) + '/helper'

include OpenCV

class TestOpenCV < OpenCVTestCase
  def test_constants
    # Depths
    assert_equal(0, CV_8U)
    assert_equal(1, CV_8S)
    assert_equal(2, CV_16U)
    assert_equal(3, CV_16S)
    assert_equal(4, CV_32S)
    assert_equal(5, CV_32F)
    assert_equal(6, CV_64F)

    # Load image flags
    assert_equal(-1, CV_LOAD_IMAGE_UNCHANGED)
    assert_equal(0, CV_LOAD_IMAGE_GRAYSCALE)
    assert_equal(1, CV_LOAD_IMAGE_COLOR)
    assert_equal(2, CV_LOAD_IMAGE_ANYDEPTH)
    assert_equal(4, CV_LOAD_IMAGE_ANYCOLOR)
  end

  def test_symbols
    # Depths
    assert_equal(0, DEPTH[:cv8u])
    assert_equal(1, DEPTH[:cv8s])
    assert_equal(2, DEPTH[:cv16u])
    assert_equal(3, DEPTH[:cv16s])
    assert_equal(4, DEPTH[:cv32s])
    assert_equal(5, DEPTH[:cv32f])
    assert_equal(6, DEPTH[:cv64f])

    # Inversion methods
    assert_equal(0, INVERSION_METHOD[:lu])
    assert_equal(1, INVERSION_METHOD[:svd])
    assert_equal(2, INVERSION_METHOD[:svd_sym])
    assert_equal(2, INVERSION_METHOD[:svd_symmetric])

    # Flags for DFT and DCT
    assert_equal(0, DXT_FLAG[:forward])
    assert_equal(1, DXT_FLAG[:inverse])
    assert_equal(2, DXT_FLAG[:scale])
    assert_equal(4, DXT_FLAG[:rows])

    # Interpolation methods
    assert_equal(0, INTERPOLATION_METHOD[:nn])
    assert_equal(1, INTERPOLATION_METHOD[:linear])
    assert_equal(2, INTERPOLATION_METHOD[:cubic])
    assert_equal(3, INTERPOLATION_METHOD[:area])

    # Warp affine optional flags
    assert_equal(8, WARP_FLAG[:fill_outliers])
    assert_equal(16, WARP_FLAG[:inverse_map])

    # Anti aliasing flags
    assert_equal(16, CONNECTIVITY[:aa])
    assert_equal(16, CONNECTIVITY[:anti_alias])

    # Retrieval modes
    assert_equal(0, RETRIEVAL_MODE[:external])
    assert_equal(1, RETRIEVAL_MODE[:list])
    assert_equal(2, RETRIEVAL_MODE[:ccomp])
    assert_equal(3, RETRIEVAL_MODE[:tree])

    # Approximation methods
    assert_equal(0, APPROX_CHAIN_METHOD[:code])
    assert_equal(1, APPROX_CHAIN_METHOD[:approx_none])
    assert_equal(2, APPROX_CHAIN_METHOD[:approx_simple])
    assert_equal(3, APPROX_CHAIN_METHOD[:approx_tc89_l1])
    assert_equal(4, APPROX_CHAIN_METHOD[:approx_tc89_kcos])

    # Approximation methods (polygon)
    assert_equal(0, APPROX_POLY_METHOD[:dp])

    # Match template methods
    assert_equal(0, MATCH_TEMPLATE_METHOD[:sqdiff])
    assert_equal(1, MATCH_TEMPLATE_METHOD[:sqdiff_normed])
    assert_equal(2, MATCH_TEMPLATE_METHOD[:ccorr])
    assert_equal(3, MATCH_TEMPLATE_METHOD[:ccorr_normed])
    assert_equal(4, MATCH_TEMPLATE_METHOD[:ccoeff])
    assert_equal(5, MATCH_TEMPLATE_METHOD[:ccoeff_normed])
  end

  def test_cvt_color_funcs
    flunk('FIXME: cvtColor functions are not tested yet.')
  end
end


