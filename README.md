# npc-game
Final Fantasy-style Role-Playing Game about the non-heroic non-player characters inhabiting the world in a traditional example of such a game. Multiplatform via Simple Directmedia Layer (SDL2) C++ Library.  

Requires the following libraries:  
SDL2 (https://www.libsdl.org/download-2.0.php)  
SDL2_Image (https://www.libsdl.org/projects/SDL_image/)  
SDL2_ttf (https://www.libsdl.org/projects/SDL_ttf/)  

Code by Samuel Weber (sbweber).  
npc-game.rar contains a playable, self-contained, current Windows release build of the game.  

Default Controls:  
Arrow keys: Move around the map.  
z: Interact with things on the map.  
esc: Go back/up a layer. Currently unused.  
F4: Quit the program from anywhere.  
F12: Go to the title screen.  
Click on a tile on the map to move to it.  
Click on a sprite to interact with it.  
To advance text, press any key or click anywhere in the window.  

Debug Controls:  
1: Load map "0,0.txt".  
2: Load map "0,1.txt".  
9: Go to the key rebind menu.  
f: Go to the test case for battles. Currently locks you into the battle state with no way out except the debug controls.  

Explanation of Licenses:  
LICENSE is the (MIT) license for this project itself.  
LICENSE-SDL2 is the (zlib) license for the SDL2 library.  
LICENSE.md is the (MIT) license for the FindSDL2*.cmake files, the CMakeLists.txt (further modified from its original version), and some scattered code throughout the project (this project began its life by following the TwinklebearDev SDL 2.0 Lessons, from here https://github.com/Twinklebear/TwinklebearDev-Lessons).  
