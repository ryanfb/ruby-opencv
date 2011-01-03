#!/usr/bin/env ruby
# -*- mode: ruby; coding: utf-8-unix -*- 
require 'test/unit'
require 'digest/md5'
require 'opencv'
require File.expand_path(File.dirname(__FILE__)) + '/helper'

include OpenCV

# Tests to run first; check the handful of basic operations that the later tests rely on
class TestPreliminary < OpenCVTestCase
  def test_assert_array_equal
    assert_array_equal([1, 2, 3, 4], [1, 2, 3, 4])

    # Uncomment the following line to check the fail case
    # assert_array_equal([1, 2, 3, 4], [1, 2, 3, 0])
  end

  def test_assert_cvscalar_equal
    assert_cvscalar_equal(CvScalar.new(1, 2, 3, 4), CvScalar.new(1, 2, 3, 4))
    assert_cvscalar_equal(CvScalar.new(0.1, 0.2, 0.3, 0.4), CvScalar.new(0.1, 0.2, 0.3, 0.4))

    # Uncomment the following lines to check the fail cases
    # assert_cvscalar_equal(CvScalar.new(1, 2, 3, 4), CvScalar.new(1, 2, 3, 0))
    # assert_cvscalar_equal(CvScalar.new(0.1, 0.2, 0.3, 0.4), CvScalar.new(0.1, 0.2, 0.3, 0.0))
  end
  
  def test_assert_in_delta
    assert_in_delta(1, 0.9999, 0.1)
    assert_in_delta(CvScalar.new(1, 2, 3, 4), CvScalar.new(1.01, 2.01, 3.01, 4.01), 0.1)
    assert_in_delta(CvScalar.new(1, 2, 3, 4), [1.01, 2.01, 3.01, 4.01], 0.1)
    assert_in_delta([1, 2, 3, 4], CvScalar.new(1.01, 2.01, 3.01, 4.01), 0.1)
    assert_in_delta([1, 2, 3, 4], [1.01, 2.01, 3.01, 4.01], 0.1)

    # Uncomment the following lines to check the fail cases
    # assert_in_delta(1, 0.009, 0.1)
    # assert_in_delta(CvScalar.new(1, 2, 3, 4), CvScalar.new(1.01, 2.01, 3.01, 4.01), 0.001)
    # assert_in_delta(CvScalar.new(1, 2, 3, 4), [1.01, 2.01, 3.01, 4.01], 0.001)
    # assert_in_delta([1, 2, 3, 4], CvScalar.new(1.01, 2.01, 3.01, 4.01), 0.001)
    # assert_in_delta([1, 2, 3, 4], [1.01, 2.01, 3.01, 4.01], 0.001)
  end

  def test_assert_each_cvscalar
    mat1 = CvMat.new(5, 5, :cv32f, 4)
    mat2 = CvMat.new(5, 5, :cv32f, 4)
    c = 0
    mat1.height.times { |j|
      mat1.width.times { |i|
        mat1[j, i] = CvScalar.new(c * 0.1, c * 0.2, c * 0.3, c * 0.4)
        mat2[j, i] = CvScalar.new(c, c, c, c)
        c += 1
      }
    }
    
    assert_each_cvscalar(mat1, 0.001) { |j, i, n|
      CvScalar.new(n * 0.1, n * 0.2, n * 0.3, n * 0.4)
    }
    assert_each_cvscalar(mat2) { |j, i, n|
      CvScalar.new(n, n, n, n)
    }
    
    # Uncomment the following lines to check the fail cases
    # assert_each_cvscalar(mat1, 0.001) { |j, i, n|
    #   CvScalar.new(n * 0.1, n * 0.2, n * 0.3, 0)
    # }
    # assert_each_cvscalar(mat1, 0.001) { |j, i, n|
    #   CvScalar.new(1, 2, 3, 4)
    # }
    # assert_each_cvscalar(mat2) { |j, i, n|
    #   CvScalar.new(n * 0.1, n * 0.2, n * 0.3, 0)
    # }
    # assert_each_cvscalar(mat2) { |j, i, n|
    #   CvScalar.new(1, 2, 3, 0)
    # }
  end


  def test_create_cvmat
    mat = create_cvmat(3, 4)
    assert_equal(3, mat.height)
    assert_equal(4, mat.width)
    assert_equal(:cv8u, mat.depth)
    assert_equal(4, mat.channel)
    c = 0
    mat.height.times { |j|
      mat.width.times { |i|
        s = CvScalar.new(c + 1, c + 1, c + 1, c + 1)
        assert_cvscalar_equal(s, mat[j, i])
        c += 1
      }
    }

    mat = create_cvmat(2, 3, :cv16s, 2)
    assert_equal(2, mat.height)
    assert_equal(3, mat.width)
    assert_equal(:cv16s, mat.depth)
    assert_equal(2, mat.channel)
    c = 0
    mat.height.times { |j|
      mat.width.times { |i|
        s = CvScalar.new(c + 1, c + 1, 0, 0)
        assert_cvscalar_equal(s, mat[j, i])
        c += 1
      }
    }

    mat = create_cvmat(2, 3, :cv16u, 3) { |j, i, c|
      n = j + i + c
      CvScalar.new(n, n, n, 0)
    }
    assert_equal(2, mat.height)
    assert_equal(3, mat.width)
    assert_equal(:cv16u, mat.depth)
    assert_equal(3, mat.channel)
    c = 0
    mat.height.times { |j|
      mat.width.times { |i|
        n = j + i + c
        assert_cvscalar_equal(CvScalar.new(n, n, n, 0), mat[j, i])
        c += 1
      }
    }
  end
  
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
end

