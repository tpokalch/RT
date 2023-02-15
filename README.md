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


[![rtdemo](https://i.imgur.com/h7rqq11.png | width=100)](https://youtu.be/WHm0eooveFc)

src="https://i.imgur.com/h7rqq11.png" alt="" data-canonical-src="https://youtu.be/WHm0eooveFc" width="200" height="400" />

src="https://camo.githubusercontent.com/331400aee821efda2e36ee9b3bc8bce93b975109/68747470733a2f2f6779617a6f2e636f6d2f65623563353734316236613961313663363932313730613431613439633835382e706e67" alt="" data-canonical-src="https://gyazo.com/eb5c5741b6a9a16c692170a41a49c858.png" width="200" height="400" />

# Installation

Instructions for MacOS. This project uses minilibx, which in turn uses X11 library.
1. For MacOS version newer that MacOS X install X11 and XShm exttension appropriate for your MacOS version.
2. Install minilibx from (here)[https://github.com/gcamerli/minilibx] (instructions for instalation available). This version of the project is written for MacOS Big Sur, so use minilibx version bulid on X11. But it is better to use minilibx (version) [https://github.com/dannywillems/minilibx-mac-osx] that uses Cocoa from Mac OSX, if you have MacOS X. In that case you will have to change the keycode macros defined in rtv1.h
3. You will definetely have to change -L /Location/of/mlx/static/library -lmlx to the location on libmlx.a on your computer as well as -L/Location/of/X11/dynamic/library -lX11 to the location of libX11.dylib on your computer.
4. type make or make re in your command line.
