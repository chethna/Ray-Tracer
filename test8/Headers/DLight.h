/*
 *  Light.h
 *  
 *
 *  Created by Chethna Kabeerdoss on 15/02/14.
 *  Copyright 2014 __MyCompanyName__. All rights reserved.
 *
 */
#ifndef _DLIGHT_H_
#define _DLIGHT_H_

#include "Source.h"
#include "Vector.h"
#include "Color.h"

class DLight : public Source{
	
	//Vector3d position;
	Vector3d direction;
	Color color;
	
public:
	
	DLight();
	
	DLight(Vector3d, Color);
	
	
	//virtual Vector3d getLightPosition(){return position;}
	virtual Color getColor(){ return color;}
	virtual Vector3d getLightDirection(){return direction;}
	virtual double getCosTheta(Vector3d nl, Vector3d nlh){
		double cosTheta = nl*nlh;
		return cosTheta;
	}
	
};
	DLight::DLight(){
		//position.set(0,0,0);
		direction.set(0,0,0);
		color = Color(1,1,1);
	}
	
	DLight::DLight(Vector3d d, Color c){
		//position = p;
		direction = d;
		color = c;
	}	
	
#endif