// DRACO Visualizer
// tool to display compressed and un-compressed images
// @version: v:0.1 - Jan 8, 2018
// @author parth_shel

//include header files:
#include<graphics.h>

char fileName[32];

void displayCompressedFile(FILE * fp);
void displayUncompressedFile(FILE * fp);

int main(int argc, char ** argv) {
	//usage: draco-visualizer -<mode flag> <file name>
	if(argc < 3) {
		printf("usage: draco-visualizer -<mode flag> <file name>\n");
		exit(-1);
	}
	strcpy(fileName, argv[2]);
	if(strcmp(argv[1], "-c") == 0) { //compressed file
		FILE * fp = fopen(fileName, "r");
		if(fp == NULL) {
			printf("file error!\n");
			exit(-1);
		}
		initwindow(640, 480);
		displayCompressedFile(fp);
	}
	else if(strcmp(argv[1], "-u") == 0) { //un-compressed file
		FILE * fp = fopen(fileName, "r");
		if(fp == NULL) {
			printf("file error!\n");
			exit(-1);
		}
		initwindow(640, 480);
		displayUncompressedFile(fp);
	}
	else {
		printf("incorrect usage of flag! -c for compressed file and -u for un-compressed.\n");
		exit(-1);
	}
	closegraph();
	return 0;
}

void displayCompressedFile(FILE * fp) {
	cleardevice();
	
	int blockColor, blockSize, x, y;
	while(1){
		fscanf(fp, "%d\n%d\n", &blockColor, &blockSize);
		setcolor(blockColor);
		for(int i = 0;i < blockSize;i++) {
			fscanf(fp, "%d,%d\n", &x, &y);
			_putpixel(x, y);
		}

		/*if(fgetc(fp) == EOF)
			break;
		else
			fseek(fp, -1, SEEK_CUR);*/
	}
	fclose(fp);

	while(1) {}

	return;
}

void displayUncompressedFile(FILE * fp) {
	cleardevice();

	int x, y, color;
	while(1/*fscanf(fp, "%d,%d,%d\n", &x, &y, &color) != EOF*/) {
		fscanf(fp, "%d,%d,%d\n", &x, &y, &color);
		setcolor(color);
		_putpixel(x, y);
	}
	fclose(fp);

	while(1) {}

	return;
}
