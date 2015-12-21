/*
 *  Shader.h
 *  
 *
 *  Created by Chethna Kabeerdoss on 12/04/14.
 *  Copyright 2014 __MyCompanyName__. All rights reserved.
 *
 */

//colors
static Color red(0x69,0x18,0x24);
static Color blue(0x30,0x4C,0xC9);
static Color beige(0xCF,0xBB,0xAA);
static Color black(0x00,0x00,0x00);
static Color white(0xff,0xff,0xff);
static Color green(0x40,0x6E,0x4C);
static Color yellow(0xF0,0xF0,0x7D);
static Color grey(0x80,0x7E,0x7E);

static bool softShadow =true;
static bool irr =true;
static bool shaderBool = false;

static PixMapValues ProjectonTex;
static PixMapValues ENV;
static float XX, YY;

static double accuracy = 0.000001;



//winning object 
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
static Color lambert_shader(float cF, Color cA,Color cD){
	return (cA.colorScale(cF).colorAdd(cD.colorScale(1-cF)));			
}

static Color phong_shader(double cF,double sF,double KS,double cosTR,Color cD,Color cA,Color cS,int sp,Vector3d p0,Vector3d ph, Vector3d r){
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

static Color gooch_shader(double cF,double sF,double KS,double a,double b,Color cC,Color wC,Color dC,Color cS){
	Color cCD = cC.colorAdd(dC.colorScale(a));
	Color cWD = wC.colorAdd(dC.colorScale(b));
	return (cCD.colorScale(cF).colorAdd(cWD.colorScale((1-cF)*(1-sF*KS))).colorAdd(cS.colorScale(sF*KS)));
}

static Color shader(Vector3d ph,Vector3d intersecting_ray_dir,vector<Object*> scene_objects,int index_of_winning_object,vector<Source*> scene_lights,double accuracy,double ambientLight,char s){
	//cout << index_of_winning_object << endl;
	//get color of nearest object
	Color winning_object_color;
	Vector3d winning_object_normal = scene_objects.at(index_of_winning_object)->getNormalAt(ph); //nl

	/**shader**/
	float refIndex = scene_objects.at(index_of_winning_object)->getRefractiveIndex();
	if (refIndex!=1.0) {
		Vector3d v = -1*intersecting_ray_dir;
		Vector3d n = winning_object_normal;
		float C = v*n;
		float eta = refIndex;
		float bA = C/eta;
		float bB = (C*C)-1;
		float bC = (bB/(eta*eta))+1;
		float b = bA - sqrt(bC);
		float a = -1*(1/eta);
		Vector3d t1 = (a*v)+(b*n);
		cout << t.x<<","<<t.y<<","<<t.z<<endl;
	}
	
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
				else {
					
					totalColor_r = envMap(newNpe,ENV);
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
			if(irr){
				Color irrColor = irdecence(cos,XX,YY);
				final_color = final_color.colorAdd(irrColor).colorScale(0.5);
			}
			
		}
		
		final = final.colorAdd(final_color).colorScale(.5);	
	}
	return final;
}
