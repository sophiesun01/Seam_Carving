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
    // struct rgb_img *im;
    // struct rgb_img *grad;
    // struct rgb_img *dest;
    // double *best_arr;
    // int *path;
    // read_in_img(&im, "6x5.bin");
    // calc_energy(im, &grad);
    // print_grad(grad);
    // dynamic_seam(grad, &best_arr);
    // print_arr(best_arr, grad->height, grad->width);
    // recover_path(best_arr, grad->height, grad->width, &path);
    // //printing the path
    // printf("the path is\n");
    // for(int k = 0; k < grad->height; k++){
    //     printf("%d\n", path[k]);
    // }

    // remove_seam(im, &dest, path);
    // write_img(dest, "carved.bin");
    // print_grad(dest);

    // return 0;
    struct rgb_img *im;
    struct rgb_img *cur_im;
    struct rgb_img *grad;
    double *best;
    int *path;

    read_in_img(&im, "bin_files/HJoceanSmall.bin");
    
    for(int i = 0; i < 220; i++){
        printf("i = %d\n", i);
        calc_energy(im,  &grad);
        dynamic_seam(grad, &best);
        recover_path(best, grad->height, grad->width, &path);
        remove_seam(im, &cur_im, path);
        if (i % 20 == 0){
            char filename[200];
            sprintf(filename, "bin_files/resizing_%d.bin", i);
            write_img(cur_im, filename);
        }

        destroy_image(im);
        destroy_image(grad);
        free(best);
        free(path);
        
        im = cur_im;
    }
    destroy_image(im);
    return 0;
}

//gcc main.c seamcarving.c c_img.c -o main

