#include "lodepng.h"
#include "imageData.h"
#include "functions.h"

//defining all the filters
const double median_blur[25]=
{
  0,0,1/13.0,0,0,
  0,1/13.0,1/13.0,1/13.0,0,
  1/13.0,1/13.0,1/13.0,1/13.0,1/13.0,
  0,1/13.0,1/13.0,1/13.0,0,
  0,0,1/13.0,0,0
};

const double motion_blur[81]=
{
  1/9.0,0,0,0,0,0,0,0,0,
  0,1/9.0,0,0,0,0,0,0,0,
  0,0,1/9.0,0,0,0,0,0,0,
  0,0,0,1/9.0,0,0,0,0,0,
  0,0,0,0,1/9.0,0,0,0,0,
  0,0,0,0,0,1/9.0,0,0,0,
  0,0,0,0,0,0,1/9.0,0,0,
  0,0,0,0,0,0,0,1/9.0,0,
  0,0,0,0,0,0,0,0,1/9.0
};

const double sharpen_less[25]=
{
  -1/8.0,-1/8.0,-1/8.0,-1/8.0,-1/8.0,
  -1/8.0,2/8.0,2/8.0,2/8.0,-1/8.0,
  -1/8.0,2/8.0,1,2/8.0,-1/8.0,
  -1/8.0,2/8.0,2/8.0,2/8.0,-1/8.0,
  -1/8.0,-1/8.0,-1/8.0,-1/8.0,-1/8.0,
};

const double sharpen_more[9]=
{
  -1,-1,-1,
  -1,9,-1,
  -1,-1,-1
};

const double edge_detect[9]=
{
  -1,-1,-1,
  -1,8,-1,
  -1,-1,-1
};

const double emboss[9]=
{
  -2,-1,0,
  -1,1,1,
  0,1,2,
};

//Function print_pixel prints the value of a specified pixel for you
//Can be used to print to the command line for debugging purposes
//Specified row and col must be valid (between 0 and height-1, 0 and width-1). 
void print_pixel(uint8_t *in_red,uint8_t *in_green,uint8_t *in_blue,uint8_t *in_alpha,int width,int height, int col, int row){
	if(row<0 || row>(height-1) || col<0 || col>(width-1)){
		printf("Invalid Pixel Col:%d Row:%d\n",col,row);
	}else{
		int index = row*width+col;	
		printf("Pixel Values of Col:%d Row:%d\n",col,row);
		printf("Red:%d Blue:%d Green:%d Alpha:%d\n",in_red[index],in_blue[index],in_green[index],in_alpha[index]);
	}	
}

//function for encoding a PNG
void encode(const char *filename,Image *encode_image)
{
  int width=encode_image->width;
  int height=encode_image->height;
  unsigned char *image=(unsigned char*)malloc(sizeof(unsigned char)*4*height*width);
  
  int row,col;
  int image_width=4*width;
  uint8_t *red_channel=encode_image->red_channel;
  uint8_t *green_channel=encode_image->green_channel;
  uint8_t *blue_channel=encode_image->blue_channel;
  uint8_t *alpha_channel=encode_image->alpha_channel;

  for(row=0;row<height;row++)
    for(col=0;col<width;col++)
    {
      image[image_width*row+4*col+0]=red_channel[row*width+col];
      image[image_width*row+4*col+1]=green_channel[row*width+col];
      image[image_width*row+4*col+2]=blue_channel[row*width+col];
      image[image_width*row+4*col+3]=alpha_channel[row*width+col];
    }

  unsigned int error=lodepng_encode32_file(filename,image,width,height);
  if(error)
    printf("error %u:%s\n",error,lodepng_error_text(error));

  free(image);
}

//function for decoding a PNG
Image *decode(const char *filename)
{
  Image *input_image=(Image*)malloc(sizeof(Image));
  unsigned int error;
  unsigned char *image;

  //FIX: fix this part here
  error=lodepng_decode32_file(&image,(unsigned int*)&(input_image->width),(unsigned int*)&(input_image->height),filename);
  if(error)
    printf("error %u: %s\n",error,lodepng_error_text(error));

  int width=input_image->width;
  int height=input_image->height;
  input_image->red_channel=(uint8_t*)malloc(sizeof(uint8_t)*width*height);
  input_image->green_channel=(uint8_t*)malloc(sizeof(uint8_t)*width*height);
  input_image->blue_channel=(uint8_t*)malloc(sizeof(uint8_t)*width*height);
  input_image->alpha_channel=(uint8_t*)malloc(sizeof(uint8_t)*width*height);

  int row,col;
  int image_width=width*4;
  uint8_t *red_channel=input_image->red_channel;
  uint8_t *green_channel=input_image->green_channel;
  uint8_t *blue_channel=input_image->blue_channel;
  uint8_t *alpha_channel=input_image->alpha_channel;

  for(row=0;row<height;row++)
  {
    for(col=0;col<width;col++)
    {
      red_channel[row*width+col]=image[image_width*row+4*col+0];
      green_channel[row*width+col]=image[image_width*row+4*col+1];
      blue_channel[row*width+col]=image[image_width*row+4*col+2];
      alpha_channel[row*width+col]=image[image_width*row+4*col+3];
    }
  }

  free(image);
  return input_image;
}

Image *generate_output(Image *input_image)
{
  Image *output_image=(Image*)malloc(sizeof(Image));
  int width=input_image->width;
  int height=input_image->height;
  output_image->width=width;
  output_image->height=height;
  output_image->red_channel=(uint8_t*)malloc(sizeof(uint8_t)*width*height);
  output_image->green_channel=(uint8_t*)malloc(sizeof(uint8_t)*width*height);
  output_image->blue_channel=(uint8_t*)malloc(sizeof(uint8_t)*width*height);
  output_image->alpha_channel=(uint8_t*)malloc(sizeof(uint8_t)*width*height);
  return output_image;
}

Filter *initialize_filters(int radius)
{
  int filter_size=(2*radius+1)*(2*radius+1);
  double *cos_blur=(double*)malloc(sizeof(double)*filter_size);
  calculate_cosine_filter(cos_blur,radius);

  Filter *filters=(Filter*)malloc(sizeof(Filter)*NUM_FILTERS);

  filters[0].radius=radius;
  filters[0].filter=(const double*)cos_blur;

  filters[1].radius=2;
  filters[1].filter=median_blur;

  filters[2].radius=4;
  filters[2].filter=motion_blur;

  filters[3].radius=2;
  filters[3].filter=sharpen_less;

  filters[4].radius=1;
  filters[4].filter=sharpen_more;

  filters[5].radius=1;
  filters[5].filter=edge_detect;

  filters[6].radius=1;
  filters[6].filter=emboss;

  return filters;
}
void free_image(Image *input_image)
{
  free(input_image->red_channel);
  free(input_image->green_channel);
  free(input_image->blue_channel);
  free(input_image->alpha_channel);
  free(input_image);
}