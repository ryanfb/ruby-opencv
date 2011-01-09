#!/usr/bin/env ruby
# -*- mode: ruby; coding: utf-8-unix -*- 
require 'test/unit'
require 'opencv'
require File.expand_path(File.dirname(__FILE__)) + '/helper'

include OpenCV

# Tests for OpenCV::CvPoint2D32f
class TestCvPoint2D32f < OpenCVTestCase
  class MyPoint; end
  def test_x
    point = CvPoint2D32f.new
    point.x = 1.1
    assert_in_delta(1.1, point.x, 0.001)
    point.x = 2.2
    assert_in_delta(2.2, point.x, 0.001)
  end

  def test_y
    point = CvPoint2D32f.new
    point.y = 1.1
    assert_in_delta(1.1, point.y, 0.001)
    point.y = 2.2
    assert_in_delta(2.2, point.y, 0.001)
  end
  
  def test_compatible
    assert(!(CvPoint2D32f.compatible? MyPoint.new))
    MyPoint.class_eval { def x; end }
    assert(!(CvPoint2D32f.compatible? MyPoint.new))
    MyPoint.class_eval { def y; end }
    assert(CvPoint2D32f.compatible? MyPoint.new)
    assert(CvPoint2D32f.compatible? CvPoint2D32f.new)
  end

  def test_initialize
    point = CvPoint2D32f.new
    assert_in_delta(0, point.x, 0.001)
    assert_in_delta(0, point.y, 0.001)

    point = CvPoint2D32f.new(1.1, 2.2)
    assert_in_delta(1.1, point.x, 0.001)
    assert_in_delta(2.2, point.y, 0.001)

    point = CvPoint2D32f.new(CvPoint2D32f.new(1.1, 2.2))
    assert_in_delta(1.1, point.x, 0.001)
    assert_in_delta(2.2, point.y, 0.001)

    assert_raise(ArgumentError) {
      CvPoint2D32f.new('string')
    }
    assert_raise(ArgumentError) {
      CvPoint2D32f.new(1, 2, 3)
    }
  end

  def test_to_s
    point = CvPoint2D32f.new(1.1, 2.2)
    assert_equal('<OpenCV::CvPoint2D32f:(1.1,2.2)>', point.to_s)
  end

  def test_to_ary
    a = CvPoint2D32f.new(1.1, 2.2).to_ary
    assert_in_delta(1.1, a[0], 0.001)
    assert_in_delta(2.2, a[1], 0.001)

    # Alias
    a = CvPoint2D32f.new(1.1, 2.2).to_a
    assert_in_delta(1.1, a[0], 0.001)
    assert_in_delta(2.2, a[1], 0.001)
  end
end


