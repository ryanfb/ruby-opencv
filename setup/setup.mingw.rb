#!/usr/env ruby
# Create OpenCV cygwin library from Windows DLLs.
require 'pathname'
opencv_installed_dir = "C:/Program\ Files/OpenCV"
install_libdir = "lib"
install_includedir = "include"

STDOUT.sync = true
puts ">> Please tell me path installed OpenCV."
puts ">> note: default is #{opencv_installed_dir}"
print "opencv installed directory : "
input = gets.chomp!
opencv_installed_dir = input unless input.empty?
#opencv_installed_dir = opencv_installed_dir.chomp!

puts "[step.1]"
puts ">> Checking directory..."
basedir = Pathname(opencv_installed_dir)
print ">> basedir #{basedir}"
raise "error: directory #{basedir} does not exist." unless File.directory?(basedir)
bindir = basedir + 'bin'
puts "...ok"
print ">> bindir #{bindir}"
raise "error: directory #{bindir} does not exist." unless File.directory?(bindir)
puts "...ok"
libdir = basedir + 'lib'
print ">> libdir #{libdir}"
raise "error: directory #{libdir} does not exist." unless File.directory?(libdir)
puts "...ok"

puts "[step.2]"
puts ">> Searching OpenCV Windows DLLs..."
Dir.foreach(bindir){|filename|
  next unless filename =~ /\d\d\d\.dll\Z/
  dllname = filename.scan(/(\D+)\d+\.dll/)
  `echo EXPORTS > #{dllname}.def`
  `nm "#{libdir + "#{dllname}.lib"}" | grep ' T _' | sed 's/.* T _//' >> #{dllname}.def`
  `dlltool --def #{dllname}.def --dllname "#{filename}" --output-lib #{dllname}.lib`
  `rm #{dllname}.def`
  puts ">> Create #{dllname}.lib success."
}

puts "[step.3]"
puts ">> Move libs to target directory"
puts ">> note: default is #{install_libdir}"
print "target directory : "
input = gets.chomp!
install_libdir = input unless input.empty?
if File.exist?(install_libdir)
  raise "#{install_libdir} is exist. but it is not directory." unless File.directory?(install_libdir)
else
  print "Directory #{install_libdir} does not exist. Create it? [y/n]:"
  raise "Please create directory #{install_libdir}" unless gets.chomp! == 'y'
  `mkdir -p #{install_libdir}`
end
`mv *.lib #{install_libdir}`


puts "[step.4]"
puts ">> Copy OpenCV header files(*.h / *.hpp) to target directory"
puts ">> note: default is #{install_includedir}"
print "target directory : "
input = gets.chomp!
install_includedir = input unless input.empty?
if File.exist?(install_includedir)
  raise "#{install_includedir} is exist. but it is not directory." unless File.directory?(install_libdir)
else
  print "Directory #{install_includedir} does not exist. Create it? [y/n]:"
  raise "Please create directory #{install_includedir}" unless gets.chomp! == 'y'
  `mkdir -p #{install_includedir}`
end
puts ">> Copying header files..."
[
 "cxcore/include",
 "cv/include",
 "cvaux/include",
 "ml/include",
 "otherlibs/cvcam/include",
 "otherlibs/highgui"
].each{|i|
  if File.directory?(basedir + i)
    Dir.foreach(basedir + i){|filename|
      next unless (File.extname(filename) == ".h" || File.extname(filename) == ".hpp")
      `cp "#{basedir + i + filename}" #{install_includedir}`
    }
  end
}

puts "Congratulation! Setup complete."
puts <<MESSAGE
----------------------------------------------------------------------------------------------
[Build Ruby/OpenCV for MinGW]
  cd ..
  ruby extconf.rb --with-opencv-include=#{install_includedir} --with-opencv-lib=#{install_libdir}
  make
  make install
----------------------------------------------------------------------------------------------
MESSAGE

