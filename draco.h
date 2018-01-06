// 		DRACO	
// DRAwing COmpression library for model based image compression 
// of drawings generated using Scribble.
// @version v:0.1 - Jan 6, 2018
// @author parth_shel

#include<stdlib>
#include<stdio>
#include<vector>

#define IMAGE_WIDTH 10 //COLUMNS
#define IMAGE_HEIGHT 10 //ROWS
#define DEFAULT_BKCOLR 0 //BGI BLACK

int bitmap [IMAGE_WIDTH] [IMAGE_HEIGHT] = {DEFAULT_BKCOLOR};
bool visited [IMAGE_WIDTH] [IMAGE_HEIGHT] = {false};

struct Pixel {
	int color;
	int x;
	int y;
};

std::vector<Pixel> border;

void compress(char * inputFile, char * outputFile) {
	return;
}

void decompress(char * inputFile, char * outputFile) {
	return;
}
