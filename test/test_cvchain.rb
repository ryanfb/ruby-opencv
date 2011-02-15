#!/usr/bin/env ruby
# -*- mode: ruby; coding: utf-8-unix -*- 
require 'test/unit'
require 'opencv'
require File.expand_path(File.dirname(__FILE__)) + '/helper'

include OpenCV

# Tests for OpenCV::CvChain
class TestCvChain < OpenCVTestCase
  def test_APPROX_OPTION
    assert_equal(:approx_simple, CvChain::APPROX_CHAIN_OPTION[:method])
    assert_equal(0, CvChain::APPROX_CHAIN_OPTION[:parameter])
    assert_equal(0, CvChain::APPROX_CHAIN_OPTION[:minimal_parameter])
    assert_false(CvChain::APPROX_CHAIN_OPTION[:recursive])
  end
  
  def test_initialize
    chain = CvChain.new
    assert_not_nil(chain)
    assert_equal(CvChain, chain.class)
    assert(chain.is_a? CvSeq)
  end

  def test_origin
    mat0 = create_cvmat(128, 128, :cv8u, 1) { |j, i|
      (j - 64) ** 2 + (i - 64) ** 2 <= (32 ** 2) ? CvColor::White : CvColor::Black
    }
    chain = mat0.find_contours(:mode => CV_RETR_EXTERNAL, :method => CV_CHAIN_CODE)
    assert_equal(CvChain, chain.class)
    assert_equal(64, chain.origin.x)
    assert_equal(32, chain.origin.y)

    chain.origin = CvPoint.new(32, 64)
    assert_equal(32, chain.origin.x)
    assert_equal(64, chain.origin.y)
  end

  def test_codes
    mat0 = create_cvmat(128, 128, :cv8u, 1) { |j, i|
      (j - 64) ** 2 + (i - 64) ** 2 <= (32 ** 2) ? CvColor::White : CvColor::Black
    }
    chain = mat0.find_contours(:mode => CV_RETR_EXTERNAL, :method => CV_CHAIN_CODE)
    assert_equal(Array, chain.codes.class)
    assert(chain.codes.all? { |a| (a.class == Fixnum) and (a >= 0 and a <= 7) })
  end

  def test_points
    mat0 = create_cvmat(128, 128, :cv8u, 1) { |j, i|
      (j - 64) ** 2 + (i - 64) ** 2 <= (32 ** 2) ? CvColor::White : CvColor::Black
    }
    chain = mat0.find_contours(:mode => CV_RETR_EXTERNAL, :method => CV_CHAIN_CODE)
    assert_equal(Array, chain.points.class)
    assert(chain.points.all? { |a| a.class == CvPoint })
  end

  def test_approx_chain
    flunk('FIXME: CvChain#approx_chain is not implemented yet.')
  end
end

