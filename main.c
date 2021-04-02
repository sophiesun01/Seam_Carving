#include "c_img.h"
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
    double *best_arr;
    read_in_img(&im, "6x5.bin");
    calc_energy(im, &grad);
    print_grad(grad);
    //dynamic_seam(grad, &best_arr);
    //print_arr(best_arr, grad->height, grad->width);
    return 0;
}

//gcc main.c c_img.c -o main
