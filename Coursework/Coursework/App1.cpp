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

#include "App1.h"

App1::App1()
{
	mShader = nullptr;
	light[0] = nullptr;
	light[1] = nullptr;
	light[2] = nullptr;
	textureMgr = nullptr;
}

void App1::init(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight, Input *in, bool VSYNC, bool FULL_SCREEN)
{
	BaseApplication::init(hinstance, hwnd, screenWidth, screenHeight, in, VSYNC, FULL_SCREEN);

	// ---- TEXTURE LOADING --- //
	{
		// Load pavement texture
		textureMgr->loadTexture(L"pavement", L"res/tiles_0112/tiles_0112_color_4k.jpg");
		textureMgr->loadTexture(L"pavementDisplacement", L"res/tiles_0112/tiles_0112_height_4k.png");
		textureMgr->loadTexture(L"pavementNormals", L"res/tiles_0112/tiles_0112_normal_directx_4k.png");

		// Load concrete texture
		textureMgr->loadTexture(L"concrete", L"res/concrete_0006/concrete_0006_color_2k.jpg");
		//textureMgr->loadTexture(L"concreteDisplacement", L"res/concrete_0006/concrete_0006_height_2k.png"); // not in use, so it is commented out
		textureMgr->loadTexture(L"concreteNormals", L"res/concrete_0006/concrete_0006_normal_directx_2k.png");

		// Load road texture
		textureMgr->loadTexture(L"road", L"res/asphalt_0004/asphalt_0004_color_4k.jpg");
		textureMgr->loadTexture(L"roadDisplacement", L"res/asphalt_0004/asphalt_0004_height_4k.png");
		textureMgr->loadTexture(L"roadNormals", L"res/asphalt_0004/asphalt_0004_normal_directx_4k.png");

		// Load building texture
		textureMgr->loadTexture(L"building", L"res/others_0029/others_0029_color_4k.jpg");
		textureMgr->loadTexture(L"buildingDisplacement", L"res/others_0029/others_0029_height_4k.png");
		textureMgr->loadTexture(L"buildingNormals", L"res/others_0029/others_0029_normal_directx_4k.png");

		// Rain Drips
		textureMgr->loadTexture(L"rainDrips", L"res/rain_drips.png");
		textureMgr->loadTexture(L"rainDripMask", L"res/rain_drip_mask.png");
		// ------------------------ //
	}

	//  -- Creates the pavements with the appropriate information for vertex displacement -- //
	{

		mesh.push_back(new DetailedPlaneMesh(renderer->getDevice(), renderer->getDeviceContext(), 400, { 2200, 250 }));
		meshTexture.push_back(pavement); // 0 correlating to the array of textures
		directionFace.push_back(0); // facing up

		mesh.push_back(new DetailedPlaneMesh(renderer->getDevice(), renderer->getDeviceContext(), 400, { 2200, 250 },
			{ 1000,0,0 }, { 0,0,0 }));
		meshTexture.push_back(pavement);
		directionFace.push_back(0); // facing up

		// ------------------------------------------------------------------------------------- //
	}
	// ------------------ street wall side A ------------------ //
	{

	// -------------------------------------------------------- //
	}
	// ------------------ street wall side B ------------------ //
	{

		mesh.push_back(new DetailedPlaneMesh(renderer->getDevice(), renderer->getDeviceContext(), 800, { 500, 1000 },
			{ -1000,-1250,-35 }, { 0,90,90 }));
		meshTexture.push_back(building); // setting the texture that will be rendered on this mesh
		directionFace.push_back(3); // these numeric values are used for vertex displacement

		// -------------------------------------------------------- //
	}
	// ------------------ models ------------------ //
	{
		// Street Lamps
			streetLamps.push_back(new AModel(renderer->getDevice(), "res/StreetLamp.obj"));
			streetLamps.push_back(new AModel(renderer->getDevice(), "res/StreetLamp.obj"));
			streetLamps.push_back(new AModel(renderer->getDevice(), "res/StreetLamp.obj"));

		// SUV/Car Model
			otherModels.push_back(new AModel(renderer->getDevice(), "res/Models/Intergalactic_Spaceship-(Wavefront).obj"));

	// -------------------------------------------------------- //
	}
	// --------------- Pavement Curbs --------------- //
	{

		mesh.push_back(new DetailedPlaneMesh(renderer->getDevice(), renderer->getDeviceContext(), 400, { 2200, 10 },
			{ 10,-270,00 }, { 0,0,90 }));
		meshTexture.push_back(concrete);
		directionFace.push_back(3);

		mesh.push_back(new DetailedPlaneMesh(renderer->getDevice(), renderer->getDeviceContext(), 100, { 2200, 10 },
			{ 240,-11,0 }, { 0,0,0 }));
		meshTexture.push_back(concrete);
		directionFace.push_back(0);

		mesh.push_back(new DetailedPlaneMesh(renderer->getDevice(), renderer->getDeviceContext(), 100, { 2200, 10 },
			{ -19,249,-30 }, { 0,0,-90 }));
		meshTexture.push_back(concrete);
		directionFace.push_back(4);

		// ---

		mesh.push_back(new DetailedPlaneMesh(renderer->getDevice(), renderer->getDeviceContext(), 100, { 2200, 10 },
			{ 10,-1030,00 }, { 0,0,90 }));
		meshTexture.push_back(concrete);
		directionFace.push_back(3);

		mesh.push_back(new DetailedPlaneMesh(renderer->getDevice(), renderer->getDeviceContext(), 100, { 2200, 10 },
			{ 1000,-11,0 }, { 0,0,0 }));
		meshTexture.push_back(concrete);
		directionFace.push_back(0);

		mesh.push_back(new DetailedPlaneMesh(renderer->getDevice(), renderer->getDeviceContext(), 100, { 2200, 10 },
			{ -19,1009,-30 }, { 0,0,-90 }));
		meshTexture.push_back(concrete);
		directionFace.push_back(4);
		// --------------------------------------------- //
	}
	// ------------------------------ Road ------------------------------ //
	{
		mesh.push_back(new DetailedPlaneMesh(renderer->getDevice(), renderer->getDeviceContext(), 760, { 2200, 760 },
			{ 245,-3,0 }, { 0,0,0 }));
		meshTexture.push_back(road);
		directionFace.push_back(0);
		// ------------------------------------------------------------------ //
	}

	mShader = new ManipulationShader(renderer->getDevice(), hwnd);
	depthShader = new DepthShader(renderer->getDevice(), hwnd);
	shadowShader = new ShadowShader(renderer->getDevice(), hwnd);
	lightShader = new LightShader(renderer->getDevice(), hwnd);

	// Post Processing
	renderTexture = new RenderTexture(renderer->getDevice(), screenWidth, screenHeight, SCREEN_NEAR, SCREEN_DEPTH);
	renderTexture2 = new RenderTexture(renderer->getDevice(), screenWidth, screenHeight, SCREEN_NEAR, SCREEN_DEPTH);
	waterDropShader = new WaterDropShader(renderer->getDevice(), hwnd);
	orthoMesh = new OrthoMesh(renderer->getDevice(), renderer->getDeviceContext(), screenWidth, screenHeight);	// Full screen size
	textureShader = new TextureShader(renderer->getDevice(), hwnd);

	int shadowmapWidth = 8192;
	int shadowmapHeight = 8192;
	int sceneWidth = 10000;
	int sceneHeight = 10000;

	for (int i = 0; i <2; ++i)
		shadowMap[i] = new ShadowMap(renderer->getDevice(), shadowmapWidth, shadowmapHeight);

	// Confirgure lights
	{
			// street lamps, point lights
			light[0] = new Light();
			light[0]->setAmbientColour(0.08f, 0.06f, 0.03f, 0.01f);
			light[0]->setDiffuseColour(0.889f, 0.611f, 0.316f, 1.0f);
			light[0]->setDirection(0.0f, -0.93f, 0.0f);
			light[0]->setPosition(298.246f, 1170.f, 374.632f);
			light[0]->generateOrthoMatrix((float)sceneWidth, (float)sceneHeight, 0.1f, 5000.f);
			light[0]->generateViewMatrix();

			light[1] = new Light();
			light[1]->setAmbientColour(0.08f, 0.06f, 0.03f, 0.01f);
			light[1]->setDiffuseColour(0.889f, 0.611f, 0.316f, 1.0f);
			light[1]->setDirection(0.0f, -0.93f, 0.0f);
			light[1]->setPosition(946.f, 1170.f, 920.f);
			light[1]->generateOrthoMatrix((float)sceneWidth, (float)sceneHeight, 0.1f, 5000.f);
			light[1]->generateViewMatrix();

			light[2] = new Light();
			light[2]->setAmbientColour(0.08f, 0.06f, 0.03f, 0.01f);
			light[2]->setDiffuseColour(0.889f, 0.611f, 0.316f, 1.0f);
			light[2]->setDirection(0.0f, -0.93f, 0.0f);
			light[2]->setPosition(1559.649f, 1170.f, 374.632f);
			light[2]->generateOrthoMatrix((float)sceneWidth, (float)sceneHeight, 0.1f, 5000.f);
			light[2]->generateViewMatrix();

			// directional light
			light[3] = new Light();
			light[3]->setAmbientColour(0.1f, 0.1f, 0.1f, 0.1f);
			light[3]->setDiffuseColour(0.284f, 0.316f, 0.568f, 1.0f);
			light[3]->setDirection(1.f, -1.f, -1.f);
			light[3]->setPosition(-1000.f, 600.f, 3000.f);
			light[3]->generateOrthoMatrix((float)sceneWidth, (float)sceneHeight, 0.1f, 5000.f);
			light[3]->generateViewMatrix();
			moonlight.x = light[3]->getDirection().x;
			moonlight.y = light[3]->getDirection().y;
			moonlight.z = light[3]->getDirection().z;
			
			moonlightpos.x = light[3]->getPosition().x;
			moonlightpos.y = light[3]->getPosition().y;
			moonlightpos.z = light[3]->getPosition().z;
			
			streetLamppos.x = light[2]->getPosition().x;
			streetLamppos.y = light[2]->getPosition().y;
			streetLamppos.z = light[2]->getPosition().z;
			
			streetLampdir.x = light[2]->getDirection().x;
			streetLampdir.y = light[2]->getDirection().y;
			streetLampdir.z = light[2]->getDirection().z;
	}
}

App1::~App1()
{
	// Run base application deconstructor
	BaseApplication::~BaseApplication();

	// Release the Direct3D object.
	if (&mesh)
	{ delete &mesh; }
	if (mShader)
	{ delete & mShader; mShader = 0; }
	if (lightShader)
	{ delete & lightShader; lightShader = 0; }
	if (light[0])
	{ delete light[0]; light[0] = 0; }
	if (light[1])
	{ delete light[1]; light[1] = 0; }
	if (light[2])
	{ delete light[2]; light[2] = 0; }
	if (textureMgr)
	{ delete textureMgr; textureMgr = 0; }
}

bool App1::frame()
{
	bool result;

	result = BaseApplication::frame();
	if (!result)
	{
		return false;
	}
	
	// Render the graphics
	result = render();
	if (!result)
	{
		return false;
	}

	return true;
}

bool App1::render()
{
	

	if (postProcessingOn)
	{
		firstPass();
		depthPass();
		postProcessing();
		finalPass();
	}
	else
	{
		depthPassPPOFF();
		finalPassPPOFF();
	}


	return true;
}

void App1::gui()
{
	int sceneWidth = 10000;
	int sceneHeight = 10000;


	// Force turn off unnecessary shader stages.
	renderer->getDeviceContext()->GSSetShader(NULL, NULL, 0);
	renderer->getDeviceContext()->HSSetShader(NULL, NULL, 0);
	renderer->getDeviceContext()->DSSetShader(NULL, NULL, 0);

	// Build UI
	ImGui::Text("FPS: %.2f", timer->getFPS());
	ImGui::Checkbox("Wireframe mode", &wireframeToggle);


	ImGui::Checkbox("PostProcessing", &postProcessingOn);

	//Scene creation movement
	ImGui::Text("spaceship");
	ImGui::SliderFloat("x: %.0f", &spaceship.x, -200, 200);
	ImGui::SliderFloat("y: %.0f", &spaceship.y, 0, 200);
	ImGui::SliderFloat("z: %.0f", &spaceship.z, 0, 300);

	ImGui::Text("moonlight direction");
	ImGui::SliderFloat("2x: %.0f", &moonlight.x, -2, 2);
	ImGui::SliderFloat("2y: %.0f", &moonlight.y, -2, 2);
	ImGui::SliderFloat("2z: %.0f", &moonlight.z, -2, 2);
	light[3]->setDirection(moonlight.x,moonlight.y,moonlight.z);

	ImGui::Text("moonlight position");
	ImGui::SliderFloat("3x: %.0f", &moonlightpos.x, -2000, 2000);
	ImGui::SliderFloat("3y: %.0f", &moonlightpos.y, -2000, 2000);
	ImGui::SliderFloat("3z: %.0f", &moonlightpos.z, -2000, 2000);
	light[3]->setPosition(moonlightpos.x, moonlightpos.y, moonlightpos.z);
	light[3]->generateOrthoMatrix((float)sceneWidth, (float)sceneHeight, 0.1f, 5000.f);
	light[3]->generateViewMatrix();

	ImGui::Text("street lamp light pos");
	ImGui::SliderFloat("4x: %.0f", &streetLamppos.x, -2000, 2000);
	ImGui::SliderFloat("4y: %.0f", &streetLamppos.y, -2000, 2000);
	ImGui::SliderFloat("4z: %.0f", &streetLamppos.z, -2000, 2000);
	
	ImGui::Text("street lamp light dir");
	ImGui::SliderFloat("5x: %.0f", &streetLampdir.x, -2, 2);
	ImGui::SliderFloat("5y: %.0f", &streetLampdir.y, -2, 2);
	ImGui::SliderFloat("5z: %.0f", &streetLampdir.z, -2, 2);
	light[2]->setDirection(streetLampdir.x, streetLampdir.y, streetLampdir.z);
	light[2]->setPosition(streetLamppos.x, streetLamppos.y, streetLamppos.z);
	light[2]->generateViewMatrix();

	ImGui::Text("camerax: %.2f", camera->getRotation().x);
	ImGui::Text("cameray: %.2f", camera->getRotation().y);
	ImGui::Text("cameraz: %.2f", camera->getRotation().z);

	// Render UI
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

void App1::depthPass()
{
	
	// -- Instansiations -- //
	int elements;
	XMMATRIX worldMatrix, lightViewMatrix, lightProjectionMatrix;
	// -------------------- // 

	
	
	for (int j = 0; j < 2; ++j)
	{
		worldMatrix = renderer->getWorldMatrix();
		if (j == 0)
		{
			shadowMap[0]->BindDsvAndSetNullRenderTarget(renderer->getDeviceContext());
			lightViewMatrix = light[0]->getViewMatrix();
			lightProjectionMatrix = light[0]->getOrthoMatrix();
		}
		else
		{
			shadowMap[1]->BindDsvAndSetNullRenderTarget(renderer->getDeviceContext());
			lightViewMatrix = light[3]->getViewMatrix();
			lightProjectionMatrix = light[3]->getOrthoMatrix();

		}
	// Depth Pass the Lamp Models
		if (j == 1)
	{
		// ------------------------------------------------------ //
		worldMatrix = XMMatrixTranslation(2.193, 0.110, 15.614);
		worldMatrix *= XMMatrixScaling(100.f, 130.f, 100.f);
		// Render street lamps
		streetLamps[0]->sendData(renderer->getDeviceContext());
		depthShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, lightViewMatrix, lightProjectionMatrix);
		depthShader->render(renderer->getDeviceContext(), streetLamps[0]->getIndexCount());

		// ----
		worldMatrix = renderer->getWorldMatrix();
		worldMatrix = XMMatrixTranslation(2.193, 0.110, 3.070);
		worldMatrix *= XMMatrixScaling(100.f, 130.f, 100.f);

		streetLamps[1]->sendData(renderer->getDeviceContext());

		depthShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, lightViewMatrix, lightProjectionMatrix);
		depthShader->render(renderer->getDeviceContext(), streetLamps[1]->getIndexCount());

		// ----
		worldMatrix = renderer->getWorldMatrix();
		worldMatrix = XMMatrixTranslation(-10.263, 0.110, -9.474);
		worldMatrix *= XMMatrixScaling(100.f, 130.f, 100.f);
		worldMatrix *= XMMatrixRotationY(XMConvertToRadians(180));

		streetLamps[2]->sendData(renderer->getDeviceContext());

		depthShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, lightViewMatrix, lightProjectionMatrix);
		depthShader->render(renderer->getDeviceContext(), streetLamps[2]->getIndexCount());
		// ------------------------------------------------------ //

	}
	elements = mesh.size();

	// Send geometry data, set shader parameters, render object with shader
	for (int i = 0; i < elements; i++)
	{
		mesh[i]->sendData(renderer->getDeviceContext());
		depthShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, lightViewMatrix, lightProjectionMatrix);
		depthShader->render(renderer->getDeviceContext(), mesh[i]->getIndexCount());
		mShader->render(renderer->getDeviceContext(), mesh[i]->getIndexCount());
	}

	// Draw Spaceship Model
	worldMatrix = renderer->getWorldMatrix();
	worldMatrix = XMMatrixTranslation(spaceship.x, spaceship.y, spaceship.z);
	worldMatrix *= XMMatrixScaling(10.f, 10.f, 10.f);

	otherModels[0]->sendData(renderer->getDeviceContext());
	depthShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, lightViewMatrix, lightProjectionMatrix);
	depthShader->render(renderer->getDeviceContext(), otherModels[0]->getIndexCount());


	// Set back buffer as render target and reset view port.
	renderer->setBackBufferRenderTarget();
	renderer->resetViewport();
	}


}

void App1::firstPass()
{
	// Set the render target to be the render to texture and clear it
	renderTexture->setRenderTarget(renderer->getDeviceContext());
	renderTexture->clearRenderTarget(renderer->getDeviceContext(), 0.0f, 0.0f, 1.0f, 1.0f);


	// Get the world, view, projection, and ortho matrices from the camera and Direct3D objects.
	XMMATRIX worldMatrix = renderer->getWorldMatrix();
	XMMATRIX viewMatrix = camera->getViewMatrix();
	XMMATRIX projectionMatrix = renderer->getProjectionMatrix();

	elapsedTime += timer->getTime();

	int elements = mesh.size();


	// Send geometry data, set shader parameters, render object with shader
	for (int i = 0; i < elements; i++)
	{
		mesh[i]->sendData(renderer->getDeviceContext());
		switch (meshTexture[i]) {
		case pavement:
			mShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix,
				textureMgr->getTexture(L"pavementDisplacement"), light, elapsedTime,
				textureMgr->getTexture(L"pavement"), textureMgr->getTexture(L"pavementNormals"), directionFace[i],
				shadowMap);
			break;
		case concrete:
			mShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix,
				textureMgr->getTexture(L""), light, elapsedTime,
				textureMgr->getTexture(L"concrete"), textureMgr->getTexture(L"concreteNormals"), directionFace[i],
				shadowMap);
			break;
		case road:
			mShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix,
				textureMgr->getTexture(L"roadDisplacement"), light, elapsedTime,
				textureMgr->getTexture(L"road"), textureMgr->getTexture(L"roadNormals"), directionFace[i],
				shadowMap);
			break;
		case building:
			mShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix,
				textureMgr->getTexture(L"buildingDisplacement"), light, elapsedTime,
				textureMgr->getTexture(L"building"), textureMgr->getTexture(L"buildingNormals"), directionFace[i],
				shadowMap);
			break;
		}
		mShader->render(renderer->getDeviceContext(), mesh[i]->getIndexCount());
	}

	// Draw Lamp Models
	{
		// ------------------------------------------------------ //
		worldMatrix = XMMatrixTranslation(2.193, 0.110, 15.614);
		worldMatrix *= XMMatrixScaling(100.f, 130.f, 100.f);
		// Render street lamps
		streetLamps[0]->sendData(renderer->getDeviceContext());
		lightShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix,
			textureMgr->getTexture(L"concrete"), light);
		lightShader->render(renderer->getDeviceContext(), streetLamps[0]->getIndexCount());

		// ----
		worldMatrix = renderer->getWorldMatrix();
		worldMatrix = XMMatrixTranslation(2.193, 0.110, 3.070);
		worldMatrix *= XMMatrixScaling(100.f, 130.f, 100.f);

		streetLamps[1]->sendData(renderer->getDeviceContext());

		lightShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix,
			textureMgr->getTexture(L"concrete"), light);
		lightShader->render(renderer->getDeviceContext(), streetLamps[1]->getIndexCount());

		//--
		worldMatrix = renderer->getWorldMatrix();
		worldMatrix = XMMatrixTranslation(-10.263, 0.110, -9.474);
		worldMatrix *= XMMatrixScaling(100.f, 130.f, 100.f);
		worldMatrix *= XMMatrixRotationY(XMConvertToRadians(180));

		streetLamps[2]->sendData(renderer->getDeviceContext());

		lightShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix,
			textureMgr->getTexture(L"concrete"), light);
		lightShader->render(renderer->getDeviceContext(), streetLamps[2]->getIndexCount());
		// ------------------------------------------------------ //
	}
	// Draw Spaceship Model
	worldMatrix = renderer->getWorldMatrix();
	worldMatrix = XMMatrixTranslation(spaceship.x, spaceship.y, spaceship.z);
	worldMatrix *= XMMatrixScaling(10.f, 10.f, 10.f);

	otherModels[0]->sendData(renderer->getDeviceContext());
	lightShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix,
		textureMgr->getTexture(L"concrete"), light);
	lightShader->render(renderer->getDeviceContext(), otherModels[0]->getIndexCount());

}

void App1::postProcessing()
{
	XMMATRIX worldMatrix, baseViewMatrix, orthoMatrix;

	float screenSizeY = (float)renderTexture2->getTextureWidth();
	renderTexture2->setRenderTarget(renderer->getDeviceContext());
	renderTexture2->clearRenderTarget(renderer->getDeviceContext(), 1.0f, 1.0f, 1.0f, 1.0f);

	worldMatrix = renderer->getWorldMatrix();
	baseViewMatrix = camera->getOrthoViewMatrix();
	orthoMatrix = renderTexture2->getOrthoMatrix();
	
	renderer->setZBuffer(false);
	orthoMesh->sendData(renderer->getDeviceContext());
	waterDropShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, baseViewMatrix, orthoMatrix, 
		renderTexture->getShaderResourceView(), textureMgr->getTexture(L"rainDrips"), 
		textureMgr->getTexture(L"rainDripMask"), elapsedTime, camera->getRotation());
	waterDropShader->render(renderer->getDeviceContext(), orthoMesh->getIndexCount());
	renderer->setZBuffer(true);

	// Reset the render target back to the original back buffer and not the render to texture anymore.
	renderer->setBackBufferRenderTarget();
}

void App1::finalPass()
{
	// Clear the scene. (default blue colour)
	renderer->beginScene(0.39f, 0.58f, 0.92f, 1.0f);

	// Generate the view matrix based on the camera's position.
	camera->update();


	// -------------------------- //

	renderer->setZBuffer(false);
	XMMATRIX worldMatrix = renderer->getWorldMatrix();
	XMMATRIX orthoMatrix = renderer->getOrthoMatrix();  // ortho matrix for 2D rendering
	XMMATRIX orthoViewMatrix = camera->getOrthoViewMatrix();	// Default camera position for orthographic rendering
	
	orthoMesh->sendData(renderer->getDeviceContext());
	waterDropShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, orthoViewMatrix, orthoMatrix,
		renderTexture->getShaderResourceView(), textureMgr->getTexture(L"rainDrips"),
		textureMgr->getTexture(L"rainDripMask"), elapsedTime, camera->getRotation());
	waterDropShader->render(renderer->getDeviceContext(), orthoMesh->getIndexCount());
	renderer->setZBuffer(true);
	// --------------------------- //

	// Render GUI
	gui();

	// Present the rendered scene to the screen.
	renderer->endScene();
}

// ----- Post Processing off: ------ //
void App1::depthPassPPOFF()
{

	// -- Instansiations -- //
	int elements;
	XMMATRIX worldMatrix, lightViewMatrix, lightProjectionMatrix;
	// -------------------- // 



	for (int j = 0; j < 2; ++j)
	{
		worldMatrix = renderer->getWorldMatrix();
		if (j == 0)
		{
			shadowMap[0]->BindDsvAndSetNullRenderTarget(renderer->getDeviceContext());
			lightViewMatrix = light[0]->getViewMatrix();
			lightProjectionMatrix = light[0]->getOrthoMatrix();
		}
		else
		{
			shadowMap[1]->BindDsvAndSetNullRenderTarget(renderer->getDeviceContext());
			lightViewMatrix = light[3]->getViewMatrix();
			lightProjectionMatrix = light[3]->getOrthoMatrix();

		}
		// Depth Pass the Lamp Models
		if (j == 1)
		{
			// ------------------------------------------------------ //
			worldMatrix = XMMatrixTranslation(2.193, 0.110, 15.614);
			worldMatrix *= XMMatrixScaling(100.f, 130.f, 100.f);
			// Render street lamps
			streetLamps[0]->sendData(renderer->getDeviceContext());
			depthShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, lightViewMatrix, lightProjectionMatrix);
			depthShader->render(renderer->getDeviceContext(), streetLamps[0]->getIndexCount());

			// ----
			worldMatrix = renderer->getWorldMatrix();
			worldMatrix = XMMatrixTranslation(2.193, 0.110, 3.070);
			worldMatrix *= XMMatrixScaling(100.f, 130.f, 100.f);

			streetLamps[1]->sendData(renderer->getDeviceContext());

			depthShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, lightViewMatrix, lightProjectionMatrix);
			depthShader->render(renderer->getDeviceContext(), streetLamps[1]->getIndexCount());

			// ----
			worldMatrix = renderer->getWorldMatrix();
			worldMatrix = XMMatrixTranslation(-10.263, 0.110, -9.474);
			worldMatrix *= XMMatrixScaling(100.f, 130.f, 100.f);
			worldMatrix *= XMMatrixRotationY(XMConvertToRadians(180));

			streetLamps[2]->sendData(renderer->getDeviceContext());

			depthShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, lightViewMatrix, lightProjectionMatrix);
			depthShader->render(renderer->getDeviceContext(), streetLamps[2]->getIndexCount());
			// ------------------------------------------------------ //

		}
		elements = mesh.size();

		// Send geometry data, set shader parameters, render object with shader
		for (int i = 0; i < elements; i++)
		{
			mesh[i]->sendData(renderer->getDeviceContext());
			depthShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, lightViewMatrix, lightProjectionMatrix);
			depthShader->render(renderer->getDeviceContext(), mesh[i]->getIndexCount());
			mShader->render(renderer->getDeviceContext(), mesh[i]->getIndexCount());
		}

		// Draw Spaceship Model
		worldMatrix = renderer->getWorldMatrix();
		worldMatrix = XMMatrixTranslation(spaceship.x, spaceship.y, spaceship.z);
		worldMatrix *= XMMatrixScaling(10.f, 10.f, 10.f);

		otherModels[0]->sendData(renderer->getDeviceContext());
		depthShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, lightViewMatrix, lightProjectionMatrix);
		depthShader->render(renderer->getDeviceContext(), otherModels[0]->getIndexCount());


		// Set back buffer as render target and reset view port.
		renderer->setBackBufferRenderTarget();
		renderer->resetViewport();
	}


}

void App1::finalPassPPOFF()
{
	// Clear the scene. (default blue colour)
	renderer->beginScene(0.39f, 0.58f, 0.92f, 1.0f);

	// Generate the view matrix based on the camera's position.
	camera->update();


	// -------------------------- //
	XMMATRIX worldMatrix = renderer->getWorldMatrix();
	XMMATRIX viewMatrix = camera->getViewMatrix();
	XMMATRIX projectionMatrix = renderer->getProjectionMatrix();

	elapsedTime += timer->getTime();

	int elements = mesh.size();


	// Send geometry data, set shader parameters, render object with shader
	for (int i = 0; i < elements; i++)
	{
		mesh[i]->sendData(renderer->getDeviceContext());
		switch (meshTexture[i]) {
		case pavement:
			mShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix,
				textureMgr->getTexture(L"pavementDisplacement"), light, elapsedTime,
				textureMgr->getTexture(L"pavement"), textureMgr->getTexture(L"pavementNormals"), directionFace[i],
				shadowMap);
			break;
		case concrete:
			mShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix,
				textureMgr->getTexture(L""), light, elapsedTime,
				textureMgr->getTexture(L"concrete"), textureMgr->getTexture(L"concreteNormals"), directionFace[i],
				shadowMap);
			break;
		case road:
			mShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix,
				textureMgr->getTexture(L"roadDisplacement"), light, elapsedTime,
				textureMgr->getTexture(L"road"), textureMgr->getTexture(L"roadNormals"), directionFace[i],
				shadowMap);
			break;
		case building:
			mShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix,
				textureMgr->getTexture(L"buildingDisplacement"), light, elapsedTime,
				textureMgr->getTexture(L"building"), textureMgr->getTexture(L"buildingNormals"), directionFace[i],
				shadowMap);
			break;
		}
		mShader->render(renderer->getDeviceContext(), mesh[i]->getIndexCount());
	}

	// Draw Lamp Models
	{
		// ------------------------------------------------------ //
		worldMatrix = XMMatrixTranslation(2.193, 0.110, 15.614);
		worldMatrix *= XMMatrixScaling(100.f, 130.f, 100.f);
		// Render street lamps
		streetLamps[0]->sendData(renderer->getDeviceContext());
		lightShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix,
			textureMgr->getTexture(L"concrete"), light);
		lightShader->render(renderer->getDeviceContext(), streetLamps[0]->getIndexCount());

		// ----
		worldMatrix = renderer->getWorldMatrix();
		worldMatrix = XMMatrixTranslation(2.193, 0.110, 3.070);
		worldMatrix *= XMMatrixScaling(100.f, 130.f, 100.f);

		streetLamps[1]->sendData(renderer->getDeviceContext());

		lightShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix,
			textureMgr->getTexture(L"concrete"), light);
		lightShader->render(renderer->getDeviceContext(), streetLamps[1]->getIndexCount());

		//--
		worldMatrix = renderer->getWorldMatrix();
		worldMatrix = XMMatrixTranslation(-10.263, 0.110, -9.474);
		worldMatrix *= XMMatrixScaling(100.f, 130.f, 100.f);
		worldMatrix *= XMMatrixRotationY(XMConvertToRadians(180));

		streetLamps[2]->sendData(renderer->getDeviceContext());

		lightShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix,
			textureMgr->getTexture(L"concrete"), light);
		lightShader->render(renderer->getDeviceContext(), streetLamps[2]->getIndexCount());
		// ------------------------------------------------------ //
	}
	// Draw Spaceship Model
	worldMatrix = renderer->getWorldMatrix();
	worldMatrix = XMMatrixTranslation(spaceship.x, spaceship.y, spaceship.z);
	worldMatrix *= XMMatrixScaling(10.f, 10.f, 10.f);

	otherModels[0]->sendData(renderer->getDeviceContext());
	lightShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix,
		textureMgr->getTexture(L"concrete"), light);
	lightShader->render(renderer->getDeviceContext(), otherModels[0]->getIndexCount());
	// --------------------------- //

	// Render GUI
	gui();

	// Present the rendered scene to the screen.
	renderer->endScene();
}
