#!/usr/bin/env ruby
# face_detect.rb
require "rubygems"
require "opencv"

include OpenCV

window = GUI::Window.new("face detect")
capture = CvCapture.open
detector = CvHaarClassifierCascade::load("C:/Program Files/OpenCV/data/haarcascades/haarcascade_frontalface_alt.xml")

while true
  key = GUI::wait_key(1)
  image = capture.query
  detector.detect_objects(image){|i|
    image.rectangle! i.top_left, i.bottom_right, :color => CvColor::Red
  }
  window.show image
  next unless key
  case key.chr
  when "\e"
    exit
  end
end
