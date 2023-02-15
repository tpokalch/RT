# RT
This is a somewhat complex raytracing implementation. It supports  reflection, transparency, light spots, multiple light sources, complex objects, textures, CPU multi-threading, and soft shadows.


<p align="center">
  <img width="80%" src="https://raw.githubusercontent.com/tpokalch/RT/master/screens/Screen%20Shot%202021-04-25%20at%204.16.45%20AM.png">
</p>


<p align="center">
  <img width="80%" src="https://raw.githubusercontent.com/tpokalch/RT/master/screens/Screen%20Shot%202019-09-28%20at%205.22.04%20AM.png">
</p>


<p align="center">
  <img width="80%" src="https://raw.githubusercontent.com/tpokalch/RT/master/screens/Screen%20Shot%202020-08-28%20at%202.48.52%20PM.png">
</p>


Click on video below:



<a href="https://youtu.be/WHm0eooveFc"><img src="https://i.imgur.com/5LH6p8Y.png" alt="" data-canonical-src="https://gyazo.com/eb5c5741b6a9a16c692170a41a49c858.png" width="200" height="150" />

# Installation

Instructions for MacOS. This project uses minilibx, which in turn uses X11 library.
1. For MacOS version newer that MacOS X install X11 and XShm exttension appropriate for your MacOS version.
2. Install minilibx from (here)[https://github.com/gcamerli/minilibx] (instructions for instalation available). This version of the project is written for MacOS Big Sur, so use minilibx version bulid on X11. But it is better to use minilibx (version) [https://github.com/dannywillems/minilibx-mac-osx] that uses Cocoa from Mac OSX, if you have MacOS X. In that case you will have to change the keycode macros defined in rtv1.h
3. You will definetely have to change -L /Location/of/mlx/static/library -lmlx to the location on libmlx.a on your computer as well as -L/Location/of/X11/dynamic/library -lX11 to the location of libX11.dylib on your computer.
4. type make or make re in your command line.
