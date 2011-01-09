#!/usr/bin/env ruby
# -*- mode: ruby; coding: utf-8-unix -*- 
require 'test/unit'
require 'opencv'
require File.expand_path(File.dirname(__FILE__)) + '/helper'

include OpenCV

# Tests for OpenCV::CvSize2D32f
class TestCvSize2D32f < OpenCVTestCase
  class MySize; end

  def test_width
    size = CvSize2D32f.new
    size.width = 1.1
    assert_in_delta(1.1, size.width, 0.001)
    size.width = 2.2
    assert_in_delta(2.2, size.width, 0.001)
  end

  def test_height
    size = CvSize2D32f.new
    size.height = 1.1
    assert_in_delta(1.1, size.height, 0.001)
    size.height = 2.2
    assert_in_delta(2.2, size.height, 0.001)
  end

  def test_compatible
    assert(!(CvSize2D32f.compatible? MySize.new))
    MySize.class_eval { def width; end }
    assert(!(CvSize2D32f.compatible? MySize.new))
    MySize.class_eval { def height; end }
    assert(CvSize2D32f.compatible? MySize.new)
    assert(CvSize2D32f.compatible? CvSize2D32f.new)
  end

  def test_initialize
    size = CvSize2D32f.new
    assert_in_delta(0, size.width, 0.001)
    assert_in_delta(0, size.height, 0.001)

    size = CvSize2D32f.new(1.1, 2.2)
    assert_in_delta(1.1, size.width, 0.001)
    assert_in_delta(2.2, size.height, 0.001)

    size = CvSize2D32f.new(CvSize2D32f.new(1.1, 2.2))
    assert_in_delta(1.1, size.width, 0.001)
    assert_in_delta(2.2, size.height, 0.001)

    assert_raise(ArgumentError) {
      CvSize2D32f.new('string')
    }
    assert_raise(ArgumentError) {
      CvSize2D32f.new(1, 2, 3)
    }
  end

  def test_to_s
    size = CvSize2D32f.new(1.1, 2.2)
    assert_equal('<OpenCV::CvSize2D32f:1.1x2.2>', size.to_s)
  end

  def test_to_ary
    a = CvSize2D32f.new(1.1, 2.2).to_ary
    assert_in_delta(1.1, a[0], 0.001)
    assert_in_delta(2.2, a[1], 0.001)
  end
end

