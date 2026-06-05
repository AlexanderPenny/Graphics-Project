/*
Copyright 2026 Alexander L. Penny

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

	   http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/

#pragma once
#include "DXF.h"

using namespace std;
using namespace DirectX;

class ManipulationShader : public BaseShader
{
private:
	struct MatrixBufferType
	{
		XMMATRIX world;
		XMMATRIX view;
		XMMATRIX projection;
		XMMATRIX lightView[4];
		XMMATRIX lightProjection[4];
	};

	struct LightBufferType
	{
		XMFLOAT4 diffuse[4];
		XMFLOAT4 lPosition[4];
		XMFLOAT4 ambient[4];
		XMFLOAT4 direction[4];
	};

	struct TimeBufferType
	{
		float time;
		float direction;
		XMFLOAT2 padding;
	};

public:
	ManipulationShader(ID3D11Device* device, HWND hwnd);
	~ManipulationShader();

	void setShaderParameters(ID3D11DeviceContext* deviceContext, const XMMATRIX& world, const XMMATRIX& view, 
		const XMMATRIX& projection, ID3D11ShaderResourceView* displacement, Light* light[4], float timer, 
		ID3D11ShaderResourceView* texture, ID3D11ShaderResourceView* normals, int direction, 
		ShadowMap* shadowMaps[2]);


private:
	void initShader(const wchar_t* cs, const wchar_t* ps);

private:
	ID3D11Buffer* matrixBuffer;
	ID3D11SamplerState* sampleState;
	ID3D11SamplerState* sampleStateShadow;
	ID3D11Buffer* lightBuffer;
	ID3D11Buffer* timeBuffer;
};

