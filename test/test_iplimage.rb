#!/usr/bin/env ruby
# -*- mode: ruby; coding: utf-8-unix -*- 
require 'test/unit'
require 'opencv'
require File.expand_path(File.dirname(__FILE__)) + '/helper'

include OpenCV

# Tests for OpenCV::IplImage
class TestIplImage < OpenCVTestCase
  def test_initialize
    img = IplImage.new(10, 20)
    assert_equal(10, img.width)
    assert_equal(20, img.height)
    assert_equal(:cv8u, img.depth)
    assert_equal(3, img.channel)

    img = IplImage.new(30, 40, :cv32f, 1)
    assert_equal(30, img.width)
    assert_equal(40, img.height)
    assert_equal(:cv32f, img.depth)
    assert_equal(1, img.channel)
  end

  def test_load
    img = IplImage.load(FILENAME_CAT)
    assert_equal(IplImage, img.class)
    assert_equal(375, img.width)
    assert_equal(500, img.height)
    assert_equal(:cv8u, img.depth)
    assert_equal(3, img.channel)
    assert_equal('f2e4dc5d6d3fc285203762ff53d150c7', hash_img(img))

    img = IplImage.load(FILENAME_CAT, CV_LOAD_IMAGE_GRAYSCALE)
    assert_equal(IplImage, img.class)
    assert_equal(375, img.width)
    assert_equal(500, img.height)
    assert_equal(:cv8u, img.depth)
    assert_equal(1, img.channel)
    # The following test fails only when executed by test runner. (I don't know why...)
    #   $ ruby test/runner.rb #=> fail
    #   $ ruby test/test_iplimage.rb #=> pass
    assert_equal('b1a0c1c5504961b62e15fa7d57a2e7e0', hash_img(img))

    img = IplImage.load(FILENAME_CAT, CV_LOAD_IMAGE_ANYDEPTH | CV_LOAD_IMAGE_ANYCOLOR)
    assert_equal(IplImage, img.class)
    assert_equal(375, img.width)
    assert_equal(500, img.height)
    assert_equal(:cv8u, img.depth)
    assert_equal(3, img.channel)
    assert_equal('f2e4dc5d6d3fc285203762ff53d150c7', hash_img(img))

    assert_raise(ArgumentError) {
      IplImage.load
    }
    assert_raise(TypeError) {
      IplImage.load(123)
    }
    assert_raise(TypeError) {
      IplImage.load(FILENAME_CAT, 'foobar')
    }
    assert_raise(StandardError) {
      IplImage.load('file/does/not/exist')
    }
  end

  def test_roi
    img = IplImage.new(20, 30)
    rect = img.roi
    assert_equal(0, rect.x)
    assert_equal(0, rect.y)
    assert_equal(img.width, rect.width)
    assert_equal(img.height, rect.height)

    img.set_roi(CvRect.new(2, 3, 10, 20))
    rect = img.roi
    assert_equal(2, rect.x)
    assert_equal(3, rect.y)
    assert_equal(10, rect.width)
    assert_equal(20, rect.height)

    img.reset_roi
    rect = img.roi
    assert_equal(0, rect.x)
    assert_equal(0, rect.y)
    assert_equal(img.width, rect.width)
    assert_equal(img.height, rect.height)

    img.set_roi(CvRect.new(1, 2, 5, 6)) {|image|
      rect = image.roi
      assert_equal(1, rect.x)
      assert_equal(2, rect.y)
      assert_equal(5, rect.width)
      assert_equal(6, rect.height)
    }
    rect = img.roi
    assert_equal(0, rect.x)
    assert_equal(0, rect.y)
    assert_equal(img.width, rect.width)
    assert_equal(img.height, rect.height)

    # Alias
    img.roi = CvRect.new(4, 5, 11, 12)
    rect = img.roi
    assert_equal(4, rect.x)
    assert_equal(5, rect.y)
    assert_equal(11, rect.width)
    assert_equal(12, rect.height)
  end

  def test_coi
    img = IplImage.new(20, 30)
    assert_equal(0, img.coi)

    img.set_coi(1)
    assert_equal(1, img.coi)

    img.reset_coi
    assert_equal(0, img.coi)

    img.set_coi(2) {|image|
      assert_equal(2, image.coi)
    }
    assert_equal(0, img.coi)

    # Alias
    img.coi = 1
    assert_equal(1, img.coi)
  end
end


