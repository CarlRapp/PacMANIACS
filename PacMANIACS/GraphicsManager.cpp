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
	
	vector<ModelInfo> modelInfos;

	//För varje modell
		//Ladda modell
		//ModelInfo mInfo;
		//mInfo.name = "ModelName";
		//mInfo.verticies = ObjLoader.GetVertices();
		//modelInfos.push_back(mInfo);



	//Addera antal vertexpunkter till totalVertexCount för varje modell

	int totalVertexCount = 0;
	for each (ModelInfo modelInfo in modelInfos)
	{
		totalVertexCount += modelInfo.verticies.size();
	}
	
	vector<Vertex> vertices(totalVertexCount);

	int start = 0;
	for each (ModelInfo modelInfo in modelInfos)
	{
		for each (Vertex vertex in modelInfo.verticies)
		{
			vertices.push_back(vertex);
		}

		IndexInfo indexInfo;
		indexInfo.start = start;
		indexInfo.count = modelInfo.verticies.size();

		gIndexMap.insert(pair<string, IndexInfo>(modelInfo.name, indexInfo));

		start += modelInfo.verticies.size();
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