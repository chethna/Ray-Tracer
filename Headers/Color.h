/*
 *  Color.h
 *  
 *
 *  Created by Chethna Kabeerdoss on 15/02/14.
 *  Copyright 2014 __MyCompanyName__. All rights reserved.
 *
 */
#ifndef _COLOR_H_
#define _COLOR_H_

#include "Vector.h"

class Color{
	
	int red, green, blue, special;
public:
	Color();
	
	Color(double, double, double);
	
	//method functions
	int getColorRed(){return red;}
	int getColorGreen(){ return green;}
	int getColorBlue(){ return blue;}
	//double getColorSpecial(){ return special;}
	
	void setColorRed(int redValue){red = redValue;}
	void setColorGreen(int greenValue){green = greenValue;}
	void setColorBlue(int blueValue){blue = blueValue;}
	//void setColorSpecial(double specialValue){blue = specialValue;}
	
	double brightness(){
		return (red+green+blue)/3;
	}
	
	Color colorScale(double scale) const{
		return Color((int)this->red*scale,(int)this->green*scale,(int)this->blue*scale);
	}
	
	Color colorAdd(Color color){
		return Color(red+color.getColorRed(),green+color.getColorGreen(),blue+color.getColorBlue());
	}
	
	Color colorMultiply(Color color){
		return Color(red*color.getColorRed(),green*color.getColorGreen(),blue*color.getColorBlue());
	}
	
	Color colorAverage(Color color){
		return Color((this->red+color.getColorRed())/2.0,(this->green+color.getColorGreen())/2.0,(this->blue+color.getColorBlue())/2.0);
	}
	Color colorSubtract(Color color){
		return Color((red-color.getColorRed()),(green-color.getColorGreen()),(blue-color.getColorBlue()));
	}
};
	Color::Color(){
		red = .5;
		blue = .5;
		green = .5;
	}
	
	Color::Color(double r, double g, double b){
		red = r;
		green = g;
		blue = b;
	}
	/*
	Color::Color(Vector3d o, Vector3d d){
		origin = o;
		direction = d;
	}*/
#endif