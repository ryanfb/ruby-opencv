#!/usr/bin/env ruby
# -*- mode: ruby; coding: utf-8-unix -*- 
require 'test/unit'
require 'opencv'
require File.expand_path(File.dirname(__FILE__)) + '/helper'

include OpenCV

# Tests for OpenCV::CvFont
class TestCvFont < OpenCVTestCase
  def test_FACE
    assert_equal(0, CvFont::FACE[:simplex])
    assert_equal(1, CvFont::FACE[:plain])
    assert_equal(2, CvFont::FACE[:duplex])
    assert_equal(4, CvFont::FACE[:triplex])
    assert_equal(5, CvFont::FACE[:complex_small])
    assert_equal(6, CvFont::FACE[:script_simplex])
    assert_equal(7, CvFont::FACE[:script_complex])
  end

  def test_FONT_OPTION
    assert_equal(1.0, CvFont::FONT_OPTION[:hscale])
    assert_equal(1.0, CvFont::FONT_OPTION[:vscale])
    assert_equal(0, CvFont::FONT_OPTION[:shear])
    assert_equal(1, CvFont::FONT_OPTION[:thickness])
    assert_equal(8, CvFont::FONT_OPTION[:line_type])
  end

  def test_initialize
    font = CvFont.new(:simplex)
    assert_equal(0, font.face)
    assert_equal(1.0, font.hscale)
    assert_equal(1.0, font.vscale)
    assert_equal(0, font.shear)
    assert_equal(1, font.thickness)
    assert_equal(8, font.line_type)
    assert(!font.italic)

    font = CvFont.new(:plain, :hscale => 2.5, :vscale => 3.5,
                      :shear => 0.5, :thickness => 3, :line_type => 2, :italic => false)
    assert_equal(1, font.face)
    assert_equal(2.5, font.hscale)
    assert_equal(3.5, font.vscale)
    assert_equal(0.5, font.shear)
    assert_equal(3, font.thickness)
    assert_equal(2, font.line_type)
    assert(!font.italic)

    font = CvFont.new(:simplex, :italic => true)
    assert_equal(16, font.face)
    assert(font.italic)

    assert_raise(ArgumentError) {
      CvFont.new(:foo)
    }
  end
end

