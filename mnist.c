#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include "mnist.h"

struct MNIST* mnist_new(char* images_file, char* labels_file){
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

    mnist->images = malloc(sizeof(float*) * num_images);
    unsigned char buffer[img_size];
    for(int i = 0; i < num_images; i++){
        fread(&buffer, sizeof(unsigned char), img_size, images_fptr);

        mnist->images[i] = malloc(sizeof(float) * img_size);
        for(int j = 0; j < img_size; j++)
            mnist->images[i][j] = buffer[j] / 255.0;
    }

    mnist->dataset_size = num_images;
    mnist->image_size = img_size;

    fclose(images_fptr);
    fclose(labels_fptr);

    return mnist;
}

void mnist_free(struct MNIST* mnist){
    for(int i = 0; i < mnist->dataset_size; i++){
        free(mnist->images[i]);
    }

    free(mnist->labels);
}