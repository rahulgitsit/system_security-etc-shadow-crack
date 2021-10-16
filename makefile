 
CC=gcc  #compiler
TARGET=guessword #target file name
 
all:
	$(CC) guessword.c -lcrypt -o $(TARGET)
 
clean:
	rm $(TARGET)

runall:
	./${TARGET}  passwordfile.txt  top250.txt 

