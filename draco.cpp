// DRAwing COmpression
// model based image compression for the drawings generated using scribble
// @version v:0.1 - Jan 6, 2018
// @author parth_shel

//include header files:
#include<string.h>
#include<stdlib.h>
#include<stdio.h>

#include"draco.h" //comression library

char inputFile [32]; //input file name
char outputFile [32]; //output file name

//main
int main(int argc, char ** argv) {
	//usage: draco -<compress flag> <.PIC file>
	if(argc < 3) {
		printf("usage: draco -<compress_flag> <.PIC file>\n");
		exit(-1);
	}
	else {
		if(strcmp(argv[1], "-c")) { //compress
			strcpy(inputFile, argv[2]);
			FILE * check = fopen(inputFile, "r");
			if(check == NULL) { //file does not exist
				printf("the file %s does not exist!\n", inputFile);
				exit(-1);
			}
			fclose(check);
			strcpy(outputFile, inputFile);
			strcat(outputFile, ".draco");
			compress(inputFile, outputFile);
		}
		else if(strcmp(argv[1], "-d")) { //de-compress
			strcpy(inputFile, argv[2]);
			FILE * check = fopen(inputFile, "r");
			if(check == NULL)  { //file does not exist
				printf("the file %s does not exist!\n", inputFile);
				exit(-1);
			}
			fclose(check);
			decompress(inputFile, outputFile);
		}
		else {
		printf("incorrect usage of flag! -c to compress and -d to decompress\n");
		exit(-1);
		}
	}
	return 0;
} 
