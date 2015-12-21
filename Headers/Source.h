/*
 *  Source.h
 *  
 *
 *  Created by Chethna Kabeerdoss on 16/02/14.
 *  Copyright 2014 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _Source_H_
#define _Source_H_

#include "Vector.h"
#include "Color.h"
#include "Ray.h"

class Source{
	
public:
	
	Source();
	
	//method function
	virtual Color getColor(){ return Color(0.0,0.0,0.0);}
	
	virtual Vector3d getLightPosition() {
		return Vector3d(0, 0, 0);
	}
	
	virtual Vector3d getLightDirection() {
		return Vector3d(0, 0, 0);
	}
	
    virtual float getIntensity(){ return 0.3;}
    
	virtual double getCosTheta(Vector3d nl, Vector3d nlh) {
		return 0.0;
	}
	
	
};
Source::Source() {}


#endif