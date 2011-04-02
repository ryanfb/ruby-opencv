#!/usr/bin/env ruby
# -*- mode: ruby; coding: utf-8-unix -*-
require 'test/unit'
require 'opencv'
require File.expand_path(File.dirname(__FILE__)) + '/helper'

include OpenCV

# Tests for image processing functions of OpenCV::CvMat
class TestCvMat_imageprocessing < OpenCVTestCase
  FILENAME_LENA256x256 = File.expand_path(File.dirname(__FILE__)) + '/samples/lena-256x256.jpg'
  FILENAME_LENA_INPAINT = File.expand_path(File.dirname(__FILE__)) + '/samples/lena-inpaint.jpg'
  FILENAME_INPAINT_MASK = File.expand_path(File.dirname(__FILE__)) + '/samples/inpaint-mask.bmp'
  FILENAME_LENA32x32 = File.expand_path(File.dirname(__FILE__)) + '/samples/lena-32x32.jpg'
  FILENAME_CONTOURS = File.expand_path(File.dirname(__FILE__)) + '/samples/contours.jpg'
  FILENAME_LINES = File.expand_path(File.dirname(__FILE__)) + '/samples/lines.jpg'
  FILENAME_LENA_EYES = File.expand_path(File.dirname(__FILE__)) + '/samples/lena-eyes.jpg'
  FILENAME_STR_CV = File.expand_path(File.dirname(__FILE__)) + '/samples/str-cv.jpg'
  FILENAME_STR_OV = File.expand_path(File.dirname(__FILE__)) + '/samples/str-ov.jpg'
  FILENAME_STR_CV_ROTATED = File.expand_path(File.dirname(__FILE__)) + '/samples/str-cv-rotated.jpg'

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

  def test_morphology
    mat0 = create_cvmat(64, 64, :cv8u, 1) { |j, i, c|
      if i >= 8 and i < 56 and j >= 8 and j < 56 and (i + j) % 15 != 0
        CvScalar.new(255)
      else
        CvScalar.new(0)
      end
    }

    # Open
    kernel = IplConvKernel.new(5, 5, 2, 2, :cross)
    mat1 = mat0.morphology(CV_MOP_OPEN, kernel)
    mat2 = mat0.morphology(:open, kernel)
    assert_equal('63ccb07cb93efb1563657f51e3d89252', hash_img(mat1))
    assert_equal('63ccb07cb93efb1563657f51e3d89252', hash_img(mat2))

    # Close
    mat1 = mat0.morphology(CV_MOP_CLOSE, kernel)
    mat2 = mat0.morphology(:close, kernel)
    assert_equal('831c513d6ed86bce3f15c697de4a72f8', hash_img(mat1))
    assert_equal('831c513d6ed86bce3f15c697de4a72f8', hash_img(mat2))

    # Gradient
    mat1 = mat0.morphology(CV_MOP_GRADIENT, kernel)
    mat2 = mat0.morphology(:gradient, kernel)
    assert_equal('1e8007c211d6f464cf8584e8e83b3c35', hash_img(mat1))
    assert_equal('1e8007c211d6f464cf8584e8e83b3c35', hash_img(mat2))

    # Top hat
    mat1 = mat0.morphology(CV_MOP_TOPHAT, kernel)
    mat2 = mat0.morphology(CV_MOP_TOPHAT, kernel)
    assert_equal('1760c5b63a52df37069164fe3e901aa4', hash_img(mat1))
    assert_equal('1760c5b63a52df37069164fe3e901aa4', hash_img(mat2))

    # Black hat
    mat1 = mat0.morphology(CV_MOP_BLACKHAT, kernel)
    mat2 = mat0.morphology(:blackhat, kernel)
    assert_equal('18b1d51637b912a38133341ee006c6ff', hash_img(mat1))
    assert_equal('18b1d51637b912a38133341ee006c6ff', hash_img(mat2))
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
    mat4 = mat0.morphology(CV_MOP_CLOSE, kernel)

    assert_equal('752914aae1cff07a2b3ce528b6ac3332', hash_img(mat1))
    assert_equal('0908b8f98999a198e8a1fbc743de52e5', hash_img(mat2))
    assert_equal('831c513d6ed86bce3f15c697de4a72f8', hash_img(mat3))
    assert_equal('831c513d6ed86bce3f15c697de4a72f8', hash_img(mat4))
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
    mat4 = mat0.morphology(CV_MOP_GRADIENT, kernel)

    assert_equal('e15d131ae29a58237f8d9a89669c3a47', hash_img(mat1))
    assert_equal('31d158672f699f961c59908e0bd72d5c', hash_img(mat2))
    assert_equal('1e8007c211d6f464cf8584e8e83b3c35', hash_img(mat3))
    assert_equal('1e8007c211d6f464cf8584e8e83b3c35', hash_img(mat4))
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
    mat4 = mat0.morphology(CV_MOP_TOPHAT, kernel)

    assert_equal('26f89a4f449ec8328499960acbfd44f2', hash_img(mat1))
    assert_equal('102833c2e96eaa706eea5854d2aeaf5a', hash_img(mat2))
    assert_equal('1760c5b63a52df37069164fe3e901aa4', hash_img(mat3))
    assert_equal('1760c5b63a52df37069164fe3e901aa4', hash_img(mat4))
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
    mat4 = mat0.morphology(CV_MOP_BLACKHAT, kernel)

    assert_equal('3773d2802aad82c91ea8e14a324e5fc3', hash_img(mat1))
    assert_equal('3fc6bc283fa952e1fd566944d94b3e9a', hash_img(mat2))
    assert_equal('18b1d51637b912a38133341ee006c6ff', hash_img(mat3))
    assert_equal('18b1d51637b912a38133341ee006c6ff', hash_img(mat4))
  end

  def test_smooth
    mat0 = CvMat.load(FILENAME_LENA32x32, CV_LOAD_IMAGE_GRAYSCALE)

    # Blur no scale
    mat1 = mat0.smooth(CV_BLUR_NO_SCALE)
    mat2 = mat0.smooth(:blur_no_scale, 3, 3)
    mat3 = mat0.smooth(CV_BLUR_NO_SCALE, 7, 7)
    mat4 = CvMat.new(32, 32, :cv32f, 1).smooth(:blur_no_scale)

    [mat1, mat2, mat3].each { |m|
      assert_equal(1, m.channel)
      assert_equal(:cv16u, m.depth)
    }
    assert_equal(1, mat4.channel)
    assert_equal(:cv32f, mat4.depth)

    assert_equal('3c9074c87b65117798f48e41a17b2f30', hash_img(mat1))
    assert_equal('3c9074c87b65117798f48e41a17b2f30', hash_img(mat2))
    assert_equal('9c549aa406a425a65b036c2f9a2689e0', hash_img(mat3))
    
    # Blur
    mat1 = mat0.smooth(CV_BLUR)
    mat2 = mat0.smooth(:blur, 3, 3)
    mat3 = mat0.smooth(CV_BLUR, 7, 7)
    mat4 = CvMat.new(32, 32, :cv16u, 1).smooth(:blur)
    mat5 = CvMat.new(32, 32, :cv32f, 1).smooth(CV_BLUR)
    mat6 = CvMat.new(32, 32, :cv8u, 3).smooth(:blur)

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

    # Gaussian
    mat1 = mat0.smooth(CV_GAUSSIAN)
    mat2 = mat0.smooth(:gaussian, 3, 3)
    mat3 = mat0.smooth(CV_GAUSSIAN, 3, 3, 3)
    mat4 = mat0.smooth(:gaussian, 3, 3, 3, 3)
    mat5 = mat0.smooth(CV_GAUSSIAN, 7, 7, 5, 3)

    mat6 = CvMat.new(32, 32, :cv16u, 1).smooth(CV_GAUSSIAN)
    mat7 = CvMat.new(32, 32, :cv32f, 1).smooth(CV_GAUSSIAN)
    mat8 = CvMat.new(32, 32, :cv8u, 3).smooth(CV_GAUSSIAN)

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

    # Median
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
    
    mat1 = mat0.smooth(CV_MEDIAN)
    mat2 = mat0.smooth(:median, 3)
    mat3 = mat0.smooth(CV_MEDIAN, 7)
    mat4 = CvMat.new(64, 64, :cv8u, 3).smooth(CV_MEDIAN)

    assert_equal('7343a41c542e034db356636c06134961', hash_img(mat1))
    assert_equal('7343a41c542e034db356636c06134961', hash_img(mat2))
    assert_equal('6ae59e64850377ee5470c854761551ea', hash_img(mat3))

    # Bilateral
    mat0 = create_cvmat(64, 64, :cv8u, 1) { |j, i, c|
      if i > 32
        (i + j) % 15 != 0 ? CvScalar.new(32) : CvScalar.new(224)
      else
        (i + j) % 15 != 0 ? CvScalar.new(224) : CvScalar.new(32)
      end
    }

    mat1 = mat0.smooth(CV_BILATERAL)
    mat2 = mat0.smooth(:bilateral, 3, 3)
    mat3 = mat0.smooth(CV_BILATERAL, 7, 7)
    mat4 = CvMat.new(64, 64, :cv8u, 3).smooth(CV_BILATERAL)
    flunk('FIXME: Cases of CvMat#smooth(CV_BILATERAL) are not tested yet.')
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

  def test_filter2d
    mat0 = CvMat.load(FILENAME_LENA256x256, CV_LOAD_IMAGE_GRAYSCALE)
    kernel = CvMat.new(3, 3, :cv32f, 1)
    
    # Laplacian filter kernel
    laplace4 = [0, 1, 0,
                1, -4, 1,
                0, 1, 0]
    laplace4.each_with_index { |x, i| kernel[i] = CvScalar.new(x) }

    mat1 = mat0.filter2d(kernel)
    mat2 = mat0.filter2d(kernel, CvPoint.new(-1, -1))
    mat3 = mat0.filter2d(kernel, CvPoint.new(0, 0))

    assert_equal('14a01cc47078e8f8fe4f0fd510d5521b', hash_img(mat1))
    assert_equal('14a01cc47078e8f8fe4f0fd510d5521b', hash_img(mat2))
    assert_equal('30e04de43f9240df6aadbaea6467b8fe', hash_img(mat3))
  end

  def test_copy_make_border_constant
    mat0 = create_cvmat(32, 32, :cv8u, 1) { CvScalar.new(128) }
    mat1 = mat0.copy_make_border_constant(CvSize.new(64, 48), CvPoint.new(16, 8), 255)

    assert_equal('5e231f8ca051b8f93e4aaa42d193d095', hash_img(mat1))
  end

  def test_copy_make_border_replicate
    mat0 = CvMat.load(FILENAME_LENA256x256, CV_LOAD_IMAGE_ANYCOLOR | CV_LOAD_IMAGE_ANYDEPTH)
    mat1 = mat0.copy_make_border_replicate(CvSize.new(300, 300), CvPoint.new(30, 30))

    assert_equal('ecc7e69d110f9934fa31f8ec85b30275', hash_img(mat1))
  end

  def test_integral
    mat0 = create_cvmat(3, 3, :cv8u, 1) { |j, i, n| CvScalar.new(n) }

    sum, sqsum, tilted_sum = mat0.integral(true, true)
    [sum, sqsum, tilted_sum].each { |s|
      assert_equal(mat0.height + 1, s.height)
      assert_equal(mat0.width + 1, s.width)
      assert_equal(:cv64f, s.depth)
      assert_equal(1, s.channel)
    }

    expected_sum = [0, 0, 0, 0,
                    0, 0, 1, 3,
                    0, 3, 8, 15,
                    0, 9, 21, 36]
    expected_sum.each_with_index { |x, i|
      assert_in_delta(x, sum[i][0], 0.001)
    }
    expected_sqsum = [0, 0, 0, 0,
                      0, 0, 1, 5,
                      0, 9, 26, 55,
                      0, 45, 111, 204]
    expected_sqsum.each_with_index { |x, i|
      assert_in_delta(x, sqsum[i][0], 0.001)
    }

    expected_tilted_sum = [0, 0, 0, 0,
                           0, 0, 1, 2,
                           0, 4, 7, 8,
                           4, 16, 22, 20]
    expected_tilted_sum.each_with_index { |x, i|
      assert_in_delta(x, tilted_sum[i][0], 0.001)
    }
  end

  def test_threshold
    mat0 = create_cvmat(3, 3, :cv8u, 1) { |j, i, n| CvScalar.new(n) }
    test_proc = lambda { |type, type_sym, expected_mat, expected_threshold|
      mat1 = mat0.threshold(3, 7, type)
      mat2 = mat0.threshold(3, 7, type_sym)
      mat3, th3 = mat0.threshold(5, 7, type | CV_THRESH_OTSU)
      mat4, th4 = mat0.threshold(3, 7, type_sym, true)
      mat5, th5 = mat0.threshold(5, 7, type | CV_THRESH_OTSU, true)
      [mat1, mat2, mat3, mat4, mat5].each { |m|
        expected_mat.each_with_index { |x, i|
          assert_equal(x, m[i][0])
        }
      }
      [th3, th4, th5].each { |th|
        assert_in_delta(expected_threshold, th, 0.001)
      }
    }
    # Binary
    expected = [0, 0, 0,
                0, 7, 7,
                7, 7, 7]
    test_proc.call(CV_THRESH_BINARY, :binary, expected, 3)

    # Binary inverse
    expected = [7, 7, 7,
                7, 0, 0,
                0, 0, 0]
    test_proc.call(CV_THRESH_BINARY_INV, :binary_inv, expected, 3)

    # Trunc
    expected = [0, 1, 2,
                3, 3, 3,
                3, 3, 3]
    test_proc.call(CV_THRESH_TRUNC, :trunc, expected, 3)

    # To zero
    expected = [0, 0, 0,
                0, 4, 5,
                6, 7, 8]
    test_proc.call(CV_THRESH_TOZERO, :tozero, expected, 3)

    # To zero inverse
    expected = [0, 1, 2,
                3, 0, 0,
                0, 0, 0]
    test_proc.call(CV_THRESH_TOZERO_INV, :tozero_inv, expected, 3)

    assert_raise(ArgumentError) {
      mat0.threshold(1, 2, :foobar)
    }
  end

  def test_threshold_binary
    mat0 = create_cvmat(3, 3, :cv8u, 1) { |j, i, n| CvScalar.new(n) }
    mat1 = mat0.threshold_binary(3, 7)
    expected = [0, 0, 0,
                0, 7, 7,
                7, 7, 7]
    expected.each_with_index { |x, i|
      assert_equal(x, mat1[i][0])
    }

    mat2, thresh2 = mat0.threshold_binary(5, 7, true)
    assert_in_delta(3, thresh2, 0.001)
    expected.each_with_index { |x, i|
      assert_equal(x, mat1[i][0])
    }
  end

  def test_threshold_binary_inverse
    mat0 = create_cvmat(3, 3, :cv8u, 1) { |j, i, n| CvScalar.new(n) }
    mat1 = mat0.threshold_binary_inverse(3, 7)
    expected = [7, 7, 7,
                7, 0, 0,
                0, 0, 0]
    expected.each_with_index { |x, i|
      assert_equal(x, mat1[i][0])
    }

    mat2, thresh2 = mat0.threshold_binary_inverse(5, 7, true)
    assert_in_delta(3, thresh2, 0.001)
    expected.each_with_index { |x, i|
      assert_equal(x, mat1[i][0])
    }
  end

  def test_threshold_trunc
    mat0 = create_cvmat(3, 3, :cv8u, 1) { |j, i, n| CvScalar.new(n) }
    mat1 = mat0.threshold_trunc(3)
    expected = [0, 1, 2,
                3, 3, 3,
                3, 3, 3]
    expected.each_with_index { |x, i|
      assert_equal(x, mat1[i][0])
    }

    mat2, thresh2 = mat0.threshold_trunc(5, true)
    assert_in_delta(3, thresh2, 0.001)
    expected.each_with_index { |x, i|
      assert_equal(x, mat1[i][0])
    }
  end

  def test_threshold_to_zero
    mat0 = create_cvmat(3, 3, :cv8u, 1) { |j, i, n| CvScalar.new(n) }
    mat1 = mat0.threshold_to_zero(3)
    expected = [0, 0, 0,
                0, 4, 5,
                6, 7, 8]
    expected.each_with_index { |x, i|
      assert_equal(x, mat1[i][0])
    }

    mat2, thresh2 = mat0.threshold_to_zero(5, true)
    assert_in_delta(3, thresh2, 0.001)
    expected.each_with_index { |x, i|
      assert_equal(x, mat1[i][0])
    }
  end

  def test_threshold_to_zero_inverse
    mat0 = create_cvmat(3, 3, :cv8u, 1) { |j, i, n| CvScalar.new(n) }
    mat1 = mat0.threshold_to_zero_inverse(3)
    expected = [0, 1, 2,
                3, 0, 0,
                0, 0, 0]
    expected.each_with_index { |x, i|
      assert_equal(x, mat1[i][0])
    }

    mat2, thresh2 = mat0.threshold_to_zero_inverse(5, true)
    assert_in_delta(3, thresh2, 0.001)
    expected.each_with_index { |x, i|
      assert_equal(x, mat1[i][0])
    }
  end

  def test_pyr_down
    mat0 = CvMat.load(FILENAME_LENA256x256, CV_LOAD_IMAGE_ANYCOLOR | CV_LOAD_IMAGE_ANYDEPTH)
    mat1 = mat0.pyr_down
    mat2 = mat0.pyr_down(:gaussian_5x5)

    assert_equal('de9ff2ffcf8e43f28564a201cf90b7f4', hash_img(mat1))
    assert_equal('de9ff2ffcf8e43f28564a201cf90b7f4', hash_img(mat2))
  end

  def test_pyr_up
    mat0 = CvMat.load(FILENAME_LENA256x256, CV_LOAD_IMAGE_ANYCOLOR | CV_LOAD_IMAGE_ANYDEPTH)
    mat1 = mat0.pyr_up
    mat2 = mat0.pyr_up(:gaussian_5x5)

    assert_equal('02430c6cf143d3d104e25bc829f1fa93', hash_img(mat1))
    assert_equal('02430c6cf143d3d104e25bc829f1fa93', hash_img(mat2))
  end

  def test_flood_fill
    mat0 = create_cvmat(128, 128, :cv8u, 1) { |j, i, c|
      if (i >= 32 and i < 96) and (j >= 32 and j < 96)
        CvScalar.new(255)
      elsif (i >= 16 and i < 112) and (j >= 16 and j < 112)
        CvScalar.new(192)
      else
        CvScalar.new(128)
      end
    }

    point = CvPoint.new(20, 20)
    mat1, comp1, mask1 = mat0.flood_fill(point, 0)
    mat2, comp2, mask2 = mat0.flood_fill(point, 0, CvScalar.new(64))
    mat3, comp3, mask3 = mat0.flood_fill(point, 0, CvScalar.new(0), CvScalar.new(64))
    mat4, comp4, mask4 = mat0.flood_fill(point, 0, CvScalar.new(0), CvScalar.new(64),
                                         {:connectivity => 8, :fixed_range => true, :mask_only => true})
    mat05 = mat0.clone
    mat5, comp5, mask5 = mat05.flood_fill!(point, 0, CvScalar.new(0), CvScalar.new(64),
                                           {:connectivity => 8, :fixed_range => true, :mask_only => true})

    assert_equal('8c6a235fdf4c9c4f6822a45daac5b1af', hash_img(mat1))
    assert_equal(5120.0, comp1.area)
    assert_equal(16, comp1.rect.x)
    assert_equal(16, comp1.rect.y)
    assert_equal(96, comp1.rect.width)
    assert_equal(96, comp1.rect.height)
    assert_cvscalar_equal(CvScalar.new(0, 0, 0, 0), comp1.value)
    assert_equal('98ebfcc49df7b8ebca6dd1c931ce7c34', hash_img(mask1))

    assert_equal('7456e5de74bb8b4e783d04bbf1904644', hash_img(mat2))
    assert_equal(12288.0, comp2.area)
    assert_equal(0, comp2.rect.x)
    assert_equal(0, comp2.rect.y)
    assert_equal(128, comp2.rect.width)
    assert_equal(128, comp2.rect.height)
    assert_cvscalar_equal(CvScalar.new(0, 0, 0, 0), comp2.value)
    assert_equal('3c23b0e491bb59dc9af75bf191d24458', hash_img(mask2))

    assert_equal('df720005423762ca1b68e06571f58b21', hash_img(mat3))
    assert_equal(9216.0, comp3.area)
    assert_equal(16, comp3.rect.x)
    assert_equal(16, comp3.rect.y)
    assert_equal(96, comp3.rect.width)
    assert_equal(96, comp3.rect.height)
    assert_cvscalar_equal(CvScalar.new(0, 0, 0, 0), comp3.value)

    assert_equal('7833f4c85c77056db71e33ae8072a1b5', hash_img(mat4))
    assert_equal(9216.0, comp4.area)
    assert_equal(16, comp4.rect.x)
    assert_equal(16, comp4.rect.y)
    assert_equal(96, comp4.rect.width)
    assert_equal(96, comp4.rect.height)
    assert_cvscalar_equal(CvScalar.new(220, 0, 0, 0), comp4.value)
    assert_equal('33e01cdd72d7630e4231ffa63557da3e', hash_img(mask4))

    assert_equal('7833f4c85c77056db71e33ae8072a1b5', hash_img(mat5))
    assert_equal('7833f4c85c77056db71e33ae8072a1b5', hash_img(mat05))
    assert_equal(9216.0, comp5.area)
    assert_equal(16, comp5.rect.x)
    assert_equal(16, comp5.rect.y)
    assert_equal(96, comp5.rect.width)
    assert_equal(96, comp5.rect.height)
    assert_cvscalar_equal(CvScalar.new(220, 0, 0, 0), comp5.value)
    assert_equal('33e01cdd72d7630e4231ffa63557da3e', hash_img(mask5))
  end

  def test_find_contours
    mat0 = CvMat.load(FILENAME_CONTOURS, CV_LOAD_IMAGE_GRAYSCALE)

    # Make binary image
    mat0.height.times { |j|
      mat0.width.times { |i|
        mat0[j, i] = (mat0[j, i][0] < 128) ? CvColor::Black : CvColor::White
      }
    }

    [mat0.find_contours, mat0.find_contours(:mode => CV_RETR_LIST),
     mat0.find_contours(:method => CV_CHAIN_APPROX_SIMPLE),
     mat0.find_contours(:mode => CV_RETR_LIST, :method => CV_CHAIN_APPROX_SIMPLE)].each { |contours|
      assert_not_nil(contours)
      assert_equal(8, contours.total)
      assert_not_nil(contours.h_next)
      assert_equal(4, contours.h_next.total)
      assert_not_nil(contours.h_next.h_next)
      assert_equal(8, contours.h_next.h_next.total)
      assert_not_nil(contours.h_next.h_next.h_next)
      assert_equal(4, contours.h_next.h_next.h_next.total)
      assert_nil(contours.v_next)
      assert_nil(contours.h_next.v_next)
      assert_nil(contours.h_next.h_next.v_next)
      assert_nil(contours.h_next.h_next.h_next.v_next)
    }
    
    contours = mat0.find_contours(:mode => CV_RETR_TREE)
    assert_not_nil(contours)
    assert_equal(4, contours.total)
    assert_not_nil(contours.v_next)
    assert_equal(8, contours.v_next.total)
    assert_nil(contours.v_next.v_next)
    assert_not_nil(contours.h_next)
    assert_equal(4, contours.h_next.total)
    assert_not_nil(contours.h_next.v_next)
    assert_equal(8, contours.h_next.v_next.total)
    assert_nil(contours.h_next.v_next.v_next)

    contours = mat0.find_contours(:mode => CV_RETR_CCOMP)
    assert_not_nil(contours)
    assert_equal(4, contours.total)
    assert_not_nil(contours.v_next)
    assert_equal(8, contours.v_next.total)
    assert_nil(contours.v_next.v_next)
    assert_not_nil(contours.h_next)
    assert_equal(4, contours.h_next.total)
    assert_not_nil(contours.h_next.v_next)
    assert_equal(8, contours.h_next.v_next.total)
    assert_nil(contours.h_next.v_next.v_next)

    contours = mat0.find_contours(:mode => CV_RETR_EXTERNAL)
    assert_not_nil(contours)
    assert_equal(4, contours.total)
    assert_nil(contours.v_next)
    assert_not_nil(contours.h_next)
    assert_equal(4, contours.h_next.total)
    assert_nil(contours.h_next.v_next)

    contours = mat0.find_contours(:mode => CV_RETR_TREE, :method => CV_CHAIN_APPROX_NONE)
    assert_not_nil(contours)
    assert_equal(474, contours.total)
    assert_not_nil(contours.v_next)
    assert_equal(318, contours.v_next.total)
    assert_nil(contours.v_next.v_next)
    assert_not_nil(contours.h_next)
    assert_equal(396, contours.h_next.total)
    assert_not_nil(contours.h_next.v_next)
    assert_equal(240, contours.h_next.v_next.total)
    assert_nil(contours.h_next.v_next.v_next)

    contours = mat0.find_contours(:mode => CV_RETR_EXTERNAL, :method => CV_CHAIN_CODE)
    assert_equal(474, contours.total)
    assert_equal(396, contours.h_next.total)

    contours = mat0.find_contours(:mode => CV_RETR_EXTERNAL, :method => CV_CHAIN_APPROX_TC89_L1)
    assert_equal(4, contours.total)
    assert_equal(4, contours.h_next.total)

    contours = mat0.find_contours(:mode => CV_RETR_EXTERNAL, :method => CV_CHAIN_APPROX_TC89_KCOS)
    assert_equal(4, contours.total)
    assert_equal(4, contours.h_next.total)
  end

  def test_pyr_segmentation
    mat0 = CvMat.load(FILENAME_LENA256x256, CV_LOAD_IMAGE_ANYCOLOR | CV_LOAD_IMAGE_ANYDEPTH)
    mat1, seq1 = mat0.pyr_segmentation(4, 255, 50)
    assert_equal('ebd9bad0bbc90b1d4a25289b7d59c958', hash_img(mat1))
    assert_equal(5, seq1.total)

    img0 = IplImage.load(FILENAME_CAT, CV_LOAD_IMAGE_ANYCOLOR | CV_LOAD_IMAGE_ANYDEPTH)
    img0.set_roi(CvRect.new(0, 0, 256, 512))
    img2, seq2 = img0.pyr_segmentation(2, 255, 50)
    assert_equal('963b26f51b14f175fbbf128e9b9e979f', hash_img(img2))
    assert_equal(11, seq2.total)
  end


  def test_pyr_mean_shift_filtering
    mat0 = CvMat.load(FILENAME_LENA256x256, CV_LOAD_IMAGE_ANYCOLOR | CV_LOAD_IMAGE_ANYDEPTH)
    mat1 = mat0.pyr_mean_shift_filtering(30, 30)
    mat2 = mat0.pyr_mean_shift_filtering(30, 30, 2)
    mat3 = mat0.pyr_mean_shift_filtering(30, 30, nil, CvTermCriteria.new(3, 0.01))
    
    assert_equal('6887e96bc5dfd552f76ac5411b394775', hash_img(mat1))
    assert_equal('3cd9c4983fcabeafa04be200d5e08841', hash_img(mat2))
    assert_equal('e37f0157f93fe2a98312ae6b768e8295', hash_img(mat3))
  end

  def test_watershed
    mat0 = CvMat.load(FILENAME_LENA256x256, CV_LOAD_IMAGE_ANYCOLOR | CV_LOAD_IMAGE_ANYDEPTH)
    marker = CvMat.new(mat0.cols, mat0.rows, :cv32s, 1).set_zero
    marker[150, 150] = CvScalar.new(1, 1, 1, 1)
    marker[210, 210] = CvScalar.new(2, 2, 2, 2)
    marker[40, 90] = CvScalar.new(3, 3, 3, 3)

    mat1 = mat0.watershed(marker)
    assert_equal('ee6bec03296039c8df1899d3edc4684e', hash_img(mat1))
  end

  def test_hough_lines
    mat0 = CvMat.load(FILENAME_LINES, CV_LOAD_IMAGE_ANYCOLOR | CV_LOAD_IMAGE_ANYDEPTH)
    # make a binary image
    mat = CvMat.new(mat0.rows, mat0.cols, :cv8u, 1)
    (mat0.rows * mat0.cols).times { |i|
      mat[i] = (mat0[i][0] <= 100) ? CvScalar.new(0) : CvScalar.new(255);
    }

    [CV_HOUGH_STANDARD, :standard].each { |method|
      seq = mat.hough_lines(method, 1, Math::PI / 180, 65)
      assert_equal(4, seq.size)
    }

    [CV_HOUGH_PROBABILISTIC, :probabilistic].each { |method|
      seq = mat.hough_lines(method, 1, Math::PI / 180, 40, 30, 10)
      assert_equal(4, seq.size)
    }

    [CV_HOUGH_MULTI_SCALE, :multi_scale].each { |method|
      seq = mat.hough_lines(method, 1, Math::PI / 180, 40, 2, 3)
      assert_equal(9, seq.size)
    }
  end

  def test_hough_lines_standard
    mat0 = CvMat.load(FILENAME_LINES, CV_LOAD_IMAGE_ANYCOLOR | CV_LOAD_IMAGE_ANYDEPTH)
    # make a binary image
    mat = CvMat.new(mat0.rows, mat0.cols, :cv8u, 1)
    (mat0.rows * mat0.cols).times { |i|
      mat[i] = (mat0[i][0] <= 100) ? CvScalar.new(0) : CvScalar.new(255);
    }
    seq = mat.hough_lines_standard(1, Math::PI / 180, 65)
    assert_equal(4, seq.size)

    # Uncomment the following lines to show the result
    # seq.each { |line|
    #   cos = Math::cos(line.theta)
    #   sin = Math::sin(line.theta)
    #   x0 = line.rho * cos
    #   y0 = line.rho * sin
    #   pt1 = CvPoint.new
    #   pt2 = CvPoint.new
    #   pt1.x = x0 + mat.width * 10 * (-sin)
    #   pt1.y = y0 + mat.height * 10 * (cos)
    #   pt2.x = x0 - mat.width * 10 * (-sin)
    #   pt2.y = y0 - mat.height * 10 * (cos)
    #   mat0.line!(pt1, pt2, :color => CvColor::Red, :thickness => 1, :line_type => :aa)
    # }
    # snap mat0
  end

  def test_hough_lines_probabilistic
    mat0 = CvMat.load(FILENAME_LINES, CV_LOAD_IMAGE_ANYCOLOR | CV_LOAD_IMAGE_ANYDEPTH)
    # make a binary image
    mat = CvMat.new(mat0.rows, mat0.cols, :cv8u, 1)
    (mat0.rows * mat0.cols).times { |i|
      mat[i] = (mat0[i][0] <= 100) ? CvScalar.new(0) : CvScalar.new(255);
    }
    seq = mat.hough_lines_probabilistic(1, Math::PI / 180, 40, 30, 10)
    assert_equal(4, seq.size)

    # Uncomment the following lines to show the result
    # seq.each { |points|
    #   mat0.line!(*points, :color => CvColor::Red, :thickness => 1, :line_type => :aa)
    # }
    # snap mat0
  end

  def test_hough_lines_multi_scale
    mat0 = CvMat.load(FILENAME_LINES, CV_LOAD_IMAGE_ANYCOLOR | CV_LOAD_IMAGE_ANYDEPTH)
    # make a binary image
    mat = CvMat.new(mat0.rows, mat0.cols, :cv8u, 1)
    (mat0.rows * mat0.cols).times { |i|
      mat[i] = (mat0[i][0] <= 100) ? CvScalar.new(0) : CvScalar.new(255);
    }
    seq = mat.hough_lines_multi_scale(1, Math::PI / 180, 40, 2, 3)
    assert_equal(9, seq.size)

    # Uncomment the following lines to show the result
    # seq.each { |line|
    #   cos = Math::cos(line.theta)
    #   sin = Math::sin(line.theta)
    #   x0 = line.rho * cos
    #   y0 = line.rho * sin
    #   pt1 = CvPoint.new
    #   pt2 = CvPoint.new
    #   pt1.x = x0 + mat.width * 10 * (-sin)
    #   pt1.y = y0 + mat.height * 10 * (cos)
    #   pt2.x = x0 - mat.width * 10 * (-sin)
    #   pt2.y = y0 - mat.height * 10 * (cos)
    #   mat0.line!(pt1, pt2, :color => CvColor::Red, :thickness => 1, :line_type => :aa)
    # }
    # snap mat0
  end

  def test_hough_circles
    mat0 = CvMat.load(FILENAME_LINES, CV_LOAD_IMAGE_ANYCOLOR | CV_LOAD_IMAGE_ANYDEPTH)
    # make a binary image
    mat = CvMat.new(mat0.rows, mat0.cols, :cv8u, 1)
    (mat0.rows * mat0.cols).times { |i|
      mat[i] = (mat0[i][0] <= 100) ? CvScalar.new(0) : CvScalar.new(255);
    }

    [mat.hough_circles(CV_HOUGH_GRADIENT, 1.5, 40, 100, 50, 10, 50),
     mat.hough_circles(:gradient, 1.5, 40, 100, 50, 10, 50),
     mat.hough_circles(CV_HOUGH_GRADIENT, 1.5, 40, 100, 50),
     mat.hough_circles(:gradient, 1.5, 40, 100, 50)].each { |seq|
      assert_equal(2, seq.size)
    }

    # Uncomment the following lines to show the result
    # seq = mat.hough_circles(:gradient, 1.5, 40, 100, 50, 10, 50)
    # seq.each { |circle|
    #   mat0.circle!(circle.center, circle.radius, :color => CvColor::Red, :thickness => 2)
    # }
    # snap mat0
  end

  def test_hough_circles_gradient
    mat0 = CvMat.load(FILENAME_LINES, CV_LOAD_IMAGE_ANYCOLOR | CV_LOAD_IMAGE_ANYDEPTH)
    # make a binary image
    mat = CvMat.new(mat0.rows, mat0.cols, :cv8u, 1)
    (mat0.rows * mat0.cols).times { |i|
      mat[i] = (mat0[i][0] <= 100) ? CvScalar.new(0) : CvScalar.new(255);
    }

    [mat.hough_circles_gradient(1.5, 40, 100, 50, 10, 50),
     mat.hough_circles_gradient(1.5, 40, 100, 50)].each { |seq|
      assert_equal(2, seq.size)
    }

    # Uncomment the following lines to show the result
    # seq = mat.hough_circles_gradient(1.5, 40, 100, 50, 10, 50)
    # seq.each { |circle|
    #   mat0.circle!(circle.center, circle.radius, :color => CvColor::Red, :thickness => 2)
    # }
    # snap mat0
  end

  def test_inpaint
    mat = CvMat.load(FILENAME_LENA_INPAINT, CV_LOAD_IMAGE_ANYCOLOR | CV_LOAD_IMAGE_ANYDEPTH)
    mask = CvMat.load(FILENAME_INPAINT_MASK, CV_LOAD_IMAGE_GRAYSCALE)

    [CV_INPAINT_NS, :ns].each { |method|
      result_ns = mat.inpaint(method, mask, 10)
      assert_equal('d3df4dda8642c83512fb417ffa5e1457', hash_img(result_ns))
    }
    [CV_INPAINT_TELEA, :telea].each { |method|
      result_telea = mat.inpaint(method, mask, 10)
      assert_equal('d45bec22d03067578703f2ec68567167', hash_img(result_telea))
    }

    # Uncomment the following lines to show the results
    # result_ns = mat.inpaint(:ns, mask, 10)
    # result_telea = mat.inpaint(:telea, mask, 10)
    # snap mat, result_ns, result_telea
  end

  def test_inpaint_ns
    mat = CvMat.load(FILENAME_LENA_INPAINT, CV_LOAD_IMAGE_ANYCOLOR | CV_LOAD_IMAGE_ANYDEPTH)
    mask = CvMat.load(FILENAME_INPAINT_MASK, CV_LOAD_IMAGE_GRAYSCALE)
    result = mat.inpaint_ns(mask, 10)
    assert_equal('d3df4dda8642c83512fb417ffa5e1457', hash_img(result))
    # Uncomment the following lines to show the result
    # snap mat, result
  end

  def test_inpaint_telea
    mat = CvMat.load(FILENAME_LENA_INPAINT, CV_LOAD_IMAGE_ANYCOLOR | CV_LOAD_IMAGE_ANYDEPTH)
    mask = CvMat.load(FILENAME_INPAINT_MASK, CV_LOAD_IMAGE_GRAYSCALE)
    result = mat.inpaint_telea(mask, 10)
    assert_equal('d45bec22d03067578703f2ec68567167', hash_img(result))
    # Uncomment the following lines to show the result
    # snap mat, result
  end

  def test_equalize_hist
    mat = CvMat.load(FILENAME_LENA256x256, CV_LOAD_IMAGE_GRAYSCALE)
    result = mat.equalize_hist
    assert_equal('de235065c746193d7f3de9359f63a7af', hash_img(result))

    # Uncomment the following lines to show the result
    # snap mat, result
  end

  def test_match_template
    mat = CvMat.load(FILENAME_LENA256x256, CV_LOAD_IMAGE_ANYCOLOR | CV_LOAD_IMAGE_ANYDEPTH)
    templ = CvMat.load(FILENAME_LENA_EYES, CV_LOAD_IMAGE_ANYCOLOR | CV_LOAD_IMAGE_ANYDEPTH)
    
    # sqdiff
    result = mat.match_template(templ)
    assert_equal('88663ec44be797ca883fc87bb6d7c09b', hash_img(result))
    [CV_TM_SQDIFF, :sqdiff].each { |method|
      result = mat.match_template(templ, method)
      assert_equal('88663ec44be797ca883fc87bb6d7c09b', hash_img(result))
    }

    # sqdiff_normed
    [CV_TM_SQDIFF_NORMED, :sqdiff_normed].each { |method|
      result = mat.match_template(templ, method)
      assert_equal('75c812f87184b2ccd8f83b70a8436356', hash_img(result))
    }

    # ccorr
    [CV_TM_CCORR, :ccorr].each { |method|
      result = mat.match_template(templ, method)
      assert_equal('6ebe7e48edf8fc64bcc0fd7f1e96555c', hash_img(result))
    }

    # ccorr_normed
    [CV_TM_CCORR_NORMED, :ccorr_normed].each { |method|
      result = mat.match_template(templ, method)
      assert_equal('4cf8ebcec870f8295d615a9aa345ae4d', hash_img(result))
    }

    # ccoeff
    [CV_TM_CCOEFF, :ccoeff].each { |method|
      result = mat.match_template(templ, method)
      assert_equal('248a391c5a1e1dbcf7a19f3310b5cd7a', hash_img(result))
    }
    
    # ccoeff_normed
    [CV_TM_CCOEFF_NORMED, :ccoeff_normed].each { |method|
      result = mat.match_template(templ, method)
      assert_equal('27a4e9b45ed648848f0498356bd2c5b5', hash_img(result))
    }

    # Uncomment the following lines to show the result
    # result = mat.match_template(templ)
    # pt1 = result.min_max_loc[2] # minimum location
    # pt2 = CvPoint.new(pt1.x + templ.width, pt1.y + templ.height)
    # mat.rectangle!(pt1, pt2, :color => CvColor::Black, :thickness => 3)
    # snap mat, templ, result
  end

  def test_match_shapes
    mat_cv = CvMat.load(FILENAME_STR_CV, CV_LOAD_IMAGE_GRAYSCALE)
    mat_ov = CvMat.load(FILENAME_STR_OV, CV_LOAD_IMAGE_GRAYSCALE)
    mat_cv_rotated = CvMat.load(FILENAME_STR_CV_ROTATED, CV_LOAD_IMAGE_GRAYSCALE)

    [CV_CONTOURS_MATCH_I1, :i1].each { |method|
      assert_in_delta(0, mat_cv.match_shapes(mat_cv_rotated, method), 0.00001)
      assert_in_delta(0.0010649, mat_cv.match_shapes(mat_ov, method), 0.00001)
    }

    [CV_CONTOURS_MATCH_I2, :i2].each { |method|
      assert_in_delta(0, mat_cv.match_shapes(mat_cv_rotated, method), 0.00001)
      assert_in_delta(0.0104650, mat_cv.match_shapes(mat_ov, method), 0.00001)
    }

    [CV_CONTOURS_MATCH_I3, :i3].each { |method|
      assert_in_delta(0, mat_cv.match_shapes(mat_cv_rotated, method), 0.00001)
      assert_in_delta(0.0033327, mat_cv.match_shapes(mat_ov, method), 0.00001)
    }
  end
  
  def test_match_shapes_i1
    mat_cv = CvMat.load(FILENAME_STR_CV, CV_LOAD_IMAGE_GRAYSCALE)
    mat_ov = CvMat.load(FILENAME_STR_OV, CV_LOAD_IMAGE_GRAYSCALE)
    mat_cv_rotated = CvMat.load(FILENAME_STR_CV_ROTATED, CV_LOAD_IMAGE_GRAYSCALE)

    assert_in_delta(0, mat_cv.match_shapes_i1(mat_cv_rotated), 0.00001)
    assert_in_delta(0.0010649, mat_cv.match_shapes_i1(mat_ov), 0.00001)
  end

  def test_match_shapes_i2
    mat_cv = CvMat.load(FILENAME_STR_CV, CV_LOAD_IMAGE_GRAYSCALE)
    mat_ov = CvMat.load(FILENAME_STR_OV, CV_LOAD_IMAGE_GRAYSCALE)
    mat_cv_rotated = CvMat.load(FILENAME_STR_CV_ROTATED, CV_LOAD_IMAGE_GRAYSCALE)

    assert_in_delta(0, mat_cv.match_shapes_i2(mat_cv_rotated), 0.00001)
    assert_in_delta(0.0104650, mat_cv.match_shapes_i2(mat_ov), 0.00001)
  end

  def test_match_shapes_i3
    mat_cv = CvMat.load(FILENAME_STR_CV, CV_LOAD_IMAGE_GRAYSCALE)
    mat_ov = CvMat.load(FILENAME_STR_OV, CV_LOAD_IMAGE_GRAYSCALE)
    mat_cv_rotated = CvMat.load(FILENAME_STR_CV_ROTATED, CV_LOAD_IMAGE_GRAYSCALE)

    assert_in_delta(0, mat_cv.match_shapes_i3(mat_cv_rotated), 0.00001)
    assert_in_delta(0.0033327, mat_cv.match_shapes_i3(mat_ov), 0.00001)
  end
end

