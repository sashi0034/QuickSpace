#define PI 3.14159265358979323846
//
//	Textures
//
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

float4 PS(s3d::PSInput input) : SV_TARGET
{
	float4 color0 = input.color;

	float snappedStep = 144.0;
	const float scrollSpeed = 2.0;
	const float2 vecOne = {1.0f, 1.0f};
	float2 snappedOffset = ((scrollSpeed * g_animRate) % 1.0) * vecOne * snappedStep;
	float2 snappedPos = floor((input.position.xy + snappedOffset) / snappedStep) * snappedStep - snappedOffset;
	float snappedManhattanDelta = input.position.x + input.position.y - snappedPos.x - snappedPos.y;

	// float rate = (snappedStep * 2 - snappedManhattanDelta) / (snappedStep * 2);
	// color0.x += (1.0f - color0.x) * rate;
	// color0.y += (1.0f - color0.y) * rate;
	// color0.z += (1.0f - color0.z) * rate;

	const float brightnessIntensity = sqrt(PI);
	float rate = 1 + brightnessIntensity * (snappedStep * 2 - snappedManhattanDelta) / (snappedStep * 2);
	color0 *= rate;
	
	return (color0 * input.color) + g_colorAdd;
}
