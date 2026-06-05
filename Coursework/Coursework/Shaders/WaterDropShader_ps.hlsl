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

Texture2D rainDrips : register(t0);
Texture2D rainDripMask : register(t1);
Texture2D ShaderTexture : register(t2);

SamplerState SampleType : register(s0);

struct InputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
    float3 worldPosition : TEXCOORD1;
    float3 normal : NORMAL;
};

cbuffer TimeBufferType : register(b0)
{
    float time;
    float3 cameraRotation;
};

float4 calculateLighting(float3 lightDir, float3 normal, float4 diffuse)
{
    float intensity = saturate(dot(normal, lightDir)) / 1.2; // Divided by 1.2 because it looks better if I do
    float4 colour = saturate(diffuse * intensity);
    return colour;
}

float4 main(InputType input) : SV_TARGET
{
    float4 defaultImage = ShaderTexture.Sample(SampleType, input.tex);
    float4 colour = defaultImage;
    // -------------------------------------------------------------- //
    float rainOffset = ((time / 2) + sin(time) / 5) * (0.1 + rainDrips.Sample(SampleType, input.tex).a);
    
    float3 dripLocations = float3(round(rainDrips.Sample(SampleType, input.tex).b -0.38), 0.2, 0.75);
    
    float texSampleDripMapXTime = rainDrips.Sample(SampleType, input.tex).b + time;
    float lerpOutput = lerp(dripLocations.g, dripLocations.b, rainDrips.Sample(SampleType, input.tex).b);
    
    input.normal = rainDrips.Sample(SampleType, input.tex);
    input.normal *= 2;
    
    input.normal *= (rainDripMask.Sample(SampleType, input.tex - rainOffset)) * lerpOutput;
    input.normal *= dripLocations.x;
    
    return saturate(calculateLighting(1, input.normal, colour)+colour * 0.9);
}

