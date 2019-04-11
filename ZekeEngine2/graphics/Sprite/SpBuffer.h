#pragma once
extern void InitVertexBuffer(ID3D11Buffer*& vertexBuffer, float w, float h);
extern void InitIndexBuffer(ID3D11Buffer*& indexBuffer);
extern void InitSamplerState(ID3D11SamplerState*& samplerState);

struct SVertex {
	float position[4];		
	float uv[2];			
};