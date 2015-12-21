/*
 *  Object.h
 *  
 *
 *  Created by Chethna Kabeerdoss on 15/02/14.
 *  Copyright 2014 __MyCompanyName__. All rights reserved.
 *
 */
#ifndef _OBJECT_H_
#define _OBJECT_H_

#include "Vector.h"
#include "Color.h"
#include "Ray.h"
#include "ImageReader.h"

class Object{

public:
	
	Object();
	
	//method function
	virtual Color getColor(){ return Color(0.0,0.0,0.0);}
	virtual Color getTexColor(Vector3d ph){ return Color(0.0,0.0,0.0);}

	virtual Vector3d getNormalAt(Vector3d intersection_position) {
		return Vector3d(0, 0, 0);
	}
	
	virtual Vector3d getNewNormalAt(Vector3d intersection_position) {
		return Vector3d(0, 0, 0);
	}
	
	virtual double findInsection(Ray ray){
		return 0;
	}
	//sphere
	virtual Vector3d getSphereCenter(){return Vector3d(0, 0, 0);}
	virtual double getSphereRadius(){return 1;}
	
	virtual char getMaterial(){return 'l';}
	
	virtual float getRefractiveIndex(){
		return 1.0;
	}
	
};
	Object::Object(){
		//printf("inside");
	}
	

#endif