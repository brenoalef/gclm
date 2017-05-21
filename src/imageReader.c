#include <stdio.h>
#include <stdlib.h>
#include "../include/imageReader.h"

void readPGM(const char * fileName, struct ImageData * imgData){
	FILE * fp = NULL;
	int i, j;
	char version[3];

	fp = fopen(fileName, "rb");
	if(fp == NULL){
		printf("Error opening file\n");
		exit(1);
	}

	printf("Reading image file: %s...\n", fileName);

 	fgets(version, sizeof(version), fp);
	if(strcmp(version, "P5")){
		printf("Invalid pgm file\n");
		exit(1);
	}

	fscanf(fp, "%d", &imgData->col);
	fscanf(fp, "%d", &imgData->row);
	fscanf(fp, "%d", &imgData->maxGrayValue);
	//reading the '\n'
	fgetc(fp);

	imgData->matrix = (int *) malloc(imgData->row * imgData->col * sizeof(int *));
	if(!imgData->matrix){
		printf("Out of memory\n");
		exit(1);
	}

	//Reading image matrix
	for (i = 0; i < imgData->row * imgData->col; i++){
		*(imgData->matrix + i) = fgetc(fp);
	}

	fclose(fp);
}
