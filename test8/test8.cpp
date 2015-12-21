/*
 *  test.cpp
 *  
 *
 *  Created by Chethna Kabeerdoss on 13/02/14.
 *  Copyright 2014 __MyCompanyName__. All rights reserved.
 *
 */

#include "Headers/Object.h"
#include "Headers/Source.h"
#include <cstdlib>
#include <iostream>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#include <fstream>
#include <cassert>
#include <sstream>
#include <string>
#include <math.h>
#include <time.h>
#include <Vector>
//#include "Headers/Vector.cpp"
#include "Headers/glut.h"
#include "Headers/Sphere.h"
#include "Headers/Triangle.h"
#include "Headers/Quads.h"
#include "Headers/Ray.h"
#include "Headers/Light.h"
#include "Headers/Camera.h"
#include "Headers/Color.h"
#include "Headers/SLight.h"
#include "Headers/DLight.h"
#include "Headers/ALight.h"
#include "Headers/Plane1.h"
#include "Headers/ObjectReader.h"
#include "Headers/PerilinNoise.h"
//#define PI 3.14159265


using namespace std;
int width, height;
unsigned char *pixmap;
float XX, YY;
static float mb = 0.0;
static float cb = 0.1;

//static float n = 1.8;
//defaults
Vector3d ORIGIN(0,0,0);
Vector3d Xaxis(1,0,0);
Vector3d Yaxis(0,1,0);
Vector3d Zaxis(0,0,1);

//Camera settings
//Vector3d pe(-2,-1,-10);
Vector3d pe(0,0,-10);
Vector3d vView(0,0,1);
Vector3d vUp(0,1,0);

//generated image size
float Xmax = 1280;//
float Ymax = 720;//
float sx = Xmax / Ymax;
float sy = 1.0;

//shaders
char phong = 'p';
char lambert = 'l';
char gooch = 'g';

//sphere objects - center and radius
Vector3d sc1(-.5,2.2,13);//(-6,2,-2);
Vector3d sc2(-5,-2,5);
Vector3d sc3(7,6,16);
Vector3d sc4(1.5,0,8);
Vector3d sc5(-1.5,0,8);
Vector3d sc6(0,0,0);

//
float r1 = 5;//3;
float r2 = 2;
float r3 = 1;
float r6 = 100;

//plane objects - plane normal, points in the plane-corners
Vector3d plAN(0,1,0);
Vector3d plA1(-5,5,-5);
Vector3d plA2(5,5,-5);
Vector3d plA3(5,-5,-5);
Vector3d plA4(-5,-5,-5);
//
Vector3d plBN(0,5,5);
Vector3d plB1(-5,5,0);
Vector3d plB2(-5,-5,0);
Vector3d plB3(1,5,-3);
Vector3d plB4(1,-5,-3);

//colors
Color red(0x69,0x18,0x24);
Color blue(0x30,0x4C,0xC9);
Color beige(0xCF,0xBB,0xAA);
Color black(0x00,0x00,0x00);
Color white(0xff,0xff,0xff);
Color green(0x40,0x6E,0x4C);
Color yellow(0xF0,0xF0,0x7D);
Color grey(0x80,0x7E,0x7E);

double accuracy = 0.000001;

//light source
double ambientLight = 1;
Vector3d light_position1(55,-10,4);
//Vector3d light_position1(0,50,20);
Vector3d light_position2(-55,-10,4);
Vector3d light_position(5,-5,0);
Vector3d light_direction(-1,0,0);
//Vector3d light_direction1(0,.2,3);
Vector3d light_position3(0,-30,13);

Vector3d light_direction1 = ((Vector3d(4,0,0)-light_position)).normalize();

bool shaderBool = false;
//scene objects
Sphere scene_sphere1(sc1,r1,yellow,"/Users/chethna/Documents/Chethna/RayTracer/Ray Tracer/Images/poolball_black_shifted_2.ppm","",lambert,1.0);
Sphere scene_sphere2(sc2,r2,green,"/Users/chethna/Documents/Chethna/RayTracer/Ray Tracer/Images/poolball_black_shifted_2.ppm","",lambert,1.0);
Sphere scene_sphere3(sc3,r2,red,"","",phong,1.0);
Sphere scene_sphere4(sc4,r3,white,"","",lambert,1.0);
Sphere scene_sphere5(sc5,r3,white,"","",gooch,1.0);
Sphere scene_sphere6(sc6,r6,white,"","",lambert,1.0);

//Plane scene_plane(plAN,plA1,plA2,plA3,plA4,beige,lambert);
//Plane scene_plane2(plBN,plB1,plB2,plB3,plB4,white, lambert);
Quads cylinder(1, 2, 1, yellow, Vector3d(0,1,0), Vector3d(1,0,0), Vector3d(8,0,20) , 0, 1, 1, 0, 0, -1 , lambert);// - cylinder
//Quads plane1(2, 2, 1, red, Vector3d(0,0,1), Vector3d(0,1,0), Vector3d(5,5,5) , 0, 0, 0, 0, 1, 0 , lambert);// - cylinder

Plane1 plane1(Vector3d(0,-15,0),Vector3d(0,1,0), white,"","", lambert); 
Plane1 plane2(Vector3d(23,-4,0),Vector3d(1,0,0), white,"","" ,lambert); 
Plane1 plane3(Vector3d(0,4,-50),Vector3d(0,0,-1), white, "","",lambert);

//Object file
//ObjectReader oR("3.obj");//"3.obj");//


//scene lights
Light scene_light1(light_position1,black);
Light scene_light2(light_position2,black);
DLight scene_Dlight(light_direction,black);
SLight scene_Slight(light_position3,light_direction1,45,black);
ALight scene_Alight(light_position1,Vector3d(5,0,5),Vector3d(5,0,5),3,3,black);




bool softShadow =false;
bool irr =false;
bool envMapBool =false;
bool glossy =false;
bool translucency =false;
bool cubist =false;

bool rayDepth =false;
float d = 1;

bool ambientOcclution = false;
vector<Vector3d> verts;

PixMapValues ProjectonTex;
PixMapValues ENV;
PixMapValues CUBISM;

//////////////////////////////////////////////////////////////////////////////////////////////////////////

struct RefReturn {
	Vector3d pe;
	Vector3d npe;
	float t;
	int index;
};


//method function to find the nearest object
Vector3d getTransmittedRay(Vector3d npe,Vector3d nValue, float refIndex){
	double q = npe * nValue;
	if (abs(q) == 1.0) {
		return (npe);
	} 
	else {
		if (refIndex < 1.0) {
			return ((npe) * refIndex + nValue * (1.0 - refIndex)).normalize();
			//return npe;
		} else {
			Vector3d n0 = (npe + (npe * nValue) * nValue).normalize();
			return (((npe) / refIndex) - n0 * (1.0 - (1.0 / refIndex))).normalize();
			}
	}
	
	/*Vector3d v = -1*npe;
	Vector3d n = nValue;
	//cout << n.x << ", " << n.y << ", " << n.z << endl;
	float C = v*n;
	float eta = refIndex;
	float bA = C/eta;
	float bB = (C*C)-1;
	float bC = (bB/(eta*eta))+1;
	
	//if(bC>0.0){
		float bD =sqrt(bC);
		float b = bA - bD;
		float a = -1*(1/eta);
		Vector3d t = ((a*v)+(b*n)).normalize();
		cout << "npe = " << npe.x << ", " << npe.y << ", " << npe.z << ", t = " << t.x << ", " << t.y << ", " << t.z << endl;
		return t;
	*/
}


Color irdecence(float cosTheta){
	
	//Color def(1,1,1);
	float Gamma = 0.80;
	float IntensityMax = 100;
	float factor;
	
	float t = 1+octave_noise_2d(2,.5,.005,XX,YY);
	float d = t/(cosTheta+t);
	float a = (2*d) - 1;
	float lambda = 400+(a*100);
	float Red,Green,Blue;
	
	//cout << lambda<<endl;
	float Wavelength = lambda;
	if((Wavelength >= 380) && (Wavelength<440)){
        Red = -(Wavelength - 440) / (440 - 380);
        Green = 0.0;
        Blue = 1.0;
    }else if((Wavelength >= 440) && (Wavelength<490)){
        Red = 0.0;
        Green = (Wavelength - 440) / (490 - 440);
        Blue = 1.0;
    }else if((Wavelength >= 490) && (Wavelength<510)){
        Red = 0.0;
        Green = 1.0;
        Blue = -(Wavelength - 510) / (510 - 490);
    }else if((Wavelength >= 510) && (Wavelength<580)){
        Red = (Wavelength - 510) / (580 - 510);
        Green = 1.0;
        Blue = 0.0;
    }else if((Wavelength >= 580) && (Wavelength<645)){
        Red = 1.0;
        Green = -(Wavelength - 645) / (645 - 580);
        Blue = 0.0;
    }else if((Wavelength >= 645) && (Wavelength<1000)){
		Red = 1.0;
        Green = 0.0;
        Blue = 0.0;
    }else{
        Red = 0.0;
        Green = 0.0;
        Blue = 0.0;
    };
	
    // Let the intensity fall off near the vision limits
	
    if((Wavelength >= 380) && (Wavelength<420)){
        factor = 0.3 + 0.7*(Wavelength - 380) / (420 - 380);
    }else if((Wavelength >= 420) && (Wavelength<701)){
        factor = 1.0;
    }else if((Wavelength >= 701) && (Wavelength<781)){
        factor = 0.3 + 0.7*(780 - Wavelength) / (780 - 700);
    }else{
        factor = 0.0;
    }
	
	
    int rgb[3];
	
    // Don't want 0^x = 1 for x <> 0
    rgb[0] = Red==0.0 ? 0 : (int) (IntensityMax * pow(Red * factor, Gamma));
    rgb[1] = Green==0.0 ? 0 : (int) (IntensityMax * pow(Green * factor, Gamma));
    rgb[2] = Blue==0.0 ? 0 : (int)(IntensityMax * pow(Blue * factor, Gamma));
	Color irr(rgb[0],rgb[1],rgb[2]);
	
	return irr;//def.colorAdd(irr);
}

Color envMap(Vector3d intersecting_ray_dir){
	Color final;
	Vector3d ray = intersecting_ray_dir;
	float u,v;
	float phi;
	if (ray.y > -0.0001 && ray.y < 0.0001) {
		phi = PI / 2.0;
	} else { 
		phi = acos(ray.y);
	}
	v = phi/ PI; 
	float rootValue = sqrt(1-(ray.y*ray.y));
	float inverCosValue = ray.x/rootValue;
	if (inverCosValue<(-1)) {
		inverCosValue = -1.00;
	}
	if (inverCosValue >1) {
		inverCosValue = 1.00;
		
	}
	float theta = acos(inverCosValue);
	if (ray.z < 0) {
		theta = (2*PI) - theta;
	}
	//printf("inverCosValue: %f \n",inverCosValue);
	//printf("acos(inverCosValue): %f \n",acos(inverCosValue));
	//cout << inverCosValue<<endl;
	
	u = theta/(2*PI);
	
	//cout << intersecting_ray_dir.x<<","<<intersecting_ray_dir.y<<","<<intersecting_ray_dir.z<<endl;
	/*float phi;
	 float theta;
	 
	 phi = acos(ray.y);
	 theta = atan2(ray.z,ray.x);
	 if (theta<0) {
	 theta = 2*PI;
	 }
	 float u, v;
	 u = theta/(2*PI);
	 v = (phi)/PI;
	 if(u>1.0 || v>1.0)
	 cout<<"u:" << u<<", v:"<<v<<endl;
	 */
	float x,y;
	int XmaxT = ENV.width;
	int YmaxT = ENV.height;
	//printf("xmax: %d , ymax: %d \n", XmaxT,YmaxT);	
	unsigned char *pixmap1 = ENV.pix;
	float redValue,greenValue,blueValue;
	int I, J;
	
	x = u*XmaxT;
	y = v*YmaxT;
	//printf("x: %f , y: %f \n", x,y);
	
	I = (int)x;
	J = (int)y;
	
	//printf("x: %f , y: %f\n",x,y);
	//printf("I: %d , J: %d\n",I,J);
	if (J>=1599) {
		J=1598;
	}
	int index =  (J * XmaxT + I) * 3;
	
	
	redValue = (pixmap1[index]);
	greenValue = (pixmap1[index+1]);
	blueValue = (pixmap1[index+2]);
	
	final.setColorRed(redValue);
	final.setColorGreen(greenValue);
	final.setColorBlue(blueValue);
	return final;
}

int winningObjectIndex(vector<double> object_intersections){
	//return the index of the winning intersection
	int index_of_minium_value;
	if (object_intersections.size() == 0) {
		//if there are no intersections
		return -1;
	}
	else if(object_intersections.size() == 1){
		if (object_intersections.at(0)>accuracy) {
			//if value of intersection is greater than zero then there was an intersection
			return 0;
		}
		else {
			//the only intersection value in negative
			return -1;
		}
	}
	else {
		//otherwise there is more than one intersection
		//first find the max value
		double max = 0;
		for (int i = 0; i<object_intersections.size(); i++) {
			if(max<object_intersections.at(i)){
				max = object_intersections.at(i);
			}
		}
		//then start from the max value to find the min positive value
		if(max>0){
			for (int i = 0; i<object_intersections.size(); i++) {
				if(object_intersections.at(i)>0 && object_intersections.at(i)<= max) {
					max = object_intersections.at(i);
					index_of_minium_value = i;
				}
			}
			return index_of_minium_value;
		}
		else {
			//all intersections were negative
			return -1;
		}
	}
}

//method functions to find the shade
Color lambert_shader(float cF, Color cA,Color cD){
	return (cA.colorScale(cF).colorAdd(cD.colorScale(1-cF)));			
}

Color phong_shader(double cF,double sF,double KS,double cosTR,Color cD,Color cA,Color cS,int sp,Vector3d p0,Vector3d ph, Vector3d r){
	if(sp == 0){ // No sharp specular circle
		return (cD.colorScale(cF).colorAdd(cA.colorScale(1-cF)).colorScale(1 - sF * KS).colorAdd(cS.colorScale(sF * KS))); 
	}
	else if(sp == 1){ 
		// sharp specular circle
		if(cosTR <= 1 && cosTR >= 0.95){
			sF = 1;
			return (cS.colorScale(cF));
		}
		else{
			sF = 0;
			// kS is the specular intensity that is controlled by pFactor. Greated the value of pFactor smaller will be specular and vice versa
			return (cD.colorScale(cF).colorAdd(cA.colorScale((1-cF)*(1-sF*KS))).colorAdd(cS.colorScale(sF*KS)));
		}
	}
	else{
		//rectangular
		//treat it lke an image plane
	    double Sx=9;
		double Sy=9;
		Vector3d vView(1,0,1);
		Vector3d vUp(1,-1,0);
		Vector3d n2;//(-1,0,0);
		n2 = vView.normalize();
		Vector3d n0 = (vUp % n2).normalize();//vView).normalize();
		Vector3d n1 = n0%n2;
		Vector3d normalVector(-1,0,0);
		normalVector = normalVector.normalize();
		//Vector3d n0=n2^Vup;
		//n0 = n0.normalize();
		//Vector3d n1=n0^n2;
		//n1 = n1.normalize();
		//p0=Point3f(15,5,0);
		Vector3d p00 = p0 - (Sx/2) * n0 - (Sy/2) * n1;
		//printf("%f %f %f \n",p00.x,p00.y,p00.z);
		Vector3d plr;
		int s;
		double t= (normalVector*(p00-ph))/(normalVector*r);
		if(t < 0.000001){
			s = 0;	
		}
		else{
			plr = ph + t*r;
			double x,y;
			x = ((plr - p00) * n0) / Sx;
			y = ((plr - p00) * n1) / Sy;
			//printf("%f %f \n",x,y );
			if((x >= 0 && x<= 1) && (y >=0 && y <= 1)){
				//printf("inside");
				s=1;
				if(sp>2){
					if((x >= 0.4 && x <= 0.5) || (y >= 0.4 && y <= 0.5)){
						s=0;
					}
					else {
						s=1;
					}
					
				}
			}
			else{
				s = 0;
				
			}
			
		}
		if(s==1){
			return (cS.colorScale(cF));
		}
		else{
			return (cD.colorScale(cF).colorAdd(cA.colorScale((1-cF)*(1-sF*KS))).colorAdd(cS.colorScale(sF*KS)));
		}
	}
	
}

Color gooch_shader(double cF,double sF,double KS,double a,double b,Color cC,Color wC,Color dC,Color cS){
	Color cCD = cC.colorAdd(dC.colorScale(a));
	Color cWD = wC.colorAdd(dC.colorScale(b));
	return (cCD.colorScale(cF).colorAdd(cWD.colorScale((1-cF)*(1-sF*KS))).colorAdd(cS.colorScale(sF*KS)));
}

Color shader(Vector3d ph,Vector3d intersecting_ray_dir,vector<Object*> scene_objects,int index_of_winning_object,vector<Source*> scene_lights,double accuracy,double ambientLight,char s){
	//cout << index_of_winning_object << endl;
	//get color of nearest object
	Color winning_object_color;
	/**shader**/
	if(shaderBool == true){
		winning_object_color = scene_objects.at(index_of_winning_object)->getColor();
	}
	/**texture**/
	else{
		winning_object_color = scene_objects.at(index_of_winning_object)->getTexColor(ph);
		
	}
	/**procedural**/
	/*if (((int)ph.x+(int)ph.y+(int)ph.z)%2==0) {
	 winning_object_color = black;
	 }
	 else {
	 winning_object_color = white;
	 }*/
	Vector3d winning_object_normal = scene_objects.at(index_of_winning_object)->getNormalAt(ph); //nl
	//printf("Normal: %f, %f, %f",winning_object_normal.x,winning_object_normal.y,winning_object_normal.z);
	/*normal mapping*/
	winning_object_normal = scene_objects.at(index_of_winning_object)->getNewNormalAt(ph);
	//printf("New Normal: %f, %f, %f \n",winning_object_normal.x,winning_object_normal.y,winning_object_normal.z);
	
	//printf("inside");
	Color final_color = black.colorScale(ambientLight);
	Color final(0,0,0);
	for (int light_index = 0; light_index < scene_lights.size(); light_index++) {
		//calculate the light-intersectionpoint ph normal
		Vector3d nlh;
		
		if (DLight* dl = dynamic_cast< DLight* >(scene_lights.at(light_index))) {
			nlh = -1*(scene_lights.at(light_index)->getLightDirection());
		}
		else{
			nlh = (scene_lights.at(light_index)->getLightPosition() - ph).normalize();
		}
		//define shadow ray for shooting
		//cout << 1 << " nlh = " << nlh.x << ", " << nlh.y << ", " << nlh.z << endl;
		Ray shadow_ray(ph,nlh);
		//cout << 2 << " nlh = " << nlh.x << ", " << nlh.y << ", " << nlh.z << endl;
		vector<double> secondary_intersections;
		
		bool shadowed = false;
		
		//check for intersecting objects
		double distance = (scene_lights.at(light_index)->getLightPosition() - ph).norm();
		
		for (int object_index = 0; object_index < scene_objects.size() && shadowed == false; object_index++){
			if (object_index != index_of_winning_object) {
				secondary_intersections.push_back(scene_objects.at(object_index)->findInsection(shadow_ray));
			}
			else{
				secondary_intersections.push_back(-1);
			}
		}
		double cos = scene_lights.at(light_index)->getCosTheta(winning_object_normal, nlh);//winning_object_normal * nlh;
		double shadowTotal = 1.0;
		//if(cos>0){
		for (int objects = 0;  objects<secondary_intersections.size(); objects++) {
			if(secondary_intersections.at(objects) > 0.00) {
				if(secondary_intersections.at(objects)<distance){
					shadowed = true;
					final_color = final_color.colorScale(1);
					if(softShadow){
						double t = secondary_intersections.at(objects);
						Vector3d phN = ph + (t-0.001)*nlh;
						Vector3d pcr = scene_objects.at(objects)->getSphereCenter();
						Vector3d nhN = (phN-pcr).normalize();//scene_objects.at(objects)->getSphereRadius();
						//if (nhN.norm() > 1) {
						//	cout << nhN.norm()<< endl;
						//}
						double shadowF = nlh*nhN;
						//shadowF is the same as cos
						//printf("%f \n", shadowF);
						shadowF = 1.0-shadowF*shadowF;//*0.5;
						shadowTotal *= shadowF;
					}
					//break; //cause we know the object is in shadow i think :/
				}
			}
		}
		///
	//	cout<<cos<<"....";

		if (ambientOcclution) {
			//check light from each point on the geo sphere
			int hit = 0;
			float cosSum = 0.0;
			for (int vert = 0; vert < verts.size(); vert++) {
				//check with normal and vert- if value is b/w 0-1
				float cosNV = acos(winning_object_normal*verts.at(vert));
				//cout << cosNV<<endl;
				if (cosNV<90.00) {
					Ray ray_ao(ph,verts.at(vert));
					vector<double> intersections_ao;
					for (int index = 0; index < scene_objects.size(); index++) {
						if (index != index_of_winning_object) {
							intersections_ao.push_back(scene_objects.at(index)->findInsection(ray_ao));
						} else {
							intersections_ao.push_back(-1.0);
						}
					}
					//to find out which of the object closest to the plane - sort like function
					int index_of_winning_object_ao = winningObjectIndex(intersections_ao);
					//cout << index_of_winning_object_ao<<endl;
					if (index_of_winning_object_ao>0) {
						hit++;
						Vector3d ph_ao = ray_ao.getRayOrigin()+ ray_ao.getRayDirection()*(intersections_ao.at(index_of_winning_object_ao));
						Vector3d intersecting_ray_dir_ao =  ray_ao.getRayDirection();
						Vector3d winning_object_normal_ao = scene_objects.at(index_of_winning_object_ao)->getNormalAt(ph_ao); //nl
						float cos_ao_final = 0.0;
						for (int light_index_ao = 0; light_index_ao < scene_lights.size(); light_index_ao++) {
							//calculate the light-intersectionpoint ph normal
							Vector3d nlh_ao;
							if (DLight* dl = dynamic_cast< DLight* >(scene_lights.at(light_index_ao))) {
								nlh_ao = -1*(scene_lights.at(light_index_ao)->getLightDirection());
							}
							else{
								nlh_ao = (scene_lights.at(light_index_ao)->getLightPosition() - ph_ao).normalize();
							}
							Ray shadow_ray_ao(ph_ao,nlh_ao);
							vector<double> secondary_intersections_ao;
							bool shadowed_ao = false;
							//check for intersecting objects
							double distance_ao = (scene_lights.at(light_index_ao)->getLightPosition() - ph_ao).norm();
							for (int object_index = 0; object_index < scene_objects.size() && shadowed == false; object_index++){
								if (object_index != index_of_winning_object_ao) {
									secondary_intersections_ao.push_back(scene_objects.at(object_index)->findInsection(shadow_ray_ao));
								}
								else{
									secondary_intersections_ao.push_back(-1);
								}
							}
							float cos_ao = 0.0;
							//cout << winning_object_normal_ao.x<<","<<winning_object_normal_ao.y<<","<<winning_object_normal_ao.z<<endl;
							cos_ao = winning_object_normal_ao*nlh_ao;//scene_lights.at(light_index_ao)->getCosTheta(winning_object_normal_ao, nlh_ao);//winning_object_normal * nlh;
							//cout<<cos_ao<<endl;
							float shadowTotal_ao = 1.0;
							for (int objects = 0;  objects<secondary_intersections_ao.size(); objects++) {
								if( secondary_intersections_ao.at(objects) > 0.00) {
									if(secondary_intersections_ao.at(objects)<distance_ao){
										shadowed_ao = true;
										shadowTotal_ao = 0.0;
									}
								}
							}
							cos_ao = cos_ao*shadowTotal_ao;
							cos_ao_final = cos_ao_final+ cos_ao;
							//cout<<cos_ao_final<<endl;
						}
						cos_ao_final = cos_ao_final/hit;
						//cout<<cos_ao_final<<endl;
						cosSum = cosSum + cos_ao_final;
						//cout<<cosSum<<endl;
					}
				}
			}
			cosSum = cosSum/verts.size();
			if (cosSum!=0.0) {
				//cout<<cosSum<<"...."<<cos<<"...";
			}
			cos = cos + cosSum;
			if (cosSum!=0.0) {
				//cout<<cos<<endl;
			}
		}
		
		
		///
		double cF = ((cos+1.0)/2.0);//*(shadowF+1))/1.5;
		final_color = winning_object_color.colorScale(cF*(shadowTotal)).colorAdd(black.colorScale((1-cF)*(1.0-shadowTotal)));
		if (final_color.getColorRed() < 0.0 || final_color.getColorRed() > 255.0) {
			cout << "final color is not OK at first point" << endl;
		}
		//}
		
		if(shadowed == false){
			//final_color = black;
			//cos angle
			double cfactor;
			double sfactor;
			
			if (s == 'l') {
				//printf("lambert");
				cfactor = (cos+1)/2;
				final_color = final_color.colorAdd(lambert_shader(cfactor,winning_object_color,black));
			}
			else if(s == 'p'){
				//printf("phong");
				cfactor = (cos+1)/2;
				Vector3d v = -1*intersecting_ray_dir;
				Vector3d r = -1*v+(2*(v*winning_object_normal)*winning_object_normal);
				double cosR = nlh*r;
				sfactor = (cosR+1)/2;
				float phongConstant; //(>=20)
				int sp;//specular 0-soft, 1-hard round, 2 - rectangular, 3 - french window
				sp=0; 
				if(sp==0){
					phongConstant = 20;
				}
				else {
					phongConstant = 100000;
				}			
				double KS = powf(sfactor,phongConstant);
				final_color =final_color.colorAdd(phong_shader(cfactor,sfactor,KS,cosR,winning_object_color,black,white,sp,scene_lights.at(light_index)->getLightPosition(),ph,r));
				Color totalColor_r(0,0,0);
				
				
				Vector3d newNpe = r;
				if(glossy){
					Vector3d rp;
					rp.x = rand();//-1;
					rp.y = rand();//-1;
					rp.z = rand();//-1;
					rp = rp.normalize();
					//cout << "rp:"<<rp.x<<","<<rp.y<<","<<rp.z<<endl;
					float alpha = (float)0.5*(rand()/RAND_MAX);
					//cout << alpha<<endl;
					newNpe = newNpe + 0.2*rp;
					newNpe = newNpe.normalize();
					//cout << "newNpe:"<<newNpe.x<<","<<newNpe.y<<","<<newNpe.z<<endl;
				}
				Vector3d newPe = ph; 
				Ray ray_r(newPe,newNpe);
				//find intersections
				vector<double> intersections_r;
				for (int index = 0; index < scene_objects.size(); index++) {
					if (index != index_of_winning_object) {
						intersections_r.push_back(scene_objects.at(index)->findInsection(ray_r));
					} else {
						intersections_r.push_back(-1.0);
					}
					//printf("%f \n",scene_objects.at(index)->findInsection(ray));
				}
				//to find out which of the object closest to the plane - sort like function
				int index_of_winning_object_r = winningObjectIndex(intersections_r);
				if (index_of_winning_object_r>=0) {
					//cout << "inside reflection";
					//determine the postion and direction of the point of intersecton
					Vector3d phr = newPe+ newNpe*(intersections_r.at(index_of_winning_object_r)-0.0001);
					Vector3d intersecting_ray_dir_r = newNpe;
					//area light - 500
					int as =5;
					for (int num = 0 ; num < as; num++) {
						char s_r = scene_objects.at(index_of_winning_object_r)->getMaterial();
						//for shading
						Color intersection_color_r =shader(phr,intersecting_ray_dir_r,scene_objects,index_of_winning_object_r,scene_lights,accuracy,ambientLight,s_r);
						totalColor_r = totalColor_r.colorAdd(intersection_color_r);
						
					}
					totalColor_r = totalColor_r.colorScale(0.5);
				}
				else if(envMapBool){
					
					totalColor_r = envMap(newNpe);
					//totalColor_r = totalColor_r.colorScale(0.5);
				}
				
				final_color = final_color.colorScale(.2).colorAdd(totalColor_r.colorScale(.7));
			}
			else if( s == 'g'){
				//printf("gooch");
				cfactor = (cos+1)/2;
				Vector3d v = -1*intersecting_ray_dir;
				Vector3d r = -1*v+(2*(v*winning_object_normal)*winning_object_normal);
				double cosR = nlh*r;			
				sfactor = (cosR+1)/2;
				double phongConstant = 10000000; //(>=20)
				double KS = powf(sfactor,phongConstant);
				Color coolColor = blue;
				Color warmColor = red;
				Color diffuseColor = beige;
				Color specularColor = white;
				double alpha = 0.0001;
				double beta = 0.0001;
				final_color = final_color.colorAdd(gooch_shader(cfactor,sfactor,KS,alpha,beta, coolColor, warmColor,diffuseColor, specularColor));
				
			}
			else if(s == 't'){
				//printf("transperent");
				cfactor = (cos+1)/2;
				Vector3d v = -1*intersecting_ray_dir;
				Vector3d r = -1*v+(2*(v*winning_object_normal)*winning_object_normal);
				double cosR = nlh*r;
				sfactor = (cosR+1)/2;
				float phongConstant; //(>=20)
				int sp;//specular 0-soft, 1-hard round, 2 - rectangular, 3 - french window
				sp=0; 
				if(sp==0){
					phongConstant = 20;
				}
				else {
					phongConstant = 100000;
				}			
				double KS = powf(sfactor,phongConstant);
				final_color =final_color.colorAdd(phong_shader(cfactor,sfactor,KS,cosR,winning_object_color,black,white,sp,scene_lights.at(light_index)->getLightPosition(),ph,r));
				Color totalColor_t(0,0,0);
				//
				Vector3d t2 = ORIGIN;
				Vector3d t1 = ORIGIN;
				RefReturn refreturn;
				float refIndex = scene_objects.at(index_of_winning_object)->getRefractiveIndex();
				
				t1 = getTransmittedRay(intersecting_ray_dir,scene_objects.at(index_of_winning_object)->getNewNormalAt(ph),refIndex);
					//ray has been transmitted
					//cout <<"newT1"<< t1.x<<","<<t1.y<<","<<t1.z<<endl;
					refreturn.pe = ph;
					refreturn.npe = t1;
					
				/*if(translucency){
					Vector3d rp;
					rp.x = rand();//-1;
					rp.y = rand();//-1;
					rp.z = rand();//-1;
					rp = rp.normalize();
					//cout << "rp:"<<rp.x<<","<<rp.y<<","<<rp.z<<endl;
					float alpha = (float)(rand()/RAND_MAX);
					//cout << alpha<<endl;
					t1 = t1 + alpha*rp;
					t1 = t1.normalize();
					refreturn.npe = t1;
					//cout << "newNpe:"<<newNpe.x<<","<<newNpe.y<<","<<newNpe.z<<endl;
				}*/
				
					//set new ray
					Ray newPoint(ph,t1);
					Ray newRef(ph,t1);
					//find the 't' in the object for the new transmitted ray
					float newT = scene_objects.at(index_of_winning_object)->findInsection(newPoint);
					if (newT<0) {
						
						//cout << "tr"<<endl;
						vector<double> intersections_t;
						for (int index = 0; index < scene_objects.size(); index++) {
							if (index != index_of_winning_object) {
								intersections_t.push_back(scene_objects.at(index)->findInsection(newRef));
							} else {
								intersections_t.push_back(-1.0);
							}
							//printf("%f \n",scene_objects.at(index)->findInsection(ray));
						}
						//to find out which of the object closest to the plane - sort like function
						int index_of_winning_object_t = winningObjectIndex(intersections_t);
						if (index_of_winning_object_t>=0) {
							//cout << "inside reflection";
							//determine the postion and direction of the point of intersecton
							Vector3d pht = newRef.getRayOrigin()+ newRef.getRayDirection()*(intersections_t.at(index_of_winning_object_t));
							Vector3d intersecting_ray_dir_t =  newRef.getRayDirection();
							int as =5;
							for (int num = 0 ; num < as; num++) {
								char s_t = scene_objects.at(index_of_winning_object_t)->getMaterial();
								//for shading
								Color intersection_color_t =shader(pht,intersecting_ray_dir_t,scene_objects,index_of_winning_object_t,scene_lights,accuracy,ambientLight,s_t);
								totalColor_t = totalColor_t.colorAdd(intersection_color_t);
								
							}
							totalColor_t = totalColor_t.colorScale(1);
						}
						else {
							if(envMapBool){
								//cout << "ev"<<cout;
								totalColor_t = envMap(newRef.getRayDirection());
								//totalColor_r = totalColor_r.colorScale(0.5);
							}
						}
						final_color = final_color.colorScale(0).colorAdd(totalColor_t.colorScale(1.0));
	
					}
					else{
						//ray has a hit point- ph2
						Vector3d newPhT = newPoint.getRayOrigin()+ newPoint.getRayDirection()*(newT+0.1);
						Vector3d newNhT = scene_objects.at(index_of_winning_object)->getNormalAt(newPhT);
						//cout <<"ph"<< newPhT.x<<","<<newPhT.y<<","<<newPhT.z<<endl;
						//cout <<"no"<< newNhT.x<<","<<newNhT.y<<","<<newNhT.z<<endl;
						
						//get transmitted ray 2
						t2 = getTransmittedRay(t1,newNhT,1/refIndex);
						
						if(translucency){
							Vector3d rp;
							rp.x = rand();//-1;
							rp.y = rand();//-1;
							rp.z = rand();//-1;
							rp = rp.normalize();
							//cout << "rp:"<<rp.x<<","<<rp.y<<","<<rp.z<<endl;
							float alpha = (float)(rand()/RAND_MAX);
							//cout << alpha<<endl;
							t2 = t2 + 0.2*rp;
							t2 = t2.normalize();
							//cout << "newNpe:"<<newNpe.x<<","<<newNpe.y<<","<<newNpe.z<<endl;
						}
						
							Ray ray_t(newPhT,t2);
							//find intersections
							vector<double> intersections_t;
							for (int index = 0; index < scene_objects.size(); index++) {
								if (index != index_of_winning_object) {
									intersections_t.push_back(scene_objects.at(index)->findInsection(ray_t));
								} else {
									intersections_t.push_back(-1.0);
								}
								//printf("%f \n",scene_objects.at(index)->findInsection(ray));
							}
							//to find out which of the object closest to the plane - sort like function
							int index_of_winning_object_t = winningObjectIndex(intersections_t);
							if (index_of_winning_object_t>=0) {
								//cout << "inside reflection";
								//determine the postion and direction of the point of intersecton
								Vector3d pht = ray_t.getRayOrigin()+ ray_t.getRayDirection()*(intersections_t.at(index_of_winning_object_t));
								Vector3d intersecting_ray_dir_t =  ray_t.getRayDirection();
								int as =5;
								for (int num = 0 ; num < as; num++) {
									char s_t = scene_objects.at(index_of_winning_object_t)->getMaterial();
									//for shading
									Color intersection_color_t =shader(pht,intersecting_ray_dir_t,scene_objects,index_of_winning_object_t,scene_lights,accuracy,ambientLight,s_t);
									totalColor_t = totalColor_t.colorAdd(intersection_color_t);
									
								}
								totalColor_t = totalColor_t.colorScale(1);
							}
							
							
							else {
								if(envMapBool){
									totalColor_t = envMap(ray_t.getRayDirection());
									//totalColor_r = totalColor_r.colorScale(0.5);
								}
							}
						final_color = final_color.colorScale(0).colorAdd(totalColor_t.colorScale(1.0));
							
						}
				
			}
			if(irr){
				Color irrColor = irdecence(cos);
				final_color = final_color.colorAdd(irrColor).colorScale(0.5);
			}
			
		}
		
		final = final.colorAdd(final_color).colorScale(.5);	
	///
		/*
		
		if (ambientOcclution) {
			//check light from each point on the geo sphere
			int hit = 0;
			float cosSum = 0.0;
			for (int vert = 0; vert < verts.size(); vert++) {
				//check with normal and vert- if value is b/w 0-1
				float cosNV = winning_object_normal*verts.at(vert);
				//cout << cosNV<<endl;
				if (cosNV>=0.0 && cosNV<=1.00) {
					Ray ray_ao(ph,verts.at(vert));
					vector<double> intersections_ao;
					for (int index = 0; index < scene_objects.size(); index++) {
						if (index != index_of_winning_object) {
							intersections_ao.push_back(scene_objects.at(index)->findInsection(ray_ao));
						} else {
							intersections_ao.push_back(-1.0);
						}
					}
					//to find out which of the object closest to the plane - sort like function
					int index_of_winning_object_ao = winningObjectIndex(intersections_ao);
					if (index_of_winning_object_ao>=0) {
						hit++;
						Vector3d ph_ao = ray_ao.getRayOrigin()+ ray_ao.getRayDirection()*(intersections_ao.at(index_of_winning_object_ao));
						Vector3d intersecting_ray_dir_ao =  ray_ao.getRayDirection();
						float cos_ao_final = 0.0;
						for (int light_index_ao = 0; light_index_ao < scene_lights.size(); light_index_ao++) {
							//calculate the light-intersectionpoint ph normal
							Vector3d nlh_ao;
							if (DLight* dl = dynamic_cast< DLight* >(scene_lights.at(light_index_ao))) {
								nlh_ao = -1*(scene_lights.at(light_index_ao)->getLightDirection());
							}
							else{
								nlh_ao = (scene_lights.at(light_index_ao)->getLightPosition() - ph_ao).normalize();
							}
							Ray shadow_ray_ao(ph_ao,nlh_ao);
							vector<double> secondary_intersections_ao;
							bool shadowed_ao = false;
							//check for intersecting objects
							double distance_ao = (scene_lights.at(light_index_ao)->getLightPosition() - ph_ao).norm();
							for (int object_index = 0; object_index < scene_objects.size() && shadowed == false; object_index++){
								if (object_index != index_of_winning_object) {
									secondary_intersections.push_back(scene_objects.at(object_index)->findInsection(shadow_ray));
								}
								else{
									secondary_intersections.push_back(-1);
								}
							}
							double cos_ao = cos_ao+scene_lights.at(light_index_ao)->getCosTheta(winning_object_normal, nlh_ao);//winning_object_normal * nlh;
							double shadowTotal_ao = 1.0;
							for (int objects = 0;  objects<secondary_intersections.size(); objects++) {
								if(secondary_intersections.at(objects) > 0.00) {
									if(secondary_intersections.at(objects)<distance_ao){
										shadowed_ao = true;
										shadowTotal_ao = 0.0;
									}
								}
							}
							cos_ao = cos_ao*shadowTotal_ao;
							cos_ao_final = cos_ao_final+ cos_ao;
						}
						cos_ao_final = cos_ao_final/scene_lights.size();
						cosSum = cosSum + cos_ao_final;
					}
				}
			}
			cosSum = cosSum/verts.size();
			final = final.colorScale(cosSum);
		}
	*/
		 ///
	
	}
	return final;
}


PixMapValues intersection(){
	
	char s;// = 'p';//l,p,g
	//vector for objects in the scene
	Vector3d pp;
	Vector3d pc;
	Vector3d npe;
	Vector3d n0;
	Vector3d n1;
	Vector3d n2;
	
	Vector3d nline(0,0,0);
	float s0 = 600;
	Vector3d entryp(10,10,2);
	float lengthLinear;
	
	n2 = vView.normalize();
	n0 = (vUp % vView).normalize();
	n1 = (n0%n2).normalize();
	
	//push objects in the scene in a vector array
	vector<Object*> scene_objects;
	scene_objects.push_back(dynamic_cast<Object*> (&scene_sphere1));
	scene_objects.push_back(dynamic_cast<Object*> (&scene_sphere2));
	//scene_objects.push_back(dynamic_cast<Object*> (&scene_sphere3));
	//scene_objects.push_back(dynamic_cast<Object*> (&scene_sphere4));
	//scene_objects.push_back(dynamic_cast<Object*> (&scene_sphere5));
	//scene_objects.push_back(dynamic_cast<Object*> (&scene_sphere6));
	//scene_objects.push_back(dynamic_cast<Object*> (&scene_plane));
	//scene_objects.push_back(dynamic_cast<Object*> (&plane1));
	//scene_objects.push_back(dynamic_cast<Object*> (&plane2));
	//scene_objects.push_back(dynamic_cast<Object*> (&plane3));
	
	//scene_objects.push_back(dynamic_cast<Object*> (&cylinder));
	//scene_objects.push_back(dynamic_cast<Object*> (&scene_plane2));
	
	//Object
	
	/* vector<TriangleVert> objectTri = oR.getTriangles();
	 vector<TriangleVert> objectTex = oR.getTriangleTexture();
	 vector<Triangle> triArray;
	 for (int i = 0; i<objectTri.size(); i++) {
	 TriangleVert tv(objectTri.at(i));
	 TriangleVert tt(objectTex.at(i));
	 Triangle tri(tv.vet1,tv.vet2,tv.vet3,green,phong,"",ProjectonTex,tt.vet1,tt.vet2,tt.vet3,1.0);
	 triArray.push_back(tri);
	 }
	 for (int i = 0; i<triArray.size(); i++) {
	 scene_objects.push_back(dynamic_cast<Object*> (&triArray.at(i)));
	 }
	 printf("sceneobj size %lu",scene_objects.size());*/
	 
	//push lights in the scene in a vector array
	vector<Source*> scene_lights;
	scene_lights.push_back(dynamic_cast<Source*> (&scene_light1));
	//scene_lights.push_back(dynamic_cast<Source*> (&scene_light2));
	//scene_lights.push_back(dynamic_cast<Source*> (&scene_Dlight));
	//scene_lights.push_back(dynamic_cast<Source*> (&scene_Slight));
	//scene_lights.push_back(dynamic_cast<Source*> (&scene_Alight));
	
	
	if (rayDepth) {
		sx = sx*d;
		sy = sy*d;
		//pe = pe+(2- ((rand()/RAND_MAX) * 4)*n0)+(2- ((rand()/RAND_MAX) * 4)*n1);
	}
	
	//Camera scene_camera(pe,vView,vUp,d);
	pc = pe + d*n2;
	Vector3d p00 = pc - (n0*sx/2) - (n1*sy/2);
	//scene_camera.getImagePlaneCenter() - scene_camera.getCameran0()*(sx/2) - scene_camera.getCameran1()*(sy);//pc -n0*(sx/2) - n1*(sy/2);
	/*
	 string fileName ="marble.ppm";
	 string newFileName ="img1_new.ppm";
	 FILE *readFile;
	 readFile = fopen(fileName.c_str(), "r");
	 ImageReader ir;
	 
	 PixMapValues result =ir.readPPM(readFile);
	 
	 */
	Vector3d temp = pe;
	for(int y=0; y<Ymax; y++){
		for (int x = 0; x < Xmax; x++) {
			int k =  (y * Xmax + x) * 3; 
			if (rayDepth) {
				pe = temp;
				//cout << "pe:"<<pe.x<<","<<pe.y<<","<<pe.z<<endl;
			}
			if (cubist) {
				//cout << "inside - pe:"<<pe.x<<","<<pe.y<<","<<pe.z<<endl;
				pe = temp;
				unsigned char *pixmapCube;
				pixmapCube = CUBISM.pix;
				float r, g, b;
				r = (float)pixmapCube[k];
				r = r/255.0;
				g = (float)pixmapCube[k+1];
				g= g/255.0;
				b = (float)pixmapCube[k+2];
				b = b/255.0;
				
				pe.x = pe.x+(cb*(pixmapCube[k]/255.00));
				pe.y = pe.y+(cb*(pixmapCube[k+1]/255.00));
				pe.z = pe.z+(cb*(pixmapCube[k+2]/255.00));
				//cout << "r:"<<r<<",g:"<<g<<",b:"<<b<<endl;
							
			}
			
		/*	if (rayDepth) {
				//cout << "inside - pe:"<<pe.x<<","<<pe.y<<","<<pe.z<<endl;
				pe = temp;
				pe.x = pe.x+1.0 * rand()/RAND_MAX;;
				pe.y = pe.y+1.0 * rand()/RAND_MAX;;
				pe.z = pe.z+1.0 * rand()/RAND_MAX;;
			//	cout << "inside - pe:"<<pe.x<<","<<pe.y<<","<<pe.z<<endl;
				float d1 = .001;
				sx = (d1/d)*sx;
				//sy = (d1/d)*sy;
			}*/
			
			Vector3d totalColor(0,0,0);
			float randX = 1.0 * rand()/RAND_MAX;
			float randY = 1.0 * rand()/RAND_MAX;
			float samples = 10;
			XX= x;
			YY = y;
			for (int num = 0 ; num < (samples); num++) {
				float X = x + (num/samples) + (randX/samples);
				float Y = y + (num/samples) + (randY/samples);
				
				float i = X/Xmax;
				float j = Y/Ymax;

				pp = p00 + n0 * sx * i + n1 * sy * j;
				
				if (rayDepth) {
					pe = temp;
					float randdd = (2.0-(4.0*rand()/RAND_MAX));
					//cout <<randdd <<endl;
					pe = pe+((0.5-(1.0*rand()/RAND_MAX))*n0)+((0.5-(1.0*rand()/RAND_MAX))*n1);
				}
				npe = (pp - pe).normalize();
				/*
				if(rayDepth){
					float sxD = sx*d;
					float syD = sy*d;
					pp = p00 + n0 * sxD * i + n1 * syD * j;
					npe = (pp - (pe+(2- ((rand()/RAND_MAX) * 4)*n0)+(2- ((rand()/RAND_MAX) * 4)*n1))).normalize();
				}*/
				
				/*//motion blur
				Vector3d sc1(mb+(num*0.1),2.2,13);
				Sphere scene_sphere1(sc1,r1,yellow,"","",lambert,1.0);
				scene_objects.push_back(dynamic_cast<Object*> (&scene_sphere1));
				*/
				
				/*Vector3d pp = p00+(x*sx*scene_camera.getCameran0())+(y*sy*scene_camera.getCameran1());
				 Vector3d npe = (pp - scene_camera.getCameraPosition()).normalize();*/
				Ray ray(pe,npe);
				
				//find intersections
				vector<double> intersections;
				for (int index = 0; index < scene_objects.size(); index++) {
					intersections.push_back(scene_objects.at(index)->findInsection(ray));
					//printf("%f \n",scene_objects.at(index)->findInsection(ray));
				}
				
				//to find out which of the object closest to the plane - sort like function
				int index_of_winning_object = winningObjectIndex(intersections);
				if (index_of_winning_object>=0) {
					//determine the postion and direction of the point of intersecton
					Vector3d ph = pe+ npe*(intersections.at(index_of_winning_object)-0.2);
					Vector3d intersecting_ray_dir = npe;
					//area light - 500
					int as =2;
					for (int num = 0 ; num < as; num++) {
					s = scene_objects.at(index_of_winning_object)->getMaterial();
					//for shading
					Color intersection_color =shader(ph,intersecting_ray_dir,scene_objects,index_of_winning_object,scene_lights,accuracy,ambientLight,s);
					totalColor.set(totalColor.x + intersection_color.getColorRed(),totalColor.y + intersection_color.getColorGreen(),totalColor.z + intersection_color.getColorBlue());
					}
				
					totalColor.x = totalColor.x/(as)*1.8;
					totalColor.y = totalColor.y/(as)*1.8;
					totalColor.z = totalColor.z/(as)*1.8;					
				}
				else if(envMapBool){
					Color intersection_color = envMap(npe);
					totalColor.set(totalColor.x + intersection_color.getColorRed(),totalColor.y + intersection_color.getColorGreen(),totalColor.z + intersection_color.getColorBlue());
					
				}
				//motion blur
				//scene_objects.erase(scene_objects.begin()+scene_objects.size()-1);
			}
			//else {
			float c1 = totalColor.x/samples;
			float c2 = totalColor.y/samples;
			float c3 = totalColor.z/samples;
			if ((c1)>255) {
				c1 = 255;
			}
			if (c2>255) {
				c2 = 255;
			}
			if (c3>255) {
				c3 = 255;
			}
			pixmap[k++] = c1;
			pixmap[k++] = c2;
			pixmap[k] = c3;
			
			//cout << k<<endl;
			//}
			//}
		}
	}
	PixMapValues pixValue;
	pixValue.pix = pixmap;
	pixValue.width = Xmax;
	pixValue.height = Ymax;
	return pixValue;
}

//////
static void windowResize(int w, int h)
{   
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0,(w/2),0,(h/2),0,1); 
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity() ;
}
static void windowDisplay(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glRasterPos2i(0,0);
	glPixelStorei(GL_UNPACK_ALIGNMENT,1);
	glDrawPixels(width, height, GL_RGB, GL_UNSIGNED_BYTE, pixmap);
	glFlush();
}
static void processMouse(int button, int state, int x, int y)
{
	if(state == GLUT_UP)
		exit(0);               // Exit on mouse click.
}
static void init(void)
{
	glClearColor(1,1,1,1); // Set background color.
}


///////

///////

int main(int argc, char *argv[]){
	//printf("main");
	
	width  = Xmax;
	height = Ymax;
	pixmap = new unsigned char[width*height*3];
	/*string fileName ="light.ppm";//weavenormal.ppm";
	 //string newFileName ="img1_new.ppm";
	 FILE *readFile;
	 readFile = fopen(fileName.c_str(), "r");
	 ImageReader ir;
	 
	 ProjectonTex =ir.readPPM(readFile);*/
	
	//env map
	if(envMapBool){
		string fileName ="hdr3.ppm";
		FILE *readFile;
		readFile = fopen(fileName.c_str(), "r");
		ImageReader ir;
		ENV =ir.readPPM(readFile);
	}
	
	if(cubist){
		cout<<"reading cubist painting value"<<endl;
		string fileName ="wp.ppm";
		FILE *readFile;
		readFile = fopen(fileName.c_str(), "r");
		ImageReader ir;
		CUBISM =ir.readPPM(readFile);
	}
	
	if(ambientOcclution){
		cout<<"reading geodesic sphere"<<endl;
		ObjectReader oR("012.obj");
		verts = oR.getVerts();
		/*for (int i = 0; i<verts.size(); i++){
			cout << verts.at(i).x<<","<<verts.at(i).y<<","<<verts.at(i).z<<endl;
		}*/
	}
	
	intersection();
	
	/*FILE *writeFile;
	 writeFile= fopen(newFileName.c_str(),"w");
	 
	 ir.writePPM(writeFile);
	 */
	/*
	for (int i = 0; i <= 96; i++)
	{
		string name;
		FILE *writeFile;
		ImageReader ir;
		
		cout <<"inside";
		std::stringstream sstm;
		sstm << "proj6_" << i << ".ppm";
		name = (sstm.str()).c_str();
		
		writeFile = fopen(name.c_str(), "w");
		
		PixMapValues pixValue = intersection();
		
		ir.writePPM(writeFile,pixmap,Xmax,Ymax);
		
		//motion blur
		//mb=mb+0.2;
		
		//cubist
		cb = cb+0.1;
	}
 */
	//cout << "n: "<<n<<endl;
	glutInit(&argc, argv);
	glutInitWindowPosition(100, 100); // Where the window will display on-screen.
	glutInitWindowSize(width, height);
	glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
	glutCreateWindow("Homework One");
	init();
	glutReshapeFunc(windowResize);
	glutDisplayFunc(windowDisplay);
	glutMouseFunc(processMouse);
	glutMainLoop();
	return 0;
	
}

