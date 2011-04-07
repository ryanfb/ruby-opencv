#!/usr/bin/env ruby
# -*- mode: ruby; coding: utf-8-unix -*- 
require 'test/unit'
require 'opencv'
require File.expand_path(File.dirname(__FILE__)) + '/helper'

include OpenCV

# Tests for OpenCV::IplImage
class TestIplImage < OpenCVTestCase
  def should_classify_images_as(filename, classification)
    assert_equal(OpenCV::IplImage::load(filename, OpenCV::CV_LOAD_IMAGE_GRAYSCALE).smoothness[0], classification)
  end

  def test_smoothness
    asset_path = File.join(File.dirname(__FILE__), 'samples')

    for image in Array.new(9) { |e| e = File.join(asset_path, "smooth%d.jpg") % e } do
      should_classify_images_as image, :smooth
    end

    for image in Array.new(2) { |e| e = File.join(asset_path, "messy%d.jpg") % e } do
      should_classify_images_as image, :messy
    end

    for image in Array.new(10) { |e| e = File.join(asset_path, "blank%d.jpg") % e } do
      should_classify_images_as image, :blank
    end

    for image in Array.new(2) { |e| e = File.join(asset_path, "partially_blank%d.jpg") % e } do
      should_classify_images_as image, :blank
    end
  end
end
