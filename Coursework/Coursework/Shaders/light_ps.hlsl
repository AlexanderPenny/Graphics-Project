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

Texture2D texture0 : register(t0);
SamplerState sampler0 : register(s0);

cbuffer LightBuffer : register(b0)
{
    float4 diffuseColour[4];
    float4 lightPosition[4];
    float4 ambientColour[4];
    float4 lightDirection[4];
};

struct InputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
    float3 worldPosition : TEXCOORD1;
};

float4 calculateLighting(float3 lightDir, float3 normal, float4 diffuse)
{
    float intensity = saturate(dot(normal, lightDir)) / 1.2; // Divided by 1.2 because it looks better if I do
    float4 colour = saturate(diffuse * intensity);
    return colour;
}

float4 main(InputType input) : SV_TARGET
{
    float4 textureColour;
    float4 lightColour[4];
    float3 lightVector[3];
    float cosTheta;
    bool isInsideCone;
    
	// Sample the texture. Calculate light intensity and colour, return light*texture for final pixel colour.
    textureColour = texture0.Sample(sampler0, input.tex);

    
    for (int i = 0; i < 3; i++)
    {
        lightVector[i] = normalize(lightPosition[i] - input.worldPosition);
        cosTheta = dot(-lightVector[i], lightDirection[i]);
        isInsideCone = cosTheta > 0.9;
        lightColour[i] = saturate(ambientColour[i] + calculateLighting(lightVector[i], input.normal, diffuseColour[i]) * isInsideCone);
    }
    lightColour[3] = ambientColour[3] + calculateLighting(lightDirection[3], input.normal, diffuseColour[3]);
    
    
    return lightColour[0] * textureColour +
       lightColour[1] * textureColour +
       lightColour[2] * textureColour +
       lightColour[3] * textureColour;
}
