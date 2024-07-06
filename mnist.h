#ifndef CUMBER_MNIST_H
#define CUMBER_MNIST_H

struct MNIST {
    float** images; // [Index of image, Image pixel]
    char* labels;

    int dataset_size;
    int image_size;

    int label_max;
};

struct MNIST* mnist_new(char* images_file, char* labels_file);
void mnist_free(struct MNIST* mnist);

#endif //CUMBER_MNIST_H
