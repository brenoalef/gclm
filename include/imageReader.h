struct ImageData{
	int row;
	int col;
	int maxGrayValue;
	int *matrix;
};

void readPGM(const char * fileName, struct ImageData * imgData);
