#!/usr/bin/env ruby
# -*- mode: ruby; coding: utf-8-unix -*- 
require 'test/unit'
require 'opencv'
require File.expand_path(File.dirname(__FILE__)) + '/helper'

include OpenCV

# Tests for OpenCV::CvSeq
class TestCvSeq < OpenCVTestCase
  def test_initialize
    assert_not_nil(CvSeq.new(CvIndex))
    assert_not_nil(CvSeq.new(CvPoint))
    assert_not_nil(CvSeq.new(CvPoint2D32f))
    assert_not_nil(CvSeq.new(CvPoint3D32f))
  end

  def test_total
    seq1 = CvSeq.new(CvPoint)
    assert_equal(0, seq1.total)

    seq1.push(CvPoint.new(1, 2))
    assert_equal(1, seq1.total)

    seq1.push(CvPoint.new(3, 4))
    assert_equal(2, seq1.total)
    # Alias
    assert_equal(2, seq1.length)
    assert_equal(2, seq1.size)
  end

  def test_empty
    assert(CvSeq.new(CvPoint).empty?)
    assert(CvSeq.new(CvPoint2D32f).empty?)
    assert(CvSeq.new(CvPoint3D32f).empty?)
  end
  
  def test_aref
    seq1 = CvSeq.new(CvPoint).push(CvPoint.new(10, 20), CvPoint.new(30, 40), CvPoint.new(50, 60))

    assert_equal(CvPoint, seq1[0].class)
    assert_equal(10, seq1[0].x)
    assert_equal(20, seq1[0].y)
    assert_equal(30, seq1[1].x)
    assert_equal(40, seq1[1].y)
    assert_equal(50, seq1[2].x)
    assert_equal(60, seq1[2].y)
  end

  def test_push
    seq1 = CvSeq.new(CvPoint).push(CvPoint.new(10, 20), CvPoint.new(30, 40))
    
    assert_equal(2, seq1.total)
    assert_equal(CvPoint, seq1[0].class)
    assert_equal(10, seq1[0].x)
    assert_equal(20, seq1[0].y)
    assert_equal(CvPoint, seq1[1].class)
    assert_equal(30, seq1[1].x)
    assert_equal(40, seq1[1].y)

    seq2 = CvSeq.new(CvPoint).push(CvPoint.new(50, 60))
    seq2.push(seq1)
    assert_equal(3, seq2.total)
    assert_equal(CvPoint, seq2[0].class)
    assert_equal(50, seq2[0].x)
    assert_equal(60, seq2[0].y)
    assert_equal(CvPoint, seq2[1].class)
    assert_equal(10, seq2[1].x)
    assert_equal(20, seq2[1].y)
    assert_equal(CvPoint, seq2[2].class)
    assert_equal(30, seq2[2].x)
    assert_equal(40, seq2[2].y)

    assert_raise(TypeError) {
      seq1.push(CvPoint2D32f.new(55.5, 66.6))
    }

    assert_raise(TypeError) {
      seq3 = CvSeq.new(CvPoint2D32f).push(CvPoint2D32f.new(55.5, 66.6))
      seq1.push(seq3)
    }
  end

  def test_pop
    seq1 = CvSeq.new(CvPoint).push(CvPoint.new(10, 20), CvPoint.new(30, 40))
    point1 = seq1.pop
    assert_equal(CvPoint, point1.class)
    assert_equal(30, point1.x)
    assert_equal(40, point1.y)

    assert_equal(1, seq1.total)
    assert_equal(CvPoint, seq1[0].class)
    assert_equal(10, seq1[0].x)
    assert_equal(20, seq1[0].y)

    assert_nil(CvSeq.new(CvPoint).pop)
  end

  def test_clear
    seq1 = CvSeq.new(CvPoint).push(CvPoint.new(10, 20), CvPoint.new(30, 40))
    seq1.clear
    assert_not_nil(seq1)
    assert_equal(0, seq1.total)
  end

  def test_unshift
    seq1 = CvSeq.new(CvPoint).unshift(CvPoint.new(10, 20), CvPoint.new(30, 40))
    
    assert_equal(2, seq1.total)
    assert_equal(CvPoint, seq1[0].class)
    assert_equal(30, seq1[0].x)
    assert_equal(40, seq1[0].y)
    assert_equal(CvPoint, seq1[1].class)
    assert_equal(10, seq1[1].x)
    assert_equal(20, seq1[1].y)

    seq2 = CvSeq.new(CvPoint).unshift(CvPoint.new(50, 60))
    seq2.unshift(seq1)
    assert_equal(3, seq2.total)
    assert_equal(CvPoint, seq1[0].class)
    assert_equal(30, seq1[0].x)
    assert_equal(40, seq1[0].y)
    assert_equal(CvPoint, seq1[1].class)
    assert_equal(10, seq1[1].x)
    assert_equal(20, seq1[1].y)
    assert_equal(CvPoint, seq2[2].class)
    assert_equal(50, seq2[2].x)
    assert_equal(60, seq2[2].y)

    assert_raise(TypeError) {
      seq1.unshift(CvPoint2D32f.new(55.5, 66.6))
    }

    assert_raise(TypeError) {
      seq3 = CvSeq.new(CvPoint2D32f).push(CvPoint2D32f.new(55.5, 66.6))
      seq1.unshift(seq3)
    }
  end

  def test_shift
    seq1 = CvSeq.new(CvPoint).push(CvPoint.new(10, 20), CvPoint.new(30, 40))
    point1 = seq1.shift
    assert_equal(CvPoint, point1.class)
    assert_equal(10, point1.x)
    assert_equal(20, point1.y)

    assert_equal(1, seq1.total)
    assert_equal(CvPoint, seq1[0].class)
    assert_equal(30, seq1[0].x)
    assert_equal(40, seq1[0].y)

    assert_nil(CvSeq.new(CvPoint).shift)
  end

  def test_first
    seq1 = CvSeq.new(CvPoint).push(CvPoint.new(10, 20), CvPoint.new(30, 40), CvPoint.new(50, 60))
    point1 = seq1.first
    assert_equal(CvPoint, point1.class)
    assert_equal(10, point1.x)
    assert_equal(20, point1.y)
  end

  def test_last
    seq1 = CvSeq.new(CvPoint).push(CvPoint.new(10, 20), CvPoint.new(30, 40), CvPoint.new(50, 60))
    point1 = seq1.last
    assert_equal(CvPoint, point1.class)
    assert_equal(50, point1.x)
    assert_equal(60, point1.y)
  end

  def test_each
    seq1 = CvSeq.new(CvPoint).push(CvPoint.new(10, 20), CvPoint.new(30, 40), CvPoint.new(50, 60))
    i = 0
    seq1.each { |s|
      assert_equal(CvPoint, s.class)
      assert_equal(seq1[i].x, s.x)
      assert_equal(seq1[i].y, s.y)
      i += 1
    }
    assert_equal(3, i)
  end

  def test_each_index
    seq1 = CvSeq.new(CvPoint).push(CvPoint.new(10, 20), CvPoint.new(30, 40), CvPoint.new(50, 60))
    n = 0
    seq1.each_index { |i|
      assert_equal(n, i)
      n += 1
    }
    assert_equal(3, n)
  end

  def test_insert
    seq1 = CvSeq.new(CvPoint).push(CvPoint.new(10, 20), CvPoint.new(30, 40))

    seq1.insert(1, CvPoint.new(50, 60))
    assert_equal(3, seq1.total)
    assert_equal(CvPoint, seq1[0].class)
    assert_equal(10, seq1[0].x)
    assert_equal(20, seq1[0].y)
    assert_equal(CvPoint, seq1[1].class)
    assert_equal(50, seq1[1].x)
    assert_equal(60, seq1[1].y)
    assert_equal(CvPoint, seq1[2].class)
    assert_equal(30, seq1[2].x)
    assert_equal(40, seq1[2].y)
  end

  def test_remove
    seq1 = CvSeq.new(CvPoint).push(CvPoint.new(10, 20), CvPoint.new(30, 40), CvPoint.new(50, 60))

    seq1.remove(1)
    assert_equal(2, seq1.total)
    assert_equal(CvPoint, seq1[0].class)
    assert_equal(10, seq1[0].x)
    assert_equal(20, seq1[0].y)
    assert_equal(CvPoint, seq1[1].class)
    assert_equal(50, seq1[1].x)
    assert_equal(60, seq1[1].y)
  end

  def test_h_prev
    flunk('FIXME: CvSeq#h_prev is not tested yet.')
  end

  def test_h_next
    flunk('FIXME: CvSeq#h_next is not tested yet.')
  end

  def test_v_prev
    flunk('FIXME: CvSeq#v_prev is not tested yet.')
  end

  def test_v_next
    flunk('FIXME: CvSeq#v_next is not tested yet.')
  end
end

