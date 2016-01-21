# npc-game
Final Fantasy-style Role-Playing Game about the non-heroic non-player characters inhabiting the world in a traditional example of such a game. Multiplatform via Simple Directmedia Layer (SDL2) C++ Library. Also uses Boost.  

Requires the following libraries:  
SDL2 (https://www.libsdl.org/download-2.0.php)  
SDL2_Image (https://www.libsdl.org/projects/SDL_image/)  
SDL2_ttf (https://www.libsdl.org/projects/SDL_ttf/)  
Boost (http://sourceforge.net/projects/boost/files/latest/download?source=files)  

Code by Samuel Weber (sbweber).  

Default keys:  
Arrow keys: Move around the map.  
z: Interact with things on the map.  
esc: Go back/up a layer. Currently unused.  
alt-f4: Quit the program from anywhere.  

Debug keys:  
1: Load "map1.txt".  
2: Load "map2.txt".  
9: Go to the key rebind menu.  
0: Go to the title screen.  
f: Go to the test case for drawing a text box.  

Explanation of Licenses:  
LICENSE is the (MIT) license for this project itself.  
LICENSE-SDL2 is the (zlib) license for the SDL2 library.  
LICENSE.md is the (MIT) license for the FindSDL2*.cmake files, the CMakeLists.txt (further modified from its original version), and some scattered code throughout the project (this project began its life by following the TwinklebearDev SDL 2.0 Lessons, from here https://github.com/Twinklebear/TwinklebearDev-Lessons).  
Copyright.txt is the (BSD 3-clause) license for the Boost libraries.  
