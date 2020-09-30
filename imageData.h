#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "lodepng.h"

#define NUM_FILTERS 8

typedef struct
{
  int radius;
  const double *filter;
} Filter;

typedef struct
{
  int width;
  int height;
  uint8_t *red_channel;
  uint8_t *blue_channel;
  uint8_t *green_channel;
  uint8_t *alpha_channel;
} Image;

void print_pixel(uint8_t *in_red,uint8_t *in_green,uint8_t *in_Blue,uint8_t *in_alpha,int width,int height, int row, int col);
void encode(const char *filename,Image *encode_image);
Image *decode(const char *filename);
Filter *initialize_filters(int radius);
Image *generate_output(Image *input_image);
void free_image(Image *input_image);