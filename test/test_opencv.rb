#!/usr/bin/env ruby
#  -*- mode: ruby; coding: utf-8-unix -*- 

require 'test/unit'
require 'digest/md5'
require 'opencv'

include OpenCV

class TestOpenCV < Test::Unit::TestCase
  # mat_types = [CV_8UC1, CV_8UC2, CV_8UC3, CV_8UC4,
  #              CV_8SC1, CV_8SC2, CV_8SC3,  CV_8SC4,
  #              CV_16UC1, CV_16UC2, CV_16UC3, CV_16UC4,
  #              CV_16SC1, CV_16SC2, CV_16SC3, CV_16SC4,
  #              CV_32SC1, CV_32SC2, CV_32SC3, CV_32SC4,
  #              CV_32FC1, CV_32FC2, CV_32FC3, CV_32FC4,
  #              CV_64FC1, CV_64FC2, CV_64FC3, CV_64FC4]

  # mat_types_single = [CV_8UC1, CV_8SC1,
  #                     CV_16UC1, CV_16SC1,
  #                     CV_32SC1, CV_32FC1,
  #                     CV_64FC1]

  def setup
    @depths = [CV_8U, CV_8S,
               CV_16U, CV_16S,
               CV_32S, CV_32F,
               CV_64F]
    @depthsize = {
      CV_8U => 1,
      CV_8S => 1,
      CV_16U => 2,
      CV_16S => 2,
      CV_32S => 4,
      CV_32F => 4,
      CV_64F => 8
    }
  end

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
end


