#pragma once

#include <vector>
#include "utils.h"

class Perceptron
{
    std::vector<float> weights;
    float bias;

    const int MUT_RATE;

private:
    float WeightedSum(std::vector<float> inputs);
    float ActivationFunction(float x); // sigmoid function

public:
    Perceptron(int weightsCount);

    float Output(std::vector<float> inputs);
    void Mutate();
};

class NeuralNetwork
{
    std::vector<Perceptron> hiddenLayer;
    std::vector<Perceptron> outputLayer;

    int inputCount;
    int hiddenCount;
    int outputCount;

public:
    NeuralNetwork(int inputCount = 4, int hiddenCount = 4, int outputCount = 1);

    bool FeedForward(std::vector<float> inputs);
    void ApplyMutation();
};