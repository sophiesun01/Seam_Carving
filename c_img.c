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
    //creating the general case of what left, right, up, and down should be
    int left = x-1;
    int right = x+1;
    int up = y-1;
    int down = y+1;

    int sum_x = 0;
    int sum_y = 0;

    for(int col = 0; col <3; col++){
        //changing the unique edge cases so it wraps around
        if(x == 0){
            left = width - 1;
        }
        if(x == width-1){
            right = 0;
        }
        if(y == 0){
            up = height-1;
            if(x == 2 && y == 0){
                printf("%d\n", up);
            }
        }
        if(y == height-1){
            down = 0;
        }
        //getting the pixels around the pixel at (y, x)
        pix_left = get_pixel(im, y, left, col);
        pix_right = get_pixel(im, y, right, col);
        pix_up = get_pixel(im, up, x, col);
        pix_down = get_pixel(im, down, x, col);

        //calculating the differential to get the gradient
        int x_diff = pix_right - pix_left;
        int y_diff = pix_up - pix_down;
        sum_x += pow(x_diff, 2);
        sum_y += pow(y_diff, 2);
    }
    //taking the x, y sums and performing operations to turn it into the gradient 
    double sum = sum_x + sum_y;

    //printf("y: %d, x: %d\n", y, x);
    //printf("%lf\n", sum);
    double square_root = (double)sqrt(sum);
    uint8_t grad = (uint8_t)(square_root/10);
    //printf("%d\n", grad);
    return grad;
}


void calc_energy(struct rgb_img *im, struct rgb_img **grad){
    uint8_t energy;

    //getting the height and width of the image
    size_t height = im->height;
    size_t width = im->width;

    //creating an image to store the gradient energy value
    create_img(grad, height, width);

    //inserting the height and width of the grad struct 
    (*grad)->height = height;
    (*grad)->width = width;

    for(int y=0; y< im->height; y++){
        for(int x=0; x< (im)->width; x++){
            energy = dual_grad(y, x, im, height, width);
            set_pixel(*grad, y, x, energy, energy, energy);
        }
    }
}

double min2(double emid, double eside){
    if(emid < eside){
        return emid;
    }
    else{
        return eside;
    }
}

double min3(double emid, double eleft, double eright){
    if(emid <= eright && emid <= eleft){
        return emid;
    }
    else if(eright < eleft){
        return eright;
    }
    else{
        return eleft;
    }
}

void dynamic_seam(struct rgb_img *grad, double **best_arr)
{
    //i = y coordinate, j = x coordinate
    int i = 0, j = 0;
    double emid, eright, eleft, e_min;
    uint8_t *grad_arr = grad->raster;
    size_t height = grad->height;
    size_t width = grad->width;

    //initializing the array
    *best_arr = (double *)malloc(sizeof(double)* (height) * (width));
    while(j< width){
        (*best_arr)[j] = grad_arr[3*j];
        j++;
    }
    for(i = 1; i < height; i++){
        for(j = 0; j < width; j++){
            emid = (*best_arr)[(i-1)*width+j];
            if(j == 0){
                eright = (*best_arr)[(i-1)*width+(j+1)];
                e_min = min2(emid, eright) + grad_arr[3 * (i*width +j)];
            }
            else if(j == width-1){
                eleft = (*best_arr)[(i-1)*width+(j-1)];
                e_min = min2(emid, eleft) + grad_arr[3 * (i*width +j)];
            }
            //not at the first or last index of the row
            else{
                eleft = (*best_arr)[(i-1)*width+(j-1)];
                eright = (*best_arr)[(i-1)*width+(j+1)];
                e_min = min3(emid, eleft, eright) + grad_arr[3 * (i*width +j)];
            }
            (*best_arr)[i*width +j] = e_min;

        }

    }
}

int int_min2(double *best, int cur, int next){
    if(best[cur] < best[next]){
        return cur;
    }
    else{
        return next;
    }
}

int int_min3(double *best, int mid, int left, int right)
{
    if(best[mid] <= best[right] && best[mid] <= best[left]){
        return mid;
    }
    else if(best[right] < best[left]){
        return right;
    }
    else{
        return left;
    }
}

void recover_path(double *best, int height, int width, int **path){
    *path = (int *)malloc(sizeof(int)*height);
    int i = height-1;
    int j = 0;
    int cur, next, temp_min;
    int row_min = width * height - 1; //last index of the array
    int left, right, mid;

    //find the min in the last row
    while(j < width-1){
        cur = i*width + j;
        next = cur + 1;
        //stores the min from the last row in last_ind and in the last entry of **path
        temp_min = int_min2(best, cur, next);
        row_min = int_min2(best, temp_min, row_min);
        j++;
    }

    //this insures you are giving the index of the row and not of the whole array
    (*path)[i] = row_min % width;

    for(i = height-2; i>=0; i--){

        if(row_min == 0){
            mid = row_min-width;
            right = mid+1;
            row_min = int_min2(best, mid, right);

        }
        else if(row_min == width-1){
            mid = row_min-width;
            left = mid-1;
            row_min = int_min2(best, mid, left);

        }
        else{
            mid = row_min-width;
            left = mid-1;
            right = mid+1;
            row_min = int_min3(best, mid, left, right);


        }

        //same kerfuffle here as above
        (*path)[i] = row_min%width;
    }
}

void remove_seam(struct rgb_img *src, struct rgb_img **dest, int *path){
    
}