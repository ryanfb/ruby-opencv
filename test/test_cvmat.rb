#!/usr/bin/env ruby
#  -*- mode: ruby; coding: utf-8-unix -*- 

require 'test/unit'
require 'opencv'
require 'pp'
require File.expand_path(File.dirname(__FILE__)) + '/test_opencv'

include OpenCV

# Tests for OpenCV::CvMat
class TestCvMat < TestOpenCV
  def make_cvmat(h, w, depth = :cv8u, channel = 4)
    m = CvMat.new(h, w, depth, channel)
    count = 1
    h.times { |j|
      w.times { |i|
        m[i, j] = CvScalar.new(*([count] * channel))
        count += 1
      }
    }
    m
  end
  
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

  def test_to_s
    m = CvMat.new(10, 20)
    assert_equal('<OpenCV::CvMat:20x10,depth=cv8u,channel=3>', m.to_s)
    m = CvMat.new(10, 20, :cv16s)
    assert_equal('<OpenCV::CvMat:20x10,depth=cv16s,channel=3>', m.to_s)
    m = CvMat.new(10, 20, :cv32f, 1)
    assert_equal('<OpenCV::CvMat:20x10,depth=cv32f,channel=1>', m.to_s)
  end

  def test_parent
    m1 = CvMat.new(10, 20)
    assert((not m1.has_parent?))
    assert_nil(m1.parent)

    flunk('FIXME: resolve unexpected ABORT of CvMat#to_CvMat')
    # m2 = m1.to_CvMat
    # assert(m2.has_parent?)
    # assert_same(m1, m2.parent)
  end

  def test_inside
    m = CvMat.new(20, 10)
    assert(m.inside? CvPoint.new(0, 0))
    assert(m.inside? CvPoint.new(9, 19))
    assert((not m.inside? CvPoint.new(10, 0)))
    assert((not m.inside? CvPoint.new(0, 20)))
    assert((not m.inside? CvPoint.new(10, 20)))
  end

  def test_to_IplConvKernel
    kernel = CvMat.new(10, 20).to_IplConvKernel(CvPoint.new(2, 3))
    assert_equal(10, kernel.rows)
    assert_equal(20, kernel.cols)
    assert_equal(2, kernel.anchor.x)
    assert_equal(3, kernel.anchor.y)
    assert_equal(2, kernel.anchor_x)
    assert_equal(3, kernel.anchor_y)
  end

  def test_create_mask
    mask = CvMat.new(10, 20).create_mask
    assert_equal(20, mask.width)
    assert_equal(10, mask.height)
    assert_equal(:cv8u, mask.depth)
    assert_equal(1, mask.channel)
  end

  def test_fields
    m = CvMat.new(20, 10)
    assert_equal(10, m.width)
    assert_equal(10, m.columns)
    assert_equal(10, m.cols)
    assert_equal(20, m.height)
    assert_equal(20, m.rows)
    assert_equal(:cv8u, m.depth)
    assert_equal(3, m.channel)

    m = CvMat.new(20, 10, :cv16s, 1)
    assert_equal(10, m.width)
    assert_equal(10, m.columns)
    assert_equal(10, m.cols)
    assert_equal(20, m.height)
    assert_equal(20, m.rows)
    assert_equal(:cv16s, m.depth)
    assert_equal(1, m.channel)
  end

  def test_clone
    m1 = make_cvmat(10, 20)
    m2 = m1.clone
    assert_equal(m1.data, m2.data)
  end

  def test_copy
    m1 = make_cvmat(10, 20, CV_32F, 1)

    m2 = m1.copy
    assert_equal(m1.data, m2.data)
    
    m2 = CvMat.new(10, 20, CV_32F, 1)
    m1.copy(m2)
    m1.height.times { |j|
      m1.width.times { |i|
        assert(is_same_float_array(m1[i, j].to_ary, m2[i, j].to_ary))
      }
    }

    assert_equal(m1.data, m2.data)

    flunk('FIXME: CvUnmatchedSizes and CvUnmatchedFormats are not implemented yet')
    m2 = CvMat.new(1, 2, CV_32F, 1)
    assert_raise(CvUnmatchedSizes) {
      m1.copy(m2)
    }

    m2 = CvMat.new(10, 20, CV_32F, 3)
    assert_raise(CvUnmatchedFormats) {
      m1.copy(m2)
    }
    m2 = CvMat.new(10, 20, CV_8U, 1)
    assert_raise(CvUnmatchedFormats) {
      m1.copy(m2)
    }
    
    a = m1.copy(2)
    assert_equal(2, a.size)
    a.each { |m|
      m1.height.times { |j|
        m1.width.times { |i|
          assert(is_same_float_array(m1[i, j].to_ary, m[i, j].to_ary))
        }
      }
    }
    assert_nil(m1.copy(-1))
  end

  def test_convert_depth
    m = CvMat.new(10, 20, :cv32f)
    assert_equal(:cv8u, m.to_8u.depth)
    assert_equal(:cv8s, m.to_8s.depth)
    assert_equal(:cv16u, m.to_16u.depth)
    assert_equal(:cv16s, m.to_16s.depth)
    assert_equal(:cv32s, m.to_32s.depth)
    assert_equal(:cv32f, m.to_32f.depth)
    assert_equal(:cv64f, m.to_64f.depth)
  end

  def test_vector
    m = CvMat.new(1, 2)
    assert(m.vector?)

    m = CvMat.new(2, 2)
    assert((not m.vector?))
  end

  def test_square
    m = CvMat.new(2, 2)
    assert(m.square?)
    m = CvMat.new(1, 2)
    assert((not m.square?))
  end

  def test_to_CvMat
    flunk('FIXME: resolve unexpected ABORT of CvMat#to_CvMat')
    # m1 = CvMat.new(2, 2)
    # m2 = m1.to_CvMat
    # assert_same(m1, m2.parent)
  end

  def test_sub_rect
    m1 = make_cvmat(10, 10)

    assert_raise(ArgumentError) {
      m1.sub_rect
    }

    m2 = m1.sub_rect(CvRect.new(0, 0, 2, 3))
    assert_equal(2, m2.width)
    assert_equal(3, m2.height)
    m2.height.times { |j|
      m2.width.times { |i|
        assert(is_same_float_array(m1[i, j].to_ary, m2[i, j].to_ary))
      }
    }
    
    topleft = CvPoint.new(2, 3)
    m2 = m1.sub_rect(topleft, CvSize.new(4, 5))
    assert_equal(4, m2.width)
    assert_equal(5, m2.height)
    m2.height.times { |j|
      m2.width.times { |i|
        assert(is_same_float_array(m1[topleft.x + i, topleft.y + j].to_ary, m2[i, j].to_ary))
      }
    }

    topleft = CvPoint.new(1, 2)
    m2 = m1.sub_rect(topleft.x, topleft.y, 3, 4)
    assert_equal(3, m2.width)
    assert_equal(4, m2.height)
    m2.height.times { |j|
      m2.width.times { |i|
        assert(is_same_float_array(m1[topleft.x + i, topleft.y + j].to_ary, m2[i, j].to_ary))
      }
    }

    # Alias
    m2 = m1.subrect(CvRect.new(0, 0, 2, 3))
    assert_equal(2, m2.width)
    assert_equal(3, m2.height)
    m2.height.times { |j|
      m2.width.times { |i|
        assert(is_same_float_array(m1[i, j].to_ary, m2[i, j].to_ary))
      }
    }
  end

  def test_slice_width
    m1 = make_cvmat(10, 40, :cv32f, 1)
    ml, mr = m1.slice_width(2)
    [ml, mr].each { |m|
      assert_equal(10, m.height)
      assert_equal(20, m.width)
      assert_equal(:cv32f, m.depth)
      assert_equal(1, m.channel)
    }

    ml.height.times { |j|
      ml.width.times { |i|
        assert(is_same_float_array(m1[i, j].to_ary, ml[i, j].to_ary))
        assert(is_same_float_array(m1[20 + i, j].to_ary, mr[i, j].to_ary))
      }
    }
  end

  def test_slice_height
    m1 = make_cvmat(10, 20, :cv32f, 1)
    mt, mb = m1.slice_height(2)
    [mt, mb].each { |m|
      assert_equal(5, m.height)
      assert_equal(20, m.width)
      assert_equal(:cv32f, m.depth)
      assert_equal(1, m.channel)
    }

    mt.height.times { |j|
      mt.width.times { |i|
        assert(is_same_float_array(m1[i, j].to_ary, mt[i, j].to_ary))
        assert(is_same_float_array(m1[i, 5 + j].to_ary, mb[i, j].to_ary))
      }
    }
  end
  
  def test_row
    m1 = make_cvmat(10, 20)

    m2 = m1.row(2)
    assert_equal(1, m2.height)
    assert_equal(m1.width, m2.width)
    m1.width.times { |i|
      assert(is_same_float_array(m1[i, 2].to_ary, m2[i].to_ary))
    }

    m2, m3 = m1.row(1, 2)
    [m2, m3].each { |m|
      assert_equal(1, m.height)
      assert_equal(m1.width, m.width)
    }
    m1.width.times { |i|
      assert(is_same_float_array(m1[i, 1].to_ary, m2[i].to_ary))
      assert(is_same_float_array(m1[i, 2].to_ary, m3[i].to_ary))
    }
  end

  def test_col
    m1 = make_cvmat(10, 20)

    m2 = m1.col(2)
    assert_equal(1, m2.width)
    assert_equal(m1.height, m2.height)
    m1.height.times { |j|
      assert(is_same_float_array(m1[2, j].to_ary, m2[j].to_ary))
    }

    m2, m3 = m1.col(1, 2)
    [m2, m3].each { |m|
      assert_equal(1, m.width)
      assert_equal(m1.height, m.height)
    }
    m1.height.times { |j|
      assert(is_same_float_array(m1[1, j].to_ary, m2[j].to_ary))
      assert(is_same_float_array(m1[2, j].to_ary, m3[j].to_ary))
    }
  end

  def test_each_row
    m1 = make_cvmat(2, 3)
    a = [[1, 2, 3], [4, 5, 6]]
    a.map! { |a1|
      a1.map! { |a2|
        CvScalar.new(a2, a2, a2, a2).to_ary
      }
    }

    j = 0
    m1.each_row { |r|
      a[j].size.times { |i|
        assert(is_same_float_array(a[j][i].to_ary, r[i].to_ary))
      }
      j += 1
    }
  end

  def test_each_col
    m1 = make_cvmat(2, 3)
    a = [[1, 4], [2, 5], [3, 6]]
    a.map! { |a1|
      a1.map! { |a2|
        CvScalar.new(a2, a2, a2, a2).to_ary
      }
    }

    j = 0
    m1.each_col { |c|
      a[j].size.times { |i|
        assert(is_same_float_array(a[j][i].to_ary, c[i].to_ary))
      }
      j += 1
    }

    # Alias
    j = 0
    m1.each_column { |c|
      a[j].size.times { |i|
        assert(is_same_float_array(a[j][i].to_ary, c[i].to_ary))
      }
      j += 1
    }
  end

  def test_diag
    m = make_cvmat(5, 5)
    # a = [1.1, 7.7, 14.3, 20.9, 27.5].map { |x| CvScalar.new(x, x, x, x) }
    a = [1, 7, 13, 19, 25].map { |x| CvScalar.new(x, x, x, x) }
    d = m.diag

    a.each_with_index { |s, i|
      assert(is_same_float_array(s.to_ary, d[i].to_ary))
    }

    # a = [2.2, 8.8, 15.4, 22.0].map { |x| CvScalar.new(x, x, x, x) }
    a = [2, 8, 14, 20].map { |x| CvScalar.new(x, x, x, x) }
    d = m.diag(1)
    a.each_with_index { |s, i|
      assert(is_same_float_array(s.to_ary, d[i].to_ary))
    }

    # a = [6.6, 13.2, 19.8, 26.4].map { |x| CvScalar.new(x, x, x, x) }
    a = [6, 12, 18, 24].map { |x| CvScalar.new(x, x, x, x) }
    d = m.diag(-1)
    a.each_with_index { |s, i|
      assert(is_same_float_array(s.to_ary, d[i].to_ary))
    }

    # Alias
    # a = [1.1, 7.7, 14.3, 20.9, 27.5].map { |x| CvScalar.new(x, x, x, x) }
    a = [1, 7, 13, 19, 25].map { |x| CvScalar.new(x, x, x, x) }
    d = m.diagonal
    a.each_with_index { |s, i|
      assert(is_same_float_array(s.to_ary, d[i].to_ary))
    }
  end

  def test_size
    m = CvMat.new(2, 3)
    assert_equal(3, m.size.width)
    assert_equal(2, m.size.height)
  end

  def test_dims
    m = CvMat.new(2, 3)
    assert_equal([2, 3], m.dims)
  end

  def test_dim_size
    m = CvMat.new(2, 3)
    assert_equal(2, m.dim_size(0))
    assert_equal(3, m.dim_size(1))
  end

  def test_aref
    m = make_cvmat(2, 3)
    assert(is_same_float_array([1, 1, 1, 1], m[0]))
    assert(is_same_float_array([5, 5, 5, 5], m[4]))
    assert(is_same_float_array([2, 2, 2, 2], m[1, 0]))
    assert(is_same_float_array([4, 4, 4, 4], m[0, 1]))

    # Alias
    assert(is_same_float_array([1, 1, 1, 1], m.at(0)))

    flunk('FIXME: cvGetND cases do not seem to work well')
  end

  def test_aset
    m = make_cvmat(2, 3)
    m[0] = CvScalar.new(10, 10, 10, 10)
    assert(is_same_float_array([10, 10, 10, 10], m[0]))

    m[1, 0] = CvScalar.new(20, 20, 20, 20)
    assert(is_same_float_array([20, 20, 20, 20], m[1, 0]))

    flunk('FIXME: cvSetND cases do not seem to work well')
  end

  def test_fill
    m1 = make_cvmat(2, 3)
    m2 = m1.fill(CvScalar.new(1, 2, 3, 4))
    m1.fill!(CvScalar.new(1, 2, 3, 4))
    m2.height.times { |j|
      m2.width.times { |i|
        assert(is_same_float_array([1, 2, 3, 4], m1[i, j]))
        assert(is_same_float_array([1, 2, 3, 4], m2[i, j]))
      }
    }

    m1 = make_cvmat(5, 5)
    m0 = m1.clone
    mask = CvMat.new(m1.height, m1.width, :cv8u, 1).clear
    2.times { |j|
      2.times { |i|
        mask[i, j] = CvScalar.new(1, 1, 1, 1)
      }
    }

    m2 = m1.fill(CvScalar.new(1, 2, 3, 4), mask)
    m1.fill!(CvScalar.new(1, 2, 3, 4), mask)
    m2.height.times { |j|
      m2.width.times { |i|
        if i < 2 and j < 2
          assert(is_same_float_array([1, 2, 3, 4], m1[i, j]))
          assert(is_same_float_array([1, 2, 3, 4], m2[i, j]))
        else
          assert(is_same_float_array(m0[i, j], m1[i, j]))
          assert(is_same_float_array(m0[i, j], m2[i, j]))
        end
      }
    }
    
    # Alias
    m1 = make_cvmat(2, 3)
    m2 = m1.set(CvScalar.new(1, 2, 3, 4))
    m1.set!(CvScalar.new(1, 2, 3, 4))
    m2.height.times { |j|
      m2.width.times { |i|
        assert(is_same_float_array([1, 2, 3, 4], m1[i, j]))
        assert(is_same_float_array([1, 2, 3, 4], m2[i, j]))
      }
    }

    m1 = make_cvmat(5, 5)
    m0 = m1.clone
    mask = CvMat.new(m1.height, m1.width, CV_8U, 1).clear
    2.times { |j|
      2.times { |i|
        mask[i, j] = CvScalar.new(1, 1, 1, 1)
      }
    }

    m2 = m1.set(CvScalar.new(1, 2, 3, 4), mask)
    m1.set!(CvScalar.new(1, 2, 3, 4), mask)
    m2.height.times { |j|
      m2.width.times { |i|
        if i < 2 and j < 2
          assert(is_same_float_array([1, 2, 3, 4], m1[i, j]))
          assert(is_same_float_array([1, 2, 3, 4], m2[i, j]))
        else
          assert(is_same_float_array(m0[i, j], m1[i, j]))
          assert(is_same_float_array(m0[i, j], m2[i, j]))
        end
      }
    }
  end

  def test_clear
    m1 = make_cvmat(2, 3)
    m2 = m1.clear
    m1.clear!
    m2.height.times { |j|
      m2.width.times { |i|
        assert(is_same_float_array([0, 0, 0, 0], m1[i, j]))
        assert(is_same_float_array([0, 0, 0, 0], m2[i, j]))
      }
    }

    # Alias
    m1 = make_cvmat(2, 3)
    m2 = m1.set_zero
    m1.set_zero!
    m2.height.times { |j|
      m2.width.times { |i|
        assert(is_same_float_array([0, 0, 0, 0], m1[i, j]))
        assert(is_same_float_array([0, 0, 0, 0], m2[i, j]))
      }
    }
  end

  def test_identity
    m1 = make_cvmat(5, 5)
    m2 = m1.identity
    m1.identity!
    m2.height.times { |j|
      m2.width.times { |i|
        if i == j
          assert(is_same_float_array([1, 0, 0, 0], m1[i, j].to_ary))
          assert(is_same_float_array([1, 0, 0, 0], m2[i, j].to_ary))
        else
          assert(is_same_float_array([0, 0, 0, 0], m1[i, j].to_ary))
          assert(is_same_float_array([0, 0, 0, 0], m2[i, j].to_ary))
        end
      }
    }
  end

  def test_range
    m1 = CvMat.new(1, 10, CV_32S, 1)
    m2 = m1.range(0, m1.cols)
    m1.range!(0, m1.cols)
    m2.width.times { |i|
      assert(is_same_float_array([i, 0, 0, 0], m1[i, 0].to_ary))
      assert(is_same_float_array([i, 0, 0, 0], m2[i, 0].to_ary))
    }
  end

  def test_reshape
    m = make_cvmat(2, 3, CV_8U, 3)
    assert_raise(TypeError) {
      m.reshape(1)
    }

    vec = m.reshape(:rows => 1)

    # These tests are failed.
    # The order of arguments of cvReshape() are 
    #   const CvArr* arr, CvMat* header, int new_cn, int new_rows
    # but in rb_reshape in cvmat.cpp is
    #   const CvArr* arr, CvMat* header, int new_rows, int new_cn
    # The order of new_rows and new_cn is wrong.
    flunk('FIXME: The order of arguments of cvReshape() is wrong.')
    assert_equal(6, vec.width)
    assert_equal(1, vec.height)
    size = m.width * m.height
    size.times { |i|
      assert(is_same_float_array(m[i].to_ary, vec[i].to_ary))
    }

    ch1 = m.reshape(:channel => 1)
    assert_equal(9, ch1.width)
    assert_equal(6, ch1.height)

    m.height.times { |j|
      m.width.times { |i|
        s1 = ch1[i * 3, j * 3]
        s2 = ch1[i * 3 + 1, j * 3]
        s3 = ch1[i * 3 + 2, j * 3]
        assert(is_same_float_array(m[i, j].to_ary, [s1, s2, s3, 0]))
      }
    }
  end

  def test_repeat
    m1 = make_cvmat(2, 3)
    assert_raise(TypeError) {
      m1.repeat(1)
    }
    m2 = CvMat.new(6, 9)
    m2 = m1.repeat(m2)
    m2.height.times { |j|
      m2.width.times { |i|
        a = m1[i % m1.width, j % m1.height].to_ary
        assert(is_same_float_array(m2[i, j].to_ary, a))
      }
    }
    
  end
  
  # def test_avg_sdv
  #   m = CvMat.new(1, 8, CV_32F)
  #   [2, 4, 4, 4, 5, 5, 7, 9].each_with_index { |v, i|
  #     m[i] = CvScalar.new(v, 0, 0, 0)
  #   }
  #   avg = m.avg
  #   assert_in_delta(avg[0], 5.0, 0.01)
  #   avg, sdv = m.avg_sdv
  #   assert_in_delta(avg[0], 5.0, 0.01)
  #   assert_in_delta(sdv[0], 2.0, 0.01)
  # end

end



