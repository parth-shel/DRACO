// DRAwing COmpression
// model based image compression for the drawings generated using scribble
// @version v:0.1 - Jan 6, 2018
// @author parth_shel

//include header files:
#include<string.h>
#include<stdlib.h>
#include<stdio.h>

#include"draco.h" //comression library

char * inputFile; //input file name
char * outputFile; //output file name

//main
int main(int argc, char ** argv) {
	//usage: draco -<compress flag> <.PIC file>
	if(argc < 3) {
		printf("usage: draco -<compress_flag> <.PIC file>\n");
		exit(-1);
	}
	else {
	inputFile = (char *) malloc(32 * sizeof(char));
	outputFile = (char *) malloc(32 * sizeof(char));
		if(strcmp(argv[1], "-c") == 0) { //compress
			strcpy(inputFile, argv[2]);
			FILE * check = fopen(inputFile, "r");
			if(check == NULL) { //file does not exist
				printf("the file %s does not exist!\n", inputFile);
				exit(-1);
			}
			fclose(check);
			strcpy(outputFile, argv[2]);
			strcat(outputFile, ".draco");
			printf("here;%s\n%s\n", inputFile, outputFile);
			compress(inputFile, outputFile);
		}
		else if(strcmp(argv[1], "-d") == 0) { //de-compress
			strcpy(inputFile, argv[2]);
			FILE * check = fopen(inputFile, "r");
			if(check == NULL)  { //file does not exist
				printf("the file %s does not exist!\n", inputFile);
				exit(-1);
			}
			fclose(check);
			strcpy(outputFile, argv[2]);
			strcat(outputFile, ".draco");
			decompress(inputFile, outputFile);
		}
		else {
		printf("incorrect usage of flag! -c to compress and -d to decompress\n");
		exit(-1);
		}
	}
	return 0;
} 
