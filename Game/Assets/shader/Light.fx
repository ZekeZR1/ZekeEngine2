struct VS_IN
{
	float4 pos : POSITION0;
	float4 nor : NORMAL0;
};

struct VS_OUT
{
	float4 pos  : SV_POSITION;
	float4 posw : POSITION0;
	float4 norw : NORMAL0;
};

cbuffer VSCb : ConstantBuffer : register(b0)
{
	float4x4 World;         
	float4x4 View;          
	float4x4 Projection;    
}

VS_OUT vs_main(VS_IN input)
{
	VS_OUT output;
	float3 nor;
	float  col;

	output.posw = mul(input.pos, World);
	output.pos = mul(output.posw, View);
	output.pos = mul(output.pos, Projection);

	output.norw = mul(input.nor, World);

	return output;
}

struct PS_IN
{
	float4 pos  : SV_POSITION;
	float4 posw : POSITION0;
	float4 norw : NORMAL0;
};

cbuffer PSCb : ConstantBuffer : register(b0)
{
	float4   Light;         //光源座標
	float4   Attenuation;   //光源減衰パラメータ
}

float4 ps_main(PS_IN input) : SV_Target{
	float3 dir;
	float  len;
	float  colD;
	float  colA;
	float  col;

	//点光源の方向
	dir = Light.xyz - input.posw.xyz;

	//点光源の距離
	len = length(dir);

	//点光源の方向をnormalize
	dir = dir / len;

	//拡散
	colD = saturate(dot(normalize(input.norw.xyz), dir));
	//減衰
	colA = saturate(1.0f / (Attenuation.x + Attenuation.y * len + Attenuation.z * len * len));

	col = colD * colA;
	return float4(col, col, col, 1.0f);
}