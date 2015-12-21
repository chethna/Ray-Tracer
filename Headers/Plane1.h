/*
 *  Plane.h
 *  Raytracer
 *
 *  Created by Amber on 2/15/14.
 *  Copyright 2014 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef PLANE1_H
#define PLANE1_H

#define Epsilon 0.000001

#include "Object.h"


class Plane1 : public Object
{
		
public:
	Vector3d normal;
	Vector3d origin;
	Color color;
	char material;
	string texFileName, normalTexFileName ;
	PixMapValues result,normalTexFile;
	
	Plane1(Vector3d originValue, Vector3d normalValue,Color colorValue,string texFileNameValue, string normalTexFileNameValue,char materialValue )
	{	
		origin = originValue;
		normal = normalValue;
		color = colorValue;
		material = materialValue;
		normal = normal.normalize();
		
		//texture file
		if (texFileNameValue.length()!=0) {
			texFileName = texFileNameValue;
			FILE *readFile;
			readFile = fopen(texFileName.c_str(), "r");
			ImageReader ir;
			result =ir.readPPM(readFile);
		}
		
		//normal texture file
		if (normalTexFileNameValue.length()!=0) {
			normalTexFileName = normalTexFileNameValue;
			FILE *readFile;
			readFile = fopen(normalTexFileName.c_str(), "r");
			ImageReader ir;
			normalTexFile =ir.readPPM(readFile);
		}
	}
	
	Plane1()
	{
		normal.set(0.0, 1.0, 0.0);
		origin.set(0.0, 0.0, 0.0);
		texFileName ="";
	};
	
	virtual ~Plane1() {}
	
	virtual double findInsection(Ray ray)
	{
		Vector3d pe = ray.getRayOrigin();
		Vector3d npe = ray.getRayDirection();
		
		float denom = npe *(normal);
		if (denom > Epsilon) {
			float d = (pe - origin)*(normal);
			float th = d/denom;
			
			if (th > Epsilon) {
				//r.t = th;
				//r.hit = this;
				return th;
			}
		}
		return 0;
		
		
	};
	
	virtual Vector3d getNormalAt(Vector3d point)
	{
		return normal;
	}

	virtual Color getColor(){ return color;}
	
	virtual Color getTexColor(Vector3d ph) {
		if (texFileName.length()==0) {
			//printf("No Texture file for plane!");
			//abort();
			return color;
		}
		Vector3d Xaxis(1,0,0);
		Vector3d Yaxis(0,1,0);
		Vector3d Zaxis(0,0,1); 
		Vector3d N0(1,0,0);
		Vector3d N1(0,0,1);
		Color final;
		Vector3d n = normal;
		Vector3d N2 = n;
		if ((N2%Xaxis).norm()==0.0) {
			N0 = Zaxis;
			N1 = Yaxis;
		}
		if ((N2%Zaxis).norm()==0.0) {
			N0 = Xaxis;
			N1 = Yaxis;
		}
		
		
		float u,v;
		int s = 1;
		float n0Value = (N0*ph)/s;
		float n1Value = (N1*ph)/s;
		float n2Value = (N2*ph)/s;
		
		u = n0Value - int(n0Value);
		v = n1Value - int(n1Value);
		int XmaxT = result.width;
		int YmaxT = result.height;
		//printf("xmax: %d , ymax: %d \n", XmaxT,YmaxT);

		unsigned char *pixmap1 = result.pix;
		
		if (u<0) {
			u = 1+u;
		}
		if (v<0) {
			v = 1+v;
		}
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
		//printf("x: %f , y: %f\n",x,y);
		//printf("I: %d , J: %d\n",I,J);
		
		int index =  (J * XmaxT + I) * 3;
		
		final.setColorRed(pixmap1[index]);
		final.setColorGreen(pixmap1[index+1]);
		final.setColorBlue(pixmap1[index+2]);
		
		return final;
	}		
	
	virtual Vector3d getNewNormalAt(Vector3d ph) {
		if (normalTexFileName.length()==0) {
			return getNormalAt(ph);
		}
		Color final;
		Vector3d Xaxis(1,0,0);
		Vector3d Yaxis(0,1,0);
		Vector3d Zaxis(0,0,1); 
		
		Vector3d N0(1,0,0);
		Vector3d N1(0,0,1);
		Vector3d n = normal;
		Vector3d N2 = n;
		if ((N2%Xaxis).norm()==0.0) {
			N0 = Zaxis;
			N1 = Yaxis;
		}
		if ((N2%Zaxis).norm()==0.0) {
			N0 = Xaxis;
			N1 = Yaxis;
		}
		
		
		float u,v;
		int s = 20;
		float n0Value = (N0*ph)/s;
		float n1Value = (N1*ph)/s;
		float n2Value = (N2*ph)/s;
		
		u = n0Value - int(n0Value);
		v = n1Value - int(n1Value);
		int XmaxT = normalTexFile.width;
		int YmaxT = normalTexFile.height;
		//printf("xmax: %d , ymax: %d \n", XmaxT,YmaxT);
		
		unsigned char *pixmap1 = normalTexFile.pix;
		
		if (u<0) {
			u = 1+u;
		}
		if (v<0) {
			v = 1+v;
		}
		//printf("u: %f , v: %f \n", u,v);
		
		float x,y;
		
		x = u*XmaxT;
		y = v*YmaxT;
		
		//printf("x: %f , y: %f \n", x,y);
		
		int I, J;
		I = (int)x;
		J = (int)y;
		//printf("x: %f , y: %f\n",x,y);
		//printf("I: %d , J: %d\n",I,J);
		
		int index =  (J * XmaxT + I) * 3;
		
		float redValue,greenValue,blueValue;
        if(index>=0){
         
            redValue = (float)(pixmap1[index]);
            greenValue = (pixmap1[index+1]);
            blueValue = (pixmap1[index+2]);
		
            
            float R = (2*(redValue)) - 1;
            float G = (2*(greenValue)) - 1;
            float B = blueValue;
		
            //printf("Normal: %f, %f, %f \n",nhz.x,nhz.y,nhz.z);
            Vector3d N = (N0*R) + (N1*G) + (N2*B);
            N=N.normalize();
            //printf("New Normal: %f, %f, %f \n",N.x,N.y,N.z);
            return N;
        }
        else{
            return 0;
        }
	}
	
	virtual char getMaterial(){return material;}
    

};

#endif
