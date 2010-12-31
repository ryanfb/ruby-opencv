#!/usr/bin/env ruby
#  -*- mode: ruby; coding: utf-8-unix -*- 

require 'test/unit'
require 'opencv'
require 'pp'
require File.expand_path(File.dirname(__FILE__)) + '/test_opencv'

include OpenCV

# Tests for OpenCV::CvMat
class TestCvMat < TestOpenCV
  def test_avg_sdv
    m = CvMat.new(1, 8, CV_32F)
    [2, 4, 4, 4, 5, 5, 7, 9].each_with_index { |v, i|
      # m[i] = CvScalar.new(v, 0, 0, 0)
      m[i][0] = v
    }
    avg = m.avg
    assert_in_delta(avg[0], 5.0, 0.01)
    avg, sdv = m.avg_sdv
    assert_in_delta(avg[0], 5.0, 0.01)
    assert_in_delta(sdv[0], 2.0, 0.01)
  end

end



