#include "c_img.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stdint.h>

int main(void){
    struct rgb_img *im;
    struct rgb_img *grad;
    read_in_img(&im, "3x4.bin");
    calc_energy(im, &grad);
    print_grad(grad);
    return 0;
}

//gcc main.c c_img.c -o main
