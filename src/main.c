#include <gb/gb.h>
#include <stdio.h>
#include <rand.h>
#include <gb/drawing.h>
#include <types.h>
#include <gb/bgb_emu.h>

#define MIN(A,B) ((A)<(B)?(A):(B))
#define MAX(A,B) ((A)>(B)?(A):(B))

#include "engine.h"
#include "fixed_points.h"

#include "../res/tiles.h"
#define Blank_Tiles BlankTiles

uint8_t joy;

uint16_t i=0;
uint16_t FOV=30;
Vector3 v;
Vector2 projection;

Vector3 vOffset;

void main(){
    //int16_t test = (20<<SHIFT_AMOUNT); //Should be 20
    SPRITES_8x8;
    DISPLAY_OFF;
    HIDE_BKG;
    HIDE_SPRITES;

    DISPLAY_ON;

    Vector3_New_Fixed(0, 0, 0, &cameraPos);
    

    // printf("%x\n", test);
    // printf("%u\n", test);
    // printf("%u\n", test>>SHIFT_AMOUNT);
    // printf("%u\n", test&SHIFT_MASK / 6 / 2);
    //printf("%d.%lld\n", test >> SHIFT_AMOUNT, (long long)(test & SHIFT_MASK) * 100000 / (1 << SHIFT_AMOUNT));

    box(0,0,0,0,M_NOFILL); //For some reason doing this at the start makes it so 'color()' actually works the first time lol

    color(WHITE, BLACK, SOLID);
    box(0,0,159,23,M_FILL);
    box(0,120,159,143,M_FILL);
    box(2,2,155,6,M_FILL);

    gotogxy(1,1);
    // gprintf("%u %u", pa.x, pa.y);
    // gotogxy(1,2);
    // gprintf("%u %u", pb.x, pb.y);
    // gotogxy(1,3);
    // gprintf("%u %u", pc.x, pc.y);

    Vector3_New_Fixed(0,0,512, &vOffset);

    while(TRUE) {
        set_sprite_data(76, 240, Blank_Tiles); //Clear render viewport (cpy to vram)

        //BGB_MESSAGE("Test");
        
        joy = joypad(); //Scan inputs
        if(joy & J_UP)
        {
            cameraPos.z+=40;
        }
        if(joy & J_DOWN)
        {
            cameraPos.z-=40;
        }
        if(joy & J_LEFT)
        {
            cameraPos.x-=40;
        }
        if(joy & J_RIGHT)
        {
            cameraPos.x+=40;
        }
        if(joy & J_A)
        {
            cameraPos.y+=40;
        }
        if(joy & J_B)
        {
            cameraPos.y-=40;
        }
        if(joy & J_START)
        {
            FOV+=1;
        }

        //Debug print
        gotogxy(1,1);
        gprintf("%x   ", cameraPos.x);
        gotogxy(6,1);
        gprintf("%x   ", cameraPos.y);
        gotogxy(11,1);
        gprintf("%x   ", cameraPos.z);

        //Precalculate vertex projections
        for(i=0; i<sizeof(vertexList)/sizeof(vertexList[0]); i++)
        {
            Vector3_Add(&vertexList[i], &vOffset, &v);
            Vector3_Sub(&v, &cameraPos, &v);
            Vector3_Project(&v, FOV, &projection);
            projectedVertexList[i] = projection;
        }

        //Draw with projected vertices
        for(i=0; i<sizeof(vertexIndicies)/sizeof(vertexIndicies[0]); i+=2)
        {
            line(projectedVertexList[vertexIndicies[i]].x, projectedVertexList[vertexIndicies[i]].y, projectedVertexList[vertexIndicies[i+1]].x, projectedVertexList[vertexIndicies[i+1]].y);
        }
        
        wait_vbl_done();
    }
}