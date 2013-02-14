#include "stdafx.h"
#include "ObjLoader.h"


ObjToken ObjLoader::GetToken(string ObjLine)
{
	string tokenStr;

	//	Check what the first section of
	//	the string is. (What token it is)
	for(int i = 0; i < ObjLine.length(); ++i)
		if(ObjLine[i] != ' ')
			tokenStr += ObjLine[i];
		else
			break;

	//	Convert the token to
	//	the correct enum.
	if(tokenStr == TOKEN_MTL_GROUP)
		return MATERIAL_GROUP;

	if(tokenStr == TOKEN_MTL_USE)
		return MATERIAL_USE;

	if(tokenStr == TOKEN_VERTEX_GROUP)
		return VERTEX_GROUP;

	if(tokenStr == TOKEN_VERTEX_POSITION)
		return VERTEX_POSITION;

	if(tokenStr == TOKEN_VERTEX_NORMAL)
		return VERTEX_NORMAL;

	if(tokenStr == TOKEN_VERTEX_TEXPOS)
		return VERTEX_TEXPOS;

	if(tokenStr == TOKEN_FACE_DATA)
		return FACE_DATA;

	//	If there is a undefined token
	//	pass this. (Usually comments/blank rows)
	return UNDEFINED;
}

Position ObjLoader::ConvertToPosition(string Line, ObjToken Token)
{
	float x, y, z;
	x = y = z = 0;

	//	Index for getting the floats 
	int i = 0;
	int nMax = 0;
	string tString;

	switch(Token)
	{
		case VERTEX_POSITION:
			i = TOKEN_VERTEX_POSITION.length();
			nMax = 3;
			break;

		case VERTEX_TEXPOS:
			i = TOKEN_VERTEX_TEXPOS.length();
			nMax = 2;
			break;

		case VERTEX_NORMAL:
			i = TOKEN_VERTEX_NORMAL.length();
			nMax = 3;
			break;
	}
	i++;

	for(int n = 0; n < nMax; ++n)
	{
		tString = "";
		while(true)
		{
			if(i >= Line.length())
				break;

			if(Line[i] == ' ')
			{
				++i;
				break;
			}

			tString += Line[i];
			++i;
		}

		if(n == 0)
			x = (float)(atof(tString.c_str()));
		if(n == 1)
			y = (float)(atof(tString.c_str()));
		if(n == 2)
			z = (float)(atof(tString.c_str()));
	}



	return Position(x, y, z);
}

void ObjLoader::GetVertices(string Line, vector<Position> &Positions, vector<Position> &TexCoords, vector<Position> &Normals, vector<Vertex> &Vertices)
{
	int i = TOKEN_FACE_DATA.length() + 1;

	string	tempData;

	int**	vData			=	new int*[3];
	int		indexDivider	=	0;

	//	This will divide the string up into
	//	three sub sections, each with its
	//	own vertex data
	for(int vertexIndex = 0; vertexIndex < 3; ++vertexIndex)
	{
		vData[vertexIndex]	=	new int[3];

		while(true)
		{
			if(i >= Line.length())
			{
				vData[vertexIndex][indexDivider]	=	stoi(tempData);
				break;
			}

			if(Line[i] == ' ')
			{
				vData[vertexIndex][indexDivider]	=	stoi(tempData);
				break;
			}

			if(Line[i] == '/')
			{
				vData[vertexIndex][indexDivider]	=	stoi(tempData);
				++indexDivider;
				tempData = "";
			}
			else
				tempData	+=	Line[i];

			++i;
		}

		++i;
		indexDivider = 0;
		tempData = "";
	}

	
	for(int i = 0; i < 3; ++i)
	{
		Vertices.push_back(
			Vertex(
				Positions[vData[i][0] - 1], 
				TexCoords[vData[i][1] - 1], 
				Normals[vData[i][2] - 1]
			)
		);
	}
}

ObjLoader::ObjLoader(string ObjFile)
{
	vertexPositions		=	vector<Position>();
	vertexTexturePos	=	vector<Position>();
	vertexNormals		=	vector<Position>();
	objVertices			=	vector<Vertex>();


	ifstream myfile;
	myfile.open (ObjFile);
	string sLine;
	ObjToken Token;

	while (!myfile.eof())
	{
		getline(myfile, sLine);

		if(sLine.length() > 0)
		{
			Token = GetToken(sLine);
			switch(Token)
			{
				case VERTEX_POSITION:
					vertexPositions.push_back(ConvertToPosition(sLine, Token));
				break;

				case VERTEX_TEXPOS:
					vertexTexturePos.push_back(ConvertToPosition(sLine, Token));
				break;

				case VERTEX_NORMAL:
					vertexNormals.push_back(ConvertToPosition(sLine, Token));
				break;

				case FACE_DATA:
					GetVertices(sLine, vertexPositions, vertexTexturePos, vertexNormals, objVertices);
				break;
			}
		}
	}
	myfile.close();

	Vertex*	vertices	=	new Vertex[objVertices.size()];
	for(int i = 0; i < objVertices.size(); ++i)
		vertices[i]	=	objVertices[i];

}

vector<Vertex>* ObjLoader::GetVertices()
{
	return &objVertices;
}