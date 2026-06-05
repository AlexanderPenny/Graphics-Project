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

// Application.h
#ifndef _APP1_H
#define _APP1_H

// Includes
#include "DXF.h"
#include "ManipulationShader.h"
#include "LightShader.h"
#include "ShadowShader.h"
#include "DepthShader.h"
#include "WaterDropShader.h"
#include "TextureShader.h"

enum TextureIDs
{
	pavement,
	concrete,
	road,
	building
};

class App1 : public BaseApplication
{
public:

	App1();
	~App1();
	void init(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight, Input* in, bool VSYNC, bool FULL_SCREEN);

	bool frame();

protected:
	void gui();

	bool render();
	// stages
	void firstPass();
	void postProcessing();
	void finalPass();
	void depthPass();

	void finalPassPPOFF();
	void depthPassPPOFF();

private:
	/*LightShader* lShader;*/
	vector<BaseMesh*> mesh;
	vector<TextureIDs> meshTexture;
	vector<int> directionFace;

	vector<AModel*> streetLamps;
	vector<AModel*> otherModels;

	Light* light[4];
	float elapsedTime = 0;

	XMFLOAT3 spaceship;
	XMFLOAT3 moonlight;
	XMFLOAT3 moonlightpos;
	XMFLOAT3 streetLamppos;
	XMFLOAT3 streetLampdir;

	// Shadows
	ShadowShader* shadowShader;
	DepthShader* depthShader;
	ShadowMap* shadowMap[2];

	// -- other shaders -- //
	ManipulationShader* mShader;
	LightShader* lightShader;

	// post processing:
	RenderTexture* renderTexture;
	RenderTexture* renderTexture2;
	WaterDropShader* waterDropShader;
	OrthoMesh* orthoMesh;
	TextureShader* textureShader;
	// ------------------- // 
	bool postProcessingOn = true; // choose if everything is either getting renderered to an orthomesh with PP or not
};

#endif