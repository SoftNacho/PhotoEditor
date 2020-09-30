#include "imageData.h"
#include "functions.h"

const double gmonomult[3]={.299,.587,.114};

int main(int argc,char *argv[])
{
  //Handles user input, to get file names and command
  if(argc<4 || argc>6)
  {
    printf("Incorrect number of arguments\n");
    printf("Number of arguments: %d\n",argc);
    exit(1);
  }

  const char *input_filename=argv[1];
  printf("Inputfile: %s\n",input_filename);
  const char *output_filename=argv[2];
  printf("Outputfile: %s\n",output_filename);
  char garbage[2];
  int command;
  int radius=3;

  if(1!=sscanf(argv[3],"%d%1s",&command,garbage) || command<0 || command>10)
  {
    printf("Incorrect command\n");
    exit(1);
  }

  if(((command==0) && argc==5 && 1!=sscanf(argv[4],"%d%1s",&radius,garbage)) || radius<1)
  {
    printf("Incorrect radius value\n");
    exit(1);
  }
  
  //Create filters and images
  Filter *filters=initialize_filters(radius);
  Image *input_image=decode(input_filename);
  printf("Width: %d, height: %d\n",input_image->width,input_image->height);
  Image *output_image=generate_output(input_image);

  uint8_t *in_red=input_image->red_channel;
  uint8_t *in_blue=input_image->blue_channel;
  uint8_t *in_green=input_image->green_channel;
  uint8_t *in_alpha=input_image->alpha_channel;
  uint8_t *out_red=output_image->red_channel;
  uint8_t *out_blue=output_image->blue_channel;
  uint8_t *out_green=output_image->green_channel;
  uint8_t *out_alpha=output_image->alpha_channel;
  int height=input_image->height;
  int width=input_image->width;


  //Run chosen command to call functions from functions.c
  switch(command)
  {
    case(0):
    {
      convolve_image(in_red,in_green,in_blue,in_alpha,out_red,out_green,out_blue,
                    out_alpha,filters[0].filter,filters[0].radius,width,height);
      encode(output_filename,output_image);
      break;
    }
    case(1):
    {
      convolve_image(in_red,in_green,in_blue,in_alpha,out_red,out_green,out_blue,
                    out_alpha,filters[1].filter,filters[1].radius,width,height);
      encode(output_filename,output_image);
      break;
    }
    case(2):
    {
      convolve_image(in_red,in_green,in_blue,in_alpha,out_red,out_green,out_blue,
                    out_alpha,filters[2].filter,filters[2].radius,width,height);
      encode(output_filename,output_image);
      break;
    }
    case(3):
    {
      convolve_image(in_red,in_green,in_blue,in_alpha,out_red,out_green,out_blue,
                    out_alpha,filters[3].filter,filters[3].radius,width,height);
      encode(output_filename,output_image);
      break;
    }
    case(4):
    {
      convolve_image(in_red,in_green,in_blue,in_alpha,out_red,out_green,out_blue,
                    out_alpha,filters[4].filter,filters[4].radius,width,height);
      encode(output_filename,output_image);
      break;
    }
    case(5):
    {
      convolve_image(in_red,in_green,in_blue,in_alpha,out_red,out_green,out_blue,
                    out_alpha,filters[5].filter,filters[5].radius,width,height);
      encode(output_filename,output_image);
      break;
    }
    case(6):
    {
      convert_to_gray(in_red,in_green,in_blue,in_alpha,out_red,out_green,out_blue,
                    out_alpha,gmonomult,width,height);
      encode(output_filename,output_image);
      break;
     }
    case(7):
    {
      flip_vertical(in_red,in_green,in_blue,in_alpha,out_red,out_green,out_blue,
                  out_alpha,width,height);
      encode(output_filename,output_image);
      break;
    }
    case(8):
    {
      color_threshold(in_red,in_green,in_blue,in_alpha,out_red,out_green,out_blue,
                  out_alpha,width,height,10,150,10);
      encode(output_filename,output_image);
      break;
    }
    default:
      exit(1);
  }

 
  free((double*)filters[0].filter);
  free(filters);
  free_image(input_image);
  free_image(output_image);
  return 0;
}