#ifndef MANDELBROT_H
#define MANDELBROT_H

#include <complex>
#include <vector>




/**
 Write Header of a Bitmap Image
 @param image is a reference to the file containing our image
 @param height is number of pixels tall the image is
 @param width is the number of pixels wide the image is
*/
void writeHeader(std::ofstream &image, int height, int width);
/**
 Write DIB header of a Bitmap Image. The DIB is metadata for the image.
 @param image is a reference to the file we are writing into
 @param height is number of pixels tall the image is
 @param width is the number of pixels wide the image is
 @param dpi is the dots per inch of the image when printed
*/
void writeDIB(std::ofstream &image, int height, int width, int dpi);
/**
 Write Pixels from a collection of arrays. All three arrays must be the same size.
 
 @param image is a reference to the file we are writting into
 @param red is an array storing the red component of each pixel
 @param green is an array storing the green component of each pixel
 @param blue is an array storing the blue component of each pixel
 @param pixels is the number of elements in the array
*/
void writePixels(std::ofstream &image,
	unsigned char* red,
	unsigned char* green,
	unsigned char* blue,
	int pixels);

/**
 Take an integer (32 bit) and break it up into bytes (8bit).
 @param value is the integer to break up
 @param byte is which byte you want (0 to 4)
 @return the byte requested as a char
 */
char getByte(int value, int byte);
/**
 Convert Dots Per Inch to Pixels Per Meter. Both are useful formats, but dpi is more commom. Bitmaps use PPM.
 @param dpi is the dots per inch
 @return equivelant measurement in pixels per meter
 */int dpiToPPM(int dpi);





#endif // MANDELBROT_H