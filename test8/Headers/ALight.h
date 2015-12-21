/*
 *  Light.h
 *  
 *
 *  Created by Chethna Kabeerdoss on 15/02/14.
 *  Copyright 2014 __MyCompanyName__. All rights reserved.
 *
 */
#ifndef _ALIGHT_H_
#define _ALIGHT_H_

#include "Source.h"
#include "Vector.h"
#include "Color.h"

class ALight : public Source{
	
	Vector3d position;
	//Vector3d direction;
	Vector3d n0,n1;
	float sx,sy;
	
	Vector3d p00;
	Color color;
	
public:
	
	ALight();
	
	ALight(Vector3d, Vector3d, Vector3d, float, float, Color);
	
	
	virtual Vector3d getLightPosition(){
		//Vector3d vView(0,0,1);
		//Vector3d vUp(1,0,0);
		//Vector3d n2;//(-1,0,0);
		//n2 = vView.normalize();
		//Vector3d n0 = (vUp % n2).normalize();//vView).normalize();
		//Vector3d n1 = n0%n2;
		
		float randX = 1.0 * rand()/RAND_MAX;
		float randY = 1.0 * rand()/RAND_MAX;
//		float x = p00.x+(n0*sx*randX);
//		float y = p00.y+(n1*sy*randY);
		Vector3d pp = p00 + (n0 * sx*randX) + (n1 * sy*randY);
		//printf("point: %f %f f",pp.x,pp.y,pp.z);
		return pp;
	}
	virtual Color getColor(){ return color;}
	
	virtual double getCosTheta(Vector3d nl, Vector3d nlh){
		double cosTheta = (nl*nlh);
		return cosTheta;
	}
	
	/*virtual double getCosTheta(Vector3d nl, Vector3d nlh){
		Vector3d an2,an1,tempa, an0, ap00, apl;
		an2 = direction.normalize();
		tempa = an2%avup;
		an0 = tempa.normalize();
		an1 = an0%an2;
		ap00 = position - ((x/2)-an0) - ((y/2)*an1);
		//apl = ap00+ (
		double cosTheta = (nl*nlh)*((-1*nlh)*direction);
		penumbraValue = -1*direction*nlh;
		if (penumbra<penumbraValue) {
			penumbraValue = 0;
		}
		else {
			penumbraValue =1;
		}
		cosTheta = penumbra*cosTheta;

		return cosTheta;
	}*/
	
};
	ALight::ALight(){
		position.set(0,0,0);
		n0.set(0,1,0);
		n1.set(1,0,0);
		sx = 1;
		sy = 1;
		color = Color(1,1,1);
		p00= position - (sx/2) * n0 - (sy/2)*n1;
	}
	
	ALight::ALight(Vector3d posValue, Vector3d n0Value, Vector3d n1Value, float sxValue,float syValue, Color colorValue){
		position = posValue;
		n0 = n0Value;
		n1 = n1Value;
		sx = sxValue;
		sy = syValue;
		color = colorValue;
		n0 = n0.normalize();
		n1 = n1.normalize();
		 p00= position - (sx/2) * n0 - (sy/2)*n1;
	}	
	
#endif