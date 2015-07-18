#ifndef __RENDERER_H__
#define __RENDERER_H__

#define ALLEGRO_STATICLINK
#include "allegro5/allegro.h"
#include "allegro5/allegro_font.h"
#include "allegro5/allegro_ttf.h"
#include "allegro5/allegro_image.h"
#include "allegro5/allegro_primitives.h"
#include "allegro5/allegro_native_dialog.h"
#include "../main.h"
#include "../graphics/sprite.h"

#include <iostream>

class BatchRenderer{
private:
    std::vector<Sprite*> SPRITES;
    std::vector<ALLEGRO_BITMAP*> LAYERS;
    std::vector<bool> LAYERUPDATE;
public:


    BatchRenderer();
    void MakeInitialization();
    void FlushLayers();
    void FlushSprites();
    void ProcessAllSprites();
    void FullRender();
    void RenderSprite(int sprID);
    void AddSprite(Sprite* NewSprite);
};

#endif // __RENDERER_H__
