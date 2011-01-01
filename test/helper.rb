#!/usr/bin/env ruby
# -*- mode: ruby; coding: utf-8-unix -*- 
require 'test/unit'
require 'digest/md5'
require 'opencv'
include OpenCV

class OpenCVTestCase < Test::Unit::TestCase
  def get_sample(filename, iscolor = nil)
    IplImage::load('samples/' + filename, iscolor)
  end
  
  def snap(*images)
    win = []
    images.size.times { |i| win << GUI::Window.new("snap-#{i}") }
    win.each_with_index { |w, i| w.show images[i] }
    
    GUI::wait_key
    GUI::Window::destroy_all
  end

  def hash_img(img)
    # Compute a hash for an image, useful for image comparisons
    Digest::MD5.hexdigest(img.data)
  end

  alias original_assert_in_delta assert_in_delta
  # alias original_assert_equal assert_equal

  def assert_cvscalar_equal(expected, actual, message = nil)
    assert_equal(CvScalar, actual.class, message)
    assert_array_equal(expected.to_ary, actual.to_ary, message)
  end

  def assert_array_equal(expected, actual, message = nil)
    assert_equal(expected.size, actual.size, message)
    expected.zip(actual) { |e, a|
      assert_equal(e, a, message)
    }
  end
  
  def assert_in_delta(expected, actual, delta)
    if expected.is_a? CvScalar or actual.is_a? CvScalar
      expected = expected.to_ary if expected.is_a? CvScalar
      actual = actual.to_ary if actual.is_a? CvScalar
      assert_in_delta(expected, actual ,delta)
    elsif expected.is_a? Array and actual.is_a? Array
      assert_equal(expected.size, actual.size)
      expected.zip(actual) { |e, a|
        original_assert_in_delta(e, a, delta)
      }
    else
      original_assert_in_delta(expected, actual, delta)
    end
  end

  def create_cvmat(height, width, depth = :cv8u, channel = 4, &block)
    m = CvMat.new(height, width, depth, channel)
    block = lambda { |j, i, c| CvScalar.new(*([c + 1] * channel)) } unless block_given?
    count = 0
    height.times { |j|
      width.times { |i|
        m[i, j] = block.call(j, i, count)
        count += 1
      }
    }
    m
  end
end

