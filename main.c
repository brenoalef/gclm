#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "include/imageReader.h"
#include "include/glcm.h"
#include "include/main.h"

int main(int argc, char * argv[]){
	int i;
	clock_t start, end;
	double t = 0, at = 0;

	if(argc != 2){
		printf("Usage: ./evaluateGLCM.e file\n");
		exit(1);
	}

	//Reading image
	struct ImageData imgData;
	readPGM(argv[1], &imgData);

	start = clock();

	//Evaluating GLCM and some of its attributes
	struct GLCM glcm;
	evaluateGLCM(&imgData, &glcm, RANGE);
	free(glcm.glcm);
	free(imgData.matrix);

	end = clock();
	t = ((double) (end - start))/CLOCKS_PER_SEC;

	//Writing results
	writeResults(&glcm, t);
	printf("Results saved to glcmResults.txt\n");

}

void writeResults(struct GLCM * glcm, double t){
	FILE * fp = NULL;
	fp = fopen("glcmResults.txt", "w+");

	if(fp == NULL){
		printf("Error creating results file\n");
		exit(1);
	}

	fprintf(fp, "Contrast: %.4f\n", glcm->contrast);
	fprintf(fp, "Correlation: %.4f\n", glcm->correlation);
	fprintf(fp, "Energy: %.4f\n", glcm->energy);
	fprintf(fp, "Homogeneity: %.4f\n", glcm->homogeneity);
	fprintf(fp, "Executions time: %f\n", t);

	fclose(fp);
}
