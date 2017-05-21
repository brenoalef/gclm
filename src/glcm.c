#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "../include/imageReader.h"
#include "../include/glcm.h"

struct ImageData getQuantizedImage(struct ImageData * imgData, struct ImageData * quantizedImage, unsigned char range){
	int i = 0;

	quantizedImage->row = imgData->row;
	quantizedImage->col = imgData->col;
	quantizedImage->maxGrayValue = imgData->maxGrayValue / range;
	quantizedImage->matrix = (int *) malloc(quantizedImage->row * quantizedImage->col * sizeof(int *));

	if(!imgData->matrix){
		printf("Out of memory\n");
		exit(1);
	}

	for (i = 0; i < (quantizedImage->row * quantizedImage->col); i++){
		*(quantizedImage->matrix + i) = *(imgData->matrix + i) / range;
	}

	return *(quantizedImage);
}

void getGLCM(struct ImageData * imgData, struct GLCM * glcm){
	int i;

	for(i = 0; i < imgData->row * imgData->col; i++) {
		if((i + 1)%imgData->col){
			*(glcm->glcm + *(imgData->matrix + i)*glcm->size + *(imgData->matrix + i + 1)) += 1;
		}
	}
}

void normalizeGLCM(struct GLCM * glcm){
	float sum = 0;
	int i;

	for(i = 0; i< glcm->size * glcm->size; i++){
		sum += *(glcm->glcm + i);
	}

	for(i = 0; i< glcm->size * glcm->size; i++){
		*(glcm->glcm + i) = *(glcm->glcm + i) / sum;
	}
}

void evaluateContrast(struct GLCM * glcm){
	float contrast = 0;
	int i;

	for(i = 0; i < (glcm->size * glcm->size); i++){
		//p(i,j)*(i-j)^2
		contrast += *(glcm->glcm + i)*(((i/glcm->size) - (i% glcm->size))*((i/glcm->size) - (i% glcm->size)));
	}

	glcm->contrast = contrast;
}

void evaluateCorrelation(struct GLCM * glcm){
	int i;
	float mi = 0, mj = 0, sigi = 0, sigj = 0, correlation = 0;

	for(i = 0; i < (glcm->size * glcm->size); i++){
		//p(i,j)*i
		mi += *(glcm->glcm + i) * (i/glcm->size);
		//p(i,j)*j
		mj += *(glcm->glcm + i) * (i% glcm->size);
	}

	for(i = 0; i < (glcm->size * glcm->size); i++){
		//p(i,j)*(i-mi)^2
		sigi += *(glcm->glcm + i) * (((i/glcm->size) - mi)*((i/glcm->size) - mi));
		//p(i,j)*(j-mj)^2
		sigj += *(glcm->glcm + i) * (((i% glcm->size) - mj)*((i% glcm->size) - mj));
	}

	sigi = sqrt(sigi);
	sigj = sqrt(sigj);

	//(p(i,j)*(i-mi)*(j-mj))/(sigi*sigj)
	for(i = 0; i < (glcm->size * glcm->size); i++){
		correlation += (*(glcm->glcm + i) * ((i/glcm->size) - mi) * ((i% glcm->size) - mj));
	}

	correlation /= (sigi * sigj);
	glcm->correlation = correlation;
}

void evaluateEnergy(struct GLCM * glcm){
	float energy = 0;
	int i;

	for(i = 0; i< glcm->size * glcm->size; i++){
		//p(i,j)^2
		energy += *(glcm->glcm + i) * *(glcm->glcm + i);
	}

	glcm->energy = energy;
}

void evaluateHomogeneity(struct GLCM * glcm){
	float homogeneity = 0;
	int i;

	for(i = 0; i < (glcm->size * glcm->size); i++){
		//p(i,j)/(1 + abs(i - j))
		homogeneity += *(glcm->glcm + i)/(1 + abs((i/glcm->size) - (i% glcm->size)));
	}

	glcm->homogeneity = homogeneity;
}

void evaluateGLCM(struct ImageData * imgData, struct GLCM * glcm, unsigned char range){
	struct ImageData quantizedImage;
	quantizedImage = getQuantizedImage(imgData, &quantizedImage, range);

	glcm->size = range;

	glcm->glcm = calloc(glcm->size * glcm->size , sizeof(float));
	if(!glcm->glcm){
		printf("Out of memory\n");
		exit(1);
	}

	getGLCM(&quantizedImage, glcm);
	free(quantizedImage.matrix);

	normalizeGLCM(glcm);

	evaluateContrast(glcm);
	evaluateCorrelation(glcm);
	evaluateEnergy(glcm);
	evaluateHomogeneity(glcm);
}
