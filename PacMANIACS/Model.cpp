#include "Model.h"

Model::Model()
{
	vertexBuffer = 0;

	position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	rotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
}

Model::Model(const Model& other)
{
}


Model::~Model()
{
	shader->~Shader();
	vertexBuffer->~Buffer();
}

void Model::SetPosition(float x, float y, float z)
{
	position = D3DXVECTOR3(x, y, z);
}

void Model::UpdateScale(float x, float y, float z)
{
	scale = D3DXVECTOR3(x, y, z);
}

bool Model::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, vector<Vertex>* vertices, LPCSTR textureFilename)
{
	bool result;


	// Initialize the vertex and index buffer that hold the geometry for the triangle.
	result = InitializeBuffers(device, deviceContext, vertices, textureFilename);
	if(!result)
	{
		return false;
	}

	return true;
}

void Model::Frame(float dt)
{
	D3DXMATRIX matTranslation;
	D3DXMatrixTranslation(&matTranslation, position.x, position.y, position.z);

	D3DXMATRIX matRotation;
	D3DXMatrixRotationYawPitchRoll(&matRotation, rotation.x, rotation.y, rotation.z);

	D3DXMATRIX matScale;
	D3DXMatrixScaling(&matScale, scale.x, scale.y, scale.z);

	transformation =  matScale * matRotation * matTranslation;
}

void Model::Render(ID3D11DeviceContext* deviceContext, Camera& camera, D3DXMATRIX* projection)
{
	// Put the vertex and index buffers on the graphics pipeline to prepare them for drawing.
	RenderBuffers(deviceContext, camera, projection);

	return;
}

D3DXMATRIX Model::GetTransformation()
{
	return transformation;
}

bool Model::InitializeBuffers(ID3D11Device* device, ID3D11DeviceContext* deviceContext, vector<Vertex>* vertices, LPCSTR textureFilename)
{
	vertexCount = vertices->size();

	Vertex *verts = new Vertex[vertexCount];
	for (int i = 0; i < vertexCount; i++)
		verts[i] = vertices->at(i);

	BUFFER_INIT_DESC vertexBufferDesc;
	vertexBufferDesc.ElementSize	=	sizeof(Vertex);
	vertexBufferDesc.InitData		=	verts;
	vertexBufferDesc.NumElements	=	vertices->size();
	vertexBufferDesc.Type			=	VERTEX_BUFFER;
	vertexBufferDesc.Usage			=	BUFFER_DEFAULT;
	
	vertexBuffer = new Buffer();
	//	Try initializing the vertex buffer
	if(FAILED(vertexBuffer->Init(device, deviceContext, vertexBufferDesc)))
		::MessageBox(0, "Initializing vertex buffer failed! [ObjLoader]", "Error", MB_OK);

	shader = new Shader();

	D3D11_INPUT_ELEMENT_DESC inputDesc[]	=	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXTURECOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	if(FAILED(shader->Init(device, deviceContext, "Shaders/Shader.fx", inputDesc, 3)))
		::MessageBox(0, "Initializing shader failed! [Shader]", "Error", MB_OK);

	//	Add the grass texture
	if(D3DX11CreateShaderResourceViewFromFile(device, textureFilename, 0, 0, &texture, 0 ))
		::MessageBox(0, "Failed to create shader resource! [Model->bthcolor]", "Error", MB_OK);
	shader->SetResource("gtex", texture);
	

	return true;
}

void Model::RenderBuffers(ID3D11DeviceContext* deviceContext, Camera& camera, D3DXMATRIX* projection)
{
	//	Set topology to TriangleList
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//	Apply the vertex and index buffer
	vertexBuffer->Apply();

	D3DXMATRIX world;
	D3DXMatrixTranslation(&world, 0, 0, 0);
	world *= transformation;

	//	Give the shader the World, View
	//	and Projection matrix
	shader->SetResource("gtex", texture);
	shader->SetMatrix("World", world);
	shader->SetMatrix("View", camera.GetViewMatrix());
	shader->SetMatrix("Projection", *projection);
	
	shader->SetMatrix("World", world);
	shader->SetMatrix("ViewProjection", *projection);
	shader->SetMatrix("WVP", world * camera.GetViewMatrix() * (*projection));
	
	//Skickar värden för cameran till shader
	shader->SetFloat3("CameraPos", camera.Position);

	shader->Apply(0);
	
	deviceContext->Draw(vertexCount, 0);

	return;
}

D3DXVECTOR3 Model::GetPosition()
{
	return position;
}