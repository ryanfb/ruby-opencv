#!/usr/bin/env ruby
# -*- mode: ruby; coding: utf-8-unix -*- 
require 'test/unit'
require 'opencv'
require 'pp'
require File.expand_path(File.dirname(__FILE__)) + '/helper'

include OpenCV

# Tests for OpenCV::CvMat
class TestCvMat < OpenCVTestCase
  def test_initialize
    m = CvMat.new(10, 20)
    assert_equal(10, m.rows)
    assert_equal(20, m.cols)
    assert_equal(:cv8u, m.depth)
    assert_equal(3, m.channel)
    # assert_each_cvscalar(m) {
    #   CvScalar.new(0, 0, 0, 0)
    # }
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
    m2 = m1.to_CvMat
    assert(m2.has_parent?)
    assert_same(m1, m2.parent)
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
    m1 = create_cvmat(10, 20)
    m2 = m1.clone
    assert_equal(m1.data, m2.data)
  end

  def test_copy
    m1 = create_cvmat(10, 20, CV_32F, 1)

    m2 = m1.copy
    assert_equal(m1.data, m2.data)
    
    m2 = CvMat.new(10, 20, CV_32F, 1)
    m1.copy(m2)
    assert_equal(m1.data, m2.data)
    
    a = m1.copy(2)
    assert_equal(2, a.size)

    a.each { |m|
      assert_equal(m1.height, m.height)
      assert_equal(m1.width, m.width)
      m1.height.times { |j|
        m1.width.times { |i|
          assert_cvscalar_equal(m1[j, i], m[j, i])
        }
      }
    }
    assert_nil(m1.copy(-1))

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
    m1 = CvMat.new(2, 2)
    flunk('FIXME: resolve unexpected ABORT of CvMat#to_CvMat')
    m2 = m1.to_CvMat
    assert_same(m1, m2.parent)
  end

  def test_sub_rect
    m1 = create_cvmat(10, 10)

    assert_raise(ArgumentError) {
      m1.sub_rect
    }

    m2 = m1.sub_rect(CvRect.new(0, 0, 2, 3))
    assert_equal(2, m2.width)
    assert_equal(3, m2.height)
    m2.height.times { |j|
      m2.width.times { |i|
        assert_cvscalar_equal(m1[j, i], m2[j, i])
      }
    }
    
    topleft = CvPoint.new(2, 3)
    m2 = m1.sub_rect(topleft, CvSize.new(4, 5))
    assert_equal(4, m2.width)
    assert_equal(5, m2.height)
    m2.height.times { |j|
      m2.width.times { |i|
        assert_cvscalar_equal(m1[topleft.y + j, topleft.x + i], m2[j, i])
      }
    }

    topleft = CvPoint.new(1, 2)
    m2 = m1.sub_rect(topleft.x, topleft.y, 3, 4)
    assert_equal(3, m2.width)
    assert_equal(4, m2.height)
    m2.height.times { |j|
      m2.width.times { |i|
        assert_cvscalar_equal(m1[topleft.y + j, topleft.x + i], m2[j, i])
      }
    }

    # Alias
    m2 = m1.subrect(CvRect.new(0, 0, 2, 3))
    assert_equal(2, m2.width)
    assert_equal(3, m2.height)
    m2.height.times { |j|
      m2.width.times { |i|
        assert_cvscalar_equal(m1[j, i], m2[j, i])
      }
    }
  end

  def test_slice_width
    m1 = create_cvmat(10, 40, :cv32f, 1)
    ml, mr = m1.slice_width(2)
    [ml, mr].each { |m|
      assert_equal(10, m.height)
      assert_equal(20, m.width)
      assert_equal(:cv32f, m.depth)
      assert_equal(1, m.channel)
    }

    ml.height.times { |j|
      ml.width.times { |i|
        assert_cvscalar_equal(m1[j, i], ml[j, i])
        assert_cvscalar_equal(m1[j, (m1.width / 2) + i], mr[j, i])
      }
    }
  end

  def test_slice_height
    m1 = create_cvmat(10, 20, :cv32f, 1)
    mt, mb = m1.slice_height(2)
    [mt, mb].each { |m|
      assert_equal(5, m.height)
      assert_equal(20, m.width)
      assert_equal(:cv32f, m.depth)
      assert_equal(1, m.channel)
    }

    mt.height.times { |j|
      mt.width.times { |i|
        assert_cvscalar_equal(m1[j, i], mt[j, i])
        assert_cvscalar_equal(m1[(m1.height / 2) + j, i], mb[j, i])
      }
    }
  end
  
  def test_row
    m1 = create_cvmat(10, 20)

    m2 = m1.row(2)
    assert_equal(1, m2.height)
    assert_equal(m1.width, m2.width)
    m1.width.times { |i|
      assert_cvscalar_equal(m1[2, i], m2[i])
    }

    m2, m3 = m1.row(1, 2)
    [m2, m3].each { |m|
      assert_equal(1, m.height)
      assert_equal(m1.width, m.width)
    }
    m1.width.times { |i|
      assert_cvscalar_equal(m1[1, i], m2[i])
      assert_cvscalar_equal(m1[2, i], m3[i])
    }
  end

  def test_col
    m1 = create_cvmat(10, 20)

    m2 = m1.col(2)
    assert_equal(1, m2.width)
    assert_equal(m1.height, m2.height)
    m1.height.times { |j|
      assert_cvscalar_equal(m1[j, 2], m2[j])
    }

    m2, m3 = m1.col(1, 2)
    [m2, m3].each { |m|
      assert_equal(1, m.width)
      assert_equal(m1.height, m.height)
    }
    m1.height.times { |j|
      assert_cvscalar_equal(m1[j, 1], m2[j])
      assert_cvscalar_equal(m1[j, 2], m3[j])
    }
  end

  def test_each_row
    m1 = create_cvmat(2, 3)
    a = [[1, 2, 3], [4, 5, 6]]
    a.map! { |a1|
      a1.map! { |a2|
        CvScalar.new(a2, a2, a2, a2).to_ary
      }
    }

    j = 0
    m1.each_row { |r|
      a[j].size.times { |i|
        assert_cvscalar_equal(a[j][i], r[i])
      }
      j += 1
    }
  end

  def test_each_col
    m1 = create_cvmat(2, 3)
    a = [[1, 4], [2, 5], [3, 6]]
    a.map! { |a1|
      a1.map! { |a2|
        CvScalar.new(a2, a2, a2, a2).to_ary
      }
    }

    j = 0
    m1.each_col { |c|
      a[j].size.times { |i|
        assert_cvscalar_equal(a[j][i], c[i])
      }
      j += 1
    }

    # Alias
    j = 0
    m1.each_column { |c|
      a[j].size.times { |i|
        assert_cvscalar_equal(a[j][i], c[i])
      }
      j += 1
    }
  end

  def test_diag
    m = create_cvmat(5, 5)
    a = [1, 7, 13, 19, 25].map { |x| CvScalar.new(x, x, x, x) }
    d = m.diag

    a.each_with_index { |s, i|
      assert_cvscalar_equal(s, d[i])
    }

    a = [2, 8, 14, 20].map { |x| CvScalar.new(x, x, x, x) }
    d = m.diag(1)
    a.each_with_index { |s, i|
      assert_cvscalar_equal(s, d[i])
    }

    a = [6, 12, 18, 24].map { |x| CvScalar.new(x, x, x, x) }
    d = m.diag(-1)
    a.each_with_index { |s, i|
      assert_cvscalar_equal(s, d[i])
    }

    # Alias
    a = [1, 7, 13, 19, 25].map { |x| CvScalar.new(x, x, x, x) }
    d = m.diagonal
    a.each_with_index { |s, i|
      assert_cvscalar_equal(s, d[i])
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
    m = create_cvmat(2, 3)
    assert_cvscalar_equal(CvScalar.new(1, 1, 1, 1), m[0])
    assert_cvscalar_equal(CvScalar.new(5, 5, 5, 5), m[4])
    assert_cvscalar_equal(CvScalar.new(2, 2, 2, 2), m[0, 1])
    assert_cvscalar_equal(CvScalar.new(4, 4, 4, 4), m[1, 0])

    # Alias
    assert_cvscalar_equal(CvScalar.new(1, 1, 1, 1), m.at(0))

    flunk('FIXME: cvGetND cases do not seem to work well')
  end

  def test_aset
    m = create_cvmat(2, 3)
    m[0] = CvScalar.new(10, 10, 10, 10)
    assert_cvscalar_equal(CvScalar.new(10, 10, 10, 10), m[0])

    m[1, 0] = CvScalar.new(20, 20, 20, 20)
    assert_cvscalar_equal(CvScalar.new(20, 20, 20, 20), m[1, 0])

    flunk('FIXME: cvSetND cases do not seem to work well')
  end

  def test_fill
    m1 = create_cvmat(2, 3)
    m2 = m1.fill(CvScalar.new(1, 2, 3, 4))
    m1.fill!(CvScalar.new(1, 2, 3, 4))
    m2.height.times { |j|
      m2.width.times { |i|
        assert_cvscalar_equal(CvScalar.new(1, 2, 3, 4), m1[j, i])
        assert_cvscalar_equal(CvScalar.new(1, 2, 3, 4), m2[j, i])
      }
    }

    m1 = create_cvmat(5, 5)
    m0 = m1.clone
    mask = CvMat.new(m1.height, m1.width, :cv8u, 1).clear
    2.times { |j|
      2.times { |i|
        mask[j, i] = CvScalar.new(1, 1, 1, 1)
      }
    }

    m2 = m1.fill(CvScalar.new(1, 2, 3, 4), mask)
    m1.fill!(CvScalar.new(1, 2, 3, 4), mask)
    m2.height.times { |j|
      m2.width.times { |i|
        if i < 2 and j < 2
          assert_cvscalar_equal(CvScalar.new(1, 2, 3, 4), m1[j, i])
          assert_cvscalar_equal(CvScalar.new(1, 2, 3, 4), m2[j, i])
        else
          assert_cvscalar_equal(m0[j, i], m1[j, i])
          assert_cvscalar_equal(m0[j, i], m2[j, i])
        end
      }
    }
    
    # Alias
    m1 = create_cvmat(2, 3)
    m2 = m1.set(CvScalar.new(1, 2, 3, 4))
    m1.set!(CvScalar.new(1, 2, 3, 4))
    m2.height.times { |j|
      m2.width.times { |i|
        assert_cvscalar_equal(CvScalar.new(1, 2, 3, 4), m1[j, i])
        assert_cvscalar_equal(CvScalar.new(1, 2, 3, 4), m2[j, i])
      }
    }

    m1 = create_cvmat(5, 5)
    m0 = m1.clone
    mask = CvMat.new(m1.height, m1.width, CV_8U, 1).clear
    2.times { |j|
      2.times { |i|
        mask[j, i] = CvScalar.new(1, 1, 1, 1)
      }
    }

    m2 = m1.set(CvScalar.new(1, 2, 3, 4), mask)
    m1.set!(CvScalar.new(1, 2, 3, 4), mask)
    m2.height.times { |j|
      m2.width.times { |i|
        if i < 2 and j < 2
          assert_cvscalar_equal(CvScalar.new(1, 2, 3, 4), m1[j, i])
          assert_cvscalar_equal(CvScalar.new(1, 2, 3, 4), m2[j, i])
        else
          assert_cvscalar_equal(m0[j, i], m1[j, i])
          assert_cvscalar_equal(m0[j, i], m2[j, i])
        end
      }
    }
  end

  def test_clear
    m1 = create_cvmat(2, 3)
    m2 = m1.clear
    m1.clear!
    m2.height.times { |j|
      m2.width.times { |i|
        assert_cvscalar_equal(CvScalar.new(0, 0, 0, 0), m1[j, i])
        assert_cvscalar_equal(CvScalar.new(0, 0, 0, 0), m2[j, i])
      }
    }

    # Alias
    m1 = create_cvmat(2, 3)
    m2 = m1.set_zero
    m1.set_zero!
    m2.height.times { |j|
      m2.width.times { |i|
        assert_cvscalar_equal(CvScalar.new(0, 0, 0, 0), m1[j, i])
        assert_cvscalar_equal(CvScalar.new(0, 0, 0, 0), m2[j, i])
      }
    }
  end

  def test_identity
    m1 = create_cvmat(5, 5)
    m2 = m1.identity
    m1.identity!
    m2.height.times { |j|
      m2.width.times { |i|
        if i == j
          assert_cvscalar_equal(CvScalar.new(1, 0, 0, 0), m1[j, i])
          assert_cvscalar_equal(CvScalar.new(1, 0, 0, 0), m2[j, i])
        else
          assert_cvscalar_equal(CvScalar.new(0, 0, 0, 0), m1[j, i])
          assert_cvscalar_equal(CvScalar.new(0, 0, 0, 0), m2[j, i])
        end
      }
    }
  end

  def test_range
    m1 = CvMat.new(1, 10, CV_32S, 1)
    m2 = m1.range(0, m1.cols)
    m1.range!(0, m1.cols)
    m2.width.times { |i|
      assert_cvscalar_equal(CvScalar.new(i, 0, 0, 0), m1[0, i])
      assert_cvscalar_equal(CvScalar.new(i, 0, 0, 0), m2[0, i])
    }
  end

  def test_reshape
    m = create_cvmat(2, 3, CV_8U, 3)
    assert_raise(TypeError) {
      m.reshape(1)
    }

    vec = m.reshape(:rows => 1)
    assert_equal(6, vec.width)
    assert_equal(1, vec.height)
    size = m.width * m.height
    size.times { |i|
      assert_cvscalar_equal(m[i], vec[i])
    }

    ch1 = m.reshape(:channel => 1)
    assert_equal(9, ch1.width)
    assert_equal(2, ch1.height)

    m.height.times { |j|
      m.width.times { |i|
        s1 = ch1[j, i * 3][0]
        s2 = ch1[j, i * 3 + 1][0]
        s3 = ch1[j, i * 3 + 2][0]
        assert_cvscalar_equal(m[j, i], CvScalar.new(s1, s2, s3, 0))
      }
    }
  end

  def test_repeat
    m1 = create_cvmat(2, 3, :cv8u, 3)
    assert_raise(TypeError) {
      m1.repeat(1)
    }
    m2 = CvMat.new(6, 9, :cv8u, 3)
    m2 = m1.repeat(m2)
    m2.height.times { |j|
      m2.width.times { |i|
        a = m1[j % m1.height, i % m1.width]
        assert_cvscalar_equal(m2[j, i], a)
      }
    }
  end

  def test_flip
    m0 = create_cvmat(2, 3)

    m1 = m0.clone
    m1.flip!(:x)
    m2 = m0.flip(:x)
    m3 = m0.clone
    m3.flip!(:y)
    m4 = m0.flip(:y)
    m5 = m0.clone
    m5.flip!(:xy)
    m6 = m0.flip(:xy)
    m7 = m0.clone
    m7.flip!
    m8 = m0.flip

    [m1, m2, m3, m4, m5, m6, m7, m8].each { |m|
      assert_equal(m0.height, m.height)
      assert_equal(m0.width, m.width)
    }
    m0.height.times { |j|
      m0.width.times { |i|
        ri = m0.width - i - 1
        rj = m0.height - j - 1
        assert_cvscalar_equal(m0[j, ri], m1[j, i])
        assert_cvscalar_equal(m0[j, ri], m2[j, i])
        assert_cvscalar_equal(m0[rj, i], m3[j, i])
        assert_cvscalar_equal(m0[rj, i], m4[j, i])
        assert_cvscalar_equal(m0[rj, ri], m5[j, i])
        assert_cvscalar_equal(m0[rj, ri], m6[j, i])
        assert_cvscalar_equal(m0[rj, i], m7[j, i])
        assert_cvscalar_equal(m0[rj, i], m8[j, i])
      }
    }
  end

  def test_split
    m0 = create_cvmat(2, 3, :cv8u, 3) { |j, i, c|
      CvScalar.new(c * 10, c * 20, c * 30)
    }
    m0.split.each_with_index { |m, idx|
      assert_equal(m0.height, m.height)
      assert_equal(m0.width, m.width)

      c = 0
      m0.height.times { |j|
        m0.width.times { |i|
          val = c * 10 * (idx + 1)
          assert_cvscalar_equal(CvScalar.new(val), m[j, i])
          c += 1
        }
      }
    }
  end

  def test_merge
    m0 = create_cvmat(2, 3, :cv8u, 4) { |j, i, c|
      CvScalar.new(c * 10, c * 20, c * 30, c * 40)
    }
    m1 = create_cvmat(2, 3, :cv8u, 1) { |j, i, c|
      CvScalar.new(c * 10)
    }
    m2 = create_cvmat(2, 3, :cv8u, 1) { |j, i, c|
      CvScalar.new(c * 20)
    }
    m3 = create_cvmat(2, 3, :cv8u, 1) { |j, i, c|
      CvScalar.new(c * 30)
    }
    m4 = create_cvmat(2, 3, :cv8u, 1) { |j, i, c|
      CvScalar.new(c * 40)
    }

    m = CvMat.merge(m1, m2, m3, m4)

    assert_equal(m0.height, m.height)
    assert_equal(m0.width, m.width)
    m0.height.times { |j|
      m0.width.times { |i|
        assert_cvscalar_equal(m0[j, i], m[j, i])
      }
    }
  end

  def test_mix_channels
    flunk('FIXME: CvMat.mix_channels is not implemented yet.')
  end

  def test_rand_shuffle
    m0 = create_cvmat(2, 3)
    m1 = m0.clone
    m1.rand_shuffle!
    m2 = m0.rand_shuffle
    m3 = m0.clone
    m3.rand_shuffle!(123, 234)
    m4 = m0.rand_shuffle(123, 234)
    
    assert_shuffled_equal = lambda { |src, shuffled|
      assert_equal(src.width, shuffled.width)
      assert_equal(src.height, shuffled.height)
      mat0, mat1 = [], []
      src.height { |j|
        src.width { |i|
          mat0 << src[j, i].to_s
          mat1 << shuffled[j, i].to_s
        }
      }
      assert_equal(0, (mat0 - mat1).size)
    }

    [m1, m2, m3, m4].each { |m|
      assert_shuffled_equal.call(m0, m)
    }
  end

  def test_lut
    m0 = create_cvmat(2, 3, :cv8u, 3)
    lut_mat = create_cvmat(1, 256, :cv8u, 3) { |j, i, c|
      CvScalar.new(255 - c, 255 - c, 255 - c)
    }

    m = m0.lut(lut_mat)
    assert_equal(m0.height, m.height)
    assert_equal(m0.width, m.width)
    m0.height.times { |j|
      m0.width.times { |i|
        r, g, b = m0[j, i].to_ary.map { |c| 255 - c }
        assert_cvscalar_equal(CvScalar.new(r, g, b, 0), m[j, i])
      }
    }
  end

  def test_convert_scale
    m0 = create_cvmat(2, 3, :cv32f, 4) { |j, i, c|
      CvScalar.new(-c, -c, -c, -c)
    }

    m1 = m0.convert_scale(:depth => :cv8u)
    m2 = m0.convert_scale(:scale => 1.5)
    m3 = m0.convert_scale(:shift => 10.0)
    m4 = m0.convert_scale(:depth => CV_16U)

    [m1, m2, m3, m4].each { |m|
      assert_equal(m0.height, m.height)
      assert_equal(m0.width, m.width)
    }
    m0.height.times { |j|
      m0.width.times { |i|
        assert_cvscalar_equal(CvScalar.new(0, 0, 0, 0), m1[j, i])
        a = m0[j, i].to_ary.map { |x| x * 1.5 }
        assert_in_delta(a, m2[j, i], 0.001)
        a = m0[j, i].to_ary.map { |x| x + 10.0 }
        assert_in_delta(a, m3[j, i], 0.001)
        assert_cvscalar_equal(CvScalar.new(0, 0, 0, 0), m4[j, i])
      }
    }
  end

  def test_convert_scale_abs
    m0 = create_cvmat(2, 3, :cv8u, 4) { |j, i, c|
      CvScalar.new(c, c, c, c)
    }
    
    m1 = m0.convert_scale_abs(:depth => :cv64f)
    m2 = m0.convert_scale_abs(:scale => 2)
    m3 = m0.convert_scale_abs(:shift => 10.0)
    m4 = m0.convert_scale_abs(:depth => CV_64F)

    [m1, m2, m3, m4].each { |m|
      assert_equal(m0.height, m.height)
      assert_equal(m0.width, m.width)
    }
    m0.height.times { |j|
      m0.width.times { |i|
        assert_cvscalar_equal(m0[j, i], m1[j, i])
        a = m0[j, i].to_ary.map { |x| (x * 2).abs }
        assert_in_delta(a, m2[j, i], 0.001)
        a = m0[j, i].to_ary.map { |x| (x + 10.0).abs }
        assert_in_delta(a, m3[j, i], 0.001)
        assert_cvscalar_equal(m0[j, i], m4[j, i])
      }
    }
  end

  def test_add
    m1 = create_cvmat(6, 4, :cv32f, 4) { |j, i, c|
      CvScalar.new(c * 0.1, c * 0.2, c * 0.3, c * 0.4)
    }
    m2 = create_cvmat(6, 4, :cv32f, 4) { |j, i, c|
      CvScalar.new(c * 1, c * 2, c * 3, c * 4)
    }

    # CvMat + CvMat
    m3 = m1.add(m2)
    assert_equal(m1.height, m3.height)
    assert_equal(m1.width, m3.width)
    n = 0
    m1.height.times { |j|
      m1.width.times { |i|
        s = CvScalar.new(n * 1.1, n * 2.2, n * 3.3, n * 4.4)
        assert_in_delta(s, m3[j, i], 0.001)
        n += 1
      }
    }

    # CvMat + CvScalar
    s1 = CvScalar.new(1, 2, 3, 4)
    m3 = m1.add(s1)
    assert_equal(m1.height, m3.height)
    assert_equal(m1.width, m3.width)
    n = 0
    m1.height.times { |j|
      m1.width.times { |i|
        s = CvScalar.new(n * 0.1 + 1, n * 0.2 + 2, n * 0.3 + 3, n * 0.4 + 4)
        assert_in_delta(s, m3[j, i], 0.001)
        n += 1
      }
    }

    # Alias
    m3 = m1 + m2
    assert_equal(m1.height, m3.height)
    assert_equal(m1.width, m3.width)
    n = 0
    m1.height.times { |j|
      m1.width.times { |i|
        s = CvScalar.new(n * 1.1, n * 2.2, n * 3.3, n * 4.4)
        assert_in_delta(s, m3[j, i], 0.001)
        n += 1
      }
    }

    # CvMat + CvMat with Mask
    mask = create_cvmat(6, 4, :cv8u, 1) { |j, i, c|
      (i < 3 and j < 2) ? 1 : 0
    }

    flunk('FIXME: Tests of CvMat + CvMat with Mask often (but not always) fails. Is initializing required...?')
    m4 = m1.add(m2, mask)
    assert_equal(m1.height, m4.height)
    assert_equal(m1.width, m4.width)
    n = 0
    m1.height.times { |j|
      m1.width.times { |i|
        if i < 3 and j < 2
          s = CvScalar.new(n * 1.1, n * 2.2, n * 3.3, n * 4.4)
        else
          s = CvScalar.new(0)
        end
        assert_in_delta(s, m4[j, i], 0.001)
        n += 1
      }
    }

    flunk('FIXME: Tests of CvMat + CvScalar with Mask often (but not always) fails. Is initializing required...?')
    # CvMat + CvScalar with Mask
    m4 = m1.add(s1, mask)
    assert_equal(m1.height, m4.height)
    assert_equal(m1.width, m4.width)
    n = 0
    m1.height.times { |j|
      m1.width.times { |i|
        if i < 3 and j < 2
          s = CvScalar.new(n * 0.1 + 1, n * 0.2 + 2, n * 0.3 + 3, n * 0.4 + 4)          
        else
          s = CvScalar.new(0)
        end
        assert_in_delta(s, m4[j, i], 0.001)
        n += 1
      }
    }
  end

  def test_sub
    m1 = create_cvmat(6, 4, :cv32f, 4) { |j, i, c|
      CvScalar.new(c * 0.1, c * 0.2, c * 0.3, c * 0.4)
    }
    m2 = create_cvmat(6, 4, :cv32f, 4) { |j, i, c|
      CvScalar.new(c * 1, c * 2, c * 3, c * 4)
    }

    # CvMat - CvMat
    m3 = m1.sub(m2)
    assert_equal(m1.height, m3.height)
    assert_equal(m1.width, m3.width)
    n = 0
    m1.height.times { |j|
      m1.width.times { |i|
        s = CvScalar.new(-n * 0.9, -n * 1.8, -n * 2.7, -n * 3.6)
        assert_in_delta(s, m3[j, i], 0.001)
        n += 1
      }
    }

    # CvMat - CvScalar
    s1 = CvScalar.new(1, 2, 3, 4)
    m3 = m1.sub(s1)
    assert_equal(m1.height, m3.height)
    assert_equal(m1.width, m3.width)
    n = 0
    m1.height.times { |j|
      m1.width.times { |i|
        s = CvScalar.new(n * 0.1 - 1, n * 0.2 - 2, n * 0.3 - 3, n * 0.4 - 4)
        assert_in_delta(s, m3[j, i], 0.001)
        n += 1
      }
    }

    # Alias
    m3 = m1 - m2
    assert_equal(m1.height, m3.height)
    assert_equal(m1.width, m3.width)
    n = 0
    m1.height.times { |j|
      m1.width.times { |i|
        s = CvScalar.new(-n * 0.9, -n * 1.8, -n * 2.7, -n * 3.6)
        assert_in_delta(s, m3[j, i], 0.001)
        n += 1
      }
    }

    mask = create_cvmat(6, 4, :cv8u, 1) { |j, i, c|
      (i < 3 and j < 2) ? 1 : 0
    }

    flunk('FIXME: Tests of CvMat - CvMat with Mask often (but not always) fails. Is initializing required...?')
    # CvMat - CvMat with Mask
    m4 = m1.sub(m2, mask)
    assert_equal(m1.height, m4.height)
    assert_equal(m1.width, m4.width)
    n = 0
    m1.height.times { |j|
      m1.width.times { |i|
        if i < 3 and j < 2
          s = CvScalar.new(-n * 0.9, -n * 1.8, -n * 2.7, -n * 3.6)
        else
          s = CvScalar.new(0)
        end
        assert_in_delta(s, m4[j, i], 0.001)
        n += 1
      }
    }

    flunk('FIXME: Tests of CvMat - CvScalar with Mask often (but not always) fails. Is initializing required...?')
    # CvMat - CvScalar with Mask
    m4 = m1.add(s1, mask)
    assert_equal(m1.height, m4.height)
    assert_equal(m1.width, m4.width)
    n = 0
    m1.height.times { |j|
      m1.width.times { |i|
        if i < 3 and j < 2
          s = CvScalar.new(n * 0.1 - 1, n * 0.2 - 2, n * 0.3 - 3, n * 0.4 - 4)
        else
          s = CvScalar.new(0)
        end
        assert_in_delta(s, m4[j, i], 0.001)
        n += 1
      }
    }
  end

  def test_mul
    m1 = create_cvmat(3, 3, :cv32f)
    s1 = CvScalar.new(0.1, 0.2, 0.3, 0.4)
    m2 = create_cvmat(3, 3, :cv32f) { s1 }

    # CvMat * CvMat
    m3 = m1.mul(m2)
    assert_equal(m1.height, m3.height)
    assert_equal(m1.width, m3.width)
    assert_each_cvscalar(m3, 0.001) { |j, i, c|
      n = c + 1
      CvScalar.new(n * 0.1, n * 0.2, n * 0.3, n * 0.4)
    }
    
    # CvMat * CvMat * scale
    scale = 2.5
    m3 = m1.mul(m2, scale)
    assert_equal(m1.height, m3.height)
    assert_equal(m1.width, m3.width)
    assert_each_cvscalar(m3, 0.001) { |j, i, c|
      n = (c + 1) * scale
      CvScalar.new(n * 0.1, n * 0.2, n * 0.3, n * 0.4)
    }

    # CvMat * CvScalar
    scale = 2.5
    m3 = m1.mul(s1)
    assert_equal(m1.height, m3.height)
    assert_equal(m1.width, m3.width)
    assert_each_cvscalar(m3, 0.001) { |j, i, c|
      n = c + 1
      CvScalar.new(n * 0.1, n * 0.2, n * 0.3, n * 0.4)
    }

    # CvMat * CvScalar * scale
    m3 = m1.mul(s1, scale)
    assert_equal(m1.height, m3.height)
    assert_equal(m1.width, m3.width)
    assert_each_cvscalar(m3, 0.001) { |j, i, c|
      n = (c + 1) * scale
      CvScalar.new(n * 0.1, n * 0.2, n * 0.3, n * 0.4)
    }
  end

  def test_mat_mul
    flunk('FIXME: CvMat#mat_mul is not implemented yet.')
  end

  def test_div
    m1 = create_cvmat(3, 3, :cv32f)
    s1 = CvScalar.new(0.1, 0.2, 0.3, 0.4)
    m2 = create_cvmat(3, 3, :cv32f) { s1 }

    # CvMat / CvMat
    m3 = m1.div(m2)
    assert_equal(m1.height, m3.height)
    assert_equal(m1.width, m3.width)
    assert_each_cvscalar(m3, 0.001) { |j, i, c|
      n = c + 1
      CvScalar.new(n / 0.1, n / 0.2, n / 0.3, n / 0.4)
    }
    
    # scale * CvMat / CvMat
    scale = 2.5
    m3 = m1.div(m2, scale)
    assert_equal(m1.height, m3.height)
    assert_equal(m1.width, m3.width)
    assert_each_cvscalar(m3, 0.001) { |j, i, c|
      n = (c + 1) * scale
      CvScalar.new(n / 0.1, n / 0.2, n / 0.3, n / 0.4)
    }

    # CvMat / CvScalar
    scale = 2.5
    m3 = m1.div(s1)
    assert_equal(m1.height, m3.height)
    assert_equal(m1.width, m3.width)
    assert_each_cvscalar(m3, 0.001) { |j, i, c|
      n = c + 1
      CvScalar.new(n / 0.1, n / 0.2, n / 0.3, n / 0.4)
    }

    # scale * CvMat / CvScalar
    m3 = m1.div(s1, scale)
    assert_equal(m1.height, m3.height)
    assert_equal(m1.width, m3.width)
    assert_each_cvscalar(m3, 0.001) { |j, i, c|
      n = (c + 1) * scale
      CvScalar.new(n / 0.1, n / 0.2, n / 0.3, n / 0.4)
    }

    # Alias
    m3 = m1 / m2
    assert_equal(m1.height, m3.height)
    assert_equal(m1.width, m3.width)
    assert_each_cvscalar(m3, 0.001) { |j, i, c|
      n = c + 1
      CvScalar.new(n / 0.1, n / 0.2, n / 0.3, n / 0.4)
    }
  end

  def test_and
    m1 = create_cvmat(6, 4)
    s1 = CvScalar.new(1, 2, 3, 4)
    m2 = create_cvmat(6, 4) { s1 }
    mask = create_cvmat(6, 4, :cv8u, 1) { |j, i, c|
      s = (i < 3 and j < 2) ? 1 : 0
      CvScalar.new(s)
    }

    # CvMat & CvMat
    m3 = m1.and(m2)
    assert_equal(m1.height, m3.height)
    assert_equal(m1.width, m3.width)
    assert_each_cvscalar(m3) { |j, i, c|
      n = c + 1
      CvScalar.new(n & 1, n & 2, n & 3, n & 4)
    }

    # CvMat & CvMat with mask
    m3 = m1.and(m2, mask)
    assert_equal(m1.height, m3.height)
    assert_equal(m1.width, m3.width)
    assert_each_cvscalar(m3) { |j, i, c|
      n = c + 1
      if i < 3 and j < 2
        CvScalar.new(n & 1, n & 2, n & 3, n & 4)
      else
        CvScalar.new(n, n, n, n)
      end
    }
    
    # CvMat & CvScalar
    m3 = m1.and(s1)
    assert_equal(m1.height, m3.height)
    assert_equal(m1.width, m3.width)
    assert_each_cvscalar(m3) { |j, i, c|
      n = c + 1
      CvScalar.new(n & 1, n & 2, n & 3, n & 4)
    }

    # CvMat & CvScalar with mask
    m3 = m1.and(s1, mask)
    assert_equal(m1.height, m3.height)
    assert_equal(m1.width, m3.width)
    assert_each_cvscalar(m3) { |j, i, c|
      n = c + 1
      if i < 3 and j < 2
        CvScalar.new(n & 1, n & 2, n & 3, n & 4)
      else
        CvScalar.new(n, n, n, n)
      end
    }

    # Alias
    m3 = m1 & m2
    assert_equal(m1.height, m3.height)
    assert_equal(m1.width, m3.width)
    assert_each_cvscalar(m3) { |j, i, c|
      n = c + 1
      CvScalar.new(n & 1, n & 2, n & 3, n & 4)
    }

    m3 = m1 & s1
    assert_equal(m1.height, m3.height)
    assert_equal(m1.width, m3.width)
    assert_each_cvscalar(m3) { |j, i, c|
      n = c + 1
      CvScalar.new(n & 1, n & 2, n & 3, n & 4)
    }
  end

  def test_or
    m1 = create_cvmat(6, 4)
    s1 = CvScalar.new(1, 2, 3, 4)
    m2 = create_cvmat(6, 4) { s1 }
    mask = create_cvmat(6, 4, :cv8u, 1) { |j, i, c|
      s = (i < 3 and j < 2) ? 1 : 0
      CvScalar.new(s)
    }
    
    # CvMat | CvMat
    m3 = m1.or(m2)
    assert_equal(m1.height, m3.height)
    assert_equal(m1.width, m3.width)
    assert_each_cvscalar(m3) { |j, i, c|
      n = c + 1
      CvScalar.new(n | 1, n | 2, n | 3, n | 4)
    }

    # CvMat | CvMat with mask
    m3 = m1.or(m2, mask)
    assert_equal(m1.height, m3.height)
    assert_equal(m1.width, m3.width)
    assert_each_cvscalar(m3) { |j, i, c|
      n = c + 1
      if i < 3 and j < 2
        CvScalar.new(n | 1, n | 2, n | 3, n | 4)
      else
        CvScalar.new(n, n, n, n)
      end
    }
    
    # CvMat | CvScalar
    m3 = m1.or(s1)
    assert_equal(m1.height, m3.height)
    assert_equal(m1.width, m3.width)
    assert_each_cvscalar(m3) { |j, i, c|
      n = c + 1
      CvScalar.new(n | 1, n | 2, n | 3, n | 4)
    }

    # CvMat | CvScalar with mask
    m3 = m1.or(s1, mask)
    assert_equal(m1.height, m3.height)
    assert_equal(m1.width, m3.width)
    assert_each_cvscalar(m3) { |j, i, c|
      n = c + 1
      if i < 3 and j < 2
        CvScalar.new(n | 1, n | 2, n | 3, n | 4)
      else
        CvScalar.new(n, n, n, n)
      end
    }

    # Alias
    m3 = m1 | m2
    assert_equal(m1.height, m3.height)
    assert_equal(m1.width, m3.width)
    assert_each_cvscalar(m3) { |j, i, c|
      n = c + 1
      CvScalar.new(n | 1, n | 2, n | 3, n | 4)
    }

    m3 = m1 | s1
    assert_equal(m1.height, m3.height)
    assert_equal(m1.width, m3.width)
    assert_each_cvscalar(m3) { |j, i, c|
      n = c + 1
      CvScalar.new(n | 1, n | 2, n | 3, n | 4)
    }
  end


  def test_xor
    m1 = create_cvmat(6, 4)
    s1 = CvScalar.new(1, 2, 3, 4)
    m2 = create_cvmat(6, 4) { s1 }
    mask = create_cvmat(6, 4, :cv8u, 1) { |j, i, c|
      s = (i < 3 and j < 2) ? 1 : 0
      CvScalar.new(s)
    }
    
    # CvMat ^ CvMat
    m3 = m1.xor(m2)
    assert_equal(m1.height, m3.height)
    assert_equal(m1.width, m3.width)
    assert_each_cvscalar(m3) { |j, i, c|
      n = c + 1
      CvScalar.new(n ^ 1, n ^ 2, n ^ 3, n ^ 4)
    }

    # CvMat ^ CvMat with mask
    m3 = m1.xor(m2, mask)
    assert_equal(m1.height, m3.height)
    assert_equal(m1.width, m3.width)
    assert_each_cvscalar(m3) { |j, i, c|
      n = c + 1
      if i < 3 and j < 2
        CvScalar.new(n ^ 1, n ^ 2, n ^ 3, n ^ 4)
      else
        CvScalar.new(n, n, n, n)
      end
    }
    
    # CvMat ^ CvScalar
    m3 = m1.xor(s1)
    assert_equal(m1.height, m3.height)
    assert_equal(m1.width, m3.width)
    assert_each_cvscalar(m3) { |j, i, c|
      n = c + 1
      CvScalar.new(n ^ 1, n ^ 2, n ^ 3, n ^ 4)
    }

    # CvMat ^ CvScalar with mask
    m3 = m1.xor(s1, mask)
    assert_equal(m1.height, m3.height)
    assert_equal(m1.width, m3.width)
    assert_each_cvscalar(m3) { |j, i, c|
      n = c + 1
      if i < 3 and j < 2
        CvScalar.new(n ^ 1, n ^ 2, n ^ 3, n ^ 4)
      else
        CvScalar.new(n, n, n, n)
      end
    }

    # Alias
    m3 = m1 ^ m2
    assert_equal(m1.height, m3.height)
    assert_equal(m1.width, m3.width)
    assert_each_cvscalar(m3) { |j, i, c|
      n = c + 1
      CvScalar.new(n ^ 1, n ^ 2, n ^ 3, n ^ 4)
    }

    m3 = m1 ^ s1
    assert_equal(m1.height, m3.height)
    assert_equal(m1.width, m3.width)
    assert_each_cvscalar(m3) { |j, i, c|
      n = c + 1
      CvScalar.new(n ^ 1, n ^ 2, n ^ 3, n ^ 4)
    }
  end

  def test_not
    m1 = create_cvmat(6, 4, :cv8s)
    m2 = m1.not;
    m3 = m1.clone
    m3.not!
    [m2, m3].each { |m|
      assert_equal(m1.height, m.height)
      assert_equal(m1.width, m.width)
      assert_each_cvscalar(m) { |j, i, c|
        n = c + 1
        CvScalar.new(~n, ~n, ~n, ~n)
      }
    }
  end

  def test_eq
    m1 = create_cvmat(6, 4, :cv8u, 1) { |j, i, c|
      n = (c.even?) ? 10 : c
      CvScalar.new(n, 0, 0, 0)
    }
    m2 = create_cvmat(6, 4, :cv8u, 1) { |j, i, c|
      CvScalar.new(10, 0, 0, 0)
    }
    s1 = CvScalar.new(10, 0, 0, 0)
    m3 = m1.eq(m2)
    m4 = m1.eq(s1)
    m5 = m1.eq(10)

    [m3, m4, m5].each { |m|
      assert_equal(m1.height, m.height)
      assert_equal(m1.width, m.width)
      assert_each_cvscalar(m) { |j, i, c|
        n = (c.even?) ? 0xff : 0
        CvScalar.new(n, 0, 0, 0)
      }
    }
  end

  def test_gt
    m1 = create_cvmat(6, 4, :cv8u, 1) { |j, i, c|
      CvScalar.new(c, 0, 0, 0)
    }
    m2 = create_cvmat(6, 4, :cv8u, 1) { |j, i, c|
      CvScalar.new(10, 0, 0, 0)
    }
    s1 = CvScalar.new(10, 0, 0, 0)
    m3 = m1.gt(m2)
    m4 = m1.gt(s1)
    m5 = m1.gt(10)

    [m3, m4, m5].each { |m|
      assert_equal(m1.height, m.height)
      assert_equal(m1.width, m.width)
      assert_each_cvscalar(m) { |j, i, c|
        n = (c > 10) ? 0xff : 0
        CvScalar.new(n, 0, 0, 0)
      }
    }
  end

  def test_ge
    m1 = create_cvmat(6, 4, :cv8u, 1) { |j, i, c|
      CvScalar.new(c, 0, 0, 0)
    }
    m2 = create_cvmat(6, 4, :cv8u, 1) { |j, i, c|
      CvScalar.new(10, 0, 0, 0)
    }
    s1 = CvScalar.new(10, 0, 0, 0)
    m3 = m1.ge(m2)
    m4 = m1.ge(s1)
    m5 = m1.ge(10)

    [m3, m4, m5].each { |m|
      assert_equal(m1.height, m.height)
      assert_equal(m1.width, m.width)
      assert_each_cvscalar(m) { |j, i, c|
        n = (c >= 10) ? 0xff : 0
        CvScalar.new(n, 0, 0, 0)
      }
    }
  end

  def test_lt
    m1 = create_cvmat(6, 4, :cv8u, 1) { |j, i, c|
      CvScalar.new(c, 0, 0, 0)
    }
    m2 = create_cvmat(6, 4, :cv8u, 1) { |j, i, c|
      CvScalar.new(10, 0, 0, 0)
    }
    s1 = CvScalar.new(10, 0, 0, 0)
    m3 = m1.lt(m2)
    m4 = m1.lt(s1)
    m5 = m1.lt(10)

    [m3, m4, m5].each { |m|
      assert_equal(m1.height, m.height)
      assert_equal(m1.width, m.width)
      assert_each_cvscalar(m) { |j, i, c|
        n = (c < 10) ? 0xff : 0
        CvScalar.new(n, 0, 0, 0)
      }
    }
  end

  def test_le
    m1 = create_cvmat(6, 4, :cv8u, 1) { |j, i, c|
      CvScalar.new(c, 0, 0, 0)
    }
    m2 = create_cvmat(6, 4, :cv8u, 1) { |j, i, c|
      CvScalar.new(10, 0, 0, 0)
    }
    s1 = CvScalar.new(10, 0, 0, 0)
    m3 = m1.le(m2)
    m4 = m1.le(s1)
    m5 = m1.le(10)

    [m3, m4, m5].each { |m|
      assert_equal(m1.height, m.height)
      assert_equal(m1.width, m.width)
      assert_each_cvscalar(m) { |j, i, c|
        n = (c <= 10) ? 0xff : 0
        CvScalar.new(n, 0, 0, 0)
      }
    }
  end

  def test_in_range
    m0 = create_cvmat(6, 4, :cv8u, 1) { |j, i, c|
      CvScalar.new(c + 5, 0, 0, 0)
    }
    m1 = create_cvmat(6, 4, :cv8u, 1) { |j, i, c|
      CvScalar.new(10, 0, 0, 0)
    }
    m2 = create_cvmat(6, 4, :cv8u, 1) { |j, i, c|
      CvScalar.new(20, 0, 0, 0)
    }
    s1 = CvScalar.new(10, 0, 0, 0)
    s2 = CvScalar.new(20, 0, 0, 0)

    m3 = m0.in_range(m1, m2)
    m4 = m0.in_range(s1, s2)
    m5 = m0.in_range(10, 20)

    [m3, m4, m5].each { |m|
      assert_equal(m0.height, m.height)
      assert_equal(m0.width, m.width)
      assert_each_cvscalar(m) { |j, i, c|
        val = m0[j, i][0]
        n = ((val >= 10) and (val < 20)) ? 0xff : 0
        CvScalar.new(n, 0, 0, 0)
      }
    }
  end

  def test_abs_diff
    m0 = create_cvmat(6, 4, :cv32f, 4) { |j, i, c|
      CvScalar.new(-10 + 10.5, 20 + 10.5, -30 + 10.5, 40 - 10.5)
    }
    m1 = create_cvmat(6, 4, :cv32f, 4) { |j, i, c|
      CvScalar.new(c + 10.5, c - 10.5, c + 10.5, c - 10.5)
    }
    m2 = create_cvmat(6, 4, :cv32f, 4) { |j, i, c|
      CvScalar.new(c, c, c, c)
    }

    s1 = CvScalar.new(-10, 20, -30, 40)
    m3 = m1.abs_diff(m2)
    m4 = m0.abs_diff(s1)

    [m3, m4].each { |m|
      assert_equal(m1.width, m.width)
      assert_equal(m1.height, m.height)
      assert_each_cvscalar(m, 0.001) {
        CvScalar.new(10.5, 10.5, 10.5, 10.5)
      }
    }
  end

  def test_count_non_zero
    m0 = create_cvmat(6, 4, :cv32f, 1) { |j, i, c|
      n = 0
      n = 1 if i == 0
      CvScalar.new(n, 0, 0, 0)
    }
    assert_equal(6, m0.count_non_zero)
  end

  def test_sum
    m0 = create_cvmat(6, 4, :cv32f, 1) { |j, i, c|
      CvScalar.new(c, c, c, c)
    }
    assert_cvscalar_equal(CvScalar.new(276, 0, 0, 0), m0.sum)

    m0 = create_cvmat(6, 4, :cv32f, 1) { |j, i, c|
      CvScalar.new(-c)
    }
    assert_cvscalar_equal(CvScalar.new(-276, 0, 0, 0), m0.sum)
  end

  def test_avg_sdv
    m0 = create_cvmat(6, 4, :cv32f, 4) { |j, i, c|
      CvScalar.new(c * 0.1, -c * 0.1, c, -c)
    }
    assert_in_delta(CvScalar.new(1.15, -1.15, 11.5, -11.5), m0.avg, 0.001)
    assert_in_delta(CvScalar.new(0.69221, 0.69221, 6.9221, 6.9221), m0.sdv, 0.001)
    avg, sdv = m0.avg_sdv
    assert_in_delta(CvScalar.new(1.15, -1.15, 11.5, -11.5), avg, 0.001)
    assert_in_delta(CvScalar.new(0.69221, 0.69221, 6.9221, 6.9221), sdv, 0.001)

    mask = create_cvmat(6, 4, :cv8u, 1) { |j, i, c|
      n = (i == j) ? 1 : 0
      CvScalar.new(n)
    }
    assert_in_delta(CvScalar.new(0.75, -0.75, 7.5, -7.5), m0.avg(mask), 0.001)
    assert_in_delta(CvScalar.new(0.55901, 0.55901, 5.5901, 5.5901), m0.sdv(mask), 0.001)

    avg, sdv = m0.avg_sdv(mask)
    assert_in_delta(CvScalar.new(0.75, -0.75, 7.5, -7.5), avg, 0.001)
    assert_in_delta(CvScalar.new(0.55901, 0.55901, 5.5901, 5.5901), sdv, 0.001)
  end
end


