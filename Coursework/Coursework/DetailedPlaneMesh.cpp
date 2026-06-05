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

--------------------
GitHub Link to project: https://github.com/AlexanderPenny/Graphics-Project/
*/

#include "DetailedPlaneMesh.h"

DetailedPlaneMesh::DetailedPlaneMesh(ID3D11Device* device, ID3D11DeviceContext* deviceContext, int lresolution, XMFLOAT2 lsize, XMFLOAT3 lposition, XMFLOAT3 lrotation)
	: PlaneMesh(device, deviceContext)
{
	// in this derived class, the resolution is being used differently
	// so that this variable actually represents the resolution rather than the size
	resolution = lresolution; 
	size = lsize;
	position = lposition;
	rotation = lrotation;

	initBuffers(device);
}

DetailedPlaneMesh::~DetailedPlaneMesh()
{
	BaseMesh::~BaseMesh();
}

void DetailedPlaneMesh::initBuffers(ID3D11Device* device)
{
	// creates the rotation matrices (although I could probably just do it on one line for memory efficiency
	XMMATRIX yawMatrix = XMMatrixRotationY(XMConvertToRadians(rotation.y));
	XMMATRIX pitchMatrix = XMMatrixRotationX(XMConvertToRadians(rotation.x));
	XMMATRIX rollMatrix = XMMatrixRotationZ(XMConvertToRadians(rotation.z));
	XMMATRIX rotationMatrix = yawMatrix * pitchMatrix * rollMatrix; // combines all the rotation matrices into one
/*	XMMATRIX rotationMatrix = XMMatrixRotationZ(XMConvertToRadians(rotation.z)) 
		* XMMatrixRotationX(XMConvertToRadians(rotation.x)) 
		* XMMatrixRotationZ(XMConvertToRadians(rotation.z)); */// puts all the rotation matrices into one
	XMVECTOR vertexVector, rotatedVertex;

	VertexType* vertices;
	unsigned long* indices;
	int index, i, j;
	float positionX, positionZ, u, v, increment;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;

	// Calculate the number of vertices in the terrain mesh.
	vertexCount = (resolution - 1) * (resolution - 1) * 24;


	indexCount = vertexCount;
	vertices = new VertexType[vertexCount];
	indices = new unsigned long[indexCount];


	index = 0;
	// UV coords.
	u = 0;
	v = 0;
	increment = 1.0f / resolution;

	for (j = position.z; j < (size.x - 1) + position.z; j++) 
	{
		for (i = position.x; i < (size.y - 1)+position.x; i++)
		{


			// Upper left.
			positionX = (float)i;
			positionZ = (float)(j);

			vertices[index].position = XMFLOAT3(positionX, position.y, positionZ);
			vertices[index].texture = XMFLOAT2(u, v);
			vertices[index].normal = XMFLOAT3(0.0, 1.0, 0.0);
			indices[index] = index;
			vertexVector = XMLoadFloat3(&vertices[index].position);
			rotatedVertex = XMVector3TransformCoord(vertexVector, rotationMatrix);
			XMStoreFloat3(&vertices[index].position, rotatedVertex);
			index++;

			// Upper right.
			positionX = (float)(i + 1);
			positionZ = (float)(j + 1);

			vertices[index].position = XMFLOAT3(positionX, position.y, positionZ);
			vertices[index].texture = XMFLOAT2(u + increment, v + increment);
			vertices[index].normal = XMFLOAT3(0.0, 1.0, 0.0);
			indices[index] = index;
			 vertexVector = XMLoadFloat3(&vertices[index].position);
			 rotatedVertex = XMVector3TransformCoord(vertexVector, rotationMatrix);
			XMStoreFloat3(&vertices[index].position, rotatedVertex);
			index++;


			// lower left
			positionX = (float)(i);
			positionZ = (float)(j + 1);


			vertices[index].position = XMFLOAT3(positionX, position.y, positionZ);
			vertices[index].texture = XMFLOAT2(u, v + increment);
			vertices[index].normal = XMFLOAT3(0.0, 1.0, 0.0);
			indices[index] = index;
			 vertexVector = XMLoadFloat3(&vertices[index].position);
			 rotatedVertex = XMVector3TransformCoord(vertexVector, rotationMatrix);
			XMStoreFloat3(&vertices[index].position, rotatedVertex);
			index++;

			// Upper left
			positionX = (float)(i);
			positionZ = (float)(j);

			vertices[index].position = XMFLOAT3(positionX, position.y, positionZ);
			vertices[index].texture = XMFLOAT2(u, v);
			vertices[index].normal = XMFLOAT3(0.0, 1.0, 0.0);
			indices[index] = index;
			 vertexVector = XMLoadFloat3(&vertices[index].position);
			 rotatedVertex = XMVector3TransformCoord(vertexVector, rotationMatrix);
			XMStoreFloat3(&vertices[index].position, rotatedVertex);
			index++;

			// Bottom right
			positionX = (float)(i + 1);
			positionZ = (float)(j);

			vertices[index].position = XMFLOAT3(positionX, position.y, positionZ);
			vertices[index].texture = XMFLOAT2(u + increment, v);
			vertices[index].normal = XMFLOAT3(0.0, 1.0, 0.0);
			indices[index] = index;
			 vertexVector = XMLoadFloat3(&vertices[index].position);
			 rotatedVertex = XMVector3TransformCoord(vertexVector, rotationMatrix);
			XMStoreFloat3(&vertices[index].position, rotatedVertex);
			index++;

			// Upper right.
			positionX = (float)(i + 1);
			positionZ = (float)(j + 1);

			vertices[index].position = XMFLOAT3(positionX, position.y, positionZ);
			vertices[index].texture = XMFLOAT2(u + increment, v + increment);
			vertices[index].normal = XMFLOAT3(0.0, 1.0, 0.0);
			indices[index] = index;
			 vertexVector = XMLoadFloat3(&vertices[index].position);
			 rotatedVertex = XMVector3TransformCoord(vertexVector, rotationMatrix);
			 XMStoreFloat3(&vertices[index].position, rotatedVertex);
			index++;

			u += increment;

		}

		u = 0;
		v += increment;
	}

	// Set up the description of the static vertex buffer.
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;
	// Give the subresource structure a pointer to the vertex data.
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;
	// Now create the vertex buffer.
	device->CreateBuffer(&vertexBufferDesc, &vertexData, &vertexBuffer);

	// Set up the description of the static index buffer.
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;
	// Give the subresource structure a pointer to the index data.
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;
	// Create the index buffer.
	device->CreateBuffer(&indexBufferDesc, &indexData, &indexBuffer);

	// Release the arrays now that the buffers have been created and loaded.
	delete[] vertices;
	vertices = 0;
	delete[] indices;
	indices = 0;
}