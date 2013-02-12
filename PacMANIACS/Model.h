#ifndef _MODEL_H_
#define _MODEL_H_

#include <d3d11.h>
#include <d3dx10math.h>

#include "ObjLoader.h"
#include "Buffer.h"
#include "Shader.h"
#include "Camera.h"

class Model
{
private:
	struct VertexType
	{
		D3DXVECTOR3 position;
		D3DXVECTOR2 texture;
		D3DXVECTOR3 normal;
	};

public:
	Model();
	Model(const Model&);
	~Model();

	int GetIndexCount();
	D3DXMATRIX GetTransformation();
	D3DXVECTOR3 GetPosition();

	bool Initialize(ID3D11Device*, ID3D11DeviceContext*, vector<Vertex>*, LPCSTR);
	void Frame(float);
	void Render(ID3D11DeviceContext*, Camera&, D3DXMATRIX*, D3DXVECTOR3*);
	void SetPosition(float, float, float);
	void UpdateScale(float, float, float);

private:
	Buffer *vertexBuffer;
	Shader *shader;
	int vertexCount;
	ID3D11ShaderResourceView* texture;
	D3DXVECTOR3 position, rotation, scale;
	D3DXMATRIX transformation;

	bool InitializeBuffers(ID3D11Device*, ID3D11DeviceContext*, vector<Vertex>*, LPCSTR);
	void RenderBuffers(ID3D11DeviceContext*,  Camera&, D3DXMATRIX*, D3DXVECTOR3*);
};


#endif