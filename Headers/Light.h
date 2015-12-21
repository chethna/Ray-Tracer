/*
 *  Light.h
 *  
 *
 *  Created by Chethna Kabeerdoss on 15/02/14.
 *  Copyright 2014 __MyCompanyName__. All rights reserved.
 *
 */
#ifndef _LIGHT_H_
#define _LIGHT_H_

#include "Source.h"
#include "Vector.h"
#include "Color.h"

class Light : public Source{
	
	Vector3d position;
	Color color;
    float intensity;
	
public:
	
	Light();
	
	Light(Vector3d, Color, float);
	
	virtual Vector3d getLightPosition(){return position;}
	virtual Color getColor(){ return color;}
    virtual float getIntensity(){ return intensity;}
	virtual double getCosTheta(Vector3d nl, Vector3d nlh){
		double cosTheta = (nl*nlh);
		return cosTheta;
	}
	
};
	Light::Light(){
		position.set(0,0,0);
		color = Color(1,1,1);
        intensity = 0.3;
	}
	
	Light::Light(Vector3d p, Color c, float i){
		position = p;
		color = c;
        intensity = i;
	}

#endif