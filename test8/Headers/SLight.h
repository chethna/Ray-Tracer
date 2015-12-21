/*
 *  Light.h
 *  
 *
 *  Created by Chethna Kabeerdoss on 15/02/14.
 *  Copyright 2014 __MyCompanyName__. All rights reserved.
 *
 */
#ifndef _SLIGHT_H_
#define _SLIGHT_H_

#include "Source.h"
#include "Vector.h"
#include "Color.h"

//#define PI 3.14159265

class SLight : public Source{
	
	Vector3d position;
	Vector3d direction;
	Color color;
	float angle; //spot light angle
	
public:
	
	SLight();
	
	SLight(Vector3d, Vector3d, float, Color);
	
	
	virtual Vector3d getLightPosition(){return position;}
	virtual Color getColor(){ return color;}
	
	virtual double getCosTheta(Vector3d nh, Vector3d nlh){
		
		//find the dist pl-ph.norm()
		//dist*direction - sFactor
		//hard code cut off
		
		//if(sfactor>cutoff)
		//calculate coneFactor
		//multiple coneFactor w/color
		//if(sfactor<cutoff
		//make it black
		float cutOff = 0.9;
		double angleFormed = direction*(-1*nlh);
		double cosTheta = (1.0 - (1.0 - angleFormed) * 1.0/(1.0 - cutOff));//(nh*nlh);
		float  deg = acos(angleFormed)*(180.0 / PI);
		//printf("%f \n",angleFormed);
		if (angleFormed<cutOff) {
			cosTheta = -1;

		}
		return (cosTheta);
	}
	
};
	SLight::SLight(){
		position.set(0,0,0);
		direction.set(0,0,0);
		angle = 30 ;
		color = Color(1,1,1);
	}
	
	SLight::SLight(Vector3d posValue, Vector3d dirValue, float angleValue, Color colorValue){
		position = posValue;
		direction = dirValue;
		angle = angleValue;
		color = colorValue;
	}	
	
#endif