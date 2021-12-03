# -*- ruby -*-

require 'rubygems'
require 'hoe'
require './lib/version'

Hoe.new('opencv', OpenCV::VERSION) do |p|
  p.author = ['Masakazu Yonekura']
  p.changes = p.paragraphs_of('History.txt', 0..1).join("\n\n")
  p.description = <<EOF
OpenCV wrapper for Ruby
EOF
  p.rubyforge_name = 'opencv'
  p.developer('lsxi', 'masakazu.yonekura@gmail.com')
  p.email = ['masakazu.yonekura@gmail.com']
  
  p.need_tar = false
  p.need_zip = false
  # p.rdoc_pattern = 
  # p.remote_rdoc_dir =
  # p.rsync =
  p.spec_extras = {
    :extensions => %w{ext/extconf.rb}
  }
  p.summary = 'OpenCV wrapper for Ruby.'
  # p.test_globs = 'spec/**/*_spec.rb'
  p.clean_globs |= ['*.o']
  
  p.url = 'http://blueruby.mydns.jp/opencv'
  
  # p.extra_deps << 
end

# vim: syntax=Ruby
