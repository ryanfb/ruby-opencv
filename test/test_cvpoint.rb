#!/usr/bin/env ruby
# -*- mode: ruby; coding: utf-8-unix -*- 
require 'test/unit'
require 'opencv'
require File.expand_path(File.dirname(__FILE__)) + '/helper'

include OpenCV

# Tests for OpenCV::CvPoint
class TestCvPoint < OpenCVTestCase
  class MyPoint; end
  def test_x
    point = CvPoint.new
    point.x = 100
    assert_equal(100, point.x)
    point.x = 200
    assert_equal(200, point.x)
  end

  def test_y
    point = CvPoint.new
    point.y = 100
    assert_equal(100, point.y)
    point.y = 200
    assert_equal(200, point.y)
  end
  
  def test_compatible
    assert(!(CvPoint.compatible? MyPoint.new))
    MyPoint.class_eval { def x; end }
    assert(!(CvPoint.compatible? MyPoint.new))
    MyPoint.class_eval { def y; end }
    assert(CvPoint.compatible? MyPoint.new)
    assert(CvPoint.compatible? CvPoint.new)
  end

  def test_initialize
    point = CvPoint.new
    assert_equal(0, point.x)
    assert_equal(0, point.y)

    point = CvPoint.new(10, 20)
    assert_equal(10, point.x)
    assert_equal(20, point.y)

    point = CvPoint.new(CvPoint.new(10, 20))
    assert_equal(10, point.x)
    assert_equal(20, point.y)

    assert_raise(ArgumentError) {
      CvPoint.new('string')
    }
    assert_raise(ArgumentError) {
      CvPoint.new(1, 2, 3)
    }
  end

  def test_to_s
    point = CvPoint.new(10, 20)
    assert_equal('<OpenCV::CvPoint:(10,20)>', point.to_s)
  end

  def test_to_ary
    a = CvPoint.new(10, 20).to_ary
    assert_equal(10, a[0])
    assert_equal(20, a[1])
  end
end


