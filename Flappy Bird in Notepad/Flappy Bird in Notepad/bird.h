#pragma once

#include <vector>
#include "object.h"

// forward declare to break circular dependency
class NeuralNetwork;

#define PIPE_WIDTH 40
#define PIPE_HEIGHT 150

// flappy bird
class Bird : public Object
{
    Vector2D velocity; // in pixels
    Vector2D gravity;
    bool alive;

    int score;

    NeuralNetwork* nn;
    std::vector<float> inputs; // inputs to be used by agents

public:
    static int totalDeadBirds;

    Bird(Object o, Vector2D vel = Vector2D::Up());

    Bird(Vector2D vel, unsigned int w, unsigned int h, std::string nm, wchar_t* b, Vector2D p, unsigned int s);

    int GetScore() const;

    void SetVelocity(Vector2D vel);

    void Jump();

    void UpdateInputs(std::vector<Object*>* pipes, int closestTopIndex);

    void Update(std::vector<Object*>* pipes, int closestTopIndex);

    // doesnt work because when pos.x reaches the right end of the pipe closest pipe becomes the next
    // so find a different way of increasing the score
    void IncreaseScore(std::vector<Object*>* pipes, int closestTopIndex);

    bool CollidesWith(const std::vector<Object*>* pipes, int closestTopIndex); // closest pipe

    void MutateBird();

    bool IsDead() const;
    void Die();

    // totalDeadBirds is used to create the next generation
    static void ResetTotalDeadBirds();
    static int GetTotalDeadBirds();
};