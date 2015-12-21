/*
 *  Triangle.h
 *  
 *
 *  Created by Chethna Kabeerdoss on 15/02/14.
 *  Copyright 2014 __MyCompanyName__. All rights reserved.
 *
 */
#ifndef _TRIANGLE_H_
#define _TRIANGLE_H_
#include <cstdlib>
#include <iostream>
#include <GLUT/glut.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <fstream>
#include "math.h"
#include "Vector.h"
#include "Color.h"
#include "Ray.h"
#include "Object.h"

class Triangle : public Object{
	
	Vector3d normal; //vector pendicular to the surface
	Vector3d p1, p2, p3;
	Vector3d t1, t2, t3;
	PixMapValues result;
	float s = 0.0;
	float t = 0.0;
	Color color;
	char material;
	string fileName;
public:
	
	Triangle();
	
	Triangle(Vector3d,Vector3d,Vector3d,Color colorValue, char materialChar);
	
	Triangle(Vector3d,Vector3d,Vector3d,Color, char, string, PixMapValues,Vector3d,Vector3d,Vector3d);

	//method functions
	void setSandT(float sValue, float tValue){
		this->s = sValue;
		this->t = tValue;
	}
	Vector3d getPlanePoint1(){return p1;}
	Vector3d getPlanePoint2(){return p2;}
	Vector3d getPlanePoint3(){return p3;}
	virtual Color getColor(){ return color;}
	
	//normal vectoTriangler at the point of intersection of the plane is the same as the normal of the plane
	virtual Vector3d getNormalAt(Vector3d point){
		return normal;
	}
	
	virtual Color getTexColor(Vector3d ph) {
		if (fileName.length()==0) {
			//printf("No Texture file for plane!");
			//abort();
			return color;
		}
		
		float w0 = 1 - (s+ t);
		float u0 = t1.x;
		float u1 = t2.x;
		float u2 = t3.x;
		float v0 = t1.y;
		float v1 = t2.y;
		float v2 = t3.y;
		float u = (w0*u0)+(s*u1)+(t*u2);
		float v = (w0*v0)+(s*v1)+(t*v2);
		if (u<(-0.99999)) {
			u = -0.99;
		}
		if (u >0.99999) {
			u = 0.99;
			
		}
		if (v<(-0.99999)) {
			v = -0.99;
		}
		if (v>0.99999) {
			v = 0.99;
			
		}
		Color final;
		
		int XmaxT = result.width;
		int YmaxT = result.height;
		//printf("xmax: %d , ymax: %d \n", XmaxT,YmaxT);
		
		unsigned char *pixmap1 = result.pix;
		
		//printf("s: %f , t: %f \n", s,t);
		
		//printf("u: %f , v: %f \n", u,v);
		
		float x,y;
		
		x = u*XmaxT;
		y = v*YmaxT;
		
		//solid textures
		//x = ((u+1)/2)*XmaxT;
		//y = ((v+1)/2)*YmaxT;
		
		//printf("x: %f , y: %f \n", x,y);
		
		int I, J;
		I = (int)x;
		J = (int)y;
		if (I>XmaxT) {
			I = I - XmaxT;
		}
		if(J>YmaxT){
			J = J -YmaxT;	
		}
		//printf("x: %f , y: %f\n",x,y);
		//printf("I: %d , J: %d\n",I,J);
		
		int index =  (J * XmaxT + I) * 3;
		
		final.setColorRed(pixmap1[index]);
		final.setColorGreen(pixmap1[index+1]);
		final.setColorBlue(pixmap1[index+2]);
		
		return final;
		
	}
	
	
	virtual double findInsection(Ray ray){
		Vector3d pe = ray.getRayOrigin();
		Vector3d npe = ray.getRayDirection();
		float a = npe*normal;
		//if(a!= 0.000001){
			float t = ((p1 - pe)*normal)/a; 
			Vector3d ph = pe + npe*(t);
			//
			Vector3d A = (p3-p1)%(p2-p1);
			Vector3d A1 = (p1-ph)%(p2-ph);
			Vector3d A2 = (p2-ph)%(p3-ph);
			Vector3d A3 = (p3-ph)%(p1-ph);
			
		float alpha = A3.norm()/A.norm();
		float beta = A1.norm()/A.norm();
		float gamma = A2.norm()/A.norm();
		float sum = alpha+beta+gamma;
		
		float U = A.x <= 0.01 ? (A.y <= 0.01 ? A1.z / A.z : A1.y / A.y) : A1.x / A.x;
		float V = A.x <= 0.01 ? (A.y <= 0.01 ? A2.z / A.z : A2.y / A.y) : A2.x / A.x;
		
		if (abs(sum-1.00)<0.01 && alpha>=0 && beta>=0 && gamma>=0 && alpha<1.00 && beta<1.00 && gamma<1.00) {
			if (t>0.000001) {
				//setSandT(alpha,gamma);
				//cout << "u:"<<U<<"v:"<<V<<endl;
				setSandT(U,V);
				return t;
			}
			else {
				//setSandT(0,0);
				return -1;
			}

		}
		else {
			//setSandT(0,0);
			return -1;
		}
		//}	
			//
			/*if ((triangleArea(p1,p2,p3)-triangleArea(p1,p2,ph)-triangleArea(p2,p3,ph)-triangleArea(p1,p3,ph))<0.000001) {
				return t;
			}*/
		//	else {
		//		return -1;
		//	}

		//}
		//else{
			//ray is parallel to the plane
		//	return 0;
		//}
	}
	
	double triangleArea(Vector3d p1, Vector3d p2, Vector3d p3){
		double a = (p1-p2).norm();
		double b = (p2-p3).norm();
		double c = (p3-p1).norm();
		double s = (a+b+c)/2;
		//area of triangle
		return sqrt(s*(s-a)*(s-b)*(s-c));
	}
	
	
	virtual char getMaterial(){return material;}
	
	virtual Vector3d getNewNormalAt(Vector3d point){
	return normal;
}
};
	Triangle::Triangle(){
		normal.set(0,0,1); // default orienation in the x direction
		p1.set(-1,1,-1); //default at the corner
		p2.set(1,1,-1);
		p3.set(1,-1,-1);
		
		color = Color(.5,.5,.5);
		material = 'l';
	}
	
	Triangle::Triangle(Vector3d point1, Vector3d point2, Vector3d point3, Color colorValue, char materialChar){
		p1 = point1;
		p2 = point2;
		p3 = point3;
		Vector3d a = (p2-p1);
		Vector3d b = (p3-p1);
		Vector3d c = a%b;
		normal = c.normalize();
		color = colorValue;
		material = materialChar;
	}

Triangle::Triangle(Vector3d point1, Vector3d point2, Vector3d point3, Color colorValue, char materialChar, string fileNameValue, PixMapValues value,Vector3d tx1,Vector3d tx2,Vector3d tx3){
	
	p1 = point1;
	p2 = point2;
	p3 = point3;
	Vector3d a = (p2-p1);
	Vector3d b = (p3-p1);
	Vector3d c = a%b;
	normal = c.normalize();
	color = colorValue;
	material = materialChar;
	result = value;
	t1 = tx1;
	t2 = tx2;
	t3 = tx3;
	fileName = fileNameValue;
	
}

#endif

