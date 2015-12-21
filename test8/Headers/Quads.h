/*
 *  Quads.h
 *  
 *
 *  Created by Chethna Kabeerdoss on 24/02/14.
 *  Copyright 2014 __MyCompanyName__. All rights reserved.
 *
 */


/****
 Name \ Parameters	       a02	a12	a22	a21	a00
 Plane	                   0	0	0	1	0
 Generalized Cylinder	   1	1	0	0  -1
 Ellipsoid	               1	1	1	0  -1
 Cone	                   1	1  -1	0   0
 Hyperboloid of one sheet  1	1  -1	0  -1
 Hyperboloid of one sheet  1	1  -1	0	1
 Elliptic Paraboloid	   1	1	0  -1	0
 Hyperbolic Paraboloid	   1   -1	0  -1	0
  ***/

#ifndef _QUADS_H_
#define _QUADS_H_

#include "math.h"
#include "cmath"
#include "Vector.h"
#include "Color.h"
#include "Object.h"
#include "Ray.h"

class Quads : public Object{
	
	int a02,a12,a21,a00,a22,s0,s1,s2,id;
	Vector3d v2,vup,pc;
	Vector3d n0,n1,n2;
	Color color;
	char material;
public:
	
	Quads();
	
	Quads(int,int,int,Color,Vector3d,Vector3d,Vector3d,int,int,int,int,int,int,char);
	
	//Vector3d getQuadsCenter(){return center;}
	//double getQuadsRadius(){return radius;}
	virtual Color getColor(){ return color;}
	
	virtual Vector3d getNormalAt(Vector3d point){
		//normal always points away from the center of the Quads
		int a[] = { a02, a12, a22 };
		int s[] = { s0, s1, s2 };
		Vector3d n[] = { n0, n1, n2 };
		Vector3d gradient(0,0,0);
		Vector3d v = (point-pc);
		//printf("%f %f %f \n", point.x,point.y,point.z);
		for (int i = 0; i < 3; i++)
		{
			gradient = gradient + ((2.00 * a[i] * (n[i] * v)) / (s[i] * s[i])) * n[i];
		}
		gradient = gradient + (a21 / s2) * n2;
		gradient.normalize();
		return gradient;
	}
	
	
	virtual double findInsection(Ray ray){
		Vector3d pe = ray.getRayOrigin();
		Vector3d npe = ray.getRayDirection();
		//center = getQuadsCenter();
		double a = 0.0;
		double b = 0.0;
		double c = 0.0;
		double delta;

		a = a02 * pow(((n0 * npe) / s0),2) + a12 * pow(((n1 * npe) / s1),2) + a22 * pow(((n2 * npe) / s2),2);
		b = a02 * ((2 * (n0 * npe) * (n0 * (pe - pc))) / pow(s0,2)) + a12 * ((2 * (n1* npe) * (n1 *(pe - pc))) / pow(s1,2)) + 
		a22 * ((2 * (n2 * npe) * (n2 * (pe - pc))) / pow(s2,2)) + (a21 * (n2 * npe) / s2);
		c = a02 * pow(((n0 * (pe - pc)) / s0),2) + a12 * pow(((n1 * (pe - pc)) / s1),2) + a22 * pow(((n2 * (pe - pc)) / s2),2) 
		+ a21 * ((n2 * (pe -pc)) / s2) + a00;
		delta = ((b*b)-(4.0*a*c));
		
		if (delta<0.0|| a==0.0|| b== 0.0 ||c == 0.0) {
			return -1;
		}
		
		delta = sqrt(delta);
		//ray intersects the Quads
		double t1 = (-b-delta)/(2.0*a);
		double t2 = (-b+delta)/(2.0*a);
		
		if (t1<=0.00001 && t2 <= 0.00001) {
			// both intersections are behind the ray origin
			return -1;
		}
		
		double t = 0.0;
		if (t1<=0.00001) {
			t = t2;
		}
		else {
			t = (t1<t2) ? t1: t2;
		}
		if (t<0.00001) {
			// Too close to intersection
			return -1;
		}

		return t1;		
	}
	
	
	virtual char getMaterial(){return material;}
};
Quads::Quads(){
	a02 = 1;
	a12 = 1;
	a22 = -1;
	a21 = 0;
	a00 = 0;
	s0 = 0;
	s1 = 0;
	s2 = 0;
	color=Color(0,0,0);
	v2 = Vector3d(0,0,0);
	vup = Vector3d(0,0,0);
	pc = Vector3d(0,0,0);
	n0 = Vector3d(0,0,0);
	n1 = Vector3d(0,0,0);
	n2 = Vector3d(0,0,0);
}

Quads::Quads(int s0,int s1,int s2, Color color, Vector3d v2, Vector3d vup, Vector3d pc, int id, 
			 int a02, int a12, int a22, int a21, int a00 , char materialChar ){
	this->a02 = a02;
	this->a12 = a12;
	this->a22 = a22;
	this->a21 = a21;
	this->a00 = a00;
	this->s0 = s0;
	this->s1 = s1;
	this->s2 = s2;
	this->v2 = v2;
	this->vup = vup;
	this->pc = pc;
	this->color = color;
	this->id = id;
	material = materialChar;
	v2 = v2.normalize();
	vup = vup.normalize();
	n2=v2;//direction 
	n0=(n2%vup).normalize();
	//n0 = n0.Normalize();
	n1=n0%n2;
}

#endif