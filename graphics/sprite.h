#ifndef __SPRITE_H_
#define __SPRITE_H_

#define ALLEGRO_STATICLINK
#include "allegro5/allegro.h"
#include "allegro5/allegro_font.h"
#include "allegro5/allegro_ttf.h"
#include "allegro5/allegro_image.h"
#include "allegro5/allegro_primitives.h"
#include "allegro5/allegro_native_dialog.h"
#include "../main.h"

#include <vector>

class Sprite{
private:
    ALLEGRO_BITMAP* SpriteImage;
public:
    int width;
    int height;
    float rotation;
    float scale_w;
    float scale_h;
    float alpha;
    ALLEGRO_COLOR tint_color;
    ALLEGRO_COLOR sprColor;
    float tintp;
    bool mirror_x;
    bool mirror_y;
    int layer;
    float xpos;
    float ypos;
    bool selfdestruct;


    Sprite();
    Sprite(char* FilePath);
    Sprite(ALLEGRO_BITMAP* MyBitmap);
    void Initialize(char* FilePath);
    void Initialize(ALLEGRO_BITMAP* MyBitmap);
    void SetLayer(int nlayer);
    void SetScale(float x, float y);
    void SetAlpha(float alph);
    void SetColor(ALLEGRO_COLOR tintcolor);
    void SetSelfDestruct();
    ALLEGRO_BITMAP* GetBitmap();
    void SetPosition(float x, float y);
    Sprite* GetPointer();
    void Dispose();
    ~Sprite();
};

#endif // __SPRITE_H_
