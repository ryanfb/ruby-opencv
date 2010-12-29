#!/usr/bin/env ruby
#/usr/local/bin/ruby
=begin
create Makefile script for Ruby/OpenCV

usage : ruby extconf.rb
        make && make install

VC : ruby extconf.rb
     nmake
=end
require "mkmf"

# option "opencv"
# extconf.rb --with-opencv-lib=/path/to/opencv/lib
# extconf.rb --with-opencv-include=/path/to/opencv/include

dir_config("opencv", "/usr/local/include/opencv2", "/usr/local/lib")
if CONFIG["arch"].include?("darwin")
  dir_config("ffcall", "/opt/local/include", "/opt/local/lib")
else
  dir_config("ffcall", "/usr/local/include", "/usr/local/lib")
end
dir_config("libxml2", "/usr/include", "/usr/lib")

opencv_headers = ["core/core_c.h", "core/core.hpp", "imgproc/imgproc_c.h",
                  "imgproc/imgproc.hpp", "video/tracking.hpp", "features2d/features2d.hpp",
                  "flann/flann.hpp", "calib3d/calib3d.hpp", "objdetect/objdetect.hpp",
                  "legacy/compat.hpp", "legacy/legacy.hpp", "highgui/highgui_c.h",
                  "highgui/highgui.hpp"]

opencv_libraries = ["opencv_calib3d", "opencv_contrib", "opencv_core", "opencv_features2d",
                    "opencv_flann", "opencv_gpu", "opencv_highgui", "opencv_imgproc",
                    "opencv_legacy", "opencv_ml", "opencv_objdetect", "opencv_video"]


puts ">> check require libraries..."
case CONFIG["arch"]
when /mswin32/
  have_library("msvcrt", nil)
  opencv_libraries.each{|lib|
    have_library(lib)
  }
else
  opencv_libraries.each{|lib|
    raise "lib#{lib} not found." unless have_library(lib)
  }
  #have_library("ml")
  have_library("stdc++")
end

# check require headers
puts ">> check require headers..."
opencv_headers.each{|header|
  raise "#{header} not found." unless have_header(header)
}
#have_header("ml.h")
have_header("stdarg.h")

# check require functions.
# todo

# optional libraies check.
puts ">> ----- optional -----"
puts ">> check ffcall..."
# check ffcall
if have_library("callback") && have_header("callback.h")
  puts ">> support OpenCV::GUI"
else
  puts ">> ! unsupport OpenCV::GUI (if need it. install ffcall)"
  puts "http://www.haible.de/bruno/packages-ffcall.html"
end

# step-final. create Makefile
create_makefile("opencv")
