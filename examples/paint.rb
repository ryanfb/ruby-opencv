#!/bin/env ruby
# paint.rb
require "rubygems"
gem "opencv"
require "opencv"

include OpenCV

window = GUI::Window.new("free canvas")
canvas = CvMat.new(500, 500, 0, 3).fill!(0xFF) # create white canvas
window.show canvas

colors = CvColor::constants.collect{|i| i.to_s }

usage =<<USAGE
[mouse]
drag            - draw
right button    - fill by color
[keyborad]
1 to 9          - change thickness of line
type color name - change color
esc             - exit
USAGE
puts usage

point = nil

# drawing option
opt = {
  :color => CvColor::Black,
  :tickness => 1
}

window.on_mouse{|m|
  case m.event
  when :move
    if m.left_button?
      canvas.line!(point, m, opt) if point
      point = m
    end
  when :left_button_down
    canvas.line!(m, m, opt)
    point = m
  when :left_button_up
    point = nil
  when :right_button_down
    mask = canvas.flood_fill!(m, opt[:color])    
  end
  window.show canvas
}

color_name = ''
while key = GUI.wait_key
  next if key < 0
  case key.chr
  when "\e" # [esc] - exit
    exit
  when '1'..'9'
    puts "change thickness to #{key.chr.to_i}."
    opt[:thickness] = key.chr.to_i
  else
    color_name << key.chr
    choice = colors.find_all{|i| i =~ /\A#{color_name}/i}
    if choice.size == 1
      color,= choice
      puts "change color to #{color}."
      opt[:color] = CvColor::const_get(color)
    end
    color_name = '' if choice.length < 2
  end
end

