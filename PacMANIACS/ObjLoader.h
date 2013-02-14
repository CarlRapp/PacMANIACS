#ifndef OBJLOADER_H
#define OBJLOADER_H
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "Buffer.h"
#include "Shader.h"
using namespace std;

//	Define here what the string values
//	for each token
const	string	TOKEN_MTL_GROUP			= "mtllib";
const	string	TOKEN_MTL_USE			= "usemtl";

const	string	TOKEN_VERTEX_GROUP		= "g";
const	string	TOKEN_VERTEX_POSITION	= "v";
const	string	TOKEN_VERTEX_NORMAL		= "vn";
const	string	TOKEN_VERTEX_TEXPOS		= "vt";

const	string	TOKEN_FACE_DATA			= "f";

enum ObjToken
{
	MATERIAL_GROUP, 
	MATERIAL_USE, 

	VERTEX_GROUP,
	VERTEX_POSITION,
	VERTEX_NORMAL,
	VERTEX_TEXPOS,

	FACE_DATA,

	UNDEFINED
};

//	Struct to easily save the 
//	data for a position
struct Position
{
	float x, y, z;

	Position() {};
	Position(float X, float Y, float Z)
	{
		x = X;
		y = Y;
		z = Z;
	}
};
struct Vertex
{
	D3DXVECTOR3 vPosition;
	D3DXVECTOR3 vNormal;
	D3DXVECTOR2 vTexCoord;
	

	Vertex() {};
	Vertex(Position pos, Position tex, Position normal)
	{
		vPosition	=	D3DXVECTOR3(pos.x, pos.y, pos.z);
		vNormal		=	D3DXVECTOR3(normal.x, normal.y, normal.z);
		vTexCoord	=	D3DXVECTOR2(tex.x, 1 - tex.y);
	}

};

class ObjLoader
{
	ID3D11DeviceContext*	mDeviceContext;

	vector<Position>	vertexPositions;
	vector<Position>	vertexTexturePos;
	vector<Position>	vertexNormals;
	vector<Vertex>		objVertices;

	ObjToken GetToken(string ObjLine);
	Position ConvertToPosition(string Line, ObjToken Token);
	void GetVertices(string Line, vector<Position> &Positions, vector<Position> &TexCoords, vector<Position> &Normals, vector<Vertex> &Vertices);

public:
	ObjLoader(void);
	ObjLoader(string ObjFile);
	~ObjLoader(void);

	vector<Vertex>* GetVertices();
};
#endif