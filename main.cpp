/*********************************************************************/
/*                    - PRESTO: 2D GAME ENGINE -                     */
/*                     By Aaron C.d.C (c) 2015                       */
/*                   SOME RIGHTS MAY BE RESERVED                     */
/*                                                                   */
/*    This piece of software is freeware and open source. You, under */
/*    any circumstances, shall not sell or make profit, in any way,  */
/*    of this software.                                              */
/*                                                                   */
/*    Some portions of this software rely on the ALLEGRO 5 library,  */
/*    and the OpenAL and OpenGL libraries, as well as the Windows    */
/*    API, in order to work. These libraries may have their own      */
/*    license. That license must be distributed with the binaries.   */
/*                                                                   */
/*    This software is distributed "as it is", and without any kind  */
/*    of warranty. When making changes, you should also keep this    */
/*    information within the source, and always keep the original    */
/*    license.                                                       */
/*********************************************************************/

/* IMPORTANT NOTE */
/*
 Originally, the engine was designed to use
 several layers, then render every sprite into
 these, and then merge them with the back buffer.
 
 However, this was terribly slow and that's the
 reason why I decided to rewrite the whole engine.
 
 The truth is, doing this is not only slower, but
 also unnecessary.
 
  - PLEASE USE THIS PROJECT FOR REFERENCE ONLY -
*/


#include <iostream>

#define ALLEGRO_STATICLINK
#include "allegro5/allegro.h"
#include "allegro5/allegro_font.h"
#include "allegro5/allegro_ttf.h"
#include "allegro5/allegro_image.h"
#include "allegro5/allegro_primitives.h"
#include "allegro5\allegro_native_dialog.h"
#include "allegro5\allegro_acodec.h"
#include "allegro5\allegro_audio.h"
#include "allegro5\allegro_memfile.h"

#include "main.h"
#include "graphics/renderer.h"
#include "animator/animator.h"

/* REGION - GLOBAL VARIABLES -*/

extern BatchRenderer MainRenderer;
extern GameDebugger MainDebugger;

ALLEGRO_DISPLAY* display = NULL;
ALLEGRO_BITMAP *background;
ALLEGRO_EVENT_QUEUE *ev_queue;
ALLEGRO_TIMER *maintimer;
ALLEGRO_AUDIO_STREAM *music;
ALLEGRO_EVENT events;

bool exitprogram = false;
bool redraw = false;

bool AudioFail = false;
bool ImageFail = false;
/* - END REGION - */

/* FUNCTION InitializeAllegro() */
/* Initializes the Allegro library, and the game. */
void InitializeAllegro() {
    //MainDebugger.Log("Initializing Allegro.", info_type_message);
	//Allegro startup
	if(!al_init()) {
        MainDebugger.Log("Failed to initialize Allegro! (Null pointer returned by al_init function).", info_type_error);
		al_show_native_message_box(NULL, "Error", "Allegro Init", "Failed to initialize allegro!", NULL, ALLEGRO_MESSAGEBOX_ERROR);
		return;
	}
	/* GAME SYSTEM SETUP */
	//Install the keyboard
	al_install_keyboard();
	//Starts the image addon
	al_init_image_addon();

	if(_ENABLE_MOUSE_) {
		//Installs the mouse
		al_install_mouse();
	}

	if(_ENABLE_SOUND_) {
		//Installs the audio system
		al_install_audio();
		//Initialize audio codecs
		al_init_acodec_addon();
		//Reserve 1 sample for initialization
		al_reserve_samples(1);
	}
	//Create an event queue
	ev_queue = al_create_event_queue();

	/* DISPLAY SYSTEM SETUP */
	//Creates a screen
	if(_FULLSCREEN_)
        al_set_new_display_flags(ALLEGRO_FULLSCREEN);
	display = al_create_display(_SCREEN_W_, _SCREEN_H_);

	if(!display) {
        MainDebugger.Log("Failed to initialize the renderer (al_create_display returns a null pointer).", info_type_error);
		al_show_native_message_box(NULL, "Error", "Allegro Init", "Failed to create display!", NULL, ALLEGRO_MESSAGEBOX_ERROR);
		return;
	}

    MainDebugger.GetSystemSpecs();

	//Sets the title of the window
	al_set_window_title(display, _DISPLAYNAME_);
    if(!MainDebugger.IsD3D(display))
        MainDebugger.GetGLInfo(display);
    else
        MainDebugger.GetD3DInfo(display);

	/* EVENT SYSTEM SETUP */
	//Creates the main game timer
	maintimer = al_create_timer(1.0f/_DESIRED_FPS_);
	//Start the main game timer
	al_start_timer(maintimer);
	//Register main events in the main event queue
	al_register_event_source(ev_queue, al_get_display_event_source(display));
	al_register_event_source(ev_queue, al_get_timer_event_source(maintimer));
	al_register_event_source(ev_queue, al_get_keyboard_event_source());

    al_set_blender(ALLEGRO_ADD, ALLEGRO_ALPHA, ALLEGRO_INVERSE_ALPHA);
	//End
	return;
}

int main(int args, char** argv)
{
    GameDebugger MainDebugger;
    //const char* sampletext = "Cheese";
    //MainDebugger.Log("Debugger is loggin. Say %s", info_type_message, sampletext);
    	//Initialize the allegro system and the game
	InitializeAllegro();

	MainDebugger.Log("Presto Game Engine Version %i.%i", info_type_message, _ENGINEVERSION_MAJOR_, _ENGINEVERSION_MINOR_);

    BatchRenderer MainRenderer;
    MainRenderer.MakeInitialization();
	/* REGION - LOAD RESOURCES - */
	//Load your resources here

    ALLEGRO_BITMAP* testsheet = al_load_bitmap("TestingResources\\illyadance.png");
    //background = al_load_bitmap("TestingResources\\background.gif");
    Sprite backgroundsprite("TestingResources\\background.png");
    backgroundsprite.SetLayer(0);
    backgroundsprite.SetPosition(320.0f, 240.0f);

    Sprite logosprite("TestingResources\\logo.png");
    logosprite.SetLayer(0);
    logosprite.SetPosition(_SCREEN_W_ - 140, 100.0f);
    logosprite.SetScale(.9f, .9f);
    MainRenderer.AddSprite(&backgroundsprite);
    if(!testsheet)
        al_show_native_message_box(display, "Error", "Resources", "Failed to load spritesheet", NULL, ALLEGRO_MESSAGEBOX_ERROR);
    Animator illyaAnimation(testsheet, true, al_map_rgb(0,0,0), 8, 8, 256, 256, 1,1,&MainRenderer, false);
    Animator miyuAnimation(testsheet, true, al_map_rgb(0,0,0), 8, 8, 256, 256, 1,1,&MainRenderer, true);

    illyaAnimation.enableBlur = true;

    std::vector<AnimationLoop> myAnimations;
    AnimationLoop Animation1(0,false,4500.0f);

    Animation1.AddFrame(0,0);
    Animation1.AddFrame(0,1);
    Animation1.AddFrame(0,2);
    Animation1.AddFrame(0,3);
    Animation1.AddFrame(0,4);
    Animation1.AddFrame(0,5);
    Animation1.AddFrame(0,6);
    Animation1.AddFrame(0,7);

    Animation1.AddFrame(1,0);
    Animation1.AddFrame(1,1);
    Animation1.AddFrame(1,2);
    Animation1.AddFrame(1,3);
    Animation1.AddFrame(1,4);
    Animation1.AddFrame(1,5);
    Animation1.AddFrame(1,6);
    Animation1.AddFrame(1,7);


    myAnimations.push_back(Animation1);

    illyaAnimation.SetAnimationLoopList(myAnimations);
    illyaAnimation.SetCurrentAnimation(0);
    illyaAnimation.SetBufferLayer(0);
    illyaAnimation.SetBufferScreenPosition((float)(_SCREEN_W_/2-128), (float)_SCREEN_H_-127);

    std::vector<AnimationLoop> myAnimations2;
    AnimationLoop Animation2(0,false,4500.0f);

    Animation2.AddFrame(2,0);
    Animation2.AddFrame(2,1);
    Animation2.AddFrame(2,2);
    Animation2.AddFrame(2,3);
    Animation2.AddFrame(2,4);
    Animation2.AddFrame(2,5);
    Animation2.AddFrame(2,6);
    Animation2.AddFrame(2,7);

    Animation2.AddFrame(3,0);
    Animation2.AddFrame(3,1);
    Animation2.AddFrame(3,2);
    Animation2.AddFrame(3,3);
    Animation2.AddFrame(3,4);
    Animation2.AddFrame(3,5);
    Animation2.AddFrame(3,6);
    Animation2.AddFrame(3,7);


    myAnimations2.push_back(Animation2);

    miyuAnimation.SetAnimationLoopList(myAnimations2);
    miyuAnimation.SetCurrentAnimation(0);
    miyuAnimation.SetBufferLayer(0);
    miyuAnimation.SetBufferScreenPosition((float)(_SCREEN_W_/2)+128, (float)_SCREEN_H_-127);

    MainRenderer.AddSprite(&logosprite);

	if(_ENABLE_SOUND_) {
		music = al_load_audio_stream("TestingResources\\rave.ogg",3,1024);
		if(!music){
            al_show_native_message_box(display, "Error", "ERROR LOADING ASSET:", "Failed to load resource Liquid.ogg!", NULL, ALLEGRO_MESSAGEBOX_ERROR);
            AudioFail = true;
		}

		if(!AudioFail)al_attach_audio_stream_to_mixer(music, al_get_default_mixer());
	}
	/* - END REGION -*/

	/* - WARNING - DO NOT LOAD ANYTHING INTO MEMORY PAST THIS POINT! */
	/* REGION - MAIN GAME LOOP -*/
bool eventTrigger = false;
	while(!exitprogram)
	{
		//Wait for an event to occur
eventTrigger = al_get_next_event(ev_queue, &events);
		/* REGION - EVENT HANDLER - */

if(eventTrigger)
{
    switch(events.type)
    {
    case ALLEGRO_EVENT_TIMER:
        redraw = true;
        break;
    case ALLEGRO_EVENT_DISPLAY_CLOSE:
        exitprogram = true;
        break;
    case ALLEGRO_EVENT_KEY_DOWN:
        switch(events.keyboard.keycode)
        {
            case ALLEGRO_KEY_ESCAPE:
					//Scape key: exit program
                exitprogram = 1;
            break;
				/* Add more keyboard events here */

				/* End Keyboard Event Handler Region*/
        }
        break;
    }
}else{
            illyaAnimation.UpdateAnimation();
            miyuAnimation.UpdateAnimation();
}

		/* End Event Handler Region */

/*--------------- GAME LOGIC HERE ----------------------------------------------*/

		/* REGION - DRAW ON SCREEN - */
		if(redraw && al_event_queue_is_empty(ev_queue)) {
			//Clear Screen
            al_set_target_bitmap(al_get_backbuffer(display));
			al_clear_to_color(al_map_rgb(64,64,128));
			/* DRAWING ROUTINES HERE */
/*-------------------------------------------------------------------------------*/

            MainRenderer.FullRender();

			/* End Region Drawing Routines*/
			//Swap buffers
			al_flip_display();
			//Clear redraw flag and wait for next frame
			redraw = false;
			MainDebugger.CountFrame();
            //MainRenderer.FlushSprites();
            MainRenderer.FlushLayers();
            MainRenderer.ProcessAllSprites();
		}
		/* End Draw On Screen Region*/
	}
	/* End Main Game Loop Region */

	/* REGION - GAME FINALIZATION -*/
	//Dispose all objects (bitmaps, timers, sounds, etc)
	al_destroy_timer(maintimer);
	if(_ENABLE_SOUND_ && !AudioFail){
		al_detach_audio_stream(music);
		al_drain_audio_stream(music);
	}
	if(!AudioFail){al_destroy_audio_stream(music);}
	al_destroy_display(display);
	/* End Game Finalization Region */
	//End game
    MainDebugger.Log("End of the program.", info_type_message);
    return 0;
}
