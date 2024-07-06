#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <math.h>
#include "mnist.h"

int main() {
    struct MNIST* dataset;
    struct FeedForward* neural_net;

    dataset = mnist_new("", "");
    RELU_derivative();
    train(dataset, weights);
    test(dataset, weights);

    destruct_weights(weights);
    destruct_mnist(dataset);
}
