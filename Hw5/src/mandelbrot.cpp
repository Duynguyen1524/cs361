/**
@mainpage CS 361 - Homework 5: Mandelbrot Set Image Generator
@section Description
The program generates an image of the Mandelbrot set using multithreading with a thread pool and tasks.
*/
#include <thread>
#include <vector>
#include <complex>
#include <iostream>
#include <fstream>
#include "mandelbrot.h"
#include "threadpool.h"
#include "task.h"
/**
@file
@author Justin Nguyen <jn867@drexel.edu>
@date November 8th, 2025
@section DESCRIPTION
This file contains a program that generates an image of the Mandelbrot set.
It uses a thread pool to parallelize the computation of pixel colors.
*/
/**
    Main function run the Mandelbrot set image generator with a thread manager and thread workers.
    @param argc Argument count
    @param argv Argument vector
    @return 0 for sucess and failure
*/
int main(int argc, char* argv[]) {
	if (argc != 6) {
		std::cerr << "Usage: " << argv[0] << " < the smallest x to display> < the largest x to display> < smallest y to display> < largest y to display> <filename>\n";
		return 1;
	}
	long double x_1 = std::stold(argv[1]);
	long double x_2 = std::stold(argv[2]);
	long double y_1 = std::stold(argv[3]);
	long double y_2 = std::stold(argv[4]);
	const char* filename = argv[5];
	std::ofstream image(filename,std::ios::binary);
	int dpi = 150;//pixel per inch
	const int width = 1500;
	int height = width * (y_2 - y_1) / (x_2 - x_1);

	writeHeader(image, height, width);
	writeDIB(image, height, width, dpi);
	int pixels = width * height;
	unsigned char* red = new unsigned char[pixels];
	unsigned char* green = new unsigned char[pixels];
	unsigned char* blue = new unsigned char[pixels];
	
	auto manager = std::make_shared<drawManagerTask>(red, green, blue, height, x_1, x_2, y_1, y_2);
	ThreadPool pool(manager);
	pool.run();
	writePixels(image, red, green, blue, pixels);
	image.close();
	delete[] red;
	delete[] green;
	delete[] blue;
	std::cout <<"Made Image " << filename << std::endl;
	return 0;


}




/**
    Helper function to get a specific byte from an integer.
    @param value The integer value to extract a byte from.
    @param byte The index of the byte to extract (0 = least significant).
    @return The extracted byte.
*/
char getByte(int value, int byte)
{
	int newValue = value;
	
	unsigned char rem;
	for(int i=0; i <= byte; i++)
	{
		rem = static_cast<unsigned char>( newValue%256 );
		newValue = newValue/256;
	}
	return rem;
}

/**
	Convert Dots Per Inch (DPI) to Pixels Per Meter (PPM).
	@param dpi The dots per inch value.
	@return The equivalent pixels per meter value.
*/
int dpiToPPM(int dpi)
{
	float inchesPerMeter = 39.3701/1;
	float convert = dpi*inchesPerMeter;
	return static_cast<int>(convert);
}

void writeHeader(std::ofstream &image, int height, int width)
{
	//How many pixel does the image have
	int pixels = height*width;
	//Make the header. It is always 14 bytes
	int headerSize = 14;
	//Array to store the header
	char* header = new char[headerSize];
	//The header is 14 Bytes
	//The DIB is 40 bytes
	int offset = headerSize + 40;
	//Each Pixel is another 3 bytes
	int totalBits = pixels*3+offset;
	//Make the Header
	//First 2 Bytes are BM for bitmap
	header[0] = 'B';
	header[1] = 'M';
	//Next 4 bytes are the total size of the file
	header[2] = getByte(totalBits,0);
	header[3] = getByte(totalBits,1);
	header[4] = getByte(totalBits,2);
	header[5] = getByte(totalBits,3);
	//Next for bits are 0 (reserved for other uses)
	header[6] = 0;
	header[7] = 0;
	header[8] = 0;
	header[9] = 0;
	//Last 4 bytes are offset
	//Where do the pixels start
	header[10] = getByte(offset,0);
	header[11] = getByte(offset,1);
	header[12] = getByte(offset,2);
	header[13] = getByte(offset,3);
	//Write the Header to the file in binary
	image.write(header, headerSize);
	//Exit the Function
	return;
}
/**
    Write DIB header of a Bitmap Image. The DIB is metadata for the image.
	@param image is a reference to the file we are writing into
    @param height is number of pixels tall the image is
	@param width is the number of pixels wide the image is
	@param dpi is the dots per inch of the image when printed

*/
//Using the BITMAPINFOHEADER standard
void writeDIB(std::ofstream &image, int height, int width, int dpi)
{
	//Convert DPI to Pixels Per Meter
	int resolution = dpiToPPM(dpi);
	//Fixed Size of 40 Bytes
	int sizeDIB = 40;
	//Make array of bytes
	char* DIB = new char[sizeDIB];
	//Set Values
	//First 4 bytes are header size of this header (40)
	DIB[0] = getByte(40,0);
	DIB[1] = getByte(40,1);
	DIB[2] = getByte(40,2);
	DIB[3] = getByte(40,3);
	//Bitmap Width (4 bytes)
	DIB[4] = getByte(width,0);
	DIB[5] = getByte(width,1);
	DIB[6] = getByte(width,2);
	DIB[7] = getByte(width,3);
	//Height (4 bytes)
	DIB[8] = getByte(height,0);
	DIB[9] = getByte(height,1);
	DIB[10] = getByte(height,2);
	DIB[11] = getByte(height,3);
	//Color Plane (2 bytes) is always 1
	DIB[12] = 1;
	DIB[13] = 0;
	//Color Depth (2 bytes) we are using 24 (three 8 bit colors)
	DIB[14] = getByte(24,0);
	DIB[15] = getByte(24,1);
	//Compression (4 bytes) 0 means none
	DIB[16] = 0;
	DIB[17] = 0;
	DIB[18] = 0;
	DIB[19] = 0;
	//Uncompressed Size (4 bytes)
	//0 because we aren't using compression
	DIB[20] = 0;
	DIB[21] = 0;
	DIB[22] = 0;
	DIB[23] = 0;
	//Horizontal Resolution (4 bytes)
	//Pixel per meter
	DIB[24] = getByte(resolution, 0);
	DIB[25] = getByte(resolution, 1);
	DIB[26] = getByte(resolution, 2);
	DIB[27] = getByte(resolution, 3);
	//Vertical Resolution (4 bytes)
	//Pixel per meter
	DIB[28] = getByte(resolution, 0);
	DIB[29] = getByte(resolution, 1);
	DIB[30] = getByte(resolution, 2);
	DIB[31] = getByte(resolution, 3);
	//Color Pallet (4 bytes)
	//0 means all
	DIB[32] = 0;
	DIB[33] = 0;
	DIB[34] = 0;
	DIB[35] = 0;
	//Number of important colors
	//0 mean all equal
	DIB[36] = 0;
	DIB[37] = 0;
	DIB[38] = 0;
	DIB[39] = 0;
	//Write the Header to the file in binary
	image.write(DIB, sizeDIB);
	//Exit the Function
	return;
}
/**
    Write Pixels from a collection of arrays. All three arrays must be the same size.
	@param image is a reference to the file we are writting into
	@param red is an array storing the red component of each pixel
	@param green is an array storing the green component of each pixel
	@param blue is an array storing the blue component of each pixel
	@param pixels is the number of elements in the array
    @return void
*/
void writePixels(std::ofstream &image,
	unsigned char* red,
	unsigned char* green,
	unsigned char* blue,
	int pixels)
{
	char* pixel = new char[3];
	for(int i=0; i < pixels; i++)
	{
		pixel[2] = red[i];
		pixel[1] = green[i];
		pixel[0] = blue[i];
		image.write(pixel, 3);
	}
	return;
}


