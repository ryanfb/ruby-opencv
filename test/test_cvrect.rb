#!/usr/bin/env ruby
# -*- mode: ruby; coding: utf-8-unix -*- 
require 'test/unit'
require 'opencv'
require File.expand_path(File.dirname(__FILE__)) + '/helper'

include OpenCV

# Tests for OpenCV::CvRect
class TestCvRect < OpenCVTestCase
  class MyRect; end

  def test_x
    rect = CvRect.new
    rect.x = 100
    assert_equal(100, rect.x)
    rect.x = 200
    assert_equal(200, rect.x)
  end

  def test_y
    rect = CvRect.new
    rect.y = 100
    assert_equal(100, rect.y)
    rect.y = 200
    assert_equal(200, rect.y)
  end

  def test_width
    rect = CvRect.new
    rect.width = 100
    assert_equal(100, rect.width)
    rect.width = 200
    assert_equal(200, rect.width)
  end

  def test_height
    rect = CvRect.new
    rect.height = 100
    assert_equal(100, rect.height)
    rect.height = 200
    assert_equal(200, rect.height)
  end

  def test_compatible
    assert(!(CvRect.compatible? MyRect.new))
    MyRect.class_eval { def x; end }
    assert(!(CvRect.compatible? MyRect.new))
    MyRect.class_eval { def y; end }
    assert(!(CvRect.compatible? MyRect.new))
    MyRect.class_eval { def width; end }
    assert(!(CvRect.compatible? MyRect.new))
    MyRect.class_eval { def height; end }
    assert(CvRect.compatible? MyRect.new)
    assert(CvRect.compatible? CvRect.new)
  end

  def test_initialize
    rect = CvRect.new
    assert_equal(0, rect.x)
    assert_equal(0, rect.y)
    assert_equal(0, rect.width)
    assert_equal(0, rect.height)

    rect = CvRect.new(10, 20, 30, 40)
    assert_equal(10, rect.x)
    assert_equal(20, rect.y)
    assert_equal(30, rect.width)
    assert_equal(40, rect.height)

    rect = CvRect.new(CvRect.new(10, 20, 30, 40))
    assert_equal(10, rect.x)
    assert_equal(20, rect.y)
    assert_equal(30, rect.width)
    assert_equal(40, rect.height)

    assert_raise(ArgumentError) {
      CvRect.new('string')
    }
    assert_raise(ArgumentError) {
      CvRect.new(1, 2, 3)
    }
  end

  def test_center
    center = CvRect.new(10, 20, 35, 45).center
    assert_in_delta(27.5, center.x, 0.01)
    assert_in_delta(42.5, center.y, 0.01)
  end

  def test_points
    points = CvRect.new(10, 20, 35, 45).points
    assert_equal(4, points.size)
    assert_in_delta(10, points[0].x, 0.01)
    assert_in_delta(20, points[0].y, 0.01)
    assert_in_delta(10, points[1].x, 0.01)
    assert_in_delta(65, points[1].y, 0.01)
    assert_in_delta(45, points[2].x, 0.01)
    assert_in_delta(65, points[2].y, 0.01)
    assert_in_delta(45, points[3].x, 0.01)
    assert_in_delta(20, points[3].y, 0.01)
  end

  def test_top_left
    tl = CvRect.new(10, 20, 35, 45).top_left
    assert_equal(10, tl.x)
    assert_equal(20, tl.y)
  end

  def test_top_right
    tr = CvRect.new(10, 20, 35, 45).top_right
    assert_equal(45, tr.x)
    assert_equal(20, tr.y)
  end

  def test_bottom_left
    bl = CvRect.new(10, 20, 35, 45).bottom_left
    assert_equal(10, bl.x)
    assert_equal(65, bl.y)
  end

  def test_bottom_right
    br = CvRect.new(10, 20, 35, 45).bottom_right
    assert_equal(45, br.x)
    assert_equal(65, br.y)
  end

  def test_max_rect
    rect1 = CvRect.new(10, 20, 30, 40)
    rect2 = CvRect.new(30, 40, 70, 80)
    rect3 = CvRect.max_rect(rect1, rect2)
    assert_equal(10, rect3.x)
    assert_equal(20, rect3.y)
    assert_equal(90, rect3.width)
    assert_equal(100, rect3.height)
  end
end

