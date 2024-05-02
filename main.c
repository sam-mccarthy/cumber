#define INPUT_SIZE 768
#define OUTPUT_SIZE 10
#define HIDDEN_LAYERS 2
#define HIDDEN_SIZE 300

#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <math.h>

struct MNIST {
    double** images; // [Index of image, Image pixel]
    unsigned char* labels;

    int dataset_size;
    int image_size;
};

double*** allocate_weights(){
    double*** ptr = malloc(sizeof(double**) * (HIDDEN_LAYERS + 1));

    for(int i = 0; i < HIDDEN_LAYERS + 1; i++){
        int left_layer  = i == 0                ? INPUT_SIZE : HIDDEN_SIZE;
        int right_layer = i == HIDDEN_LAYERS    ? OUTPUT_SIZE : HIDDEN_SIZE;

        ptr[i] = malloc(sizeof(double*) * left_layer);

        for(int j = 0; j < left_layer; j++) {
            ptr[i][j] = malloc(sizeof(double) * right_layer);
            for(int k = 0; k < right_layer; k++)
                ptr[i][j][k] = rand() / (double)RAND_MAX;
        }
    }

    return ptr;
}

void destruct_weights(double*** weights){
    for(int i = 0; i < HIDDEN_LAYERS + 1; i++){
        int left_layer  = i == 0                ? INPUT_SIZE : HIDDEN_SIZE;
        int right_layer = i == HIDDEN_LAYERS    ? OUTPUT_SIZE : HIDDEN_SIZE;

        for(int j = 0; j < left_layer; j++)
            free(weights[i][j]);
        free(weights[i]);
    }

    free(weights);
}

void destruct_mnist(struct MNIST* mnist){
    for(int i = 0; i < mnist->dataset_size; i++){
        free(mnist->images[i]);
    }

    free(mnist->labels);
}

struct MNIST* read_mnist(char* images_file, char* labels_file){
    struct MNIST* mnist = malloc(sizeof(struct MNIST));
    FILE* images_fptr;
    FILE* labels_fptr;

    int magic;
    int num_images;
    int num_rows;
    int num_cols;
    int img_size;

    int num_labels;

    images_fptr = fopen(images_file, "r");
    labels_fptr = fopen(labels_file, "r");

    fread(&magic, sizeof(int), 1, images_fptr);
    if(magic != 2051) return NULL;

    fread(&num_images, sizeof(int), 1, images_fptr);
    fread(&num_rows, sizeof(int), 1, images_fptr);
    fread(&num_cols, sizeof(int), 1, images_fptr);
    img_size = num_rows * num_cols;

    fread(&magic, sizeof(int), 1, labels_fptr);
    if(magic != 2049) return NULL;

    fread(&num_labels, sizeof(int), 1, labels_fptr);
    if(num_labels != num_images) return NULL;

    mnist->labels = malloc(sizeof(unsigned char) * num_labels);
    fread(&mnist->labels, sizeof(unsigned char), num_labels, labels_fptr);

    mnist->images = malloc(sizeof(double*) * num_images);
    unsigned char buffer[img_size];
    for(int i = 0; i < num_images; i++){
        fread(&buffer, sizeof(unsigned char), img_size, images_fptr);

        mnist->images[i] = malloc(sizeof(double) * img_size);
        for(int j = 0; j < img_size; j++)
            mnist->images[i][j] = buffer[j] / 255.0;
    }

    mnist->dataset_size = num_images;
    mnist->image_size = img_size;

    return mnist;
}

double sigmoid(double value){
    return 1 / (1 + exp(-value));
}

double** forward_pass(double* inputs, double*** weights){
    double** vectors = malloc(sizeof(double*) * HIDDEN_LAYERS + 2);
    vectors[0] = malloc(sizeof(double) * INPUT_SIZE);
    memcpy(vectors[0], inputs, INPUT_SIZE);

    for(int i = 0; i < HIDDEN_LAYERS + 1; i++){
        int left_layer  = i == 0                ? INPUT_SIZE : HIDDEN_SIZE;
        int right_layer = i == HIDDEN_LAYERS    ? OUTPUT_SIZE : HIDDEN_SIZE;

        vectors[i + 1] = malloc(sizeof(double) * right_layer);
        memset(vectors[i + 1], 0, right_layer);
        for(int j = 0; j < left_layer; j++){
            for(int k = 0; k < right_layer; k++){
                vectors[i + 1][k] += weights[i][j][k] * vectors[i][j];
                if(j == left_layer - 1)
                    vectors[i + 1][k] = sigmoid(vectors[i + 1][k]);
            }
        }
    }

    return vectors;
}

void backpropagate(double** vectors, double*** weights, double* outputs, unsigned char expected_value){

}

void train(struct MNIST* dataset, double*** weights){
    for(int i = 0; i < dataset->dataset_size; i++) {
        double* outputs = forward_pass(dataset->images[i], weights);
        backpropagate(dataset->images[i], weights, outputs);
    }
}

double test(struct MNIST* dataset, double*** weights){
    int successes = 0;
    for(int i = 0; i < dataset->dataset_size; i++) {
        double* outputs = forward_pass(dataset->images[i], weights);

        int max_index = 0;
        double max_confidence = 0;
        for(int j = 0; j < OUTPUT_SIZE; j++)
            if(outputs[j] > max_confidence){
                max_index = j;
                max_confidence = outputs[j];
            }

        if(max_index == dataset->labels[i])
            successes++;
    }

    return successes / (double)dataset->dataset_size;
}

int main() {
    struct MNIST* dataset;
    double*** weights;

    weights = allocate_weights();
    dataset = read_mnist("", "");

    train(dataset, weights);
    test(dataset, weights);

    destruct_weights(weights);
    destruct_mnist(dataset);
}
