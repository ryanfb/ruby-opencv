#!/usr/bin/env ruby
# -*- mode: ruby; coding: utf-8-unix -*- 
require 'test/unit'
require 'opencv'
require File.expand_path(File.dirname(__FILE__)) + '/helper'

include OpenCV

# Tests for OpenCV::CvTwoPoints
class TestCvTwoPoints < OpenCVTestCase
  def setup
    @twopoints = CvTwoPoints.new
  end
  
  def test_initialize
    assert_not_nil(@twopoints)
    assert_equal(CvTwoPoints, @twopoints.class)
  end

  def test_point
    assert_not_nil(@twopoints.point1)
    assert_not_nil(@twopoints.point2)
    assert_equal(CvPoint, @twopoints.point1.class)
    assert_equal(CvPoint, @twopoints.point2.class)
  end

  def test_aref
    assert_not_nil(@twopoints[0])
    assert_not_nil(@twopoints[1])
    assert_equal(CvPoint, @twopoints[0].class)
    assert_equal(CvPoint, @twopoints[1].class)
  end

  def test_to_ary
    assert_equal(Array, @twopoints.to_ary.class)
    assert_equal(2, @twopoints.to_ary.size)
    assert_equal(2, @twopoints.to_a.size)
  end
end

