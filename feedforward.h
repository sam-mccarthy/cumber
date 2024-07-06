#ifndef CUMBER_FEEDFORWARD_H
#define CUMBER_FEEDFORWARD_H

#include "mnist.h"

struct WeightSet {
    float* weights;
    float* biases;

    int size_x;
    int size_y;
    int size;
};

struct Layer {
    float* nodes;

    int count;
};

struct FeedForward {
    struct Layer* layers;
    struct WeightSet* weights;

    struct Layer* output_layer;

    int layer_count;
    int weight_count;
    int training_epoch;
};

void weights_init(struct WeightSet* weights, int size_x, int size_y);
void layer_init(struct Layer* layer, int count);

float layer_calc_mse(struct Layer* x, struct Layer* y);

struct FeedForward* ffnn_new(int input_size, int hidden_layers, int hidden_layer_size, int output_size);
void ffnn_free();

void ffnn_set_inputs(struct FeedForward* neural_net, struct Layer* inputs);
void ffnn_forward_pass(struct FeedForward* neural_net);
void ffnn_back_propagate(struct FeedForward* neural_net);

void ffnn_train(struct FeedForward* neural_net, struct MNIST* dataset);
void ffnn_test(struct FeedForward* neural_net, struct MNIST* dataset);

void ffnn_write(struct FeedForward* neural_net, char* filename);
struct FeedForward* ffnn_read(char* filename);

void layer_bridge(struct Layer* x, struct Layer* y, struct WeightSet* weights);

#endif //CUMBER_FEEDFORWARD_H