#!/usr/bin/env ruby
# -*- mode: ruby; coding: utf-8-unix -*- 
require 'test/unit'
require 'opencv'
require File.expand_path(File.dirname(__FILE__)) + '/helper'

include OpenCV

# Tests for OpenCV::CvContour
class TestCvContour < OpenCVTestCase
  def test_initialize
    contour = CvContour.new
    assert_not_nil(contour)
    assert_equal(CvContour, contour.class)
    assert(contour.is_a? CvSeq)
  end

  def test_rect
    contour = CvContour.new
    assert_not_nil(contour.rect)
    assert_equal(CvRect, contour.rect.class)
  end

  def test_color
    contour = CvContour.new
    assert_equal(0, contour.color)
    contour.color = 1
    assert_equal(1, contour.color)
  end

  def test_reserved
    reserved = CvContour.new.reserved
    assert_equal(Array, reserved.class)
    assert_equal(3, reserved.size)
  end
end

