#include "GraphicsManager.h"


GraphicsManager::GraphicsManager(void)
{

}


GraphicsManager::~GraphicsManager(void)
{
	
}


void GraphicsManager::LoadModels()
{
	//Vertex *vertecies;

	int totalVertexCount = 0;
	vector<vector<Vertex>> vertexVectors;
	//Ladda modeller
	//Addera antal vertexpunkter till totalVertexCount för varje modell
	//vertexVectors.push_back(ObjLoader.GetVertices());


	for each (vector<Vertex> modelVerticies in vertexVectors)
	{
		totalVertexCount += modelVerticies.size();
	}

	
	vector<Vertex> vertices(totalVertexCount);

	for each (vector<Vertex> modelVerticies in vertexVectors)
	{
		for each (Vertex vertex in modelVerticies)
		{
			vertices.push_back(vertex);
		}
	}

	BUFFER_INIT_DESC bufferDesc;
	bufferDesc.ElementSize = sizeof(Vertex);
	bufferDesc.InitData = &vertices;
	bufferDesc.NumElements = totalVertexCount;
	bufferDesc.Type = VERTEX_BUFFER;
	bufferDesc.Usage = BUFFER_DEFAULT;

	if(FAILED(gVertexBuffer->Init(gDevice, gDeviceContext, bufferDesc)))
	{
		::MessageBox(0, "Failed to initalize VertexBuffer(GraphicsManager)", "Error", MB_OK);
	}

}
