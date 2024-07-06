#include <stdlib.h>
#include <libc.h>
#include "feedforward.h"

void weights_init(struct WeightSet* weights, int size_x, int size_y){
    weights->size_x = size_x;
    weights->size_y = size_y;
    weights->size = weights->size_x * weights->size_y;

    weights->weights = malloc(sizeof(float) * weights->size);
    for(int i = 0; i < weights->size; i++)
        weights->weights[i] = rand() / (float)RAND_MAX;
    weights->biases = malloc(sizeof(float) * weights->size_y);
}

void layer_init(struct Layer* layer, int count){
    layer->count = count;
    layer->nodes = malloc(sizeof(float) * layer->count);
    memset(layer->nodes, 0, layer->count);
}

float layer_calc_mse(struct Layer* x, struct Layer* y){
    int count = x->count > y->count ? y->count : x->count;
    float total = 0;

    for(int i = 0; i < count; i++) {
        float diff = x->nodes[i] - y->nodes[i];
        total += diff * diff;
    }

    return total / count;
}

struct FeedForward* ffnn_new(int input_size, int hidden_layers, int hidden_layer_size, int output_size) {
    struct FeedForward* neural_net = malloc(sizeof(struct FeedForward));
    neural_net->layer_count = 2 + hidden_layers;
    neural_net->weight_count = 1 + hidden_layers;

    neural_net->layers = malloc(sizeof(struct Layer) * neural_net->layer_count);
    neural_net->weights = malloc(sizeof(struct WeightSet) * (neural_net->weight_count));

    layer_init(neural_net->layers, input_size);
    for(int i = 0; i < neural_net->weight_count; i++){
        int layer_x_size, layer_y_size;

        if(i == 0) {
            layer_x_size = input_size;
            layer_y_size = hidden_layers > 0 ? hidden_layer_size : output_size;
        } else if(i == neural_net->layer_count - 1) {
            layer_x_size = hidden_layers > 0 ? hidden_layer_size : input_size;
            layer_y_size = output_size;
        } else {
            layer_x_size = hidden_layer_size;
            layer_y_size = hidden_layer_size;
        }

        layer_init(neural_net->layers + i + 1, layer_y_size);
        weights_init(neural_net->weights + i, layer_x_size, layer_y_size);
    }

    neural_net->output_layer = neural_net->layers + neural_net->layer_count - 1;

    neural_net->training_epoch = 0;

    return neural_net;
}

void ffnn_free(struct FeedForward* neural_net) {
    free(neural_net->layers);
    free(neural_net->weights);
    free(neural_net);
}

void ffnn_set_inputs(struct FeedForward* neural_net, struct Layer* inputs){
    memcpy(neural_net->layers, inputs, sizeof(struct Layer));
}

void ffnn_forward_pass(struct FeedForward* neural_net) {
    for(int weight_idx = 0; weight_idx < neural_net->weight_count; weight_idx++){
        struct WeightSet* weight_set = neural_net->weights + weight_idx;
        layer_bridge(neural_net->layers + weight_idx, neural_net->layers + weight_idx + 1, weight_set);
    }
}

static float relu_derivative(float x){
    return x < 0 ? 0 : 1;
}

void ffnn_back_propagate(struct FeedForward* neural_net, struct Layer* expected) {
    struct Layer error;
    layer_init(&error, neural_net->output_layer->count);

    for(int i = 0; i < error.count; i++)
        error.nodes[i] = neural_net->output_layer->nodes[i] - expected->nodes[i];

    for(int weight_set_idx = neural_net->weight_set_count - 1; weight_set_idx >= 0; weight_set_idx--){
        struct WeightSet* weight_set = neural_net->weights + weight_set_idx;

        for(int weight_idx = 0; weight_idx <= weight_set->size; weight_idx++){
            float delta;
            if(weight_idx == neural_net->weight_count - 1){
                delta = relu_derivative()
            }
        }

    }
}

void ffnn_train(struct FeedForward* neural_net, struct MNIST* dataset) {

}

void ffnn_test(struct FeedForward* neural_net, struct MNIST* dataset) {

}

void ffnn_write(struct FeedForward* neural_net, char* filename) {

}

struct FeedForward* ffnn_read(char* filename) {
    return NULL;
}

void layer_bridge(struct Layer* x, struct Layer* y, struct WeightSet* weights){
    for(int idy; idy < weights->size_y; idy++){
        y->nodes[idy] = weights->biases[idy];
        for(int idx; idx < weights->size_x; idx++){
            y->nodes[idy] += x->nodes[idx] * weights->weights[idx + idy * weights->size_x];
        }

        if(y->nodes[idy] < 0)
            y->nodes[idy] = 0;
    }
}