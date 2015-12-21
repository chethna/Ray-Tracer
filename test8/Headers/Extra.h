/*
 *  Extra.h
 *  
 *
 *  Created by Chethna Kabeerdoss on 12/04/14.
 *  Copyright 2014 __MyCompanyName__. All rights reserved.
 *
 */

//method function to find the nearest object
static Color irdecence(float cosTheta,float XX,float YY){
	
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

static Color envMap(Vector3d intersecting_ray_dir,PixMapValues ENV){
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
		u = theta/(2*PI);
	
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
