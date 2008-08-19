#!/usr/bin/env ruby
# snake.rb
require "rubygems"
gem "opencv"
require "opencv"
include OpenCV

puts <<USAGE
usage:
  left-click: set point
  right-click: do snake
USAGE

window = GUI::Window.new "snake demo"
image = CvMat.new(256, 256, :cv8u, 1).clear!
image.circle!(CvPoint.new(128,128), 40, :color => CvColor::White, :thickness =>  -1)
display = image.GRAY2BGR

window.show display

points = []

window.on_mouse{|mouse|
  case mouse.event
  when :left_button_down
    display[mouse.x, mouse.y] = CvColor::Red
    puts "set point (#{mouse.x},#{mouse.y})"
    points << CvPoint.new(mouse.x, mouse.y)
    window.show display
  when :right_button_down
    if points.length < 3
      puts "please set more point!"
      next
    end
    points = image.snake_image(points, 1.0, 0.5, 1.5, CvSize.new(3, 3), 100)
    display = image.GRAY2BGR
    display.poly_line! points, :color => CvColor::Red, :is_closed => true
    window.show display
  end
}

GUI::wait_key

