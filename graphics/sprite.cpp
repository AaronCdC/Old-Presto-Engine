#include <iostream>

#define ALLEGRO_STATICLINK
#include "allegro5/allegro.h"
#include "allegro5/allegro_font.h"
#include "allegro5/allegro_ttf.h"
#include "allegro5/allegro_image.h"
#include "allegro5/allegro_primitives.h"
#include "allegro5/allegro_native_dialog.h"
#include "../main.h"
#include "sprite.h"

Sprite::Sprite()
{
    //WARNING: Make sure to start a new instance when using an empty constructor!!
}

Sprite::Sprite(char* FilePath)
{
    SpriteImage = al_load_bitmap(FilePath);
    width = al_get_bitmap_width(SpriteImage);
    height = al_get_bitmap_height(SpriteImage);
    rotation = .0f;
    scale_h = 1.0f;
    scale_w = 1.0f;
    alpha = 1.0f;
    tint_color = al_map_rgba_f(1.0f,1.0f,1.0f,1.0f);
    tintp = .0f;
    mirror_x = false;
    mirror_y = false;
    layer = 0;
    xpos = .0f;
    ypos = .0f;
    selfdestruct = false;
}

Sprite::Sprite(ALLEGRO_BITMAP* MyBitmap)
{
    SpriteImage = MyBitmap;
    width = al_get_bitmap_width(SpriteImage);
    height = al_get_bitmap_height(SpriteImage);
    rotation = .0f;
    scale_h = 1.0f;
    scale_w = 1.0f;
    alpha = 1.0f;
    tint_color = al_map_rgba_f(1.0f,1.0f,1.0f,1.0f);
    tintp = .0f;
    mirror_x = false;
    mirror_y = false;
    layer = 0;
    xpos = .0f;
    ypos = .0f;
    selfdestruct = false;
}

void Sprite::Initialize(char* FilePath)
{
    SpriteImage = al_load_bitmap(FilePath);
    width = al_get_bitmap_width(SpriteImage);
    height = al_get_bitmap_height(SpriteImage);
    rotation = .0f;
    scale_h = 1.0f;
    scale_w = 1.0f;
    alpha = 1.0f;
    tint_color = al_map_rgba_f(1.0f,1.0f,1.0f,1.0f);
    tintp = .0f;
    mirror_x = false;
    mirror_y = false;
    layer = 0;
    xpos = .0f;
    ypos = .0f;
    selfdestruct = false;
    return;
}

void Sprite::Initialize(ALLEGRO_BITMAP* MyBitmap)
{
    SpriteImage = MyBitmap;
    width = al_get_bitmap_width(SpriteImage);
    height = al_get_bitmap_height(SpriteImage);
    rotation = .0f;
    scale_h = 1.0f;
    scale_w = 1.0f;
    alpha = 1.0f;
    tint_color = al_map_rgba_f(1.0f,1.0f,1.0f,1.0f);
    tintp = .0f;
    mirror_x = false;
    mirror_y = false;
    layer = 0;
    xpos = .0f;
    ypos = .0f;
    selfdestruct = false;
    return;
}

void Sprite::SetLayer(int nlayer)
{
    layer = nlayer;
    return;
}

ALLEGRO_BITMAP* Sprite::GetBitmap()
{
    return SpriteImage;
}

void Sprite::SetPosition(float x, float y)
{
    xpos = x;
    ypos = y;
    return;
}

void Sprite::SetScale(float x, float y)
{
    scale_w = x;
    scale_h = y;
    return;
}

void Sprite::SetAlpha(float alph)
{
    tint_color = al_map_rgba_f(1.0f,1.0f,1.0f,alph);
    return;
}

void Sprite::SetColor(ALLEGRO_COLOR tintcolor)
{
    tint_color = tintcolor;
    return;
}

void Sprite::SetSelfDestruct()
{
    selfdestruct = true;
    return;
}

void Sprite::Dispose()
{
    al_destroy_bitmap(SpriteImage);
    //delete this;
    return;
}

Sprite* Sprite::GetPointer()
{
    return this;
}

Sprite::~Sprite()
{
    //al_destroy_bitmap(SpriteImage);
}
