#include "seamcarving.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stdint.h>

void print_arr(double *arr, size_t height, size_t width){
    int i, j;
    for(i = 0; i < height; i++){
        for(j = 0; j < width; j++){
            printf("%lf ", arr[i*width+j]);
        }
        printf("\n");
    }
}
int main(void){
    struct rgb_img *im;
    struct rgb_img *grad;
    struct rgb_img *dest;
    double *best_arr;
    int *path;
    read_in_img(&im, "6x5.bin");
    calc_energy(im, &grad);
    print_grad(grad);
    dynamic_seam(grad, &best_arr);
    print_arr(best_arr, grad->height, grad->width);
    recover_path(best_arr, grad->height, grad->width, &path);

    //printing the path
    printf("the path is\n");
    for(int k = 0; k < grad->height; k++){
        printf("%d\n", path[k]);
    }

    remove_seam(im, &dest, path);
    write_img(dest, "carved.bin");


    return 0;
}

//gcc main.c seamcarving.c c_img.c -o main
