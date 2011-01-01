#!/usr/bin/env ruby
# -*- mode: ruby; coding: utf-8-unix -*- 
require 'test/unit'

if Test::Unit.methods.include? :setup_argv
  src_testdir = File.dirname(File.expand_path(__FILE__))
  srcdir = File.dirname(src_testdir)
  Test::Unit.setup_argv {|files|
    if files.empty?
      [src_testdir]
    else
      files.map {|f|
        if File.exist? "#{src_testdir}/#{f}"
          "#{src_testdir}/#{f}"
        elsif File.exist? "#{srcdir}/#{f}"
          "#{srcdir}/#{f}"
        elsif File.exist? f
          f
        else
          raise ArgumentError, "not found: #{f}"
        end
      }
    end
  }
elsif Test::Unit.constants.include? 'AutoRunner'
  Test::Unit::AutoRunner.run(true, './')
else
  raise Error, 'Test runner not found'
end

