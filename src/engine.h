#pragma once

#include <gb/gb.h>
#include <types.h>
#include <stdlib.h>

#include "fixed_points.h"

typedef struct {
    int16_t x,y,z;
} Vector3;

typedef struct {
    int16_t x,y;
} Vector2;

void Vector2_Bound(Vector2*v, int16_t xMin, int16_t yMin, int16_t xMax, int16_t yMax)
{
    (*v).y = MAX(yMin,MIN((*v).y,yMax));
    (*v).x = MAX(xMin,MIN((*v).x,xMax));
}

Vector3 cameraPos;
Vector3 cameraRot;

void Vector3_New_Fixed(fixed x, fixed y, fixed z, Vector3*out)
{
    Vector3 newV = { x, y, z };
    *out = newV;
    free(&newV);
}
void Vector3_New_Int(int16_t x, int16_t y, int16_t z, Vector3*out)
{
    Vector3 newV = { x<<SHIFT_AMOUNT,y<<SHIFT_AMOUNT,z<<SHIFT_AMOUNT };
    *out = newV;
    free(&newV);
}
void Vector3_Add(Vector3* a, Vector3* b, Vector3*out)
{
    Vector3 newV = { a->x+b->x, a->y+b->y, a->z+b->z };
    *out = newV;
    free(&newV);
}
void Vector3_Sub(Vector3* a, Vector3* b, Vector3*out)
{
    Vector3 newV = { a->x-b->x, a->y-b->y, a->z-b->z };
    *out = newV;
    free(&newV);
}
void Vector3_Mult(Vector3* a, int16_t b, Vector3*out)
{
    Vector3 newV = { a->x * b, a->y * b, a->z * b};
    *out = newV;
    free(&newV);
}
void Vector3_Div(Vector3* a, int16_t b, Vector3*out)
{
    Vector3 newV = {a->x / b,a->y / b,a->z / b};
    *out = newV;
    free(&newV);
}
// void Vector3_Scale(Vector3* a, Vector3*b, Vector3*out)
// {
//     Vector3 newV = {a->x * b->x,a->y * b->y,a->z * b->z};
//     *out = newV;
//     free(&newV);
// }
// void Vector3_DivScale(Vector3* a, Vector3*b, Vector3*out)
// {
//     Vector3 newV = {a->x / b->x,a->y / b->y,a->z / b->z};
//     *out = newV;
//     free(&newV);
// }
void Vector3_Project(Vector3*a, int16_t FOV, Vector2*out)
{
    Vector2 newV;
    if(a->z==0)
    {
        Vector2 newVv = {0,0};
        newV=newVv;
        free(&newVv);
    }
    else
    {
        Vector2 newVv = {((a->x*FOV)/(a->z)),((a->y*FOV)/(a->z))};
        newV=newVv;
        free(&newVv);
    }
    newV.x+=80;
    newV.y+=48+24;
    *out = newV;
    free(&newV);
    free(&FOV);
}
void Vector3_Dot(Vector3*a,Vector3*b,int16_t*out)
{
    *out = (a->x*b->x)+(a->y*b->y)+(a->z*b->z);
}
// void Vector3_Cross()
// {

// }

void draw_3d_line(Vector3*a,Vector3*b)
{
    Vector2 sa,sb;
    Vector3_Project(a, 220, &sa);
    Vector3_Project(b, 220, &sb);

    line(sa.x,sa.y,sb.x,sb.y);
    free(&sa); free(&sb);
}

Vector3 vertexList[]=
{
    {256,256,256}, //0
    {256,256,512}, //1
    {256,512,256}, //2
    {256,512,512}, //3
    {512,256,256}, //4
    {512,256,512}, //5
    {512,512,256}, //6
    {512,512,512}, //7
};
Vector2 projectedVertexList[8];
uint8_t vertexIndicies[]=
{
    0,1,
    0,2,
    0,4,
    4,5,
    4,6,
    5,7,
    5,1,
    1,3,
    2,3,
    2,6,
    6,7,
    3,7
};