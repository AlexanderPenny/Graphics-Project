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

#include "CubeMesh.h"

class DetailedCubeMesh : public CubeMesh
{
public:
	DetailedCubeMesh(ID3D11Device* device, ID3D11DeviceContext* deviceContext, int lresolution = 100, XMFLOAT3 lsize = { 100,100,100 }, XMFLOAT3 lposition = { 0, 0, 0 });
	~DetailedCubeMesh();

	void initBuffers(ID3D11Device* device);
protected:
	XMFLOAT3 size;
	XMFLOAT3 rotation = { 0,0,0 };
	XMFLOAT3 position;
};