= opencv

OpenCV Sourceforge Project
http://sourceforge.net/projects/opencvlibrary/

Ruby/OpenCV Author's Web Page
http://blueruby.mydns.jp/opencv

== DESCRIPTION:

OpenCV Ruby Wrapper

== FEATURES/PROBLEMS:

* First release rubygems, Some OpenCV function wrapped.

== SYNOPSIS:

# Show image via GUI Window.

require "rubygems"
gem "opencv"
require "opencv"

image = OpenCV::IplImage.load("sample.jpg")
window = OpenCV::GUI::Window.new("preview")
window.show(image)
OpenCV::GUI::wait_key

# other sample code, see examples/*.rb

== REQUIREMENTS:

* OpenCV 1.0 or later.
  http://sourceforge.net/projects/opencvlibrary/
* ffcall (optional)
  http://www.haible.de/bruno/packages-ffcall.html

== INSTALL:

gem install opencv

== LICENSE:

The BSD Liscense

see LICENSE.txt
