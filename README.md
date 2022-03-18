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


[![rtdemo](https://i.imgur.com/4axhpzH.jpg)](https://www.youtube.com/watch?v=Qp7saN0JnSI)


# Instalation

Instructions for MacOS. This project uses minilibx, which in turn uses X11 library.
1. Install X11 and XShm exttension appropriate for your MacOS version.
2. Install minilibx form (here)[https://github.com/gcamerli/minilibx] (instructions for instalation available)
3. You will definetely have to change -L /Library/Developer/CommandLineTools/usr/lib/ -lmlx to the location on libmlx.a on your computer as well as -L/opt/X11/lib -lX11 to the location of libX11.dylib on your computer.
4. Make or make re.

