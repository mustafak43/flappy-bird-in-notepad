#include "neuralnetwork.h"

Perceptron::Perceptron(int weightsCount) : MUT_RATE(5) // 5 percent mutation rate
{
    for (int i = 0; i < weightsCount; i++)
    {
        weights.push_back(RandomFloat(-1, 1));
    }
    bias = RandomFloat(-1, 1);
}

float Perceptron::WeightedSum(std::vector<float> inputs)
{
    float sum = 0;
    for (int i = 0; i < inputs.size(); i++)
    {
        sum += weights[i] * inputs[i];
    }
    sum += 1 * bias;

    return sum;
}
float Perceptron::ActivationFunction(float x)
{
    return 1 / (1 + exp(-x));
}
float Perceptron::Output(std::vector<float> inputs)
{
    return ActivationFunction(WeightedSum(inputs));
}
void Perceptron::Mutate()
{
    for (int i = 0; i < weights.size(); i++)
    {
        if (RandomInt(0, 100) < MUT_RATE)
        {
            float dW = RandomFloat(-1, 1); // delta weight
            weights[i] += dW < 0 ? dW + 0.5f : dW - 0.5f;
        }
    }
}

NeuralNetwork::NeuralNetwork(int inputCount, int hiddenCount, int outputCount)
{
    this->inputCount = inputCount;
    this->hiddenCount = hiddenCount;
    this->outputCount = outputCount;

    for (int i = 0; i < hiddenCount; i++)
    {
        hiddenLayer.push_back(Perceptron(inputCount));
    }

    for (int i = 0; i < outputCount; i++)
    {
        outputLayer.push_back(Perceptron(hiddenCount));
    }
}
bool NeuralNetwork::FeedForward(std::vector<float> inputs)
{
    // feed forward goes here
    std::vector<float> hiddenOutputs(hiddenCount, 0);
    std::vector<float> outputOutputs(outputCount, 0);

    // calculate hidden outputs
    for (int i = 0; i < hiddenLayer.size(); i++)
    {
        hiddenOutputs[i] = hiddenLayer[i].Output(inputs);
    }
    // calculate output outputs
    for (int i = 0; i < outputLayer.size(); i++)
    {
        outputOutputs[i] = outputLayer[i].Output(hiddenOutputs);
    }

    // assuming there'll be only 1 output node
    if (outputOutputs[0] >= 0.5f)
        return true;
    return false;
}
void NeuralNetwork::ApplyMutation()
{
    // mutate hidden layer
    for (int i = 0; i < hiddenLayer.size(); i++)
        hiddenLayer[i].Mutate();

    // mutate output layer
    for (int i = 0; i < outputLayer.size(); i++)
        outputLayer[i].Mutate();
}