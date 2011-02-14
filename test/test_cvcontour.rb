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

  def test_approx_poly
    flunk('FIXME: CvContour#approx_poly is not implemented yet.')
  end
  
  def test_bounding_rect
    mat0 = create_cvmat(128, 128, :cv8u, 1) { |j, i|
      (j - 64) ** 2 + (i - 64) ** 2 <= (32 ** 2) ? CvColor::White : CvColor::Black
    }
    contours = mat0.find_contours
    rect = contours.bounding_rect
    assert_equal(CvRect, rect.class)
    assert_equal(32, rect.x)
    assert_equal(32, rect.y)
    assert_equal(65, rect.width)
    assert_equal(65, rect.height)
  end

  def test_create_tree
    flunk('FIXME: CvContour#create_tree is not tested yet.')
  end

  def test_in
    mat0 = create_cvmat(128, 128, :cv8u, 1) { |j, i|
      (j - 64) ** 2 + (i - 64) ** 2 <= (32 ** 2) ? CvColor::White : CvColor::Black
    }
    contour = mat0.find_contours
    assert(contour.in? CvPoint.new(64, 64))
    assert_false(contour.in? CvPoint.new(0, 0))
    assert_nil(contour.in? CvPoint.new(64, 32))
  end

  def test_measure_distance
    mat0 = create_cvmat(128, 128, :cv8u, 1) { |j, i|
      (j - 64) ** 2 + (i - 64) ** 2 <= (32 ** 2) ? CvColor::White : CvColor::Black
    }
    contour = mat0.find_contours
    assert_in_delta(-0.7071, contour.measure_distance(CvPoint.new(63, 32)), 0.01)
    assert_in_delta(31.01, contour.measure_distance(CvPoint.new(64, 64)), 0.01)
  end

  def test_point_polygon_test
    mat0 = create_cvmat(128, 128, :cv8u, 1) { |j, i|
      (j - 64) ** 2 + (i - 64) ** 2 <= (32 ** 2) ? CvColor::White : CvColor::Black
    }
    contour = mat0.find_contours

    assert_equal(1, contour.point_polygon_test(CvPoint.new(64, 64), 0))
    assert_equal(1, contour.point_polygon_test(CvPoint.new(64, 64), false))
    assert_equal(-1, contour.point_polygon_test(CvPoint.new(0, 0), 0))
    assert_equal(-1, contour.point_polygon_test(CvPoint.new(0, 0), false))
    assert_equal(0, contour.point_polygon_test(CvPoint.new(64, 32), 0))
    assert_equal(0, contour.point_polygon_test(CvPoint.new(64, 32), false))

    assert_in_delta(-0.7071, contour.point_polygon_test(CvPoint.new(63, 32), 1), 0.01)
    assert_in_delta(-0.7071, contour.point_polygon_test(CvPoint.new(63, 32), true), 0.01)
    assert_in_delta(31.01, contour.point_polygon_test(CvPoint.new(64, 64), 1), 0.01)
    assert_in_delta(31.01, contour.point_polygon_test(CvPoint.new(64, 64), true), 0.01)
  end
end

