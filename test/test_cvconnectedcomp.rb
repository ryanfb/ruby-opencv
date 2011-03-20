#!/usr/bin/env ruby
# -*- mode: ruby; coding: utf-8-unix -*- 
require 'test/unit'
require 'opencv'
require File.expand_path(File.dirname(__FILE__)) + '/helper'

include OpenCV

# Tests for OpenCV::CvConnectedComp
class TestCvConnectedComp < OpenCVTestCase
  def setup
    mat0 = create_cvmat(128, 128, :cv8u, 1) { |j, i, c|
      if (i >= 32 and i < 96) and (j >= 32 and j < 96)
        CvScalar.new(255)
      elsif (i >= 16 and i < 112) and (j >= 16 and j < 112)
        CvScalar.new(192)
      else
        CvScalar.new(128)
      end
    }

    point = CvPoint.new(20, 20)
    @mat, @connected_comp, @mask = mat0.flood_fill(point, 0, CvScalar.new(0), CvScalar.new(64),
                                                   :connectivity => 8, :fixed_range => true, :mask_only => true)
  end
  
  def test_initialize
    connected_comp = CvConnectedComp.new
    assert_not_nil(connected_comp)
    assert_equal(CvConnectedComp, connected_comp.class)
    assert_not_nil(connected_comp.area)
    assert_not_nil(connected_comp.value)
    assert_not_nil(connected_comp.rect)
    assert_not_nil(connected_comp.contour)
  end

  def test_area
    assert_in_delta(9216.0, @connected_comp.area, 0.01)
  end

  def test_value
    assert_equal(CvScalar, @connected_comp.value.class)
    assert_cvscalar_equal(CvScalar.new(220, 0, 0, 0), @connected_comp.value)
  end

  def test_rect
    assert_equal(CvRect, @connected_comp.rect.class)
    assert_equal(16, @connected_comp.rect.x)
    assert_equal(16, @connected_comp.rect.y)
    assert_equal(96, @connected_comp.rect.width)
    assert_equal(96, @connected_comp.rect.height)

    @connected_comp.rect = CvRect.new(1, 2, 3, 4);
    assert_equal(1, @connected_comp.rect.x)
    assert_equal(2, @connected_comp.rect.y)
    assert_equal(3, @connected_comp.rect.width)
    assert_equal(4, @connected_comp.rect.height)
  end

  def test_contour
    assert_equal(CvContour, @connected_comp.contour.class)
    assert_not_nil(@connected_comp.contour)
  end
end

