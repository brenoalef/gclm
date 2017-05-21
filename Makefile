all: evaluateGLCM.e clean
evaluateGLCM.e: main.o
	gcc main.o glcm.o imageReader.o -o evaluateGLCM.e -lm
main.o: include/main.h imageReader.o glcm.o
	gcc -c main.c
glcm.o: include/glcm.h imageReader.o
	gcc -c src/glcm.c -lm
imageReader.o: include/imageReader.h
	gcc -c src/imageReader.c
clean:
	rm -f *.o
