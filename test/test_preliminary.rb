#!/usr/bin/env ruby
#  -*- mode: ruby; coding: utf-8-unix -*- 

require 'test/unit'
require 'digest/md5'
require 'opencv'
require 'pp'
require_relative 'test_opencv'

include OpenCV

# Tests to run first; check the handful of basic operations that the later tests rely on
class TestPreliminary < TestOpenCV
  def test_lena
    # Check that the lena jpg image has loaded correctly
    img = get_sample('lena.jpg', false)
    # snap(img)     # uncomment this line to view the image, when regilding
    assert_equal('2980cef5ac9bc061a5ab3f04775f3cf0', hash_img(img))    
  end

  def test_load
    assert_raise(ArgumentError) { IplImage::load }
    assert_raise(TypeError) { IplImage::load(4) }
    assert_raise(ArgumentError) { IplImage::load('foo.jpg', 1, 1) }
    assert_raise(StandardError) { IplImage::load('foo.jpg', 'foobar') }
  end

  def test_types
    assert_equal(IplImage.new(7, 5, CV_8U, 1).class, IplImage)
    assert_equal(CvMat.new(5, 7, CV_32F).class, CvMat)
  end

  def test_to_s
    [1, 4, 64, 512, 640].each { |w|
      [1, 4, 64, 480, 512].each { |h|
        [1, 2, 3, 4].each { |c|
          @depths.each { |d|
            expected_size = w * h * c * @depthsize[d]

            mat = CvMat.new(w, h, d, c)
            assert_equal(expected_size, mat.data.size)

            # img = IplImage.new(w, h, d, c)
            # expected_size += 4 - (expected_size % 4) unless (expected_size % 4) == 0
            # assert_equal(expected_size, img.data.to_s.size)
          }
        }
      }
    }
  end
end


