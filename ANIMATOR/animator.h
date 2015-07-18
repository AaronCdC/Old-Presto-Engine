#ifndef __ANIMATOR_H__
#define __ANIMATOR_H__

#include <vector>

#include "../graphics/renderer.h"
#include "../graphics/sprite.h"

typedef struct frame {
    int row;
    int column;
}FRAME;

class AnimationLoop {
private:
    std::vector<FRAME> AnimFrames;
    int anim_mode;
    bool invert_anim;
    float tick;
    float framerate;
public:
    int TotalFrames;
    unsigned int current_frame;

//Functions
AnimationLoop(int mode, bool inverted, float framerate);
bool UpdateLoop();
void ResetAnimation();
void AddFrame(int rowy, int columnx);
FRAME GetCurrentFrame();
FRAME GetPreviousFrame();
FRAME GetFrame(int index);
};

class Animator {
private:

    ALLEGRO_BITMAP *framesheet;
    int sp_width;
    int sp_height;
    int gridw;
    int gridh;
    int framew;
    int frameh;
    std::vector <AnimationLoop> AnimationList;
    int cur_anim;
    int scalex;
    int scaley;
    BatchRenderer* myRenderer;
public:

    bool hasColorMask;
    bool enableBlur;
    bool enableColorDecomposition;
    ALLEGRO_COLOR mask_color;
    Sprite buffer;
    Sprite blurBuffer, blurBuffer2, blurBuffer3;
    bool updatebuffer;

//FUNCTIONS

Animator(ALLEGRO_BITMAP* SpriteSheet, bool hasMask, ALLEGRO_COLOR maskColor,
                   int gridwidth, int gridheight, int framewidth, int frameheight,
                   int scx, int scy, BatchRenderer* mainRenderer, bool eCD);
void SetAnimationLoopList(std::vector<AnimationLoop> myList);
void SetCurrentAnimation(int animation);
void UpdateAnimation();
void ResetAnimation();
void UpdateBuffer();
void SetBufferScreenPosition(float x, float y);
void SetBufferLayer(int layerno);
void DrawBuffer(float x, float y, ALLEGRO_BITMAP* destination);
void ToBatchRenderer();
~Animator();
};

#endif // __ANIMATOR_H__
