#include "imageData.h"
#include "functions.h"

/*
 * AUTHOR:       Lisa Gentil
 * NETID:        gentil2
 * LECTURE:      BL2
 * DISCUSSION:   BD4
 * MP/LAB:       MP6: For this machine problem, we had to implement several functions.
 * 			The first one is calculate_cosine_filter which calculates the 
 * 			cosine filter values needed to use the filters. The second one
 * 			is the convolve_image function (the most important and challenging)
 * 			what this function does is that it blurs a picture more or less 
 * 			depending on the value of the radius input by the user. The third 
 * 			function, convert_to_gray, takes a colored picture as input and 
 * 			outputs the black and white (grayscale) version of it. The 
 * 			flip_vertical function, the fourth one, turns the input picture
 * 			upside down, switching each row i with the row (height-1)-i, so
 * 			the first row is switched with the last one, etc. Finally the 
 * 			fifth function, color_threshold, takes the input picture and 
 * 			outputs the pixels whose RGB components exceeded a set threshold,
 * 			in other words it outputs the pixels whose color is brighter than
 * 			the threshold and sets the other pixels to black.  
 * TERM:         Spring 2016
 * PARTNERS:     N/A
 */



/*
 * calculate_cosine_filter - calculates the cosine filter values
 * INTPUTS:  int radius- integer indicating the size of array. 
 *           The filter has 2*radius+1 rows and 2*radius+1 columns
 * OUTPUTS:  cosine_fitler - pointer to the 1-D array of size 
 *           (2*radius+1)*(2*radius+1). Appropriate space has already been
 *           allocated. This function must compute
 *           the appropriate values of this array as described in the wiki
 * RETURN VALUE: none
 */
void calculate_cosine_filter(double *cos_filter,int radius)
{
	//declaring variables
	double weight = 0;
	int neg_r = -radius;
	int pos_r = radius;
	int x, y, z;
	
	//initializing variables
	z = 0;

	for (y = neg_r; y <= pos_r; y++)
	{
		for (x = neg_r; x <= pos_r; x++)
		{
			cos_filter[z] = cos((M_PI * x) / (2 * radius)) + cos((M_PI * y) / (2 * radius));
			z++;
		}//end for x
	}//end for y
		
	int z_last = z;

	//calculate weight of the array
	for (z = 0; z <= z_last; z++ )
	{
		weight = weight + cos_filter[z];
	}//end for weight 

	double weight_tot = weight;
	
	//normalizing the filter
	for (z = 0; z <= z_last; z++)
	{
		cos_filter[z] = cos_filter[z] / weight_tot;
	}//end for normalize	
}//end calculate_cosine_filter

/* convolve_image - performs a convolution between a filter and image
 * INPUTS: in_red - pointer to the input red channel (1-D array)
 *         in_green - pointer to the input green channel (1-D array)
 *         in_blue - pointer to the input blue channel (1-D array) 
 *         in_alpha - pointer to the input alpha channel (1-D array)
 *         filter - pointer to the convolution filter (1-D array)
 *         radius - radius of the convolution filter
 *         width - width of the input image
 *         height - height of the input image
 * OUTPUTS: out_red - pointer to the output red channel (1-D array)
 *          out_green - pointer to the output green channel (1-D array)
 *          out_blue - pointer to the output blue channel (1-D array)
 *          out_alpha - pointer to the output alpha channel (1-D array)
 * RETURN VALUES: none
 * NOTE: Input image values are already loaded into the input arrays
 *       Don't alter the values of the input image.
 *       Filter is already initialized and contains desired values.
 *       Appropriate space has been allocated for output arrays. 
 */
void convolve_image(uint8_t *in_red,uint8_t *in_green,uint8_t *in_blue,
                   uint8_t *in_alpha, uint8_t *out_red,uint8_t *out_green,
                   uint8_t *out_blue,uint8_t *out_alpha,const double *filter,
                   int radius,int width,int height)
{
	//declaring variables
	int x, y, i, j, index;
	int neg_r = -radius;
	int pos_r = radius;
	int ii, jj, filter_index, image_index;

	for(x = 0; x < width; x++)
	{
		for (y = 0; y < height; y++)
		{
			//out_alpha[index] = in_alpha[index];
			index = y * width + x;

			if(radius < 1)
			{
				out_red[index] = in_red[index];
				out_green[index] = in_green[index];
				out_blue[index] = in_blue[index];
			}//end if
		
			else
			{
				//declaring and initializing variables 
				double red_convolution = 0;
				double green_convolution = 0;
				double blue_convolution = 0;

				for(i = neg_r; i <= pos_r; i++)
				{
					for(j = neg_r; j <= pos_r; j++)
					{
						int current_x = x + i;
						int current_y = y + j;

						if((current_x >= 0) && (current_x < width) && (current_y >= 0) && (current_y < height))
						{
							//initializing variables
				                	ii = i + radius;
                                        		jj = j + radius;
                                        		filter_index = jj * (2 * radius + 1) + ii;
                                        		image_index = current_y * width + current_x;

							//computing temporary output RGB values
							red_convolution = red_convolution + (filter[filter_index] * in_red[image_index]);
							green_convolution = green_convolution + (filter[filter_index] * in_green[image_index]);
							blue_convolution = blue_convolution + (filter[filter_index] * in_blue[image_index]); 	
						}//end if current x y values
                                                     
					}//end for j
				}//end for i
				
				//assigning output RGB values according to conditions:
				//if value < 0, value set to 0,
				//if value > 255, value set to 255,
				//if 0 > value > 255, value set to value.
                                if (red_convolution < 0)
                                	out_red[index] = 0;
                              	else if (red_convolution > 255)
                                	out_red[index] = 255;
				else
					out_red[index] = red_convolution;

                              	if (green_convolution < 0)
                                  	out_green[index] = 0;
                             	else if (green_convolution > 255)
                                	out_green[index] = 255;
				else
					out_green[index] = green_convolution;

                                if (blue_convolution < 0)
                                     	out_blue[index] = 0;
            			else if (blue_convolution > 255)
                                    	out_blue[index] = 255;
				else
					out_blue[index] = blue_convolution;
			}//end else

                	out_alpha[index] = in_alpha[index];
		}//end for y
	}//end outer for x
}//end convolve_image

/* convert_to_gray - convert the input image to grayscale
 * INPUTS: in_red - pointer to the input red channel (1-D array)
 *         in_green - pointer to the input green channel (1-D array)
 *         in_blue - pointer to the input blue channel (1-D array)
 *         in_alpha - pointer to the input alpha channel (1-D array)
 *         gmonomult - pointer to array with constants to be multipiled with 
 *                     RBG channels to convert image to grayscale (3 element array)
 *         width - width of the input image
 *         height - height of the input image
 * OUTPUTS: out_red - pointer to the output red channel (1-D array)
 *          out_green - pointer to the output green channel (1-D array)
 *          out_blue - pointer to the output blue channel (1-D array)
 *          out_alpha - pointer to the output alpha channel (1-D array)
 * RETURN VALUES: none
 * NOTE: Input image values are already loaded into the input arrays
 *       gmonomult contains the desired grayscale weights.
 *       Appropriate space has been allocated for output arrays. 
 *       Don't alter the values of the input image.
 */
void convert_to_gray(uint8_t *in_red,uint8_t *in_green,uint8_t *in_blue,
                   uint8_t *in_alpha,uint8_t *out_red,uint8_t *out_green,
                   uint8_t *out_blue,uint8_t *out_alpha,
                   const double *gmonomult,int width,int height)
{
	//declaring and initializing variables
    	double grayscale_num = 0;
	int index;
	int array_length = width * height;
	double red, green, blue;

	//computing grayscale numbers for each pixel
	for (index = 0; index < array_length; index++)
	{
		//computing value of RGB input channels to corresponding gmonomult weights
		red = in_red[index] * gmonomult[0];
		green = in_green[index] * gmonomult[1];
		blue = in_blue[index] * gmonomult[2];
		
		//computing grayscale number
		grayscale_num = red + green + blue;
		
		//assigning value to RGB output channels
		out_red[index] = grayscale_num;
		out_green[index] = grayscale_num;
		out_blue[index] = grayscale_num;

		//passing on value from alpha input to alpha output
		out_alpha[index] = in_alpha[index];		
	}//end for loop
}//end convert_to_gray

/* flip_vertical- flips image vertically such that the bottom row of pixels
 *                in the input image is the top row of pixels in the output 
 *                image, the second most bottom row in the input image is the 
 *                second row from the top of the input image, and so on. This should
 *                flip the image upside-down.
 * INPUTS: in_red - pointer to the input red channel (1-D array)
 *         in_green - pointer to the input green channel (1-D array)
 *         in_blue - pointer to the input blue channel (1-D array)
 *         in_alpha - pointer to the input alpha channel (1-D array)
 *         width - width of the input image
 *         height - height of the input image
 * OUTPUTS: out_red - pointer to the output red channel (1-D array)
 *          out_green - pointer to the output green channel (1-D array)
 *          out_blue - pointer to the output blue channel (1-D array)
 *          out_alpha - pointer to the output alpha channel (1-D array)
 * RETURN VALUES: none
 * NOTE: Input image values are already loaded into the input arrays
 *       Appropriate space has been allocated for output arrays. 
 *       Don't alter the values of the input image.
 */
void flip_vertical(uint8_t *in_red,uint8_t *in_green,uint8_t *in_blue,
                 uint8_t *in_alpha,uint8_t *out_red,uint8_t *out_green,
                 uint8_t *out_blue,uint8_t *out_alpha,int width,int height)
{
	//declaring and initializing variables
	int x, y;
	//x corresponds to the column number
	//y corresponds to the row number

	for (y = 0; y < height; y++)
	{
		for(x = 0; x < width; x++)
		{
			//computing RGB output values column by column and row by row
			out_red[y * width + x] = in_red[(height - 1 - y) * width +x];
                        out_green[y * width + x] = in_green[(height - 1 - y) * width +x];
                        out_blue[y * width + x] = in_blue[(height - 1 - y) * width +x];
                        out_alpha[y * width + x] = in_alpha[(height - 1 - y) * width +x];
		}//end for x
	}//end for y
}//end flip_vertical

/* color_threshold - for each pixel of input image, compares value to red, green, and blue threshold.
                     If the values exceed all three thresholds, set the output image to the input image values. 
                     Otherwise, set them to zero. Alpha should remain unchanged.
 * INPUTS: in_red - pointer to the input red channel (1-D array)
 *         in_green - pointer to the input green channel (1-D array)
 *         in_blue - pointer to the input blue channel (1-D array)
 *         in_alpha - pointer to the input alpha channel (1-D array)
 *         width - width of the input image
 *         height - height of the input image
 * OUTPUTS: out_red - pointer to the output red channel (1-D array)
 *          out_green - pointer to the output green channel (1-D array)
 *          out_blue - pointer to the output blue channel (1-D array)
 *          out_alpha - pointer to the output alpha channel (1-D array)
 * RETURN VALUES: none
 * NOTE: Input image values are already loaded into the input arrays
 *       Appropriate space has been allocated for output arrays. 
 *       Don't alter the values of the input image.
 */
void color_threshold(uint8_t *in_red,uint8_t *in_green,uint8_t *in_blue,
                 uint8_t *in_alpha,uint8_t *out_red,uint8_t *out_green,
                 uint8_t *out_blue,uint8_t *out_alpha,int width,int height,
                 int red_threshold,int blue_threshold,int green_threshold)
{
	//declaring and initializing values
	int index;
	int array_length = width * height;
	
     	for(index = 0; index < array_length; index++)
	{
		if((in_red[index] > red_threshold) && (in_green[index] > green_threshold) && (in_blue[index] > blue_threshold))
		{
			//setting output values equals to corresponding input values
			//if and only if all three values exceed the threshold 
			out_red[index] = in_red[index];
			out_green[index] = in_green[index];
			out_blue[index] = in_blue[index];
			out_alpha[index] = in_alpha[index];
		}//end if

		else
		{
			//setting pixels to black if not all input values exceed threshold
			out_red[index] = 0;
			out_green[index] = 0;
			out_blue[index] = 0;
			out_alpha[index] = 255;
		}//end else
	}//end for loop
}//end color_threshold