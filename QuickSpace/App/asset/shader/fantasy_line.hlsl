#define PI 3.14159265358979323846
//
//	Textures
//
// Texture2D		g_texture0 : register(t0);
Texture2D		g_texture1 : register(t1);
SamplerState	g_sampler0 : register(s0);
SamplerState	g_sampler1 : register(s1);
SamplerState	g_sampler2 : register(s2);

namespace s3d
{
	//
	//	VS Output / PS Input
	//
	struct PSInput
	{
		float4 position	: SV_POSITION;
		float4 color	: COLOR0;
		float2 uv		: TEXCOORD0;
	};
}

//
//	Constant Buffer
//
cbuffer PSConstants2D : register(b0)
{
	float4 g_colorAdd;
	float4 g_sdfParam;
	float4 g_sdfOutlineColor;
	float4 g_sdfShadowColor;
	float4 g_internal;
}

cbuffer AnimCb : register(b1)
{
	float g_animRate;
}

float2 calcUvTex1(s3d::PSInput input)
{
	const float2 vecOne = {1.0f, 1.0f};
	const float uvStep = 96.0;

	const float scrollSpeed = 2.0;
	float2 uv = ((input.position.xy % uvStep) / uvStep) + ((scrollSpeed * g_animRate) % 1.0) * vecOne;

	const float waveStep = 2.0;
	const float waveY = sin(input.position.y / 180.0 * PI * waveStep);
	const float waveSpeed = 1.5;
	const float waveMax = 0.5;
	const float waveAmount = waveMax * sin(g_animRate * PI * waveSpeed);
	uv.x = uv.x + waveAmount * waveY;
	
	uv = (uv + vecOne) % 1.0;
	return uv;
}

float4 PS(s3d::PSInput input) : SV_TARGET
{
	float4 color0 = input.color;

	const float2 uv1 = calcUvTex1(input);

	float4 color1 = g_texture1.Sample(g_sampler1, uv1);
	float rate = (color1.x + color1.y + color1.z) / 3;

	color0 *= 1 + rate;
	// color0.rgb = (color0.rgb * 1.0 + color1.rgb * 1.0);

	// TODO: テクスチャ使わずに(-step, -step)にループUV動かす感じで直接発効させた方がいいかも
	
	return (color0 * input.color) + g_colorAdd;
}
