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
	vector<vector<Vertex>> models;
	//Ladda modeller
	//Addera antal vertexpunkter till totalVertexCount för varje modell
	//vertexVectors.push_back(ObjLoader.GetVertices());


	for each (vector<Vertex> model in models)
	{
		totalVertexCount += model.size();
	}

	
	vector<Vertex> vertices(totalVertexCount);

	int start = 0;
	for each (vector<Vertex> model in models)
	{
		for each (Vertex vertex in model)
		{
			vertices.push_back(vertex);
		}

		IndexInfo indexInfo;
		indexInfo.start = start;
		indexInfo.count = model.size();

		gIndexMap.insert(pair<string,IndexInfo>("modelname", indexInfo));

		start += model.size();
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

void GraphicsManager::Render()
{
	//IndexInfo indexInfo = gIndexMap["modelname"];
	//gDeviceContext->Draw(indexInfo.count, indexInfo.start);
}