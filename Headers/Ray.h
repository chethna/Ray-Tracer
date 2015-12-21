/*
 *  Ray.h
 *  
 *
 *  Created by Chethna Kabeerdoss on 15/02/14.
 *  Copyright 2014 __MyCompanyName__. All rights reserved.
 *
 */
#ifndef _RAY_H_
#define _RAY_H_

#include "Vector.h"

class Ray{

	Vector3d origin, direction;
public:
	Ray();
	
	Ray(Vector3d, Vector3d);
	
	Vector3d getRayOrigin(){return origin;}
	Vector3d getRayDirection(){ return direction;}
	
};
	Ray::Ray(){
		origin.set(0,0,0);
		direction.set(1,0,0);
		}
	
	Ray::Ray(Vector3d o, Vector3d d){
		origin = o;
		direction = d;
	}

#endif