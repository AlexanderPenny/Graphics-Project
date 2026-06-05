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

#include "ManipulationShader.h"

ManipulationShader::ManipulationShader(ID3D11Device* device, HWND hwnd) : BaseShader(device, hwnd)
{
	initShader(L"manipulation_vs.cso", L"manipulation_ps.cso");
}

ManipulationShader::~ManipulationShader()
{
	// Release the sampler state
	if (sampleState)
	{
		sampleState->Release();
		sampleState = 0;
	}

	// Release the matrix constant buffer
	if (matrixBuffer)
	{
		matrixBuffer->Release();
		matrixBuffer = 0;
	}

	// Release the layout
	if (layout)
	{
		layout->Release();
		layout = 0;
	}

	// Release the light constant buffer
	if (lightBuffer)
	{
		lightBuffer->Release();
		lightBuffer = 0;
	}

	//Release base shader components
	BaseShader::~BaseShader();
}

void ManipulationShader::initShader(const wchar_t* vsFilename, const wchar_t* psFilename)
{
	D3D11_BUFFER_DESC matrixBufferDesc;
	D3D11_SAMPLER_DESC samplerDesc;
	D3D11_BUFFER_DESC lightBufferDesc;
	D3D11_BUFFER_DESC timeBufferDesc;

	// Load (+ compile) shader files
	loadVertexShader(vsFilename);
	loadPixelShader(psFilename);

	// Setup the description of the dynamic matrix constant buffer that is in the vertex shader.
	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof(MatrixBufferType);
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;
	renderer->CreateBuffer(&matrixBufferDesc, NULL, &matrixBuffer);

	// Create a texture sampler state description.
	samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	renderer->CreateSamplerState(&samplerDesc, &sampleState);
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
	samplerDesc.BorderColor[0] = 1.0f;
	samplerDesc.BorderColor[1] = 1.0f;
	samplerDesc.BorderColor[2] = 1.0f;
	samplerDesc.BorderColor[3] = 1.0f;
	renderer->CreateSamplerState(&samplerDesc, &sampleStateShadow);

	// Setup light buffer
	// Setup the description of the light dynamic constant buffer that is in the pixel shader.
	// Note that ByteWidth always needs to be a multiple of 16 if using D3D11_BIND_CONSTANT_BUFFER or CreateBuffer will fail.
	lightBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	lightBufferDesc.ByteWidth = sizeof(LightBufferType);
	lightBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	lightBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	lightBufferDesc.MiscFlags = 0;
	lightBufferDesc.StructureByteStride = 0;
	renderer->CreateBuffer(&lightBufferDesc, NULL, &lightBuffer);

	timeBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	timeBufferDesc.ByteWidth = sizeof(TimeBufferType);
	timeBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	timeBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	timeBufferDesc.MiscFlags = 0;
	timeBufferDesc.StructureByteStride = 0;
	renderer->CreateBuffer(&timeBufferDesc, NULL, &timeBuffer);


}

void ManipulationShader::setShaderParameters(ID3D11DeviceContext* deviceContext,
	const XMMATRIX& worldMatrix, const XMMATRIX& viewMatrix, const XMMATRIX& projectionMatrix,
	ID3D11ShaderResourceView* displacement, Light* light[4], float timer,
	ID3D11ShaderResourceView* texture, ID3D11ShaderResourceView* normals, int direction,
	ShadowMap* shadowMaps[2])
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	MatrixBufferType* dataPtr;

	XMMATRIX tworld, tview, tproj;


	// --- Transpose the matrices to prepare them for the shader --- //
	tworld = XMMatrixTranspose(worldMatrix);
	tview = XMMatrixTranspose(viewMatrix);
	tproj = XMMatrixTranspose(projectionMatrix);
	
	result = deviceContext->Map(matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	dataPtr = (MatrixBufferType*)mappedResource.pData;
	dataPtr->world = tworld; // worldMatrix;
	dataPtr->view = tview;
	dataPtr->projection = tproj;
	for (int i = 0; i < 4; i++)
	{
		dataPtr->lightView[i] = XMMatrixTranspose(light[i]->getViewMatrix());
		dataPtr->lightProjection[i] = XMMatrixTranspose(light[i]->getOrthoMatrix());
	}
	deviceContext->Unmap(matrixBuffer, 0);
	deviceContext->VSSetConstantBuffers(0, 1, &matrixBuffer);
	// -------------------------------------------------------------- //
	
	// --- Send light data to pixel shader --- //
	LightBufferType* lightPtr;
	deviceContext->Map(lightBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	lightPtr = (LightBufferType*)mappedResource.pData;

	lightPtr->diffuse[0] = light[0]->getDiffuseColour();
	lightPtr->direction[0] = XMFLOAT4(light[0]->getDirection().x, light[0]->getDirection().y, light[0]->getDirection().z, 0.f);
	lightPtr->lPosition[0] = XMFLOAT4(light[0]->getPosition().x, light[0]->getPosition().y, light[0]->getPosition().z,0.f);
	lightPtr->ambient[0] = light[0]->getAmbientColour();

	lightPtr->diffuse[1] = light[1]->getDiffuseColour();
	lightPtr->direction[1] = XMFLOAT4(light[1]->getDirection().x, light[1]->getDirection().y, light[1]->getDirection().z, 0.f);
	lightPtr->lPosition[1] = XMFLOAT4(light[1]->getPosition().x, light[1]->getPosition().y, light[1]->getPosition().z, 0.f);
	lightPtr->ambient[1] = light[1]->getAmbientColour();

	lightPtr->diffuse[2] = light[2]->getDiffuseColour();
	lightPtr->direction[2] = XMFLOAT4(light[2]->getDirection().x, light[2]->getDirection().y, light[2]->getDirection().z, 0.f);
	lightPtr->lPosition[2] = XMFLOAT4(light[2]->getPosition().x, light[2]->getPosition().y, light[2]->getPosition().z, 0.f);
	lightPtr->ambient[2] = light[2]->getAmbientColour();
	
	lightPtr->diffuse[3] = light[3]->getDiffuseColour();
	lightPtr->direction[3] = XMFLOAT4(light[3]->getDirection().x, light[3]->getDirection().y, light[3]->getDirection().z, 0.f);
	lightPtr->lPosition[3] = XMFLOAT4(light[3]->getPosition().x, light[3]->getPosition().y, light[3]->getPosition().z, 0.f);
	lightPtr->ambient[3] = light[3]->getAmbientColour();

	deviceContext->Unmap(lightBuffer, 0);
	deviceContext->PSSetConstantBuffers(0, 1, &lightBuffer);
	// ---------------------------------------- //

	// --- Set timer information --- //
	TimeBufferType* timePtr;
	deviceContext->Map(timeBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	timePtr = (TimeBufferType*)mappedResource.pData;
	timePtr->time = timer;
	timePtr->direction = direction; // passing this through the timePtr to be more memory efficient
	timePtr->padding = XMFLOAT2(0, 0);
	deviceContext->Unmap(timeBuffer, 0);
	deviceContext->VSSetConstantBuffers(1, 1, &timeBuffer);
	// ------------------------------ //

	// --- Set shader texture resource in the pixel & vertex shaders --- //
	ID3D11ShaderResourceView* depthMap[2];
	for (int i = 0; i < 2; ++i)
		depthMap[i] = shadowMaps[i]->getDepthMapSRV();
	deviceContext->PSSetShaderResources(0, 1, &texture);
	deviceContext->PSSetShaderResources(1, 2, depthMap); // depthMap
	deviceContext->VSSetShaderResources(0, 1, &displacement);
	deviceContext->VSSetShaderResources(1, 1, &normals);
	deviceContext->PSSetSamplers(0, 1, &sampleState);
	deviceContext->PSSetSamplers(1, 1, &sampleStateShadow); // sampleStateShadow
	deviceContext->VSSetSamplers(0, 1, &sampleState);
	// ------------------------------------------------------- //
}
