/*
 *  Camera.h
 *  
 *
 *  Created by Chethna Kabeerdoss on 15/02/14.
 *  Copyright 2014 __MyCompanyName__. All rights reserved.
 *
 */
#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "Vector.h"

class Camera{

	Vector3d campos,camdir,camright,camdown;
	Vector3d vView,vUp,n0,n1,n2;
	Vector3d pc;
	double distance;
	public:
	
	Camera();
	Camera(Vector3d,Vector3d,Vector3d,double);
	Camera(Vector3d,Vector3d,Vector3d,Vector3d,double);
	
	//method functions
	Vector3d getCameraPosition() { return campos; }
	Vector3d getCameraDirection() { return camdir; }
	Vector3d getCameraRight() { return camright; }
	Vector3d getCameraDown() { return camdown; }
	Vector3d getCameravView() { return vView; }
	Vector3d getCameravUp() { return vUp; }
	Vector3d getCameran0() { return n0; }
	Vector3d getCameran1() { return n1; }
	Vector3d getCameran2() { return n2; }
	
	double getCameraDistance() { return distance; }
	
	Vector3d getImagePlaneCenter(){ return pc;}
	
};
	Camera :: Camera(){
		campos.set(0,0,0);
		camdir.set(0,0,1);
		camright.set(0,0,0);
		camdown.set(0,0,0);
		vView.set(2,2,1);
		vUp.set(2,0,0);
		distance = 1;
		n0 = (vUp%vView)/(vUp%vView).norm();
		n1 = (n0%vView)/vView.norm();
		n2 = vView.normalize();
		pc = campos + distance*n2;
	}
	
Camera :: Camera(Vector3d pos, Vector3d view, Vector3d up,double disValue){
	campos = pos;
	vView = view;
	vUp = up;
	distance = disValue;
	n0 = (vUp%vView)/(vUp%vView).norm();
	n1 = (n0%vView)/vView.norm();
	n2 = vView.normalize();
	pc = campos + distance*n2;	
}

	Camera :: Camera(Vector3d pos, Vector3d dir, Vector3d right, Vector3d down,double disValue){
		campos = pos;
		camdir = dir;
		camright = right;
		camdown = down;
		distance = disValue;
		pc = campos + distance*camdir;
	}
 
#endif