#!/usr/bin/env ruby
# -*- mode: ruby; coding: utf-8-unix -*- 
require 'test/unit'
require 'opencv'
require File.expand_path(File.dirname(__FILE__)) + '/test_opencv'

include OpenCV

# Tests for OpenCV::CvScalar
class TestCvScalar < TestOpenCV
  def test_aref
    s = CvScalar.new
    4.times { |i| assert_in_delta(0, s[i], 0.01) }

    s = CvScalar.new(10, 20, 30, 40)
    [10, 20, 30, 40].each_with_index { |x, i|
      assert_in_delta(x, s[i], 0.01)
    }
  end

  def test_aset
    s = CvScalar.new
    [10, 20, 30, 40].each_with_index { |x, i|
      s[i] = x
    }
    [10, 20, 30, 40].each_with_index { |x, i|
      assert_in_delta(x, s[i], 0.01)
    }
  end

  def test_sub
    s1 = CvScalar.new(10, 20, 30, 40)
    s2 = CvScalar.new(2, 4, 6, 8)
    [s1.sub(s2), s1 - s2].each { |s|
      assert_in_delta(8, s[0], 0.01)
      assert_in_delta(16, s[1], 0.01)
      assert_in_delta(24, s[2], 0.01)
      assert_in_delta(32, s[3], 0.01)
    }
  end

  def test_to_s
    assert_equal("<OpenCV::CvScalar:10,20,30,40>", CvScalar.new(10, 20, 30, 40).to_s)
  end

  def test_to_ary
    a = [10, 20, 30, 40]
    s = CvScalar.new(*a)
    
    b = s.to_ary
    assert_equal(Array, b.class)
    a.each_with_index { |x, i|
      assert_in_delta(x, b[i], 0.01)
    }
  end
end

