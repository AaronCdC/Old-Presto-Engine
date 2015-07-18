/* IMPORTANT NOTE */
/*
 Originally, the engine was designed to use
 several layers (Called Intermediate layers, or IL),
 then render every sprite into these, and then merge them
 with the back buffer.
 
 However, this was terribly slow and that's the
 reason why I decided to rewrite the whole engine.
 
 The truth is, doing this is not only slower, but
 also unnecessary.
*/

#include <iostream>

#define ALLEGRO_STATICLINK
#include "allegro5/allegro.h"
#include "allegro5/allegro_font.h"
#include "allegro5/allegro_ttf.h"
#include "allegro5/allegro_image.h"
#include "allegro5/allegro_primitives.h"
#include "allegro5/allegro_native_dialog.h"
#include "../gamedebug/debugger.h"
#include "../main.h"
#include "../graphics/sprite.h"
#include "renderer.h"

GameDebugger MainDebugger;

BatchRenderer::BatchRenderer(){
    MainDebugger.Log("Batch renderer created.", info_type_message);

    #ifndef _GRAPHIC_LAYERS_
    #define _GRAPHIC_LAYERS 1
    #endif // _GRAPHIC_LAYERS_

    if(_GRAPHIC_LAYERS_ < 1)
    {
       MainDebugger.Log("_GRAPHIC_LAYERS_ is set to a wrong value in \"main.h\". Please set it to a value higher than 0. Defaulting to 1...", info_type_warning);
        #define _GRAPHIC_LAYERS 1
    }

    for(int x = 0; x < _GRAPHIC_LAYERS_; x++)
    {
        ALLEGRO_BITMAP* newLayer = al_create_bitmap(_SCREEN_W_, _SCREEN_H_);
        LAYERS.push_back(newLayer);
        LAYERUPDATE.push_back(false);
        MainDebugger.Log("Layer %i into the IL buffer.", info_type_message, x);
    }
    return;
}

void BatchRenderer::MakeInitialization()
{

    #ifndef _GRAPHIC_LAYERS_
    #define _GRAPHIC_LAYERS_ 1
    #endif // _GRAPHIC_LAYERS_

    if(_GRAPHIC_LAYERS_ < 1)
    {
       MainDebugger.Log("_GRAPHIC_LAYERS_ is set to a wrong value in \"main.h\". Please set it to a value higher than 0. Defaulting to 1...", info_type_warning);
        #define _GRAPHIC_LAYERS_ 1
    }


    if(LAYERS.size() < 1)
    {
        for(int x = 0; x < _GRAPHIC_LAYERS_; x++)
        {
            ALLEGRO_BITMAP* newLayer = al_create_bitmap(_SCREEN_W_, _SCREEN_H_);
            LAYERS.push_back(newLayer);
            LAYERUPDATE.push_back(false);
            MainDebugger.Log("Layer %i into the IL buffer.\n", info_type_message, x);
        }
    }
    MainDebugger.Log("Batch renderer Initialized.", info_type_message);
    return;
}

void BatchRenderer::FlushLayers(){
    if(LAYERS.size() > 0)
        for(int x = 0; x < _GRAPHIC_LAYERS_; x++)
        {
            LAYERUPDATE[x] = false;
            al_set_target_bitmap(LAYERS[x]);
            al_clear_to_color(al_map_rgba(0,0,0,0));
        }
    al_set_target_bitmap(al_get_backbuffer(display));
    return;
}

void BatchRenderer::FlushSprites(){
    //std::cout << "FLUSH\n";
    SPRITES.clear();
    return;
}

void BatchRenderer::RenderSprite(int sprID)
{
    //al_draw_bitmap(SPRITES[sprID]->GetBitmap(), 15,320,0);
    //std::cout << "Rendering " << sprID << " To layer " << SPRITES[sprID]->layer << "\n";

    if(SPRITES[sprID]->layer > _GRAPHIC_LAYERS_ - 1)
    {
        MainDebugger.Log("Can not render sprite %i: wrong layer (max defined layers: %i)", info_type_error,
                         sprID, SPRITES[sprID]->layer, _GRAPHIC_LAYERS_);
        return;
    }

    al_set_target_bitmap(LAYERS[SPRITES[sprID]->layer]);

    int flags = ((SPRITES[sprID]->mirror_x)?ALLEGRO_FLIP_HORIZONTAL:0) | ((SPRITES[sprID]->mirror_y)?ALLEGRO_FLIP_VERTICAL:0);
    al_draw_tinted_scaled_rotated_bitmap(SPRITES[sprID]->GetBitmap(), SPRITES[sprID]->tint_color,
                                         SPRITES[sprID]->width/2, SPRITES[sprID]->height/2,
                                         SPRITES[sprID]->xpos, SPRITES[sprID]->ypos,
                                         SPRITES[sprID]->scale_w, SPRITES[sprID]->scale_h,
                                         SPRITES[sprID]->rotation, flags);

    LAYERUPDATE[SPRITES[sprID]->layer] = true;

    al_set_target_bitmap(al_get_backbuffer(display));
    return;
}

void BatchRenderer::ProcessAllSprites()
{
    //std::cout << "Processing: " << SPRITES.size() << "\n";
    for(int x = 0; x < SPRITES.size(); x++)
    {
        if(!SPRITES[x]->selfdestruct)
            RenderSprite(x);
        else
        {
            delete SPRITES[x];
            SPRITES.erase(SPRITES.begin() + x);
        }
    }
    return;
}

void BatchRenderer::FullRender(){
    al_set_target_bitmap(al_get_backbuffer(display));
    for(int x = 0; x < _GRAPHIC_LAYERS_; x++)
    {
        //std::cout << "Rendering layer " << x << "\n";
        if(LAYERUPDATE[x])
            al_draw_bitmap(LAYERS[x], .0f, .0f, 0);
    }
    return;
}

void BatchRenderer::AddSprite(Sprite* NewSprite)
{
    SPRITES.push_back(NewSprite);
    //std::cout << "Sprite indexed " << SPRITES[SPRITES.size()-1]->width << " " << SPRITES.size() << "\n";
    return;
}
