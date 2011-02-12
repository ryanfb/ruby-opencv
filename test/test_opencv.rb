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

    # Structuring element shapes    
    assert_equal(0, CV_SHAPE_RECT)
    assert_equal(1, CV_SHAPE_CROSS)
    assert_equal(2, CV_SHAPE_ELLIPSE)
    assert_equal(100, CV_SHAPE_CUSTOM)

    # Types of morphological operations
    assert_equal(2, CV_MOP_OPEN)
    assert_equal(3, CV_MOP_CLOSE)
    assert_equal(4, CV_MOP_GRADIENT)
    assert_equal(5, CV_MOP_TOPHAT)
    assert_equal(6, CV_MOP_BLACKHAT)

    # Types of the smoothing
    assert_equal(0, CV_BLUR_NO_SCALE)
    assert_equal(1, CV_BLUR)
    assert_equal(2, CV_GAUSSIAN)
    assert_equal(3, CV_MEDIAN)
    assert_equal(4, CV_BILATERAL)

    # Thresholding types
    assert_equal(0, CV_THRESH_BINARY)
    assert_equal(1, CV_THRESH_BINARY_INV)
    assert_equal(2, CV_THRESH_TRUNC)
    assert_equal(3, CV_THRESH_TOZERO)
    assert_equal(4, CV_THRESH_TOZERO_INV)
    assert_equal(8, CV_THRESH_OTSU)

    # Retrieval mode
    assert_equal(0, CV_RETR_EXTERNAL)
    assert_equal(1, CV_RETR_LIST)
    assert_equal(2, CV_RETR_CCOMP)
    assert_equal(3, CV_RETR_TREE)

    # Approximation method
    assert_equal(0, CV_CHAIN_CODE)
    assert_equal(1, CV_CHAIN_APPROX_NONE)
    assert_equal(2, CV_CHAIN_APPROX_SIMPLE)
    assert_equal(3, CV_CHAIN_APPROX_TC89_L1)
    assert_equal(4, CV_CHAIN_APPROX_TC89_KCOS)
    assert_equal(5, CV_LINK_RUNS)

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

    # Homography calculation methods
    assert_equal(0, HOMOGRAPHY_CALC_METHOD[:all])
    assert_equal(4, HOMOGRAPHY_CALC_METHOD[:lmeds])
    assert_equal(8, HOMOGRAPHY_CALC_METHOD[:ransac])

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

    # Structuring element shapes
    assert_equal(0, STRUCTURING_ELEMENT_SHAPE[:rect])
    assert_equal(1, STRUCTURING_ELEMENT_SHAPE[:cross])
    assert_equal(2, STRUCTURING_ELEMENT_SHAPE[:ellipse])
    assert_equal(100, STRUCTURING_ELEMENT_SHAPE[:custom])

    # Types of morphological operations
    assert_equal(2, MORPHOLOGICAL_OPERATION[:open])
    assert_equal(3, MORPHOLOGICAL_OPERATION[:close])
    assert_equal(4, MORPHOLOGICAL_OPERATION[:gradient])
    assert_equal(5, MORPHOLOGICAL_OPERATION[:tophat])
    assert_equal(6, MORPHOLOGICAL_OPERATION[:blackhat])

    # Types of the smoothing
    assert_equal(0, SMOOTHING_TYPE[:blur_no_scale])
    assert_equal(1, SMOOTHING_TYPE[:blur])
    assert_equal(2, SMOOTHING_TYPE[:gaussian])
    assert_equal(3, SMOOTHING_TYPE[:median])
    assert_equal(4, SMOOTHING_TYPE[:bilateral])

    # Thresholding types
    assert_equal(0, THRESHOLD_TYPE[:binary])
    assert_equal(1, THRESHOLD_TYPE[:binary_inv])
    assert_equal(2, THRESHOLD_TYPE[:trunc])
    assert_equal(3, THRESHOLD_TYPE[:tozero])
    assert_equal(4, THRESHOLD_TYPE[:tozero_inv])
    assert_equal(8, THRESHOLD_TYPE[:otsu])
  end

  def test_cvt_color_funcs
    mat_1ch = CvMat.new(1, 1, :cv8u, 1)
    mat_1ch[0] = CvScalar.new(10)

    mat_3ch = CvMat.new(1, 1, :cv8u, 3)
    mat_3ch[0] = CvScalar.new(10, 20, 30)

    mat_4ch = CvMat.new(1, 1, :cv8u, 4)
    mat_4ch[0] = CvScalar.new(10, 20, 30, 40)

    gray_rgb = (0.299 * mat_3ch[0][0] + 0.587 * mat_3ch[0][1] + 0.114 * mat_3ch[0][2]).round
    gray_bgr = (0.299 * mat_3ch[0][2] + 0.587 * mat_3ch[0][1] + 0.114 * mat_3ch[0][0]).round

    # RGB(A) <=> RGB(A)
    [mat_3ch.BGR2BGRA, mat_3ch.RGB2RGBA].each { |m|
      assert_equal(4, m.channel)
      assert_cvscalar_equal(CvScalar.new(10, 20, 30, 255), m[0])
    }
    [mat_3ch.BGR2RGBA, mat_3ch.RGB2BGRA].each { |m|
      assert_equal(4, m.channel)
      assert_cvscalar_equal(CvScalar.new(30, 20, 10, 255), m[0])
    }
    [mat_4ch.BGRA2BGR, mat_4ch.RGBA2RGB].each { |m|
      assert_equal(3, m.channel)
      assert_cvscalar_equal(CvScalar.new(10, 20, 30, 0), m[0])
    }
    [mat_4ch.RGBA2BGR, mat_4ch.BGRA2RGB].each { |m|
      assert_equal(3, m.channel)
      assert_cvscalar_equal(CvScalar.new(30, 20, 10, 0), m[0])
    }
    [mat_3ch.BGR2RGB, mat_3ch.RGB2BGR].each { |m|
      assert_equal(3, m.channel)
      assert_cvscalar_equal(CvScalar.new(30, 20, 10, 0), m[0])
    }
    [mat_4ch.BGRA2RGBA, mat_4ch.RGBA2BGRA].each { |m|
      assert_equal(4, m.channel)
      assert_cvscalar_equal(CvScalar.new(30, 20, 10, 40), m[0])
    }

    # RGB <=> GRAY
    [mat_3ch.BGR2GRAY, mat_4ch.BGRA2GRAY].each { |m|
      assert_equal(1, m.channel)
      assert_cvscalar_equal(CvScalar.new(gray_bgr, 0, 0, 0), m[0])
    }
    [mat_3ch.RGB2GRAY, mat_4ch.RGBA2GRAY].each { |m|
      assert_equal(1, m.channel)
      assert_cvscalar_equal(CvScalar.new(gray_rgb, 0, 0, 0), m[0])
    }
    [mat_1ch.GRAY2BGR, mat_1ch.GRAY2RGB].each { |m|
      assert_equal(3, m.channel)
      assert_cvscalar_equal(CvScalar.new(10, 10, 10, 0), m[0])
    }
    [mat_1ch.GRAY2BGRA, mat_1ch.GRAY2RGBA].each { |m|
      assert_equal(4, m.channel)
      assert_cvscalar_equal(CvScalar.new(10, 10, 10, 255), m[0])
    }

    flunk('FIXME: Most cvtColor functions are not tested yet.')
  end
end


