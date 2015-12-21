/*
 *  Sphere.h
 *  
 *
 *  Created by Chethna Kabeerdoss on 15/02/14.
 *  Copyright 2014 __MyCompanyName__. All rights reserved.
 *
 */
#ifndef _SPHERE_H_
#define _SPHERE_H_

#include "math.h"
#include "Vector.h"
#include "Color.h"
#include "Object.h"
#include "Ray.h"

class Sphere : public Object{
	
	Vector3d center;
	double radius;
	Color color;
	int t;
	char material;
	string texFileName ;
	PixMapValues result;
	PixMapValues normalTexmap;
	string normalTexFileName;
    float RI;
    bool irr;
	
public:
	
	Sphere();
	
	Sphere(Vector3d, double, Color,string,string, char, float,bool);
	
	virtual Vector3d getSphereCenter(){return center;}
	virtual double getSphereRadius(){return radius;}
	virtual Color getColor(){ return color;}
    virtual float getRefractiveIndex(){ return RI;}
    virtual bool getIrrBool(){ return irr;}
	virtual Vector3d getNormalAt(Vector3d point){
	//normal always points away from the center of the sphere
		Vector3d normal_vect = ((point-center)/radius).normalize();
		return normal_vect;
	}
	
	virtual Color getTexColor(Vector3d ph) {
		if (texFileName.length()==0) {
			//printf("No Texture file for sphere!");
			//abort();
			return color;
		}
		Color final;
		Vector3d N0(1,0,0);
		Vector3d N1(0,0,1);
		Vector3d N2(0,1,0);
		float u,v;
		float n0Value = (N0*(ph-center))/radius;
		float n1Value = (N1*(ph-center))/radius;
		float n2Value = (N2*(ph-center))/radius;
		float phi;
		if (n2Value < -1.0) {
			n2Value = -1.0;
		} else if (n2Value > 1.0) {
			n2Value = 1.0;
		}
		if (n2Value > -0.0001 && n2Value < 0.0001) {
			phi = PI / 2.0;
		} else { 
			phi = acos(n2Value);
		}
		v = phi/ PI; 
		float rootValue = sqrt(1-(n2Value*n2Value));
		float inverCosValue = n1Value/rootValue;
		if (inverCosValue<(-1)) {
			inverCosValue = -1.00;
		}
		if (inverCosValue >1) {
			inverCosValue = 1.00;
			
		}
		float theta = acos(inverCosValue);
		if (n0Value < 0) {
			theta = (2*PI) - theta;
		}
		//printf("inverCosValue: %f \n",inverCosValue);
		//printf("acos(inverCosValue): %f \n",acos(inverCosValue));
		//cout << inverCosValue<<endl;
		
		u = theta/(2*PI);
		//printf("u: %f , v: %f\n",u,v);
		int XmaxT = result.width;
		int YmaxT = result.height;
		unsigned char *pixmap1 = result.pix;
		
		float x,y;
		
		x = u*XmaxT;
		y = v*YmaxT;
		
		//solid texture
		//x = ((n0Value+1)/2)*XmaxT;
		//y = ((n1Value+1)/2)*YmaxT;
		
		int I, J;
		I = (int)x;
		J = (int)y;
		//printf("x: %f , y: %f\n",x,y);
		//printf("I: %d , J: %d\n",I,J);
		
		int index =  (J * XmaxT + I) * 3;
		int index1 =  (J * XmaxT + (I+1)) * 3;
		int index2 =  ((J+1) * XmaxT + (I+1)) * 3;
		int index3 =  ((J+1) * XmaxT + I) * 3;
		
		float i ,j ;
		i = x - I;
		j = y - J;
		
		float redValue,greenValue,blueValue;
        //cout<<(pixmap1[index]*(1-i)*(1-j))+(pixmap1[index1]*(1-j)*i)+(pixmap1[index2]*i*j)+(pixmap1[index3]*(1-i)*j)<<"\n";
		redValue = (pixmap1[index]*(1-i)*(1-j))+(pixmap1[index1]*(1-j)*i)+(pixmap1[index2]*i*j)+(pixmap1[index3]*(1-i)*j);
		greenValue = (pixmap1[index+1]*(1-i)*(1-j))+(pixmap1[index1+1]*(1-j)*i)+(pixmap1[index2+1]*i*j)+(pixmap1[index3+1]*(1-i)*j);
		blueValue = (pixmap1[index+2]*(1-i)*(1-j))+(pixmap1[index1+2]*(1-j)*i)+(pixmap1[index2+2]*i*j)+(pixmap1[index3+2]*(1-i)*j);
		
		final.setColorRed(redValue);
		final.setColorGreen(greenValue);
		final.setColorBlue(greenValue);
		
		return final;
	}		
	
	virtual double findInsection(Ray ray){
		Vector3d pe = ray.getRayOrigin();
		Vector3d npe = ray.getRayDirection();
		center = getSphereCenter();
		double b = npe*(center-pe);
		double c = ((center-pe)*(center-pe))  - (radius*radius);
		//printf("b: %f  c:%f \n",b,c);
		double delta = b*b - c;
		if(delta>=0&&b>0){
			//ray intersects the sphere
				double t1 = (b-sqrt(delta));
				double t2 = (b+sqrt(delta));
			return t1;
			}
			else {
				//ray missed the sphere
				return -1;
			}

	}
	
	virtual char getMaterial(){return material;}
	
	
	virtual Vector3d getNewNormalAt(Vector3d ph) {
		if (normalTexFileName.length()==0) {
			//printf("No normal Texture file for sphere!");
			return getNormalAt(ph);
		}
		
		Color final;
		Vector3d N0(1,0,0);
		Vector3d N1(0,0,1);
		Vector3d N2(0,-1,0);
		
		float u,v;
		float n0Value = (N0*(ph-center))/radius;
		float n1Value = (N1*(ph-center))/radius;
		float n2Value = (N2*(ph-center))/radius;
		float phi = acos(n2Value);
		v = phi/ PI; 
		float rootValue = sqrt(1-(n2Value*n2Value));
		float inverCosValue = n1Value/rootValue;
		if (inverCosValue<(-1)) {
			inverCosValue = -1.00;
		}
		if (inverCosValue >1) {
			inverCosValue = 1.00;
			
		}
		float theta = acos(inverCosValue);
		
		if (n0Value < 0) {
			theta = (2*PI) - theta;
		}
		//printf("inverCosValue: %f \n",inverCosValue);
		//printf("acos(inverCosValue): %f \n",acos(inverCosValue));
		//cout << inverCosValue<<endl;
		
		u = theta/(2*PI);
		//printf("u: %f , v: %f\n",u,v);
		
		int XmaxT = normalTexmap.width;
		int YmaxT = normalTexmap.height;
		unsigned char *pixmap1 = normalTexmap.pix;
		
		float x,y;
		
		x = u*XmaxT;
		y = v*YmaxT;
		
		int I, J;
		I = (int)x;
		J = (int)y;
		//printf("x: %f , y: %f\n",x,y);
		//printf("I: %d , J: %d\n",I,J);
		
		int index =  (J * XmaxT + I) * 3;
		/*int index1 =  (J * XmaxT + (I+1)) * 3;
		int index2 =  ((J+1) * XmaxT + (I+1)) * 3;
		int index3 =  ((J+1) * XmaxT + I) * 3;*/
		
		float i ,j ;
		i = x - I;
		j = y - J;
		
		float redValue,greenValue,blueValue;
		
		redValue = (pixmap1[index]);//*(1-i)*(1-j))+(pixmap1[index1]*(1-j)*i)+(pixmap1[index2]*i*j)+(pixmap1[index3]*(1-i)*j);
		greenValue = (pixmap1[index+1]);//*(1-i)*(1-j))+(pixmap1[index1+1]*(1-j)*i)+(pixmap1[index2+1]*i*j)+(pixmap1[index3+1]*(1-i)*j);
		blueValue = (pixmap1[index+2]);//*(1-i)*(1-j))+(pixmap1[index1+2]*(1-j)*i)+(pixmap1[index2+2]*i*j)+(pixmap1[index3+2]*(1-i)*j);
		
		
		//calculate phx, phy
		float thetaX = ((x+1)/XmaxT)*(2*PI);
		float phiY = ((y+1)/YmaxT)*(PI);
		
		Vector3d phx; 
		Vector3d phy; 
		
		phx.x = radius*sin(thetaX)*sin(phi) + center.x;
		phy.x = radius*sin(theta)*sin(phiY) + center.x;
		
		phx.y = radius*cos(thetaX)*sin(phi)+center.y;
		phy.y = radius*cos(theta)*sin(phiY)+center.y;
		
		phx.z = radius*cos(phi)+center.z;
		phy.z = radius*cos(phiY)+center.z;
		
		Vector3d nhx = (phx - ph).normalize();
		Vector3d nhy = (phy - ph).normalize();
		
		float R = (2*(redValue)) - 1;
		float G = (2*(greenValue)) - 1;
		float B = blueValue;
		
		Vector3d nhz = getNormalAt(ph);
		//printf("Normal: %f, %f, %f \n",nhz.x,nhz.y,nhz.z);
		Vector3d N = (nhx*R) + (nhy*G) + (nhz*B);
		 N=N.normalize();
		//printf("New Normal: %f, %f, %f \n",N.x,N.y,N.z);
		return N;
	}
	
};
	Sphere::Sphere(){
		center.set(0,0,0);
		radius = 1.0;
		color = Color(.5,.5,.5);
		material = 'l';
        RI = 1.0;
        irr = false;
		texFileName ="";
		//result = NULL;
	}
	
	Sphere::Sphere(Vector3d centerValue, double radiusValue, Color colorValue,string texFileNameValue, string normalTexFileNameValue,char materialChar, float RIValue, bool irrValue){
		center = centerValue;
		radius = radiusValue;
		color = colorValue;
        RI = RIValue;
		material = materialChar;
        irr = irrValue;
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
			printf("inside");
			normalTexFileName = normalTexFileNameValue;
			FILE *readFile;
			readFile = fopen(normalTexFileName.c_str(), "r");
			ImageReader irN;
			normalTexmap =irN.readPPM(readFile);
		}
	}

#endif