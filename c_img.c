#include "c_img.h"
#include <stdio.h>
#include <math.h>

void create_img(struct rgb_img **im, size_t height, size_t width){
    *im = (struct rgb_img *)malloc(sizeof(struct rgb_img));
    (*im)->height = height;
    (*im)->width = width;
    (*im)->raster = (uint8_t *)malloc(3 * height * width);
}


int read_2bytes(FILE *fp){
    uint8_t bytes[2];
    fread(bytes, sizeof(uint8_t), 1, fp);
    fread(bytes+1, sizeof(uint8_t), 1, fp);
    return (  ((int)bytes[0]) << 8)  + (int)bytes[1];
}

void write_2bytes(FILE *fp, int num){
    uint8_t bytes[2];
    bytes[0] = (uint8_t)((num & 0XFFFF) >> 8);
    bytes[1] = (uint8_t)(num & 0XFF);
    fwrite(bytes, 1, 1, fp);
    fwrite(bytes+1, 1, 1, fp);
}

void read_in_img(struct rgb_img **im, char *filename){
    FILE *fp = fopen(filename, "rb");
    size_t height = read_2bytes(fp);
    size_t width = read_2bytes(fp);
    create_img(im, height, width);
    fread((*im)->raster, 1, 3*width*height, fp);
    fclose(fp);
}

void write_img(struct rgb_img *im, char *filename){
    FILE *fp = fopen(filename, "wb");
    write_2bytes(fp, im->height);
    write_2bytes(fp, im->width);
    fwrite(im->raster, 1, im->height * im->width * 3, fp);
    fclose(fp);
}

uint8_t get_pixel(struct rgb_img *im, int y, int x, int col){
    return im->raster[3 * (y*(im->width) + x) + col];
}

void set_pixel(struct rgb_img *im, int y, int x, int r, int g, int b){
    im->raster[3 * (y*(im->width) + x) + 0] = r;
    im->raster[3 * (y*(im->width) + x) + 1] = g;
    im->raster[3 * (y*(im->width) + x) + 2] = b;
}

void destroy_image(struct rgb_img *im)
{
    free(im->raster);
    free(im);
}


void print_grad(struct rgb_img *grad){
    int height = grad->height;
    int width = grad->width;
    for(int i = 0; i < height; i++){
        for(int j = 0; j < width; j++){
            printf("%d\t", get_pixel(grad, i, j, 0));
        }
    printf("\n");    
    }
}

// create helper function to get the gradiant for each colour
uint8_t dual_grad(int y, int x, struct rgb_img *im, size_t height, size_t width){
    uint8_t pix_left, pix_right, pix_up, pix_down;
    int sum_x = 0;
    int sum_y = 0;
    int x_coord = x,  y_coord = y;
    for(int col = 0; col <3; col++){
        if(x == 0){
            pix_left = get_pixel(im, y, width-1, col);
            pix_right = get_pixel(im, y, x+1, col);
            pix_up = get_pixel(im, y+1, x, col);
            pix_down = get_pixel(im, y-1, x, col);
        }
        if(x == width-1){
            pix_left = get_pixel(im, y, x-1, col);
            pix_right = get_pixel(im, y, 0, col);
            pix_up = get_pixel(im, y+1, x, col);
            pix_down = get_pixel(im, y-1, x, col);
        }
        if(y == 0){
            pix_left = get_pixel(im, y, x-1, col);
            pix_right = get_pixel(im, y, x+1, col);
            pix_up = get_pixel(im, y+1, x, col);
            pix_down = get_pixel(im, height-1, x, col);
        }
        if(y== height-1){
            pix_left = get_pixel(im, y, x-1, col);
            pix_right = get_pixel(im, y, x+1, col);
            pix_up = get_pixel(im, 0, x, col);
            pix_down = get_pixel(im, y-1, x, col);
        }
        if(x> 0 && x<width && y >0 && y <height){
            pix_left = get_pixel(im, y, x-1, col);
            pix_right = get_pixel(im, y, x+1, col);
            pix_up = get_pixel(im, y+1, x, col);
            pix_down = get_pixel(im, y-1, x, col);
        }
        int x_diff = pix_right - pix_left;
        int y_diff = pix_up - pix_down;
        sum_x += pow(x_diff, 2);
        sum_y += pow(y_diff, 2);
    }
    double sum = sum_x + sum_y;
    printf("y: %d, x: %d\n", y, x);
    printf("%lf\n", sum);
    double square_root = (uint8_t)sqrt(sum);
    uint8_t grad = (uint8_t)(square_root/10);
    return grad;
}


void calc_energy(struct rgb_img *im, struct rgb_img **grad){
    uint8_t energy;
    size_t height = im->height;
    size_t width = im->width;
    create_img(grad, height, width);
    for(int y=0; y< im->height; y++){
        for(int x=0; x< (im)->width;x++){
            energy = dual_grad(y, x, im, height, width);
            set_pixel(*grad, y, x, energy, energy, energy);
        }
    }
}


