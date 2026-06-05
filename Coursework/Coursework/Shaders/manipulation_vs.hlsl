Texture2D texture0 : register(t0);
Texture2D texture1 : register(t1);

SamplerState sampler0 : register(s0);

cbuffer MatrixBuffer : register(b0)
{
    matrix worldMatrix;
    matrix viewMatrix;
    matrix projectionMatrix;
    matrix lightViewMatrix[4];
    matrix lightProjectionMatrix[4];
};

struct InputType
{
    float4 position : POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
};

struct OutputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
    float3 worldPosition : TEXCOORD1;
    float4 lightViewPos0: TEXCOORD2;
    float4 lightViewPos1: TEXCOORD4;
    float4 lightViewPos3 : TEXCOORD3;
};

cbuffer TimeBufferType : register(b1)
{
    float time;
    float direction;
    float2 padding;
};

float GetHeight(float2 uv)
{
    return (texture0.SampleLevel(sampler0, uv, 0).r) * 30.0f;
}

OutputType main(InputType input)
{
    //---Declaration of Variables---// 
    OutputType output;             
    float3 textureNormals;           
    //------------------------------// 
	
    output.position = input.position;
    output.normal = input.normal;
    
    switch (direction)
    {
       case 0: // facing up
            output.position.y += GetHeight(input.tex);
            break; // ---
        case 1: // facing down
            output.position.y -= GetHeight(input.tex);
            break; // ---
        case 2: // facing left
            output.position.x += GetHeight(input.tex);
            input.normal.xyz = input.normal.yzx;
            break; // ---
        case 3: // facing right
            output.position.x -= GetHeight(input.tex);
            input.normal.xyz = input.normal.yzx;
            break; // ---
        case 4: // facing forward
            output.position.z += GetHeight(input.tex);
            input.normal.y = -input.normal.y;
            input.normal.xyz = input.normal.yzx;
            break; // ---
        case 5: // facing backward
            output.position.z -= GetHeight(input.tex);
            input.normal.xyz = input.normal.xzy;
            break; // ---
        default: // facing upward
            output.position.y += GetHeight(input.tex);
            break;
    }

	
	//---Manipulation of the World---//
    // Calculate the position of the vertice as viewed by the light source
    output.lightViewPos0 = mul(output.position, worldMatrix);
    output.lightViewPos0 = mul(output.lightViewPos0, lightViewMatrix[0]);
    output.lightViewPos0 = mul(output.lightViewPos0, lightProjectionMatrix[0]);
    // ----
    output.lightViewPos3 = mul(output.position, worldMatrix);
    output.lightViewPos3 = mul(output.lightViewPos3, lightViewMatrix[3]);
    output.lightViewPos3 = mul(output.lightViewPos3, lightProjectionMatrix[3]);
    // ----
    output.lightViewPos1 = mul(output.position, worldMatrix);
    output.lightViewPos1 = mul(output.lightViewPos3, lightViewMatrix[1]);
    output.lightViewPos1 = mul(output.lightViewPos3, lightProjectionMatrix[1]);
    // ----
    output.position = mul(output.position, worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix); /*
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


	//-------------------------------//

	// Store the texture coordinates for the pixel shader.
    output.tex = input.tex;

	// --- Calculate the normals and then apply them to the world --- //
    textureNormals = texture1.SampleLevel(sampler0, input.tex, 0).rgb;
    textureNormals = normalize(textureNormals - 1.0f);
    textureNormals = mul(textureNormals, (float3x3) worldMatrix);

   // output.normal = mul(input.normal, (float3x3) worldMatrix);
    output.normal = normalize(input.normal + textureNormals);
    output.normal = mul(output.normal, (float3x3) worldMatrix);
    output.normal = normalize(output.normal);
    // -------------------------------------------------------------- // 
    output.worldPosition = mul(input.position, worldMatrix).zyx;
    return output;
}