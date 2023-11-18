# Snowball Moon
A small multiplayer game where you throw snowballs at each other on a snow-covered moon. Made using Raylib

# Gameplay
2-4 players run around the surface of a snow covered moon, trying to throw snowballs at each other and getting a point when they do. The player with the most points after the time limit wins. 
Tap the attack button to throw a snowball normally or hold it down to adjust the angle to throw. 
Controllers or keyboard can be used, with keyboard using either the arrow keys, WASD, IJKL, or the numpad

# Building
Double click the .sln file that was generated in the folder. From here you can use the project as normal.  
Place the latest version of raylib in a folder called raylib-master at the base directory
	
# Building for other OpenGL targets
If you need to build for a different OpenGL version than the default (OpenGL 3.3) you can specify an openGL version in your premake command line. Just modify the bat file or add the following to your command line

## For OpenGL 1.1
--graphics=opengl11

## For OpenGL 2.1
--graphics=opengl21

## For OpenGL 4.3
--graphics=opengl43
