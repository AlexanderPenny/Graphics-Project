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
Texture2D depthMapTexture0 : register(t1);
Texture2D depthMapTexture3 : register(t2);

SamplerState sampler0 : register(s0);
SamplerState shadowSampler : register(s1);

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
    float4 lightViewPos0 : TEXCOORD2;
    float4 lightViewPos1 : TEXCOORD4;
    float4 lightViewPos3 : TEXCOORD3;
};

float4 calculateLighting(float3 lightDir, float3 normal, float4 diffuse)
{
    float intensity = saturate(dot(normal, lightDir))/1.2; // Divided by 1.2 because it looks better if I do
    float4 colour = saturate(diffuse * intensity);
    return colour;
}

// Is the gemoetry in our shadow map
bool hasDepthData(float2 uv)
{
    if (uv.x < 0.f || uv.x > 1.f || uv.y < 0.f || uv.y > 1.f)
    {
        return false;
    }
    return true;
}

bool isInShadow(Texture2D sMap, float2 uv, float4 lightViewPosition, float bias)
{
    // Sample the shadow map (get depth of geometry)
    float depthValue = sMap.Sample(shadowSampler, uv).r;
	// Calculate the depth from the light.
    float lightDepthValue = lightViewPosition.z / lightViewPosition.w;
    lightDepthValue -= bias;

	// Compare the depth of the shadow map value and the depth of the light to determine whether to shadow or to light this pixel.
    if (lightDepthValue < depthValue)
    {
        return false;
    }
    return true;
}

float2 getProjectiveCoords(float4 lightViewPosition)
{
    // Calculate the projected texture coordinates.
    float2 projTex = lightViewPosition.xy / lightViewPosition.w;
    projTex *= float2(0.5, -0.5);
    projTex += float2(0.5f, 0.5f);
    return projTex;
}

float4 main(InputType input) : SV_TARGET
{
    float shadowMapBias = 0.002f;
	float4 textureColour;
    float4 lightColour[4] = { { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 } };
    float3 lightVector[3];
    float cosTheta;
    bool isInsideCone;
    
	// Sample the texture. Calculate light intensity and colour, return light*texture for final pixel colour.
	textureColour = texture0.Sample(sampler0, input.tex);
    
    // Calculate the projected texture coordinates.
    float2 pTexCoord = getProjectiveCoords(input.lightViewPos0);
    
    // Shadow test. Is or isn't in shadow
    if (hasDepthData(pTexCoord))
    {
        
        // point lights
        for (int i = 0; i < 3; i++)
        {
            if (!isInShadow(depthMapTexture0, pTexCoord, input.lightViewPos0, shadowMapBias))
            {
                lightVector[i] = normalize(lightPosition[i] - input.worldPosition);
                cosTheta = dot(-lightVector[i], lightDirection[i]);
                isInsideCone = cosTheta > 0.9;
                lightColour[i] = saturate(ambientColour[i] + calculateLighting(lightVector[i], input.normal, diffuseColour[i]) * isInsideCone);
            }
            else
            {
                lightColour[i] = saturate(ambientColour[i]);
            }

        }
        // --------------------------------------------------------------------------------------------------- //
        pTexCoord = getProjectiveCoords(input.lightViewPos3);
        // directional light
        if (!isInShadow(depthMapTexture3, pTexCoord, input.lightViewPos3, shadowMapBias))
        {
            lightColour[3] = saturate(ambientColour[3] + calculateLighting(lightDirection[3], input.normal, diffuseColour[3]));
        }
        else
        {
            return saturate((ambientColour[3] + ambientColour[1] + ambientColour[2] + ambientColour[0]) * textureColour + (lightColour[0] * textureColour + lightColour[1] * textureColour + lightColour[2] * textureColour + lightColour[3] * textureColour));

        }
        
        return saturate(lightColour[0] * textureColour + lightColour[1] * textureColour + lightColour[2] * textureColour + lightColour[3] * textureColour + (ambientColour[3] + ambientColour[1] + ambientColour[2] + ambientColour[0]) * textureColour);
    }
    else
    {
        return saturate((ambientColour[3] + ambientColour[1] + ambientColour[2] + ambientColour[0]) * textureColour + (lightColour[0] * textureColour + lightColour[1] * textureColour + lightColour[2] * textureColour + lightColour[3] * textureColour));

    }
    
}
