#include "GraphicsManager.h"


GraphicsManager::GraphicsManager(ID3D11Device *device, ID3D11DeviceContext *deviceContext, ID3D11RenderTargetView* renderTargetView, D3DXVECTOR2 resolution)
{
	gDevice = device;
	gDeviceContext = deviceContext;
	gRenderTargetView = renderTargetView;

	ID3D11Texture2D* depthStencil;
	// Create depth stencil texture
	D3D11_TEXTURE2D_DESC descDepth;
	descDepth.Width = resolution.x;
	descDepth.Height = resolution.y;
	descDepth.MipLevels = 1;
	descDepth.ArraySize = 1;
	descDepth.Format = DXGI_FORMAT_D32_FLOAT;
	descDepth.SampleDesc.Count = 1;
	descDepth.SampleDesc.Quality = 0;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	descDepth.CPUAccessFlags = 0;
	descDepth.MiscFlags = 0;
	device->CreateTexture2D( &descDepth, NULL, &depthStencil );


	// Create the depth stencil view
	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
	ZeroMemory(&descDSV, sizeof(descDSV));
	descDSV.Format = descDepth.Format;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice = 0;
	device->CreateDepthStencilView( depthStencil, &descDSV, &gDepthStencilView );


	//Laddar shadern
	D3D11_INPUT_ELEMENT_DESC inputDesc[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXTURECOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	
	gShader = new Shader();
	if(FAILED(gShader->Init(gDevice, gDeviceContext, "Shaders/Shader.fx", inputDesc, 3)))
	{
		::MessageBox(0, "Failed to initalize Shader(GraphicsManager)", "Error", MB_OK);
	}

}


GraphicsManager::~GraphicsManager(void)
{
	
}

void GraphicsManager::SetCamera(Camera *camera)
{
	gCamera = camera;
}

void GraphicsManager::SetGameObjects(vector<GameObject*> *gameObjects)
{
	gGameObjects = gameObjects;
}

void GraphicsManager::SetModelPath(string path)
{
	modelPath = path;
}

void GraphicsManager::SetTexturePath(string path)
{
	texturePath = path;
}

ID3D11ShaderResourceView* GraphicsManager::LoadShaderResourceView(string path)
{
	ID3D11ShaderResourceView* resource = 0;
	D3DX11_IMAGE_LOAD_INFO loadInfo;
	loadInfo.MiscFlags = D3D11_RESOURCE_DIMENSION_TEXTURE2D;

	ID3D11Texture2D* tex = NULL;
	if(FAILED(D3DX11CreateTextureFromFile(gDevice,
		(char*)path.c_str(), NULL, NULL, (ID3D11Resource**)&tex, NULL)))
	{
		return 0;
	}

	else
	{
		D3D11_TEXTURE2D_DESC texDesc;
		tex->GetDesc(&texDesc);

		D3D11_SHADER_RESOURCE_VIEW_DESC desc;
		ZeroMemory( &desc, sizeof( desc ) );
		desc.Format = texDesc.Format;
		desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		desc.Texture2D.MipLevels = texDesc.MipLevels;
		desc.Texture2D.MostDetailedMip = 0;
		if(FAILED(gDevice->CreateShaderResourceView( tex, &desc, &resource )))
			return 0;

		SAFE_RELEASE( tex );
	}
	return resource;
}

void GraphicsManager::LoadModels()
{
	if (!gGameObjects)
		return;


	MAP_VERTEX vertexMap;

	int totalVertexCount = 0;
	
	//Laddar modeller.	
	ObjLoader *objLoader;	
	
	//För varje objekt
	
	for each (GameObject* gameObject in *gGameObjects)
	{
		//Om typen av objektet inte redan är laddad
		if (vertexMap.count(gameObject->GetName()) == 0)
		{
			//Ladda in modellen
			string path = modelPath + "\\" + gameObject->GetName() + ".obj";
			objLoader = new ObjLoader(path);

			//Hämtar vertexpunkterna
			vector<Vertex> verticies = *objLoader->GetVertices();

			//Sparar vertexpunkterna i vertexMap
			vertexMap.insert(pair<string, vector<Vertex>>(gameObject->GetName(), verticies));

			//Ökar det totala antalet vertexpunkter med antalet vertexpunkter i den här modellen.
			totalVertexCount += verticies.size();

			//delete objLoader;
		}
		
		
		//Om texturen inte redan är inladdad
		if (gTextureMap.count(gameObject->GetTextureName()) == 0) //gameObject.GetTextureName()
		{
			//Ladda texturen
			string path = texturePath + "\\" + gameObject->GetTextureName();
			ID3D11ShaderResourceView* texture = LoadShaderResourceView(path);	//ÄNDRA!

			//Sparar texturen i gTextureMap
			gTextureMap.insert(pair<string, ID3D11ShaderResourceView*>(gameObject->GetTextureName(), texture)); //gameObject.GetTextureName()
		}
	}
	
	//Lägger in vertexdatan för alla modellerna i en och samma vector.
	//Sparar startindex och antar vertexpunkter för varje model.
	vector<Vertex> vertices;

	int start = 0;
	
	for(MAP_VERTEX::iterator iterator = vertexMap.begin(); iterator != vertexMap.end(); iterator++) 
	{

		for each (Vertex vertex in iterator->second)
		{
			vertices.push_back(vertex);
		}

		//Skapar en IndexInfo.
		//Start är indexet efter förgående inlaggda modell.
		//Count är antalet vertexpunkter i modellen.
		IndexInfo indexInfo;
		indexInfo.start = start;
		indexInfo.count = iterator->second.size();

		//Lägger in indexinfon.
		//gIndexMap använder samma nyckel som vertexMap använder (GameObject.GetName()).
		gIndexMap.insert(pair<string, IndexInfo>(iterator->first, indexInfo));

		//Ökar på startindexet så att nästa modell kan läggas till.
		start += iterator->second.size();
	}
	
	vector<Vertex> temp = vertices;

	//Skapar vertexbuffer innehållande all vertexdata.
	BUFFER_INIT_DESC bufferDesc;
	bufferDesc.ElementSize = sizeof(Vertex);
	bufferDesc.InitData = &vertices[0];
	bufferDesc.NumElements = vertices.size();
	bufferDesc.Type = VERTEX_BUFFER;
	bufferDesc.Usage = BUFFER_DEFAULT;

	gVertexBuffer = new Buffer();
	if(FAILED(gVertexBuffer->Init(gDevice, gDeviceContext, bufferDesc)))
	{
		::MessageBox(0, "Failed to initalize VertexBuffer(GraphicsManager)", "Error", MB_OK);
	}
}

void GraphicsManager::Render()
{
	if (!gGameObjects)
		return;

	//Rensar depthbuffern
	gDeviceContext->ClearDepthStencilView( gDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0 );

	//Sätter render target
	gDeviceContext->OMSetRenderTargets( 1, &gRenderTargetView, gDepthStencilView );

	//Sätter buffer
	gVertexBuffer->Apply();

	//Sätter topologi
	gDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	D3DXMATRIX world, worldInverseTranspose, view, projection;
	//View och projection matriserna
	view		= gCamera->GetViewMatrix();
	projection	= gCamera->GetProjectionMatrix();


	//Set PerFrame data.
	gShader->SetMatrix("View",			view);
	gShader->SetMatrix("Projection",	projection);
	gShader->SetFloat3("CameraPos",		gCamera->GetPosition());

	for each (GameObject* gameObject in *gGameObjects)
	{

		//world matrisen
		world					= gameObject->GetWorldMatrix();

		//worldinversetranspose matrisen
		worldInverseTranspose	= gameObject->GetWorldInverseTranspose();

		//WVP-matrisen
		D3DXMATRIX WVP = world * view * projection;


		//Set PerObject data.
		gShader->SetMatrix("World",					world);
		gShader->SetMatrix("WVP",					WVP);
		gShader->SetMatrix("WorldInverseTranspose",	worldInverseTranspose);
		
		//Set texture.
		gShader->SetResource("Color", gTextureMap[gameObject->GetTextureName()]);
		

		gShader->Apply(0);

		//Index för vertexpunkterna
		IndexInfo indexInfo = gIndexMap[gameObject->GetName()];
		gDeviceContext->Draw(indexInfo.count, indexInfo.start);
	}
}