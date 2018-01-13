// 		DRACO	
// DRAwing COmpression library for model based image compression 
// of drawings generated using Scribble.
// @version v:0.1 - Jan 6, 2018
// @author parth_shel

#include<algorithm>
#include<vector>
#include<map>
#include<set>

#define IMAGE_WIDTH 640 //COLUMNS
#define IMAGE_HEIGHT 480 //ROWS
#define DEFAULT_BKCOLOR 0 //BGI BLACK

int bitmap [IMAGE_WIDTH] [IMAGE_HEIGHT] = {DEFAULT_BKCOLOR};
bool visited [IMAGE_WIDTH] [IMAGE_HEIGHT] = {false};

class Pixel {
	private:
	int x;
	int y;
	
	public:
	//parameterized constructor:
	Pixel(int x, int y) {
		this->x = x;
		this->y = y;
	}
	
	//getters:
	int getX() {
		return x;
	}
	int getY() {
		return y;
	}

	//operator== overloading
	bool operator==(Pixel& that)const {
		return(this->x == that.x && this->y == that.y);
	}
	
	//operator< overloading
	bool operator<(Pixel& that) const {
		if(this->x == that.x)
			return (this->y < that.y);
		else 
			return(this->x < that.x);
	}
};

std::vector<Pixel> border;
int blockColor;

void readUnCompressedFile(FILE * fp) {
	int x, y, color;
	while(fscanf(fp, "%d,%d,%d\n", &x, &y, &color) != EOF) {
		//printf("(%d,%d) = %d\n", x, y, color);
		bitmap [x] [y] = color;
	}
}

void writeCompressedFile(FILE * fp) {
	std::sort(border.begin(), border.end());
	border.erase(std::unique(border.begin(), border.end()), border.end());
	std::vector <Pixel> :: iterator itr;
	
	int num_of_pixels_on_outline = border.size();
	fprintf(fp, "%d\n%d\n", blockColor, num_of_pixels_on_outline);
	//printf("Color of block: %d\nNum. of pixels on outline: %d\n",
	//	blockColor, num_of_pixels_on_outline);

	for(itr = border.begin(); itr != border.end(); ++itr) {
		fprintf(fp, "%d,%d\n", itr->getX(), itr->getY()); 
		//printf("(%d,%d)\n", itr->getX(), itr->getY());
	}
}

int colorOf(Pixel p) {
	int x = p.getX();
	int y = p.getY();
	return bitmap[x][y];
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
	
	//basis case:
	if(colorOf(thisPixel) == blockColor) {
		visited[x][y] = true; //memoize
	}
	else {
		return;
	}

	if(liesOnBorder(thisPixel) && colorOf(thisPixel) == blockColor) {
		border.push_back(thisPixel);
		//return;
	}
	
	Pixel left = Pixel(x-1, y);
	Pixel right = Pixel(x+1, y);
	Pixel up = Pixel(x, y-1);
	Pixel down  = Pixel(x, y+1);

	//recursive step:
	if(!isVisitedPixel(left) && colorOf(left) == blockColor)
		carveOutline(left);
	
	if(!isVisitedPixel(right) && colorOf(right) == blockColor)
		carveOutline(right);
	
	if(!isVisitedPixel(up) && colorOf(up) == blockColor)
		carveOutline(up);
	
	if(!isVisitedPixel(down) && colorOf(down) == blockColor);
		carveOutline(down);
}

void compress(char * inputFile, char * outputFile) {
	FILE * in = fopen(inputFile, "r");
	if(in == NULL)
		return;
	readUnCompressedFile(in);
	fclose(in);
	FILE * redundant = fopen(outputFile, "w");
	fclose(redundant);
	
	for(int i = 0;i < IMAGE_WIDTH;i++) {
		for(int j = 0;j < IMAGE_HEIGHT;j++) {
			if(!visited[i][j] && bitmap[i][j] != DEFAULT_BKCOLOR) {
				blockColor = bitmap[i][j];
				carveOutline(Pixel(i, j));
				FILE * out = fopen(outputFile, "a");
				writeCompressedFile(out);
				fclose(out);
				border.clear();
			}
		}
	}

	return;
}

void sweepCompress(char * inputFile, char * outputFile) {
	//read un-compressed file:
	FILE * in = fopen(inputFile, "r");
	if(in == NULL)
		return;
	readUnCompressedFile(in);
	fclose(in);

	//map each color to a vector of pixels that are outlines for the blocks
	std::map<int, std::vector<Pixel>> mapOfBorders;

	//line sweeping alg.
	for(int i = 0;i < IMAGE_WIDTH;i++) {
		for(int j = 0;j < IMAGE_HEIGHT;j++) {
			Pixel thisPixel = Pixel(i, j);
			int thisColor = bitmap[i][j];
			if(liesOnBorder(thisPixel) && thisColor != DEFAULT_BKCOLOR) {
				mapOfBorders[thisColor].push_back(thisPixel);
			}
		}
	}

	//write compressed file:
	FILE * out = fopen(outputFile, "w");
	for(std::map<int, std::vector<Pixel>>::iterator mapItr = mapOfBorders.begin();
		mapItr != mapOfBorders.end(); ++mapItr) {
		
		//blockColor & size:	
		fprintf(out, "%d\n%d\n", mapItr->first, mapItr->second.size());
		
		for(std::vector<Pixel>::iterator vectorItr = mapItr->second.begin();
			vectorItr != mapItr->second.end(); ++vectorItr) {
			//pixels:
			fprintf(out, "%d,%d\n", vectorItr->getX(), vectorItr->getY());
		}
	}
	fclose(out);

	mapOfBorders.clear();

	return;
}

std::set<int> allColors;

void readCompressedFile(FILE * fp) {
	int blockColor, blockSize, x, y;
	while(1) {
		fscanf(fp, "%d\n%d\n", &blockColor, &blockSize);
		for(int i = 0;i < blockSize;i++) {
			fscanf(fp, "%d,%d\n", &x, &y);
			bitmap[x][y] = blockColor;
		}

		allColors.insert(blockColor);
		
		if(fgetc(fp) == EOF)
			break;
		else
			fseek(fp, -1, SEEK_CUR);
	}
}

void writeUnCompressedFile(FILE * fp) {
	for(int i = 0;i < IMAGE_WIDTH;i++) {
		for(int j = 0;j < IMAGE_HEIGHT;j++) {
			int thisColor = bitmap[i][j];
			if(thisColor != DEFAULT_BKCOLOR) {
				fprintf(fp, "%d,%d,%d\n", i, j, thisColor);
			}
		}
	}
}

void unMask(std::vector<Pixel> mask, int color) {
	for(std::vector<Pixel>::iterator itr = mask.begin();
		itr != mask.end(); ++itr) {
		int x = itr->getX();
		int y = itr->getY();
		bitmap[x][y] = color;
	}
}

bool liesInsideShape(Pixel p, int color) {
	int interactionsToTheLeft = 0;
	int interactionsToTheRight = 0;
	int y = p.getY();
	for(int i = 0; i < p.getX(); i++) {
		if(bitmap[i][y] == color)
			interactionsToTheLeft++;
	}
	for(int i = p.getX() + 1; i < IMAGE_WIDTH; i++) {
		if(bitmap[i][y] == color)
			interactionsToTheRight++;
	}
	if((interactionsToTheLeft %2 != 0) && (interactionsToTheRight %2 != 0)) {
		return true;
	}
	else {
		return false;
	}
}

void sweepFill() {
	for(std::set<int>::iterator itr = allColors.begin(); 
		itr != allColors.end(); ++itr) {
	
		std::vector<Pixel> mask;
		int blockColor = *itr;
		for(int i = 0; i < IMAGE_WIDTH; i++) {
			for(int j = 0; j < IMAGE_WIDTH; j++) {
				Pixel thisPixel = Pixel(i, j);
				if(liesInsideShape(thisPixel, blockColor)) {
					mask.push_back(thisPixel);
				}
			}
		}
		unMask(mask, blockColor);
		mask.clear();
	}
}

void decompress(char * inputFile, char * outputFile) {
	FILE * in = fopen(inputFile, "r");
	if(in == NULL)
		return;
	
	readCompressedFile(in);
	fclose(in);

	FILE * out = fopen(outputFile, "w");
	writeUnCompressedFile(out);
	fclose(out);

	return;
}
