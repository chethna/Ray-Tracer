/*
 *  ImageReader.h
 *
 *
 *  Created by Chethna Kabeerdoss on 24/02/14.
 *  Copyright 2014 __MyCompanyName__. All rights reserved.
 *
 */

#include <cstdlib>
#include <iostream>
#include <GLUT/glut.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <fstream>

using namespace std;

struct PixMapValues {
    unsigned char *pix;
    int width;
    int height;
    
};

class ImageReader {
    
    int width, height, maxcolor;
    unsigned char *pixmap;
    
    void readCommentChar(FILE *readFile){
        //cout << "in comment fun"<<endl;
        char fileChar = fgetc(readFile); 		//cout <<fileChar<<endl;
        while (fileChar == '#') {
            fileChar = fgetc(readFile);		//cout <<fileChar;
            while (fileChar != 0x0A) {
                fileChar = fgetc(readFile);		//cout <<fileChar<<endl;
            }
            fileChar = fgetc(readFile);		//cout <<fileChar<<endl;
        }
        ungetc(fileChar, readFile);
    }
    
public: PixMapValues readPPM(FILE *readFile){
    char magicNumber[200];
    int i = 0;
    fscanf(readFile, "%s", magicNumber);
    /*while (i<200) {
     cout << magicNumber[i];
     i++;
     }*/
    
    if((magicNumber[0]=='P')&&(magicNumber[1]=='6')){
        //cout<<endl<<"P6"<<endl;
        fgetc(readFile);
        //cout << "before while";
        readCommentChar(readFile);
        //cout << "after while";
        fscanf(readFile, "%d %d", &width, &height);
        //cout << "width:"<<width<<endl;
        //cout << "height:"<<height<<endl;
        fgetc(readFile);
        readCommentChar(readFile);
        
        pixmap = new unsigned char[width * height * 3];
        
        fscanf(readFile, "%d", &maxcolor);
        //cout <<"maxcolor:"<< maxcolor<<endl;
        
        fgetc(readFile);
        readCommentChar(readFile);
        
        int y, x, pixel;
        unsigned char red, green, blue;
        
        for(y = height - 1; y > 0; y--) {
            for(x = 0; x < width; x++) {
                red = fgetc(readFile);
                green = fgetc(readFile);
                blue = fgetc(readFile);
                
                pixel = (y * width + x) * 3;
                
                pixmap[pixel] = red;
                pixel++;
                pixmap[pixel] = green;
                pixel++;
                pixmap[pixel] = blue;
            }
        }
        PixMapValues result = {pixmap, width, height};
        return result;
    }
    
    else {
        cout << "Error: Cannot read file";
        exit(0);
    }
}
    
public: void writePPM(FILE *writeFile, unsigned char *pixValue, int width, int height) {
    
    int y, x, pixel;
    unsigned char red, green, blue;
    
    fprintf(writeFile, "P6\n");
    fprintf(writeFile, "%d %d\n", width, height);
    fprintf(writeFile, "%d\n", 255);
    
    //cout<<"InsidePPM";
    for(y = height-1; y >= 0; y--) {
        for(x = 0; x < width; x++) {
            pixel = (y * width + x) * 3; 
            red = pixValue[pixel]; 
            pixel++;
            green = pixValue[pixel];
            pixel++;
            blue = pixValue[pixel];
            fputc(red, writeFile);
            fputc(green, writeFile);
            fputc(blue, writeFile);
        }
    }
    fclose(writeFile);
}
    
};