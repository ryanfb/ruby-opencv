#!/usr/bin/env ruby
# -*- mode: ruby; coding: utf-8-unix -*- 
require 'test/unit'
require 'opencv'
require File.expand_path(File.dirname(__FILE__)) + '/helper'

include OpenCV

# Tests for OpenCV::CvPoint3D32f
class TestCvPoint3D32f < OpenCVTestCase
  class MyPoint; end
  def test_x
    point = CvPoint3D32f.new
    point.x = 1.1
    assert_in_delta(1.1, point.x, 0.001)
    point.x = 2.2
    assert_in_delta(2.2, point.x, 0.001)
  end

  def test_y
    point = CvPoint3D32f.new
    point.y = 1.1
    assert_in_delta(1.1, point.y, 0.001)
    point.y = 2.2
    assert_in_delta(2.2, point.y, 0.001)
  end

  def test_z
    point = CvPoint3D32f.new
    point.z = 1.1
    assert_in_delta(1.1, point.z, 0.001)
    point.z = 2.2
    assert_in_delta(2.2, point.z, 0.001)
  end
  
  def test_compatible
    assert(!(CvPoint3D32f.compatible? MyPoint.new))
    MyPoint.class_eval { def x; end }
    assert(!(CvPoint3D32f.compatible? MyPoint.new))
    MyPoint.class_eval { def y; end }
    assert(!(CvPoint3D32f.compatible? MyPoint.new))
    MyPoint.class_eval { def z; end }
    assert(CvPoint3D32f.compatible? MyPoint.new)
    assert(CvPoint3D32f.compatible? CvPoint3D32f.new)
  end

  def test_initialize
    point = CvPoint3D32f.new
    assert_in_delta(0, point.x, 0.001)
    assert_in_delta(0, point.y, 0.001)
    assert_in_delta(0, point.z, 0.001)

    point = CvPoint3D32f.new(1.1, 2.2, 3.3)
    assert_in_delta(1.1, point.x, 0.001)
    assert_in_delta(2.2, point.y, 0.001)
    assert_in_delta(3.3, point.z, 0.001)

    point = CvPoint3D32f.new(CvPoint3D32f.new(1.1, 2.2, 3.3))
    assert_in_delta(1.1, point.x, 0.001)
    assert_in_delta(2.2, point.y, 0.001)
    assert_in_delta(3.3, point.z, 0.001)

    assert_raise(ArgumentError) {
      CvPoint3D32f.new('string')
    }
    assert_raise(ArgumentError) {
      CvPoint3D32f.new(1, 2)
    }
  end

  def test_to_s
    point = CvPoint3D32f.new(1.1, 2.2, 3.3)
    assert_equal('<OpenCV::CvPoint3D32f:(1.1,2.2,3.3)>', point.to_s)
  end

  def test_to_ary
    a = CvPoint3D32f.new(1.1, 2.2, 3.3).to_ary
    assert_in_delta(1.1, a[0], 0.001)
    assert_in_delta(2.2, a[1], 0.001)
    assert_in_delta(3.3, a[2], 0.001)

    # Alias
    a = CvPoint3D32f.new(1.1, 2.2, 3.3).to_a
    assert_in_delta(1.1, a[0], 0.001)
    assert_in_delta(2.2, a[1], 0.001)
    assert_in_delta(3.3, a[2], 0.001)
  end
end


