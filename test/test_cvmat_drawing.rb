#!/usr/bin/env ruby
# -*- mode: ruby; coding: utf-8-unix -*- 
require 'test/unit'
require 'opencv'
require File.expand_path(File.dirname(__FILE__)) + '/helper'

include OpenCV

# Tests for drawing functions of OpenCV::CvMat
class TestCvMat_drawing < OpenCVTestCase
  def test_DRAWING_OPTION
    CvMat::DRAWING_OPTION[:color].to_ary.each { |c|
      assert_in_delta(0, c, 0.01)
    }
    assert_equal(1, CvMat::DRAWING_OPTION[:thickness])
    assert_equal(8, CvMat::DRAWING_OPTION[:line_type])
    assert_equal(0, CvMat::DRAWING_OPTION[:shift])
  end

  def test_FLOOD_FILL_OPTION
    assert_equal(4, CvMat::FLOOD_FILL_OPTION[:connectivity])
    assert((not CvMat::FLOOD_FILL_OPTION[:fixed_range]))
    assert((not CvMat::FLOOD_FILL_OPTION[:mask_only]))
  end

  def test_line
    m0 = create_cvmat(240, 320, :cv8u, 3) { CvColor::White }
    m1 = m0.clone
    m2 = m0.line(CvPoint.new(1, 0), CvPoint.new(m0.width - 1, m0.height - 1),
                 :color => CvColor::Red, :thickness => 3, :line_type => :aa)
    m1.line!(CvPoint.new(1, 0), CvPoint.new(m0.width - 1, m0.height - 1),
             :color => CvColor::Blue, :thickness => 1, :line_type => :aa)

    # Uncomment the following lines to view the image
    # GUI::Window.new('Line: Blue, thickness = 1').show(m1)
    # GUI::Window.new('Line: Red, thickness = 3').show(m2)
    # GUI::wait_key
  end

  def test_rectangle
    m0 = create_cvmat(240, 320, :cv8u, 3) { CvColor::White }
    m1 = m0.clone
    m2 = m0.rectangle(CvPoint.new(20, 20), CvPoint.new(m0.width - 20, m0.height - 20),
                      :color => CvColor::Red, :thickness => 3, :line_type => :aa)
    m1.rectangle!(CvPoint.new(20, 20), CvPoint.new(m0.width - 20, m0.height - 20),
                  :color => CvColor::Blue, :thickness => 1, :line_type => :aa)

    # Uncomment the following lines to view the image
    # GUI::Window.new('Rectangle: Blue, thickness = 1').show(m1)
    # GUI::Window.new('Rectangle: Red, thickness = 3').show(m2)
    # GUI::wait_key
  end

  def test_circle
    m0 = create_cvmat(240, 320, :cv8u, 3) { CvColor::White }
    m1 = m0.clone
    m2 = m0.circle(CvPoint.new(m0.width / 2, m0.height / 2), 80,
                   :color => CvColor::Red, :thickness => 3, :line_type => :aa)
    m1.circle!(CvPoint.new(m0.width / 2, m0.height / 2), 80,
               :color => CvColor::Blue, :thickness => 1, :line_type => :aa)
    # Uncomment the following lines to view the image
    # GUI::Window.new('Circle: Blue, thickness = 1').show(m1)
    # GUI::Window.new('Circle: Red, thickness = 3').show(m2)
    # GUI::wait_key
  end

  def test_ellipse
    m0 = create_cvmat(240, 320, :cv8u, 3) { CvColor::White }
    m1 = m0.clone
    m2 = m0.ellipse(CvPoint.new(m0.width / 2, m0.height / 2), CvSize.new(100, 60), 30, 0, 360,
                    :color => CvColor::Red, :thickness => 3, :line_type => :aa)
    m1.ellipse!(CvPoint.new(m0.width / 2, m0.height / 2), CvSize.new(100, 60), 30, 0, 360,
                :color => CvColor::Blue, :thickness => 1, :line_type => :aa)

    # Uncomment the following lines to view the image
    # GUI::Window.new('Ellipse: Blue, thickness = 1').show(m1)
    # GUI::Window.new('Ellipse: Red, thickness = 3').show(m2)
    # GUI::wait_key
  end

  def test_ellipse_box
    m0 = create_cvmat(240, 320, :cv8u, 3) { CvColor::White }
    box = CvBox2D.new(CvPoint2D32f.new(m0.width / 2, m0.height / 2), CvSize2D32f.new(120, 160), 30)
    m1 = m0.clone
    m2 = m0.ellipse_box(box, :color => CvColor::Red, :thickness => 3, :line_type => :aa)
    m1.ellipse_box!(box, :color => CvColor::Blue, :thickness => 1, :line_type => :aa)

    # Uncomment the following lines to view the image
    # GUI::Window.new('Ellipse box: Blue, thickness = 1').show(m1)
    # GUI::Window.new('Ellipse box: Red, thickness = 3').show(m2)
    # GUI::wait_key
  end

  def test_fill_poly
    m0 = create_cvmat(240, 320, :cv8u, 3) { CvColor::White }
    pt = [[CvPoint.new(10, 20), CvPoint.new(10, 150), CvPoint.new(100, 50)],
          [CvPoint.new(200, 10), CvPoint.new(200, 200), CvPoint.new(170, 200)],
          [CvPoint.new(30, 10), CvPoint.new(0, 0), CvPoint.new(90, 150)]]

    m1 = m0.clone
    m2 = m0.fill_poly(pt, :color => CvColor::Red, :line_type => :aa)
    m1.fill_poly!(pt, :color => CvColor::Blue, :line_type => :aa)

    # Uncomment the following lines to view the image
    # GUI::Window.new('Fill poly: Blue').show(m1)
    # GUI::Window.new('Fill poly: Red').show(m2)
    # GUI::wait_key
  end

  def test_fill_convex_poly
    m0 = create_cvmat(240, 320, :cv8u, 3) { CvColor::White }
    pt = [CvPoint.new(10, 20), CvPoint.new(10, 150), CvPoint.new(100, 50)]
    
    m1 = m0.clone
    m2 = m0.fill_convex_poly(pt, :color => CvColor::Red, :line_type => :aa)
    m1.fill_convex_poly!(pt, :color => CvColor::Blue, :line_type => :aa)

    # Uncomment the following lines to view the image
    # GUI::Window.new('Fill convex poly: Blue').show(m1)
    # GUI::Window.new('Fill convex poly: Red').show(m2)
    # GUI::wait_key
  end

  def test_poly_line
    m0 = create_cvmat(240, 320, :cv8u, 3) { CvColor::White }
    pt = [[CvPoint.new(10, 20), CvPoint.new(10, 150), CvPoint.new(100, 150), CvPoint.new(10, 20)],
          [CvPoint.new(100, 200), CvPoint.new(200, 190), CvPoint.new(180, 50), CvPoint.new(100, 200)]]

    m1 = m0.clone
    m2 = m0.poly_line(pt, :color => CvColor::Red, :thickness => 3, :line_type => :aa)
    m1.poly_line!(pt, :color => CvColor::Blue, :thickness => 1, :line_type => :aa)

    # Uncomment the following lines to view the image
    # GUI::Window.new('Poly line: Blue, thickness = 1').show(m1)
    # GUI::Window.new('Poly line: Red, thickness = 3').show(m2)
    # GUI::wait_key
  end

  def test_put_text
    m0 = create_cvmat(240, 320, :cv8u, 3) { CvColor::White }
    m1 = m0.clone
    m1.put_text!('test 1', CvPoint.new(60, 90), CvFont.new(:simplex), CvColor::Blue)
    font = CvFont.new(:plain, :hscale => 5.0, :vscale => 4.5,
                      :shear => 1.0, :thickness => 3, :line_type => 5, :italic => true)
    m2 = m0.put_text('test 2', CvPoint.new(30, 80), font, CvColor::Red)

    # Uncomment the following lines to view the image
    # GUI::Window.new('Put text: Blue, thickness = 1').show(m1)
    # GUI::Window.new('Put text: Red, thickness = 3').show(m2)
    # GUI::wait_key
  end
end


