#!/usr/bin/env ruby
# inpaint.rb
require "rubygems"
require "opencv"

include OpenCV

owindow = GUI::Window.new "original"
mwindow = GUI::Window.new "mask"
iwindow = GUI::Window.new "inpaint"

image = IplImage::load "inpaint.png"
b, g, r = image.split
original_mask = r.threshold_binary_inverse(0x00, 0xFF) & b.threshold_binary_inverse(0x00, 0xFF)
mask = original_mask.copy

num_dilate = 3
radius = 5
dilate_bar = mwindow.set_trackbar("dilate", 10, num_dilate){|v|
  num_dilate = v
  mask = original_mask.dilate(nil, num_dilate)
  mwindow.show mask  
}

radius_bar = mwindow.set_trackbar("radius", 30, radius){|v|
  radius = v
}

owindow.show image
mwindow.show mask

while key = GUI::wait_key
  case key.chr
  when "\e" # esc
    exit
  when "n"
    iwindow.show image.inpaint_ns(mask, radius)
  when "t"
    iwindow.show image.inpaint_telea(mask, radius)
  end
end

