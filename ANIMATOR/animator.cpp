#include <iostream>
#include <vector>

#include <allegro5\allegro.h>
#include <allegro5\allegro_native_dialog.h>
#include <allegro5\allegro_image.h>
#include "animator.h"

// ANIMATOR CLASS FUNCTIONS

Animator::Animator(ALLEGRO_BITMAP* SpriteSheet, bool hasMask, ALLEGRO_COLOR maskColor,
                   int gridwidth, int gridheight, int framewidth, int frameheight,
                   int scx, int scy, BatchRenderer* mainRenderer, bool eCD)
{
    framesheet = SpriteSheet;
    hasColorMask = hasMask;
    if(hasMask)
        mask_color = maskColor;

    sp_width = al_get_bitmap_width(framesheet);
    sp_height = al_get_bitmap_height(framesheet);

    gridw = gridwidth;
    gridh = gridheight;

    frameh = frameheight;
    framew = framewidth;

    scalex = scx;
    scaley = scy;

    ALLEGRO_BITMAP* mybuffer = al_create_bitmap(framew, frameh);
    ALLEGRO_BITMAP* bbuffer = al_create_bitmap(framew, frameh);
    ALLEGRO_BITMAP* bbuffer2 = al_create_bitmap(framew, frameh);
    buffer.Initialize(mybuffer);
    buffer.SetScale(scalex,scaley);
    blurBuffer.Initialize(bbuffer);
    blurBuffer.SetAlpha(0.75f);
    blurBuffer.SetScale(scalex,scaley);
    blurBuffer2.Initialize(bbuffer2);
    blurBuffer2.SetAlpha(0.50f);
    blurBuffer2.SetScale(scalex,scaley);
    blurBuffer3.Initialize(bbuffer2);
    blurBuffer3.SetAlpha(0.50f);
    blurBuffer3.SetScale(scalex,scaley);
    enableColorDecomposition = eCD;
    if(enableColorDecomposition)
    {
        blurBuffer.SetColor(al_map_rgba_f(1.0f, .0f, .0f, .9f));
        blurBuffer2.SetColor(al_map_rgba_f(.0f, 1.0f, .0f, .9f));
        blurBuffer3.SetColor(al_map_rgba_f(.0f, .0f, 1.0f, .9f));
    }
    myRenderer = mainRenderer;
    updatebuffer = true;
    enableBlur = false;
    ToBatchRenderer();
}

void Animator::SetAnimationLoopList(std::vector<AnimationLoop> myList)
{
    AnimationList = myList;
    return;
}

void Animator::SetCurrentAnimation(int animation)
{
    cur_anim = animation;
    return;
}

void Animator::UpdateAnimation()
{
    updatebuffer = AnimationList[cur_anim].UpdateLoop();
    if(updatebuffer)
        UpdateBuffer();

    //ToBatchRenderer();
    return;
}

void Animator::ResetAnimation()
{
    AnimationList[cur_anim].ResetAnimation();
    return;
}

void Animator::UpdateBuffer()
{
    FRAME current = AnimationList[cur_anim].GetCurrentFrame();
    al_set_target_bitmap(buffer.GetBitmap());
    al_clear_to_color(al_map_rgba(0,0,0,0));
    al_draw_bitmap_region(framesheet,framew*current.column, frameh*current.row,
                          framew, frameh,.0f,.0f,0);

    if(enableBlur || enableColorDecomposition){

    FRAME previous = AnimationList[cur_anim].GetFrame(AnimationList[cur_anim].current_frame-1);

    al_set_target_bitmap(blurBuffer.GetBitmap());
    al_clear_to_color(al_map_rgba(0,0,0,0));
    al_draw_bitmap_region(framesheet,framew*previous.column, frameh*previous.row,
                          framew, frameh,.0f,.0f,0);

    FRAME previous2 = AnimationList[cur_anim].GetFrame(AnimationList[cur_anim].current_frame-2);

    al_set_target_bitmap(blurBuffer2.GetBitmap());
    al_clear_to_color(al_map_rgba(0,0,0,0));
    al_draw_bitmap_region(framesheet,framew*previous2.column, frameh*previous2.row,
                          framew, frameh,.0f,.0f,0);

    }
    //ToBatchRenderer();
    updatebuffer = false;
    return;
}

void Animator::ToBatchRenderer()
{
    //TODO: HAX: REMOVE THESE HARD-CODED PARAMETERS
    //buffer.SetLayer(1);
    //buffer.SetPosition(_SCREEN_W_/2,_SCREEN_H_-buffer.height);
    if(enableColorDecomposition)
        myRenderer->AddSprite(&blurBuffer3);
    myRenderer->AddSprite(&blurBuffer2);
    myRenderer->AddSprite(&blurBuffer);
    myRenderer->AddSprite(&buffer);
    return;
}

void Animator::SetBufferScreenPosition(float x, float y)
{
    buffer.xpos = x;
    buffer.ypos = y;
    blurBuffer.xpos = x-1;
    blurBuffer.ypos = y;
    blurBuffer2.xpos = x+1;
    blurBuffer2.ypos = y;
    blurBuffer3.xpos = x;
    blurBuffer3.ypos = y;
    return;
}

void Animator::SetBufferLayer(int layerno)
{
    buffer.layer = layerno;
    blurBuffer.layer = layerno;
    blurBuffer2.layer = layerno;
    blurBuffer3.layer = layerno;
    return;
}

void Animator::DrawBuffer(float x, float y, ALLEGRO_BITMAP* destination)
{
    //FRAME current = AnimationList[cur_anim].GetCurrentFrame();
    //std::cout << "Drawing Requested: " << current.column << " : " <<  current.row << "\n";
    al_set_target_bitmap(destination);
    //al_draw_bitmap_region(framesheet,framew*current.column, frameh*current.row,
    //                      framew, frameh,.0f,.0f,0);
    if(enableBlur || enableColorDecomposition){
    al_draw_bitmap(blurBuffer3.GetBitmap(), x, y, 0);
    al_draw_bitmap(blurBuffer2.GetBitmap(), x, y, 0);
    al_draw_bitmap(blurBuffer.GetBitmap(), x, y, 0);
    }
    al_draw_bitmap(buffer.GetBitmap(), x, y, 0);
    return;
}

Animator::~Animator()
{
        //al_destroy_bitmap(framesheet);
        //al_destroy_bitmap(buffer.GetBitmap());
}

//ANIMATIONLOOP FUNCTIONS

AnimationLoop::AnimationLoop(int mode, bool inverted, float FrameRate)
{
    invert_anim = inverted;
    anim_mode = mode;
    current_frame = 0;
    framerate = FrameRate;
    tick = 0;
}

bool AnimationLoop::UpdateLoop()
{
    tick+=0.01f;
    bool BufferMustUpdate = false;
    if(tick >= framerate)
    {
        tick = 0.0f;
        current_frame++;
        if(current_frame >= AnimFrames.size())
            current_frame = 0;
        BufferMustUpdate = true;
    }
    return BufferMustUpdate;
}

void AnimationLoop::ResetAnimation()
{
    current_frame = 0;
    return;
}

void AnimationLoop::AddFrame(int rowy, int columnx)
{
    FRAME newFrame;
    newFrame.column = columnx;
    newFrame.row = rowy;
    AnimFrames.push_back(newFrame);
    return;
}

FRAME AnimationLoop::GetPreviousFrame()
{
    if(current_frame-1 < 0)
        return AnimFrames[AnimFrames.size()-1];
    else
        return AnimFrames[current_frame-1];
}

FRAME AnimationLoop::GetFrame(int index)
{
    if(index < 0)
        return AnimFrames[(AnimFrames.size()-1)-index];
    else
        return AnimFrames[index];
}

FRAME AnimationLoop::GetCurrentFrame()
{
    return AnimFrames[current_frame];
}
