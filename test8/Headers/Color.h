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
	
	double red, green, blue, special;
public:
	Color();
	
	Color(double, double, double);
	
	//method functions
	double getColorRed(){return red;}
	double getColorGreen(){ return green;}
	double getColorBlue(){ return blue;}
	//double getColorSpecial(){ return special;}
	
	void setColorRed(double redValue){red = redValue;}
	void setColorGreen(double greenValue){green = greenValue;}
	void setColorBlue(double blueValue){blue = blueValue;}
	//void setColorSpecial(double specialValue){blue = specialValue;}
	
	double brightness(){
		return (red+green+blue)/3;
	}
	
	Color colorScale(double scale){
		return Color(red*scale,green*scale,blue*scale);
	}
	
	Color colorAdd(Color color){
		return Color(red+color.getColorRed(),green+color.getColorGreen(),blue+color.getColorBlue());
	}
	
	Color colorMultiply(Color color){
		return Color(red*color.getColorRed(),green*color.getColorGreen(),blue*color.getColorBlue());
	}
	
	Color colorAverage(Color color){
		return Color((red+color.getColorRed())/2,(green+color.getColorGreen())/2,(blue+color.getColorBlue())/2);
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