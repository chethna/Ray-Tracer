/*
 *  ObjectReader.h
 *  
 *
 *  Created by Chethna Kabeerdoss on 26/03/14.
 *  Copyright 2014 __MyCompanyName__. All rights reserved.
 *
 */
#include <cstdlib>
#include <iostream>
#include <GLUT/glut.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <fstream>
#include "Vector.cpp"


class TriangleVert {
public: 
	Vector3d vet1;
	Vector3d vet2;
	Vector3d vet3;

	TriangleVert(Vector3d vet1Value,Vector3d vet2Value,Vector3d vet3Value)
	{
		vet1 = vet1Value;
		vet2 = vet2Value;
		vet3 = vet3Value;
	}
};

class face{
public:
	Vector3d vertice, normal, textcoord;
	face (Vector3d Vert, Vector3d Vnormal, Vector3d texturecoordinate){
		vertice = Vert;
		normal = Vnormal;
		textcoord=texturecoordinate;
	}
	face (void){
		vertice.set(0,0,0);
		normal.set(0,0,0);
		textcoord.set(0,0,0);
	}
};


class ObjectReader {
public:
	
	vector <face> faces;
	vector <Vector3d> vertices, texturecoord, normals, rotateV;
	
	 ObjectReader (string file){
		 //cout << "inside"<<endl;
		Vector3d vertice, normal, textcoord, v, t, n;
		ifstream OBJfile;
		string s;
		face f;
		char f1[15], f2[15], f3[15], f4[15], f5[15], f6[15], f7[15], f8[15], f9[15];
		
		
		OBJfile.open(file);
		
		if (!OBJfile)
			cout<<"Cannot open file";
		
		while(!OBJfile.eof()){
			//cout << "reading file"<<endl;
			OBJfile>>s;
			if (s == "v"){
				OBJfile>>vertice[0]>>vertice[1]>>vertice[2];
				vertices.push_back(vertice);
				/*
				 float x,y,z;
				 OBJfile >> x >> y >> z; // if you want to scale or translate, x = scale * x + translate.x, etc
				 Vector3d* vertex = new Vector3d(x, y, z);
				 vertices.push_back(vertex); // vertices is currently vector<Vector3d>, needs to change to vector<Vector3d*>
				 // for this to work
				 */
			}
			if (s == "vn"){
				OBJfile>>normal[0]>>normal[1]>>normal[2];
				normals.push_back(normal);
			}		
			if (s == "vt"){
				OBJfile>>textcoord[0]>>textcoord[1];//>>textcoord[2];
				//textcoord[2] = 0.0;
				texturecoord.push_back(textcoord);
			}
			if (s == "f"){
				OBJfile.getline(f1,15,'/');
				OBJfile.getline(f2, 15, '/');
				OBJfile.getline(f3, 15, ' ');
				v[0]=atoi(f1);
				t[0]=atoi(f2);
				n[0]=atoi(f3);
				OBJfile.getline(f4, 15, '/');
				OBJfile.getline(f5, 15, '/');
				OBJfile.getline(f6, 15, ' ');
				v[1]=atoi(f4);
				t[1]=atoi(f5);
				n[1]=atoi(f6);
				OBJfile.getline(f7, 15, '/');
				OBJfile.getline(f8, 15, '/');
				OBJfile>>f9;
				v[2]=atoi(f7);
				t[2]=atoi(f8);
				n[2]=atoi(f9);
				f.vertice=v;
				f.normal=n;
				f.textcoord=t;
				faces.push_back(f);
				//printf("% f %f %f", v[0],v[1],v[2]);
				// can create triangles here
				//triangle* t = new triangle();
				//t->p0 = vertices[v[0]]; // will be valid now that vertices is vector<Vector3d*>
				// same for normals, tex coords
				
			}
			s=" ";
		}
		//cout<<"close"<<endl;
		OBJfile.close();
	}

public:
	 vector<TriangleVert> getTriangles(){
		vector <TriangleVert> triangleVerts;
		for (int i =0; i<faces.size(); i++) {
			Vector3d ver =  faces.at(i).vertice;
			Vector3d vert1, vert2, vert3;
			vert1 = vertices[ver.x-1];
			vert2 = vertices[ver.y-1];
			vert3 = vertices[ver.z-1];
			TriangleVert tv(vert1,vert2,vert3);
			triangleVerts.push_back(tv);
		}
		return triangleVerts;
	}
public:
	vector<TriangleVert> getTriangleTexture(){
		vector <TriangleVert> triangleTexts;
		for (int i =0; i<faces.size(); i++) {
			Vector3d ver =  faces.at(i).textcoord;
			Vector3d vert1, vert2, vert3;
			vert1 = vertices[ver.x-1];
			vert2 = vertices[ver.y-1];
			vert3 = vertices[ver.z-1];
			TriangleVert tt(vert1,vert2,vert3);
			triangleTexts.push_back(tt);
		}
		return triangleTexts;
	}
	
public:
	vector<Vector3d> getVerts(){
		return vertices;
	}
	
};
