#ifndef __ALLEGROCONFIG_H__
#define __ALLEGROCONFIG_H__

#include "gamedebug/debugger.h"
#include <windows.h>

//DISPLAYNAME: The name to show in the window.
#define _DISPLAYNAME_ "Illya Dance"

//ENGINEVERSION: The version of the engine.
#define _ENGINEVERSION_MAJOR_ 0
#define _ENGINEVERSION_MINOR_ 5

//SCREEN PARAMETERS: The window size (width and height)
#define _SCREEN_W_ 640
#define _SCREEN_H_ 480


#define _FULLSCREEN_ 0
//COLOR DEPTH: How many bits compound a color. Must be 32.
#define _COLOR_DEPTH_ 32

//GAME SWITCHES: Enable and disable certain features.
#define _ENABLE_MOUSE_ 0
#define _ENABLE_JOYSTICK_ 0
#define _ENABLE_SOUND_ 1

//DESIRED FPS: The target FPS you want the game to run at. I'd recommend 50-60.
//50 if the game logic is too heavy. 60 is always desired, specially in games
//that require a high input precission such as fighting games, racing, etc.
//30 might be OK for platform games and RPGs.

//The less FPS, the more logic time, but the updates will be slower.
#define _DESIRED_FPS_ 50

/* IL Buffer */
/* For convenience, I added support for multiple layers.
However, using more than 1 layer can drastically decrease
the performance. My recommendation is to use only one
graphical layer (index 0). However, you can set as many
as you want, at your own risk.

More than 1 is not necessary anyway. Default is 1. */

#define _GRAPHIC_LAYERS_ 1

extern ALLEGRO_DISPLAY* display;
extern GameDebugger MainDebugger;

#endif
