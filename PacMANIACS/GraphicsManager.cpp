#include "GraphicsManager.h"


GraphicsManager::GraphicsManager(void)
{
}


GraphicsManager::~GraphicsManager(void)
{
	
}


void GraphicsManager::LoadModels(vector<GameObject> *gameObjects)
{
	//Vertex *vertecies;
	
	vector<ModelInfo> modelInfos;
	int totalVertexCount = 0;
	
	//Laddar modeller.
	ObjLoader *objLoader;	
	for each (GameObject gameObject in *gameObjects)
	{
		objLoader = new ObjLoader(modelPath + gameObject.GetName());

		ModelInfo mInfo;
		mInfo.name = gameObject.GetName();
		mInfo.verticies = *objLoader->GetVertices();
		totalVertexCount += mInfo.verticies.size();
		modelInfos.push_back(mInfo);
		//delete objLoader;
	}
	
	//Lägger in vertexdatan för alla modellerna i en och samma vector.
	//Sparar startindex och antar vertexpunkter för varje model.
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


	//Skapar vertexbuffer innehållande all vertexdata.
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