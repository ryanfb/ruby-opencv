#!/usr/bin/env ruby
# -*- mode: ruby; coding: utf-8-unix -*- 
require 'test/unit'
require 'opencv'
require File.expand_path(File.dirname(__FILE__)) + '/helper'

include OpenCV

# Tests for OpenCV::CvScalar
class TestCvScalar < OpenCVTestCase
  def test_initialize
    s = CvScalar.new
    assert_in_delta([0, 0, 0, 0], s, 0.01)

    s = CvScalar.new(1.1)
    assert_in_delta([1.1, 0, 0, 0], s, 0.01)

    s = CvScalar.new(1.1, 2.2)
    assert_in_delta([1.1, 2.2, 0, 0], s, 0.01)

    s = CvScalar.new(1.1, 2.2, 3.3)
    assert_in_delta([1.1, 2.2, 3.3, 0], s, 0.01)

    s = CvScalar.new(1.1, 2.2, 3.3, 4.4)
    assert_in_delta([1.1, 2.2, 3.3, 4.4], s, 0.01)
  end
  
  def test_aref
    assert_in_delta([0, 0, 0, 0], CvScalar.new, 0.01)
    assert_in_delta([10, 20, 30, 40], CvScalar.new(10, 20, 30, 40), 0.01)
    assert_in_delta([0.1, 0.2, 0.3, 0.4], CvScalar.new(0.1, 0.2, 0.3, 0.4), 0.01)
  end

  def test_aset
    s = CvScalar.new
    [10, 20, 30, 40].each_with_index { |x, i|
      s[i] = x
    }
    assert_in_delta([10, 20, 30, 40], s, 0.01)

    s = CvScalar.new
    [0.1, 0.2, 0.3, 0.4].each_with_index { |x, i|
      s[i] = x
    }
    assert_in_delta([0.1, 0.2, 0.3, 0.4], s, 0.01)
  end

  def test_sub
    s1 = CvScalar.new(10, 20, 30, 40)
    s2 = CvScalar.new(2, 4, 6, 8)
    [s1.sub(s2), s1 - s2].each { |s|
      assert_in_delta([8, 16, 24, 32], s, 0.01)
    }

    s3 = CvScalar.new(0.2, 0.4, 0.6, 0.8)
    [s2.sub(s3), s2 - s3].each { |s|
      assert_in_delta([1.8, 3.6, 5.4, 7.2], s, 0.01)
    }

    mat = CvMat.new(5, 5)
    mask = CvMat.new(5, 5, :cv8u, 1)
    mat.height.times { |j|
      mat.width.times { |i|
        mat[i, j] = CvScalar.new(1.5)
        mask[i, j] = (i < 2 and j < 3) ? 1 : 0
      }
    }
    mat = CvScalar.new(0.1).sub(mat, mask)

    [CvMat.new(5, 5, :cv16u, 1), CvMat.new(5, 5, :cv8u, 3)].each { |mask|
      assert_raise(TypeError) {
        CvScalar.new.sub(mat, mask)
      }
    }
  end

  def test_to_s
    assert_equal("<OpenCV::CvScalar:10,20,30,40>", CvScalar.new(10, 20, 30, 40).to_s)
    assert_equal("<OpenCV::CvScalar:0.1,0.2,0.3,0.4>", CvScalar.new(0.1, 0.2, 0.3, 0.4).to_s)
  end

  def test_to_ary
    [[10, 20, 30, 40], [0.1, 0.2, 0.3, 0.4]].each { |a|
      s = CvScalar.new(*a)
      b = s.to_ary
      assert_equal(Array, b.class)
      assert_in_delta(a, b, 0.01)
    }
  end

  def test_cvcolor
    assert_cvscalar_equal(CvColor::Black, CvScalar.new(0x0, 0x0, 0x0, 0))
    assert_cvscalar_equal(CvColor::Silver, CvScalar.new(0x0c, 0x0c, 0x0c, 0))
    assert_cvscalar_equal(CvColor::Gray, CvScalar.new(0x80, 0x80, 0x80, 0))
    assert_cvscalar_equal(CvColor::White, CvScalar.new(0xff, 0xff, 0xff, 0))
    assert_cvscalar_equal(CvColor::Maroon, CvScalar.new(0x0, 0x0, 0x80, 0))
    assert_cvscalar_equal(CvColor::Red, CvScalar.new(0x0, 0x0, 0xff, 0))
    assert_cvscalar_equal(CvColor::Purple, CvScalar.new(0x80, 0x0, 0x80, 0))
    assert_cvscalar_equal(CvColor::Fuchsia, CvScalar.new(0xff, 0x0, 0xff, 0))
    assert_cvscalar_equal(CvColor::Green, CvScalar.new(0x0, 0x80, 0x0, 0))
    assert_cvscalar_equal(CvColor::Lime, CvScalar.new(0x0, 0xff, 0x0, 0))
    assert_cvscalar_equal(CvColor::Olive, CvScalar.new(0x0, 0x80, 0x80, 0))
    assert_cvscalar_equal(CvColor::Yellow, CvScalar.new(0x0, 0xff, 0xff, 0))
    assert_cvscalar_equal(CvColor::Navy, CvScalar.new(0x80, 0x0, 0x0, 0))
    assert_cvscalar_equal(CvColor::Blue, CvScalar.new(0xff, 0x0, 0x0, 0))
    assert_cvscalar_equal(CvColor::Teal, CvScalar.new(0x80, 0x80, 0x0, 0))
    assert_cvscalar_equal(CvColor::Aqua, CvScalar.new(0xff, 0xff, 0x0, 0))
  end
end

