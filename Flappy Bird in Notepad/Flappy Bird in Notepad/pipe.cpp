#include "pipe.h"

Pipe::Pipe(Object o, Vector2D vel) : Object(o)
{
    alive = true;
    velocity = vel;
}

Pipe::Pipe(Vector2D vel, unsigned int w, unsigned int h, std::string nm, wchar_t* b, Vector2D p, unsigned int s) : Object(w, h, nm, b, p, s)
{
    velocity = vel;
}

void Pipe::SetVelocity(Vector2D vel)
{
    velocity = vel;
}

void Pipe::Update()
{
    if (position.GetX() + width >= 0)
    {
        position.Add(velocity);
    }
    else
    {
        Die();
    }
}

bool Pipe::IsDead() { return !alive; }

void Pipe::Die()
{
    alive = false;
}


PipeTB::PipeTB(int g, const std::vector<Object*>& objts)
{
    gap = g;

    // range_min + rand() % (range_max - range_min + 1)
    int bottomY = gap + rand() % (SCREEN_Y - gap + 1);
    Vector2D bottomPos(SCREEN_X + PIPE_WIDTH, bottomY);

    // place bottom randomly
    bottom = new Pipe(*(objts[PIPE_BOTTOM_INDEX]));
    bottom->SetPos(bottomPos);

    // place the top relative to bottom
    top = new Pipe(*(objts[PIPE_TOP_INDEX]));
    top->SetPos(Vector2D(bottom->GetPos().GetX(), bottom->GetPos().GetY() - gap - 150));
}

