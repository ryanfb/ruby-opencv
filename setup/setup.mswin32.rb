#!C:/Ruby/bin/ruby.exe
# Create OpenCV library for VisualC++.
raise 'Should use Ruby-mswin32.' unless RUBY_PLATFORM =~ /mswin32/
require 'pathname'
opencv_installed_dir = "C:/Program\ Files/OpenCV"
install_libdir = "lib"
install_includedir = "include"

STDOUT.sync = true
puts ">> Please tell me CPU archtecture."
archs = ["x86", "amd64", "ia64"]
archs.each_with_index{|v, i|
  puts "[#{i}].#{v}"
}
print "CPU archtecture : "
input = gets.chomp!.to_i
arch = archs[input]
puts "you choose #{arch}"

puts ">> Please tell me path installed OpenCV."
puts ">> note: default is #{opencv_installed_dir}"
print "opencv installed directory : "
input = gets.chomp!
opencv_installed_dir = input unless input.empty?

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
  open("#{dllname}.def", "w"){|def_file|
    def_file.puts "LIBRARY #{filename}"
    def_file.puts "EXPORTS"
    `dumpbin.exe /exports "#{bindir + filename}"`.split("\n").each{|line|
      f = line.scan(/\A\s+\d+\s+[0-9A-F]+\s+[0-9A-F]+\s+(.*)/).first
      def_file.puts f if f
    }
  }
  `lib.exe /def:#{dllname}.def /machine:#{arch}`
  File.unlink("#{dllname}.def")
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
  `md #{install_libdir}`
end
puts "move"
`move *.lib #{install_libdir}`

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
  `md #{install_includedir}`
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
      `copy \"#{(basedir + i + filename).to_s.gsub("/", "\\")}\" #{install_includedir}`
    }
  end
}
