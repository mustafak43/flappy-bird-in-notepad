#include "bird.h"
#include "neuralnetwork.h"

#include <iostream>

#define SCREEN_Y 150

int Bird::totalDeadBirds = 0;

Bird::Bird(Object o, Vector2D vel) : Object(o)
{
    nn = new NeuralNetwork();

    // inputs will contain 4 variables
    // agent.y - pipe.x - pipe.top.y - pipe.bottom.y
    for (int i = 0; i < 4; i++)
        inputs.push_back(0);

    alive = true;
    velocity = vel;
    gravity = Vector2D(0, 0.2f);

    score = 0;
}

Bird::Bird(Vector2D vel, unsigned int w, unsigned int h, std::string nm, wchar_t* b, Vector2D p, unsigned int s) : Object(w, h, nm, b, p, s)
{
    nn = new NeuralNetwork();

    for (int i = 0; i < 4; i++)
        inputs.push_back(0);

    alive = true;
    velocity = vel;
    gravity = Vector2D(0, 0.2f);

    score = 0;
}

int Bird::GetScore() const { return score; }

void Bird::SetVelocity(Vector2D vel)
{
    velocity = vel;
}

void Bird::Jump()
{
    if (nn->FeedForward(inputs))
    {
        SetVelocity(Vector2D(0, -3));
    }
}

void Bird::UpdateInputs(std::vector<Object*>* pipes, int closestTopIndex)
{
    if (closestTopIndex != -1)
    {
        // agent y
        inputs[0] = position.GetY();
        // pipe x
        inputs[1] = pipes->at(closestTopIndex)->GetPos().GetX();
        // bottom pipe y
        inputs[2] = pipes->at(closestTopIndex + 1)->GetPos().GetY();
        // top pipe y
        inputs[3] = pipes->at(closestTopIndex)->GetPos().GetY() + PIPE_HEIGHT;
    }
}

void Bird::Update(std::vector<Object*>* pipes, int closestTopIndex)
{
    if (pipes->size() > 0)
    {
        UpdateInputs(pipes, closestTopIndex);
        Jump();
        IncreaseScore(pipes, closestTopIndex);

        if (!CollidesWith(pipes, closestTopIndex))
        {
            velocity.Add(gravity);
            position.Add(velocity);
        }
        else Die();
    }
}

// doesnt work because when pos.x reaches the right end of the pipe closest pipe becomes the next
// so find a different way of increasing the score
void Bird::IncreaseScore(std::vector<Object*>* pipes, int closestTopIndex)
{
    if (closestTopIndex != -1)
    {
        if (pipes->at(closestTopIndex)->GetPos().GetX() + PIPE_WIDTH - 1 < position.GetX())
            score++;
    }
}

// closest pipe
bool Bird::CollidesWith(const std::vector<Object*>* pipes, int closestTopIndex)
{
    bool collides = true;

    if (closestTopIndex != -1 && !pipes->empty())
    {
        if (position.GetY() >= 0 && position.GetY() + height <= SCREEN_Y)
        {
            collides = false;
        }
        if (position.GetX() + width > pipes->at(closestTopIndex)->GetPos().GetX())
        {
            collides = true;
            // if its between top and bottom then collides = false;
            if (position.GetY() > pipes->at(closestTopIndex)->GetPos().GetY() + PIPE_HEIGHT
                && position.GetY() + height < pipes->at(closestTopIndex + 1)->GetPos().GetY())
            {
                collides = false;
            }
        }
    }
    else
        collides = false;

    return collides;
}

void Bird::MutateBird()
{
    nn->ApplyMutation();
}

bool Bird::IsDead() const { return !alive; }

void Bird::Die()
{
    alive = false;
    SetActive(false);
    totalDeadBirds++;
}

// totalDeadBirds is used to create the next generation
void Bird::ResetTotalDeadBirds()
{
    totalDeadBirds = 0;
}

int Bird::GetTotalDeadBirds()
{
    return totalDeadBirds;
}