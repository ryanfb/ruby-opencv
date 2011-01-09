#!/usr/bin/env ruby
# -*- mode: ruby; coding: utf-8-unix -*- 
require 'test/unit'
require 'opencv'
require File.expand_path(File.dirname(__FILE__)) + '/helper'

include OpenCV

# Tests for OpenCV::CvSize
class TestCvSize < OpenCVTestCase
  class MySize; end

  def test_width
    size = CvSize.new
    size.width = 100
    assert_equal(100, size.width)
    size.width = 200
    assert_equal(200, size.width)
  end

  def test_height
    size = CvSize.new
    size.height = 100
    assert_equal(100, size.height)
    size.height = 200
    assert_equal(200, size.height)
  end

  def test_compatible
    assert(!(CvSize.compatible? MySize.new))
    MySize.class_eval { def width; end }
    assert(!(CvSize.compatible? MySize.new))
    MySize.class_eval { def height; end }
    assert(CvSize.compatible? MySize.new)
    assert(CvSize.compatible? CvSize.new)
  end

  def test_initialize
    size = CvSize.new
    assert_equal(0, size.width)
    assert_equal(0, size.height)

    size = CvSize.new(10, 20)
    assert_equal(10, size.width)
    assert_equal(20, size.height)

    size = CvSize.new(CvSize.new(10, 20))
    assert_equal(10, size.width)
    assert_equal(20, size.height)

    assert_raise(ArgumentError) {
      CvSize.new('string')
    }
    assert_raise(ArgumentError) {
      CvSize.new(1, 2, 3)
    }
  end

  def test_to_s
    size = CvSize.new(10, 20)
    assert_equal('<OpenCV::CvSize:10x20>', size.to_s)
  end

  def test_to_ary
    a = CvSize.new(10, 20).to_ary
    assert_equal(10, a[0])
    assert_equal(20, a[1])

    # Alias
    a = CvSize.new(10, 20).to_a
    assert_equal(10, a[0])
    assert_equal(20, a[1])
  end
end

