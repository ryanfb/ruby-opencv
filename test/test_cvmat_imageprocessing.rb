#!/usr/bin/env ruby
# -*- mode: ruby; coding: utf-8-unix -*-
require 'test/unit'
require 'opencv'
require File.expand_path(File.dirname(__FILE__)) + '/helper'

include OpenCV

# Tests for image processing functions of OpenCV::CvMat
class TestCvMat_imageprocessing < OpenCVTestCase
  FILENAME_LENA256x256 = File.expand_path(File.dirname(__FILE__)) + '/samples/lena-256x256.jpg'
  FILENAME_LENA32x32 = File.expand_path(File.dirname(__FILE__)) + '/samples/lena-32x32.jpg'

  def test_sobel
    mat0 = CvMat.load(FILENAME_LENA256x256, CV_LOAD_IMAGE_GRAYSCALE)

    mat1 = mat0.sobel(1, 0).convert_scale_abs(:scale => 1, :shift => 0)
    mat2 = mat0.sobel(0, 1).convert_scale_abs(:scale => 1, :shift => 0)
    mat3 = mat0.sobel(1, 1).convert_scale_abs(:scale => 1, :shift => 0)
    mat4 = mat0.sobel(1, 1, 3).convert_scale_abs(:scale => 1, :shift => 0)
    mat5 = mat0.sobel(1, 1, 5).convert_scale_abs(:scale => 1, :shift => 0)

    assert_equal('30a26b7287fac75bb697bc7eef6bb53a', hash_img(mat1))
    assert_equal('b740afb13b556d55280fa785190ac902', hash_img(mat2))
    assert_equal('36c29ca64a599e0f5633f4f3948ed858', hash_img(mat3))
    assert_equal('36c29ca64a599e0f5633f4f3948ed858', hash_img(mat4))
    assert_equal('30b9e8fd64e7f86c50fb67d8703628e3', hash_img(mat5))

    assert_equal(:cv16s, CvMat.new(16, 16, :cv8u, 1).sobel(1, 1).depth)
    assert_equal(:cv32f, CvMat.new(16, 16, :cv32f, 1).sobel(1, 1).depth)

    (DEPTH.keys - [:cv8u, :cv32f]).each { |depth|
      assert_raise(RuntimeError) {
        CvMat.new(3, 3, depth).sobel(1, 1)
      }
    }

    # Uncomment the following lines to view the images
    # snap(['original', mat0], ['sobel(1,0)', mat1], ['sobel(0,1)', mat2],
    #      ['sobel(1,1)', mat3], ['sobel(1,1,3)', mat4], ['sobel(1,1,5)', mat5])
  end

  def test_laplace
    mat0 = CvMat.load(FILENAME_LENA256x256, CV_LOAD_IMAGE_GRAYSCALE)

    mat1 = mat0.laplace.convert_scale_abs(:scale => 1, :shift => 0)
    mat2 = mat0.laplace(3).convert_scale_abs(:scale => 1, :shift => 0)
    mat3 = mat0.laplace(5).convert_scale_abs(:scale => 1, :shift => 0)

    assert_equal('824f8de75bfead5d83c4226f3948ce69', hash_img(mat1))
    assert_equal('824f8de75bfead5d83c4226f3948ce69', hash_img(mat2))
    assert_equal('23850bb8cfe9fd1b82cd73b7b4659369', hash_img(mat3))

    assert_equal(:cv16s, CvMat.new(16, 16, :cv8u, 1).laplace.depth)
    assert_equal(:cv32f, CvMat.new(16, 16, :cv32f, 1).laplace.depth)

    (DEPTH.keys - [:cv8u, :cv32f]).each { |depth|
      assert_raise(RuntimeError) {
        CvMat.new(3, 3, depth).laplace
      }
    }

    # Uncomment the following line to view the images
    # snap(['original', mat0], ['laplace', mat1], ['laplace(3)', mat2], ['laplace(5)', mat3])
  end

  def test_canny
    mat0 = CvMat.load(FILENAME_LENA256x256, CV_LOAD_IMAGE_GRAYSCALE)
    mat1 = mat0.canny(50, 200)
    mat2 = mat0.canny(50, 200, 3)
    mat3 = mat0.canny(50, 200, 5)

    assert_equal('ec3e88035bb98b5c5f1a08c8e07ab0a8', hash_img(mat1))
    assert_equal('ec3e88035bb98b5c5f1a08c8e07ab0a8', hash_img(mat2))
    assert_equal('1983a6d325d11eea3261462103b0dae1', hash_img(mat3))

    # Uncomment the following line to view the images
    # snap(['canny(50,200)', mat1], ['canny(50,200,3)', mat2], ['canny(50,200,5)', mat3])
  end

  def test_pre_corner_detect
    mat0 = CvMat.load(FILENAME_LENA256x256, CV_LOAD_IMAGE_GRAYSCALE)
    mat1 = mat0.pre_corner_detect
    mat2 = mat0.pre_corner_detect(3)
    mat3 = mat0.pre_corner_detect(5)

    assert_equal('fe7c8a1d07a3dd0fb6a02d6a6de0fe9f', hash_img(mat1))
    assert_equal('fe7c8a1d07a3dd0fb6a02d6a6de0fe9f', hash_img(mat2))
    assert_equal('42e7443ffd389d15343d3c6bdc42f553', hash_img(mat3))

    # Uncomment the following lines to show the images
    # snap(['original', mat0], ['pre_coner_detect', mat1],
    #      ['pre_coner_detect(3)', mat2], ['pre_coner_detect(5)', mat3])
  end

  def test_corner_eigenvv
    mat0 = CvMat.load(FILENAME_LENA256x256, CV_LOAD_IMAGE_GRAYSCALE)
    mat1 = mat0.corner_eigenvv(3)
    mat2 = mat0.corner_eigenvv(3, 3)

    flunk('FIXME: CvMat#corner_eigenvv is not tested yet.')
  end

  def test_corner_min_eigen_val
    mat0 = CvMat.load(FILENAME_LENA256x256, CV_LOAD_IMAGE_GRAYSCALE)
    mat1 = mat0.corner_min_eigen_val(3)
    mat2 = mat0.corner_min_eigen_val(3, 3)

    flunk('FIXME: CvMat#corner_min_eigen_val is not tested yet.')
  end

  def test_corner_harris
    mat0 = CvMat.load(FILENAME_LENA256x256, CV_LOAD_IMAGE_GRAYSCALE)
    mat1 = mat0.corner_harris(3)
    mat2 = mat0.corner_harris(3, 3)
    mat3 = mat0.corner_harris(3, 3, 0.04)
    mat4 = mat0.corner_harris(3, 7, 0.01)

    assert_equal('6ceb54b54cc98a72de7cb75649fb0a12', hash_img(mat1))
    assert_equal('6ceb54b54cc98a72de7cb75649fb0a12', hash_img(mat2))
    assert_equal('6ceb54b54cc98a72de7cb75649fb0a12', hash_img(mat3))
    assert_equal('4e703deb9a418bbf37e3283f4a7d4d32', hash_img(mat4))

    # Uncomment the following lines to show the images
    # snap(['original', mat0], ['corner_harris(3)', mat1], ['corner_harris(3,3)', mat2],
    #      ['corner_harris(3,3,0.04)', mat3], ['corner_harris(3,7,0.01)', mat4])
  end

  def test_find_corner_sub_pix
    flunk('FIXME: CvMat#find_corner_sub_pix is not implemented yet.')
  end

  def test_good_features_to_track
    mat0 = CvMat.load(FILENAME_LENA32x32, CV_LOAD_IMAGE_GRAYSCALE)
    mask = create_cvmat(mat0.rows, mat0.cols, :cv8u, 1) { |j, i, c|
      if (i > 8 and i < 18) and (j > 8 and j < 18)
        CvScalar.new(1)
      else
        CvScalar.new(0)
      end
    }

    corners1 = mat0.good_features_to_track(0.2, 5)
    corners2 = mat0.good_features_to_track(0.2, 5, :mask => mask)
    corners3 = mat0.good_features_to_track(0.2, 5, :block_size => 7)
    corners4 = mat0.good_features_to_track(0.2, 5, :use_harris => true)
    corners5 = mat0.good_features_to_track(0.2, 5, :k => 0.01)
    corners6 = mat0.good_features_to_track(0.2, 5, :max => 1)

    expected1 = [[24, 7], [20, 23], [17, 11], [26, 29], [30, 24],
                 [19, 16], [28, 2], [13, 18], [14, 4]]
    assert_equal(expected1.size, corners1.size)
    expected1.each_with_index { |e, i|
      assert_equal(e[0], corners1[i].x.to_i)
      assert_equal(e[1], corners1[i].y.to_i)
    }
    expected2 = [[17, 11], [17, 16]]
    assert_equal(expected2.size, corners2.size)
    expected2.each_with_index { |e, i|
      assert_equal(e[0], corners2[i].x.to_i)
      assert_equal(e[1], corners2[i].y.to_i)
    }

    expected3 = [[21, 7], [22, 23], [18, 12], [28, 4], [28, 26],
                 [17, 27], [13, 20], [10, 11], [14, 5]]
    assert_equal(expected3.size, corners3.size)
    expected3.each_with_index { |e, i|
      assert_equal(e[0], corners3[i].x.to_i)
      assert_equal(e[1], corners3[i].y.to_i)
    }

    expected4 = [[24, 8], [20, 23], [16, 11],
                 [20, 16],[27, 28], [28, 2]]
    assert_equal(expected4.size, corners4.size)
    expected4.each_with_index { |e, i|
      assert_equal(e[0], corners4[i].x.to_i)
      assert_equal(e[1], corners4[i].y.to_i)
    }

    expected5 = [[24, 7], [20, 23], [17, 11], [26, 29], [30, 24],
                 [19, 16], [28, 2], [13, 18], [14, 4]]
    assert_equal(expected5.size, corners5.size)
    expected5.each_with_index { |e, i|
      assert_equal(e[0], corners5[i].x.to_i)
      assert_equal(e[1], corners5[i].y.to_i)
    }

    assert_equal(1, corners6.size)
    assert_equal(24, corners6[0].x.to_i)
    assert_equal(7, corners6[0].y.to_i)

    assert_raise(ArgumentError) {
      mat0.good_features_to_track(0.2, 5, :max => 0)
    }
  end

  def test_sample_line
    flunk('FIXME: CvMat#sample_line is not implemented yet.')
  end

  def test_rect_sub_pix
    mat0 = CvMat.load(FILENAME_LENA256x256, CV_LOAD_IMAGE_ANYCOLOR | CV_LOAD_IMAGE_ANYDEPTH)
    center = CvPoint2D32f.new(mat0.width / 2, mat0.height / 2)
    mat1 = mat0.rect_sub_pix(center)
    mat2 = mat0.rect_sub_pix(center, mat0.size)
    mat3 = mat0.rect_sub_pix(center, CvSize.new(512, 512))

    assert_equal('b3dc0e31260dd42b5341471e23e825d3', hash_img(mat1))
    assert_equal('b3dc0e31260dd42b5341471e23e825d3', hash_img(mat2))
    assert_equal('cc27ce8f4068efedcd31c4c782c3825c', hash_img(mat3))
  end

  def test_quadrangle_sub_pix
    mat0 = CvMat.load(FILENAME_LENA256x256, CV_LOAD_IMAGE_ANYCOLOR | CV_LOAD_IMAGE_ANYDEPTH)
    angle = 60 * Math::PI / 180
    map_matrix = CvMat.new(2, 3, :cv32f, 1)
    map_matrix[0] = CvScalar.new(Math.cos(angle))
    map_matrix[1] = CvScalar.new(-Math.sin(angle))
    map_matrix[2] = CvScalar.new(mat0.width * 0.5)
    map_matrix[3] = CvScalar.new(-map_matrix[1][0])
    map_matrix[4] = map_matrix[0]
    map_matrix[5] = CvScalar.new(mat0.height * 0.5)

    mat1 = mat0.quadrangle_sub_pix(map_matrix)
    mat2 = mat0.quadrangle_sub_pix(map_matrix, mat0.size)
    mat3 = mat0.quadrangle_sub_pix(map_matrix, CvSize.new(512, 512))

    assert_equal('f170c05fa50c3ac2a762d7b3f5c4ae2f', hash_img(mat1))
    assert_equal('f170c05fa50c3ac2a762d7b3f5c4ae2f', hash_img(mat2))
    assert_equal('4d949d5083405381ad9ea09dcd95e5a2', hash_img(mat3))
  end

  def test_resize
    mat0 = CvMat.load(FILENAME_LENA256x256, CV_LOAD_IMAGE_ANYCOLOR | CV_LOAD_IMAGE_ANYDEPTH)
    mat1 = mat0.resize(CvSize.new(512, 512))
    mat2 = mat0.resize(CvSize.new(512, 512), :linear)
    mat3 = mat0.resize(CvSize.new(512, 512), :nn)
    mat4 = mat0.resize(CvSize.new(128, 128), :area)
    mat5 = mat0.resize(CvSize.new(128, 128), :cubic)
    mat6 = mat0.clone

    assert_equal('b2203ccca2c17b042a90b79704c0f535', hash_img(mat1))
    assert_equal('b2203ccca2c17b042a90b79704c0f535', hash_img(mat2))
    assert_equal('ba8f2dee2329aaa6309de4770fc8fa55', hash_img(mat3))
    assert_equal('8a28a2748b0cfc87205d65c625187867', hash_img(mat4))
    assert_equal('de5c30fcd9e817aa282ab05388de995b', hash_img(mat5))
  end

  def test_warp_affine
    mat0 = CvMat.load(FILENAME_LENA256x256, CV_LOAD_IMAGE_ANYCOLOR | CV_LOAD_IMAGE_ANYDEPTH)
    map_matrix = CvMat.new(2, 3, :cv32f, 1)
    # center: (128, 128), angle: 25 deg., scale: 1.0
    map_matrix[0] = CvScalar.new(0.90631)
    map_matrix[1] = CvScalar.new(0.42262)
    map_matrix[2] = CvScalar.new(-42.10254)
    map_matrix[3] = CvScalar.new(-0.42262)
    map_matrix[4] = CvScalar.new(0.90631)
    map_matrix[5] = CvScalar.new(66.08774)

    mat1 = mat0.warp_affine(map_matrix)
    mat2 = mat0.warp_affine(map_matrix, :nn)
    mat3 = mat0.warp_affine(map_matrix, :linear, :fill_outliers, CvColor::Yellow)
    mat4 = mat0.warp_affine(map_matrix, :linear, :inverse_map)

    assert_equal('da3d7cdefabbaf84c4080ecd40d00897', hash_img(mat1))
    assert_equal('b4abcd12c4e1103c3de87bf9ad854936', hash_img(mat2))
    assert_equal('26f6b10e955125c91fd7e63a63cc06a3', hash_img(mat3))
    assert_equal('cc4eb5d8eb7cb2c0b76941bc38fb91b1', hash_img(mat4))

    assert_raise(TypeError) {
      mat0.warp_affine("foobar")
    }
  end

  def test_rotation_matrix2D
    mat1 = CvMat.rotation_matrix2D(CvPoint2D32f.new(10, 20), 60, 2.0)
    expected = [1.0, 1.73205, -34.64102,
                -1.73205, 1.0, 17.32051]
    assert_equal(2, mat1.rows)
    assert_equal(3, mat1.cols)
    assert_equal(:cv32f, mat1.depth)
    assert_equal(1, mat1.channel)
    expected.each_with_index { |x, i|
      assert_in_delta(x, mat1[i][0], 0.001)
    }
  end

  def test_warp_perspective
    mat0 = CvMat.load(FILENAME_LENA256x256, CV_LOAD_IMAGE_ANYCOLOR | CV_LOAD_IMAGE_ANYDEPTH)
    # Homography
    #   <src>     =>    <dst>
    # (0, 0)      =>  (50, 0)
    # (255, 0)    =>  (205, 0)
    # (255, 255)  =>  (255, 220)
    # (0, 255)    =>  (0, 275)
    map_matrix = CvMat.new(3, 3, :cv32f, 1)
    map_matrix[0] = CvScalar.new(0.72430)
    map_matrix[1] = CvScalar.new(-0.19608) 
    map_matrix[2] = CvScalar.new(50.00000) 
    map_matrix[3] = CvScalar.new(0.0) 
    map_matrix[4] = CvScalar.new(0.62489)
    map_matrix[5] = CvScalar.new(0.0)
    map_matrix[6] = CvScalar.new(0.00057)
    map_matrix[7] = CvScalar.new(-0.00165)
    map_matrix[8] = CvScalar.new(1.00000)
    
    mat1 = mat0.warp_perspective(map_matrix)
    mat2 = mat0.warp_perspective(map_matrix, :nn)
    mat3 = mat0.warp_perspective(map_matrix, :linear, :inverse_map)
    mat4 = mat0.warp_perspective(map_matrix, :linear, :fill_outliers, CvColor::Yellow)

    assert_equal('bba3a5395f9dd9a400a0083ae74d8986', hash_img(mat1))
    assert_equal('a0cc4f329f459410293b75b417fc4f25', hash_img(mat2))
    assert_equal('3e34e6ed2404056bb72e86edf02610cb', hash_img(mat3))
    assert_equal('71bd12857d2e4ac0c919652c2963b4e1', hash_img(mat4))

    assert_raise(TypeError) {
      mat0.warp_perspective("foobar")
    }
  end

  def test_find_homography
    # Nx2
    src = CvMat.new(4, 2, :cv32f, 1)
    dst = CvMat.new(4, 2, :cv32f, 1)

    # Nx3 (Homogeneous coordinates)
    src2 = CvMat.new(4, 3, :cv32f, 1)
    dst2 = CvMat.new(4, 3, :cv32f, 1)
    
    # Homography
    #   <src>     =>    <dst>
    # (0, 0)      =>  (50, 0)
    # (255, 0)    =>  (205, 0)
    # (255, 255)  =>  (255, 220)
    # (0, 255)    =>  (0, 275)
    [[0, 0], [255, 0], [255, 255], [0, 255]].each_with_index { |coord, i|
      src[i, 0] = coord[0]
      src[i, 1] = coord[1]

      src2[i, 0] = coord[0] * 2
      src2[i, 1] = coord[1] * 2
      src2[i, 2] = 2
    }
    [[50, 0], [205, 0], [255, 220], [0, 275]].each_with_index { |coord, i|
      dst[i, 0] = coord[0]
      dst[i, 1] = coord[1]

      dst2[i, 0] = coord[0] * 2
      dst2[i, 1] = coord[1] * 2
      dst2[i, 2] = 2
    }

    mat1 = CvMat.find_homography(src, dst)
    mat2 = CvMat.find_homography(src, dst, :all)
    mat3 = CvMat.find_homography(src, dst, :ransac)
    mat4 = CvMat.find_homography(src, dst, :lmeds)
    mat5, status5 = CvMat.find_homography(src, dst, :ransac, 5, true)
    mat6, status6 = CvMat.find_homography(src, dst, :ransac, 5, true)
    mat7 = CvMat.find_homography(src, dst, :ransac, 5, false)
    mat8 = CvMat.find_homography(src, dst, :ransac, 5, nil)
    mat9 = CvMat.find_homography(src, dst, :all, 5, true)
    mat10, status10 = CvMat.find_homography(src2, dst2, :ransac, 5, true)

    [mat1, mat2, mat3, mat4, mat5, mat6, mat7, mat8, mat9, mat10].each { |mat|
      assert_equal(3, mat.rows)
      assert_equal(3, mat.cols)
      assert_equal(:cv32f, mat.depth)
      assert_equal(1, mat.channel)
      [0.72430, -0.19608, 50.0,
       0.0, 0.62489, 0.0,
       0.00057, -0.00165, 1.0].each_with_index { |x, i|
        assert_in_delta(x, mat[i][0], 0.0001)
      }
    }
    
    [status5, status6, status10].each { |status|
      assert_equal(1, status.rows)
      assert_equal(4, status.cols)
      assert_equal(:cv8u, status.depth)      
      assert_equal(1, status.channel)
      4.times { |i|
        assert_in_delta(1.0, status[i][0], 0.0001)
      }
    }
  end

  def test_remap
    mat0 = CvMat.load(FILENAME_LENA256x256, CV_LOAD_IMAGE_ANYCOLOR | CV_LOAD_IMAGE_ANYDEPTH)
    matx = CvMat.new(mat0.height, mat0.width, :cv32f, 1).clear
    maty = CvMat.new(mat0.height, mat0.width, :cv32f, 1).clear

    cos30, sin30 = Math.cos(30 * Math::PI / 180), Math.sin(30 * Math::PI / 180)
    half_width, half_height = mat0.width / 2, mat0.height / 2
    mat0.height.times { |j|
      mat0.width.times { |i|
        x0 = i - half_width
        y0 = j - half_height
        x = x0 * cos30 - y0 * sin30 + half_width
        y = x0 * sin30 + y0 * cos30 + half_height
        matx[j, i] = CvScalar.new(x)
        maty[j, i] = CvScalar.new(y)
      }
    }

    mat1 = mat0.remap(matx, maty)
    mat2 = mat0.remap(matx, maty, :nn)
    mat3 = mat0.remap(matx, maty, :linear, :fill_outliers, CvColor::Yellow)

    assert_equal('586716c0262a3e03a54b9fc6e671e5f7', hash_img(mat1))
    assert_equal('5461ecdee23d5e8a9099500d631c9f0f', hash_img(mat2))
    assert_equal('1f6b73925056298c566e8e727627d929', hash_img(mat3))

    assert_raise(TypeError) {
      mat0.remap('foo', maty)
    }
    assert_raise(TypeError) {
      mat0.remap(matx, 'bar')
    }
  end

  def test_log_polar
    flunk('FIXME: CvMat#log_polar is not implemented yet.')
  end

  def test_erode
    mat0 = create_cvmat(9, 9, :cv8u, 1) { |j, i, c|
      if i >= 3 and i < 6 and j >= 3 and j < 6
        CvScalar.new(255)
      else
        CvScalar.new(0)
      end
    }

    mat1 = create_cvmat(9, 9, :cv8u, 1) { |j, i, c|
      if i >= 1 and i < 8 and j >= 1 and j < 8
        CvScalar.new(255)
      else
        CvScalar.new(0)
      end
    }

    mat2 = create_cvmat(5, 5, :cv8u, 1) { |j, i, c|
      if i == 2 or j == 2
        CvScalar.new(255)
      else
        CvScalar.new(0)
      end
    }

    mat3 = mat0.erode
    mat4 = mat0.erode(nil, 1)
    mat5 = mat1.erode(nil, 2)
    mat6 = mat1.erode(IplConvKernel.new(5, 5, 2, 2, :cross))
    mat7 = mat0.clone
    mat7.erode!
    
    assert_equal('075eb0e281328f768eb862735d16979d', hash_img(mat3))
    assert_equal('075eb0e281328f768eb862735d16979d', hash_img(mat4))
    assert_equal('9f02fc4438b1d69fea75a10dfd2b66b0', hash_img(mat5))
    assert_equal('9f02fc4438b1d69fea75a10dfd2b66b0', hash_img(mat6))
    assert_equal('075eb0e281328f768eb862735d16979d', hash_img(mat7))
  end

  def test_dilate
    mat0 = create_cvmat(9, 9, :cv8u, 1) { |j, i, c|
      if i == 4 and j == 4
        CvScalar.new(255)
      else
        CvScalar.new(0)
      end
    }

    mat1 = create_cvmat(5, 5, :cv8u, 1) { |j, i, c|
      if i == 2 or j == 2
        CvScalar.new(255)
      else
        CvScalar.new(0)
      end
    }

    mat2 = mat0.dilate
    mat3 = mat0.dilate(nil, 1)
    mat4 = mat0.dilate(nil, 2)
    mat5 = mat1.dilate(IplConvKernel.new(5, 5, 2, 2, :cross))
    mat6 = mat0.clone
    mat6.dilate!

    assert_equal('9f02fc4438b1d69fea75a10dfd2b66b0', hash_img(mat2))
    assert_equal('9f02fc4438b1d69fea75a10dfd2b66b0', hash_img(mat3))
    assert_equal('ebf07f2a0edd2fd0fe26ff5921c6871b', hash_img(mat4))
    assert_equal('2841937c35c311e947bee49864b9d295', hash_img(mat5))
    assert_equal('9f02fc4438b1d69fea75a10dfd2b66b0', hash_img(mat6))
  end

  def test_morphology_open
    mat0 = create_cvmat(64, 64, :cv8u, 1) { |j, i, c|
      if i >= 8 and i < 56 and j >= 8 and j < 56 and (i + j) % 15 != 0
        CvScalar.new(255)
      else
        CvScalar.new(0)
      end
    }

    mat1 = mat0.morphology_open
    mat2 = mat0.morphology_open(nil, 2)
    kernel = IplConvKernel.new(5, 5, 2, 2, :cross)
    mat3 = mat0.morphology_open(kernel)

    assert_equal('165c36ad069db33735f0d4c2823f43b7', hash_img(mat1))
    assert_equal('e5af47b2827ed20450222321c1678ed3', hash_img(mat2))
    assert_equal('63ccb07cb93efb1563657f51e3d89252', hash_img(mat3))
  end

  def test_morphology_close
    mat0 = create_cvmat(64, 64, :cv8u, 1) { |j, i, c|
      if i >= 8 and i < 56 and j >= 8 and j < 56 and (i + j) % 15 != 0
        CvScalar.new(255)
      else
        CvScalar.new(0)
      end
    }

    mat1 = mat0.morphology_close
    mat2 = mat0.morphology_close(nil, 2)
    kernel = IplConvKernel.new(5, 5, 2, 2, :cross)
    mat3 = mat0.morphology_close(kernel)

    assert_equal('752914aae1cff07a2b3ce528b6ac3332', hash_img(mat1))
    assert_equal('0908b8f98999a198e8a1fbc743de52e5', hash_img(mat2))
    assert_equal('831c513d6ed86bce3f15c697de4a72f8', hash_img(mat3))
  end

  def test_morphology_gradient
    mat0 = create_cvmat(64, 64, :cv8u, 1) { |j, i, c|
      if i >= 8 and i < 56 and j >= 8 and j < 56 and (i + j) % 15 != 0
        CvScalar.new(255)
      else
        CvScalar.new(0)
      end
    }

    mat1 = mat0.morphology_gradient
    mat2 = mat0.morphology_gradient(nil, 2)
    kernel = IplConvKernel.new(5, 5, 2, 2, :cross)
    mat3 = mat0.morphology_gradient(kernel)

    assert_equal('e15d131ae29a58237f8d9a89669c3a47', hash_img(mat1))
    assert_equal('31d158672f699f961c59908e0bd72d5c', hash_img(mat2))
    assert_equal('1e8007c211d6f464cf8584e8e83b3c35', hash_img(mat3))
  end

  def test_morphology_tophat
    mat0 = create_cvmat(64, 64, :cv8u, 1) { |j, i, c|
      if i >= 8 and i < 56 and j >= 8 and j < 56 and (i + j) % 15 != 0
        CvScalar.new(255)
      else
        CvScalar.new(0)
      end
    }

    mat1 = mat0.morphology_tophat
    mat2 = mat0.morphology_tophat(nil, 2)
    kernel = IplConvKernel.new(5, 5, 2, 2, :cross)
    mat3 = mat0.morphology_tophat(kernel)

    assert_equal('26f89a4f449ec8328499960acbfd44f2', hash_img(mat1))
    assert_equal('102833c2e96eaa706eea5854d2aeaf5a', hash_img(mat2))
    assert_equal('1760c5b63a52df37069164fe3e901aa4', hash_img(mat3))
  end

  def test_morphology_blackhat
    mat0 = create_cvmat(64, 64, :cv8u, 1) { |j, i, c|
      if i >= 8 and i < 56 and j >= 8 and j < 56 and (i + j) % 15 != 0
        CvScalar.new(255)
      else
        CvScalar.new(0)
      end
    }

    mat1 = mat0.morphology_blackhat
    mat2 = mat0.morphology_blackhat(nil, 2)
    kernel = IplConvKernel.new(5, 5, 2, 2, :cross)
    mat3 = mat0.morphology_blackhat(kernel)

    assert_equal('3773d2802aad82c91ea8e14a324e5fc3', hash_img(mat1))
    assert_equal('3fc6bc283fa952e1fd566944d94b3e9a', hash_img(mat2))
    assert_equal('18b1d51637b912a38133341ee006c6ff', hash_img(mat3))
  end

  def test_smooth_blur_no_scale
    mat0 = CvMat.load(FILENAME_LENA32x32, CV_LOAD_IMAGE_GRAYSCALE)

    mat1 = mat0.smooth_blur_no_scale
    mat2 = mat0.smooth_blur_no_scale(3, 3)
    mat3 = mat0.smooth_blur_no_scale(7, 7)
    mat4 = CvMat.new(32, 32, :cv32f, 1).smooth_blur_no_scale

    [mat1, mat2, mat3].each { |m|
      assert_equal(1, m.channel)
      assert_equal(:cv16u, m.depth)
    }
    assert_equal(1, mat4.channel)
    assert_equal(:cv32f, mat4.depth)

    assert_equal('3c9074c87b65117798f48e41a17b2f30', hash_img(mat1))
    assert_equal('3c9074c87b65117798f48e41a17b2f30', hash_img(mat2))
    assert_equal('9c549aa406a425a65b036c2f9a2689e0', hash_img(mat3))
  end

  def test_smooth_blur
    mat0 = CvMat.load(FILENAME_LENA32x32, CV_LOAD_IMAGE_GRAYSCALE)

    mat1 = mat0.smooth_blur
    mat2 = mat0.smooth_blur(3, 3)
    mat3 = mat0.smooth_blur(7, 7)
    mat4 = CvMat.new(32, 32, :cv16u, 1).smooth_blur
    mat5 = CvMat.new(32, 32, :cv32f, 1).smooth_blur
    mat6 = CvMat.new(32, 32, :cv8u, 3).smooth_blur

    [mat1, mat2, mat3].each { |m|
      assert_equal(1, m.channel)
      assert_equal(:cv8u, m.depth)
    }
    assert_equal(1, mat4.channel)
    assert_equal(:cv16u, mat4.depth)
    assert_equal(1, mat5.channel)
    assert_equal(:cv32f, mat5.depth)
    assert_equal(3, mat6.channel)
    assert_equal(:cv8u, mat6.depth)

    assert_equal('f2473b5b964ae8950f6a7fa5cde4c67a', hash_img(mat1))
    assert_equal('f2473b5b964ae8950f6a7fa5cde4c67a', hash_img(mat2))
    assert_equal('d7bb344fc0f6ec0da4b9754d319e4e4a', hash_img(mat3))
  end

  def test_smooth_gaussian
    mat0 = CvMat.load(FILENAME_LENA32x32, CV_LOAD_IMAGE_GRAYSCALE)
    
    mat1 = mat0.smooth_gaussian
    mat2 = mat0.smooth_gaussian(3, 3)
    mat3 = mat0.smooth_gaussian(3, 3, 3)
    mat4 = mat0.smooth_gaussian(3, 3, 3, 3)
    mat5 = mat0.smooth_gaussian(7, 7, 5, 3)

    mat6 = CvMat.new(32, 32, :cv16u, 1).smooth_gaussian
    mat7 = CvMat.new(32, 32, :cv32f, 1).smooth_gaussian
    mat8 = CvMat.new(32, 32, :cv8u, 3).smooth_gaussian

    [mat1, mat2, mat3, mat4, mat5].each { |m|
      assert_equal(1, m.channel)
      assert_equal(:cv8u, m.depth)
    }
    assert_equal(1, mat6.channel)
    assert_equal(:cv16u, mat6.depth)
    assert_equal(1, mat7.channel)
    assert_equal(:cv32f, mat7.depth)
    assert_equal(3, mat8.channel)
    assert_equal(:cv8u, mat8.depth)

    assert_equal('580c88f3e0e317a5770be3f28f31eda2', hash_img(mat1))
    assert_equal('580c88f3e0e317a5770be3f28f31eda2', hash_img(mat2))
    assert_equal('a1ffaa14522719e37d75eec18ff8b309', hash_img(mat3))
    assert_equal('a1ffaa14522719e37d75eec18ff8b309', hash_img(mat4))
    assert_equal('f7f8b4eff3240ffc8f259ce975936d92', hash_img(mat5))
  end

  def test_smooth_median
    mat0 = create_cvmat(64, 64, :cv8u, 1) { |j, i, c|
      if (i + j) % 15 != 0
        CvScalar.new(255)
      else
        CvScalar.new(0)
      end
    }
    (-1..1).each { |dy|
      (-1..1).each { |dx|
        mat0[32 + dy, 32 + dx] = CvScalar.new(0)
      }
    }
    
    mat1 = mat0.smooth_median
    mat2 = mat0.smooth_median(3)
    mat3 = mat0.smooth_median(7)
    mat4 = CvMat.new(64, 64, :cv8u, 3).smooth_median

    assert_equal('7343a41c542e034db356636c06134961', hash_img(mat1))
    assert_equal('7343a41c542e034db356636c06134961', hash_img(mat2))
    assert_equal('6ae59e64850377ee5470c854761551ea', hash_img(mat3))
  end

  def test_smooth_bilateral
    flunk('FIXME: CvMat#smooth_bilateral is not tested yet.')
    mat0 = create_cvmat(64, 64, :cv8u, 1) { |j, i, c|
      if i > 32
        (i + j) % 15 != 0 ? CvScalar.new(32) : CvScalar.new(224)
      else
        (i + j) % 15 != 0 ? CvScalar.new(224) : CvScalar.new(32)
      end
    }

    mat1 = mat0.smooth_bilateral
    mat2 = mat0.smooth_bilateral(3, 3)
    mat3 = mat0.smooth_bilateral(7, 7)
    mat4 = CvMat.new(64, 64, :cv8u, 3).smooth_bilateral
  end
end

