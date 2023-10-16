#pragma once

#include "object.h"
#include <vector>

#define SCREEN_X 150 // screen width
#define SCREEN_Y 150 // screen height

#define PIPE_WIDTH 40

#define PIPE_BOTTOM_INDEX 2
#define PIPE_TOP_INDEX 3

// represents either top or bottom pipe
class Pipe : public Object
{
    Vector2D velocity;
    bool alive;

public:
    Pipe(Object o, Vector2D vel = Vector2D::Left());

    Pipe(Vector2D vel, unsigned int w, unsigned int h, std::string nm, wchar_t* b, Vector2D p, unsigned int s);

    void SetVelocity(Vector2D vel);

    void Update();

    bool IsDead();

    void Die();
};


// encapsulator struct to hold both top and bottom pipe
class PipeTB
{
public:
    Object* top;
    Object* bottom;

    int gap;

    PipeTB(int g, const std::vector<Object*>& objts);
};
