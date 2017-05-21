struct GLCM{
	int size;
	float *glcm;
	float contrast;
	float correlation;
	float energy;
	float homogeneity;
};

struct ImageData getQuantizedImage(struct ImageData * imgData, struct ImageData * quantizedImage, unsigned char range);
void getGLCM(struct ImageData * imgData, struct GLCM * glcm);
void normalizeGLCM(struct GLCM * glcm);
void evaluateContrast(struct GLCM * glcm);
void evaluateCorrelation(struct GLCM * glcm);
void evaluateEnergy(struct GLCM * glcm);
void evaluateHomogeneity(struct GLCM * glcm);
void evaluateGLCM(struct ImageData * imgData, struct GLCM * glcm, unsigned char range);
