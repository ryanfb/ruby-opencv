#!/usr/bin/env ruby
#  -*- mode: ruby; coding: utf-8-unix -*- 

require 'test/unit'
require 'opencv'
require 'pp'
require File.expand_path(File.dirname(__FILE__)) + '/test_opencv'

include OpenCV

# Tests for OpenCV::CvMat
class TestCvMat < TestOpenCV
  def test_DRAWING_OPTION
    CvMat::DRAWING_OPTION[:color].to_ary.each { |c|
      assert_in_delta(0, c, 0.01)
    }
    assert_equal(1, CvMat::DRAWING_OPTION[:thickness])
    assert_equal(8, CvMat::DRAWING_OPTION[:line_type])
    assert_equal(0, CvMat::DRAWING_OPTION[:shift])
  end

  def test_GOOD_FEATURES_TO_TRACK_OPTION
    assert_equal(0xff, CvMat::GOOD_FEATURES_TO_TRACK_OPTION[:max])
    assert_nil(CvMat::GOOD_FEATURES_TO_TRACK_OPTION[:mask])
    assert_equal(3, CvMat::GOOD_FEATURES_TO_TRACK_OPTION[:block_size])
    assert((not CvMat::GOOD_FEATURES_TO_TRACK_OPTION[:use_harris]))
    assert_in_delta(0.04, CvMat::GOOD_FEATURES_TO_TRACK_OPTION[:k], 0.01)
  end

  def test_FLOOD_FILL_OPTION
    assert_equal(4, CvMat::FLOOD_FILL_OPTION[:connectivity])
    assert((not CvMat::FLOOD_FILL_OPTION[:fixed_range]))
    assert((not CvMat::FLOOD_FILL_OPTION[:mask_only]))
  end

  def test_FIND_CONTOURS_OPTION
    assert_equal(1, CvMat::FIND_CONTOURS_OPTION[:mode])
    assert_equal(2, CvMat::FIND_CONTOURS_OPTION[:method])
    assert_equal(0, CvMat::FIND_CONTOURS_OPTION[:offset].x)
    assert_equal(0, CvMat::FIND_CONTOURS_OPTION[:offset].y)
  end

  def test_OPTICAL_FLOW_HS_OPTION
    assert_in_delta(0.0005, CvMat::OPTICAL_FLOW_HS_OPTION[:lambda], 0.000001)
    assert_equal(1, CvMat::OPTICAL_FLOW_HS_OPTION[:criteria].max)
    assert_in_delta(0.001, CvMat::OPTICAL_FLOW_HS_OPTION[:criteria].eps, 0.00001)
  end

  def test_OPTICAL_FLOW_BM_OPTION
    assert_equal(4, CvMat::OPTICAL_FLOW_BM_OPTION[:block_size].width)
    assert_equal(4, CvMat::OPTICAL_FLOW_BM_OPTION[:block_size].height)
    assert_equal(1, CvMat::OPTICAL_FLOW_BM_OPTION[:shift_size].width)
    assert_equal(1, CvMat::OPTICAL_FLOW_BM_OPTION[:shift_size].height)
    assert_equal(4, CvMat::OPTICAL_FLOW_BM_OPTION[:max_range].width)
    assert_equal(4, CvMat::OPTICAL_FLOW_BM_OPTION[:max_range].height)
  end
  
  def test_FIND_FUNDAMENTAL_MAT_OPTION
    assert((not CvMat::FIND_FUNDAMENTAL_MAT_OPTION[:with_status]))
    assert_in_delta(1.0, CvMat::FIND_FUNDAMENTAL_MAT_OPTION[:maximum_distance], 0.01)
    assert_in_delta(0.99, CvMat::FIND_FUNDAMENTAL_MAT_OPTION[:desirable_level], 0.01)
  end

  def test_avg_sdv
    m = CvMat.new(1, 8, CV_32F)
    [2, 4, 4, 4, 5, 5, 7, 9].each_with_index { |v, i|
      m[i] = CvScalar.new(v, 0, 0, 0)
      # m[i][0] = v
    }
    avg = m.avg
    assert_in_delta(avg[0], 5.0, 0.01)
    avg, sdv = m.avg_sdv
    assert_in_delta(avg[0], 5.0, 0.01)
    assert_in_delta(sdv[0], 2.0, 0.01)
  end

end



