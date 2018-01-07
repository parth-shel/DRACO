// 		DRACO	
// DRAwing COmpression library for model based image compression 
// of drawings generated using Scribble.
// @version v:0.1 - Jan 6, 2018
// @author parth_shel

//#include<stdlib>
//#include<stdio>
#include<vector>

#define IMAGE_WIDTH 10 //COLUMNS
#define IMAGE_HEIGHT 10 //ROWS
#define DEFAULT_BKCOLOR 0 //BGI BLACK

int bitmap [IMAGE_WIDTH] [IMAGE_HEIGHT] = {DEFAULT_BKCOLOR};
bool visited [IMAGE_WIDTH] [IMAGE_HEIGHT] = {false};

class Pixel {
	private:
	int x;
	int y;
	
	public:
	Pixel(int x, int y) {
		this->x = x;
		this->y = y;
	}

	int getX() {
		return x;
	}
	int getY() {
		return y;
	}
};

std::vector<Pixel> border;
int blockColor;

void readUnCompressedFile(FILE * fp) {
	int x, y, color;
	while(fscanf(fp, "%d,%d,%d\n", &x, &y, &color) != EOF) {
		bitmap [x] [y] = color;
	}
}

void writeCompressedFile(FILE * fp) {
	int num_of_pixels_on_outline = border.size();
	fprintf(fp, "%d\n%d\n", num_of_pixels_on_outline, blockColor);
	
	std::vector <Pixel> :: iterator itr;
	for(itr = border.begin(); itr != border.end(); ++itr) {
		fprintf(fp, "%d,%d\n", *itr.getX(), *itr.getY());
	}
}

bool isValidPixel(Pixel p) {
	int x = p.getX();
	int y = p.getY();
	if(x >= 0 && x < IMAGE_WIDTH && y >= 0 && y < IMAGE_HEIGHT) {
		return true;
	}
	else {
		return false;
	}
}

bool isVisitedPixel(Pixel p) {
	if(!isValidPixel(p)) {
		return false;
	}
	else {
		int x = p.getX();
		int y = p.getY();
		return visited[x][y];
	}
}

bool liesOnEdge(Pixel p) {
	int x = p.getX();
	int y = p.getY();

	return (x == 0 || x == IMAGE_WIDTH-1 || y == 0 || y == IMAGE_HEIGHT-1);
}

bool liesOnBorder(Pixel p) {
	if(liesOnEdge(p))
		return true;
	
	int x = p.getX();
	int y = p.getY();
	
	int thisColor = bitmap[x][y];
	int leftColor = bitmap[x-1][y];
	int rightColor = bitmap[x+1][y];
	int upColor = bitmap[x][y-1];
	int downColor = bitmap[x][y+1];

	if(leftColor != thisColor || rightColor != thisColor || 
	   upColor != thisColor || downColor != thisColor) {
	   	return true;
	}
	else {
		return false;
	}
}

void carveOutline(Pixel thisPixel) {
	if(!isValidPixel(thisPixel))
		return;
	int x = thisPixel.getX();
	int y = thisPixel.getY();
	visited[x][y] = true; //memoize
	if(liesOnBorder(thisPixel)) {
		border.push_back(thisPixel);
		//return;
	}
	
	//resursive step:
	Pixel left = Pixel(x-1, y);
	Pixel right = Pixel(x+1, y);
	Pixel up = Pixel(x, y-1);
	Pixel down  = Pixel(x, y+1);

	if(!isVisitedPixel(left))
		carveOutline(left);
	
	if(!isVisitedPixel(right))
		carveOutline(right);
	
	if(!isVisitedPixel(up))
		carveOutline(up);
	
	if(!isVisitedPixel(down));
		carveOutline(down);
}

void compress(char * inputFile, char * outputFile) {
	FILE * in = fopen(inputFile, "r");
	if(in == NULL)
		return;
	readUnCompressedFile(in);
	fclose(in);
	FILE * redundant = fopen(outputFile, "w+");
	fclose(redundant);
	
	for(int i = 0;i < IMAGE_WIDTH;i++) {
		for(int j = 0;j < IMAGE_HEIGHT;j++) {
			if(!visited[i][j]) {
				blockColor = bitmap[i][j];
				carveOutline(Pixel(i, j));
				FILE * out = fopen(outputFile, "w");
				writeCompressedFile(out);
				fclose(out);
				border.clear();
			}
		}
	}

	return;
}

void readCompressedFile(FILE * fp) {

}

void writeUnCompressedFile(FILE * fp) {

}

void markOutline() {

}

void floodFill(int x, int y) {

}

void parse(FILE * in, char * out) {

}

void decompress(char * inputFile, char * outputFile) {
	FILE * in = fopen(inputFile, "r");
	if(in == NULL)
		return;
	FILE * redundant = fopen(outputFile, "w+");
	fclose(redundant);

	parse(in, outputFile);

	fclose(in);
	return;
}
