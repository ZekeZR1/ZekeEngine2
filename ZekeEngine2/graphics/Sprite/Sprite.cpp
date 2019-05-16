#include "stdafx.h"
#include "Sprite.h"
#include "SpBuffer.h"

struct SSimpleVertex {
	CVector4 pos;
	CVector2 tex;
};

const CVector2	Sprite::DEFAULT_PIVOT = { 0.5f, 0.5f };

Sprite::Sprite()
{
}

Sprite::~Sprite()
{
	if (m_vertexBuffer != NULL) {
		m_vertexBuffer->Release();
	}
	if (m_indexBuffer != NULL) {
		m_indexBuffer->Release();
	}
	if (m_samplerState != NULL) {
		m_samplerState->Release();
	}
	if (m_texture != NULL) {
		m_texture->Release();
	}
	if (pBlendState != NULL) {
		pBlendState->Release();
	}
	if (spriteRender != NULL) {
		spriteRender->Release();
	}
	if (rspriteRender != NULL) {
		rspriteRender->Release();
	}
}

void Sprite::Update(const CVector3& trans, const CQuaternion& rot, const CVector3& scale, CVector2 pivot)
{
	CVector2 localPivot = pivot;
	localPivot.x -= 0.5f;
	localPivot.y -= 0.5f;
	localPivot.x *= -2.0f;
	localPivot.y *= -2.0f;
	CVector2 halfSize = m_size;
	halfSize.x *= 0.5f;
	halfSize.y *= 0.5f;
	CMatrix mPivotTrans;

	mPivotTrans.MakeTranslation(
		{ halfSize.x * localPivot.x, halfSize.y * localPivot.y, 0.0f }
	);
	CMatrix mTrans, mRot, mScale;
	mTrans.MakeTranslation(trans);
	mRot.MakeRotationFromQuaternion(rot);
	mScale.MakeScaling(scale);
	m_world.Mul(mPivotTrans, mScale);
	m_world.Mul(m_world, mRot);
	m_world.Mul(m_world, mTrans);
}

void Sprite::InitConstantBuffer()
{
	D3D11_BUFFER_DESC desc;

	ZeroMemory(&desc, sizeof(desc));
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.ByteWidth = (((sizeof(ConstantBuffer) - 1) / 16) + 1) * 16;
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc.CPUAccessFlags = 0;
	GraphicsEngine().GetD3DDevice()->CreateBuffer(&desc, NULL, &m__cb);
}


void Sprite::InitVertexBuffer(float w, float h)
{
	//上で定義した頂点を使用して頂点バッファを作成する。
	//頂点バッファを作成するためにはD3D11_BUFFER_DESCとD3D11_SUBRESOURCE_DATAを設定する必要がある。
	float halfW = w * 0.5f;
	float halfH = h * 0.5f;
	SVertex vertex[4] = {
		//頂点１
		{
			//座標　position[4]
			-halfW,  -halfH, 0.0f, 1.0f,
			//UV座標 uv[2]
			0.0f, 1.0f
		},
		//頂点２
		{
			//座標　position[4]
			halfW, -halfH, 0.0f, 1.0f,
			//UV座標 uv[2]
			1.0f, 1.0f
		},
		//頂点３
		{
			//座標　position[4]
			-halfW,  halfH, 0.0f, 1.0f,
			//UV座標 uv[2]
			0.0f, 0.0f
		},
		//頂点４
		{
			//座標　position[4]
			halfW,  halfH, 0.0f, 1.0f,
			//UV座標 uv[2]
			1.0f, 0.0f
		},
	};
	//上で定義した頂点を使用して頂点バッファを作成する。
	//頂点バッファを作成するためにはD3D11_BUFFER_DESCとD3D11_SUBRESOURCE_DATAを設定する必要がある。
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));				//構造体を0で初期化する。
	bd.Usage = D3D11_USAGE_DEFAULT;				//バッファーで想定されている読み込みおよび書き込みの方法。
												//取りあえずはD3D11_USAGE_DEFAULTでよい。
	bd.ByteWidth = sizeof(vertex);				//頂点バッファのサイズ。頂点のサイズ×頂点数となる。
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;	//これから作成するバッファが頂点バッファであることを指定する。

	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = vertex;

	//頂点バッファの作成。
	GraphicsEngine().GetD3DDevice()->CreateBuffer(&bd, &InitData, &m_vertexBuffer);
}

void Sprite::InitIndexBuffer()
{
	//これがインデックス。頂点番号。
	int index[6] = {
		0,1,2,		//三角形一つ目
		2,1,3		//三角形二つ目
	};
	//上で定義したインデックスを使用してインデックスバッファを作成する。
	//インデックスバッファの作成も頂点バッファと同様に、
	//D3D11_BUFFER_DESCとD3D11_SUBRESOURCE_DATAを設定する必要がある。
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));				//構造体を0で初期化する。
	bd.Usage = D3D11_USAGE_DEFAULT;				//バッファーで想定されている読み込みおよび書き込みの方法。
												//取りあえずはD3D11_USAGE_DEFAULTでよい。
	bd.ByteWidth = sizeof(index);				//インデックスバッファのサイズ。
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;		//これから作成するバッファがインデックスバッファであることを指定する。

	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = index;

	//インデックスバッファの作成。
	GraphicsEngine().GetD3DDevice()->CreateBuffer(
		&bd, &InitData, &m_indexBuffer
	);
}

void Sprite::InitSamplerState()
{
	D3D11_SAMPLER_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	desc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	desc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	GraphicsEngine().GetD3DDevice()->CreateSamplerState(&desc, &m_samplerState);
}

void Sprite::InitCommon(float w, float h)
{
	m_size.x = w;
	m_size.y = h;
	//頂点バッファの初期化。
	InitVertexBuffer(w, h);
	//インデックスバッファの初期化。
	InitIndexBuffer();
	//サンプラステートの初期化。
	InitSamplerState();
	//シェーダーのロード。
	m_vs.Load("Assets/shader/sprite.fx", "VSMain", Shader::EnType::VS);
	m_ps.Load("Assets/shader/sprite.fx", "PSMain", Shader::EnType::PS);
	m_pss.Load("Assets/shader/sprite.fx", "PSMainn", Shader::EnType::PS);

	//定数バッファを初期化。
	InitConstantBuffer();

	CD3D11_DEFAULT def;
	CD3D11_BLEND_DESC BlendDesc(def);
	BlendDesc.AlphaToCoverageEnable = TRUE;
	BlendDesc.IndependentBlendEnable = TRUE;
	BlendDesc.RenderTarget[0].BlendEnable = TRUE;
	BlendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	BlendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	BlendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	GraphicsEngine().GetD3DDevice()->CreateBlendState(&BlendDesc, &pBlendState);
}

void Sprite::Init(ID3D11ShaderResourceView* srv, float w, float h)
{
	InitCommon(w, h);
	m_texture = srv;
	if (m_texture != nullptr) {
		m_texture->AddRef();	
	}
}

void Sprite::Init(const wchar_t* texFilePath, float w, float h)
{
	m_size.x = w;
	m_size.y = h;
	::InitVertexBuffer(m_vertexBuffer, w, h);
	::InitIndexBuffer(m_indexBuffer);
	::InitSamplerState(m_samplerState);
	m_effect.Load("Assets/shader/sprite.fx");
	wchar_t filePath[256];
	DirectX::CreateDDSTextureFromFileEx(
		GraphicsEngine().GetD3DDevice(),
		texFilePath,
		0,

		D3D11_USAGE_DEFAULT,
		D3D11_BIND_SHADER_RESOURCE,
		0,
		0,
		false,
		nullptr,
		&m_texture
	);
	InitConstantBuffer();

	D3D11_BLEND_DESC blendDesc;
	ZeroMemory(&blendDesc, sizeof(blendDesc));
	ID3D11Device* pd3d = GraphicsEngine().GetD3DDevice();
	blendDesc.RenderTarget[0].BlendEnable = true;
	//blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	//blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_RED | D3D11_COLOR_WRITE_ENABLE_BLUE | D3D11_COLOR_WRITE_ENABLE_GREEN;
	pd3d->CreateBlendState(&blendDesc, &pBlendState);
	{
		{
			D3D11_DEPTH_STENCIL_DESC desc;
			ZeroMemory(&desc, sizeof(desc));
			ID3D11Device* pd3d = GraphicsEngine().GetD3DDevice();
			desc.DepthEnable = true;
			desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
			desc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
			desc.StencilEnable = false;
			desc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
			desc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
			desc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
			desc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

			desc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
			desc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
			desc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
			desc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
			desc.DepthEnable = false;
			desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
			pd3d->CreateDepthStencilState(&desc, &spriteRender);
		}
		{
			D3D11_RASTERIZER_DESC desc = {};
			ID3D11Device* pd3d = GraphicsEngine().GetD3DDevice();
			desc.CullMode = D3D11_CULL_FRONT;
			desc.FillMode = D3D11_FILL_SOLID;
			desc.DepthClipEnable = true;
			desc.MultisampleEnable = true;
			pd3d->CreateRasterizerState(&desc, &rspriteRender);
		}
	}
}

void Sprite::Draw()
{
	m_effect.BeginRender();
	unsigned int vertexSize = sizeof(SVertex);
	unsigned int offset = 0;
	GraphicsEngine().GetD3DDeviceContext()->IASetVertexBuffers(
		0,
		1,
		&m_vertexBuffer,
		&vertexSize,
		&offset
	);
	GraphicsEngine().GetD3DDeviceContext()->IASetIndexBuffer(
		m_indexBuffer,
		DXGI_FORMAT_R32_UINT,
		0
	);
	////
	////シェーダーを設定。
	//ge->GetD3DDeviceContext()->VSSetShader(
	//	(ID3D11VertexShader*)m_vs.GetBody(),
	//	nullptr,
	//	0
	//);
	//ge->GetD3DDeviceContext()->PSSetShader(
	//	(ID3D11PixelShader*)m_ps.GetBody(),
	//	nullptr,
	//	0
	//);
	//ge->GetD3DDeviceContext()->IASetInputLayout(m_vs.GetInputLayout());
	////
	float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	GraphicsEngine().GetD3DDeviceContext()->OMSetBlendState(pBlendState, blendFactor, 0xffffffff);
	GraphicsEngine().GetD3DDeviceContext()->PSSetShaderResources(0, 1, &m_texture);
	GraphicsEngine().GetD3DDeviceContext()->PSSetSamplers(0, 1, &m_samplerState);
	//g_graphicsEngine->GetD3DDeviceContext()->OMSetDepthStencilState(spriteRender, 0);
	//g_graphicsEngine->GetD3DDeviceContext()->RSSetState(rspriteRender);
	ConstantBuffer cb;
	cb.WVP = m_world;
	if (m_cameraMode == Camera::enUpdateProjMatrixFunc_Ortho) {
		cb.WVP.Mul(cb.WVP, MainCamera2D().GetViewMatrix());
		cb.WVP.Mul(cb.WVP, MainCamera2D().GetProjectionMatrix());
	}
	else if(m_cameraMode == Camera::enUpdateProjMatrixFunc_Perspective){
		cb.WVP.Mul(cb.WVP, MainCamera().GetViewMatrix());
		cb.WVP.Mul(cb.WVP, MainCamera().GetProjectionMatrix());
	}
	cb.mulCol = m_mulCol;
	GraphicsEngine().GetD3DDeviceContext()->UpdateSubresource(m__cb, 0, NULL, &cb, 0, 0);
	GraphicsEngine().GetD3DDeviceContext()->VSSetConstantBuffers(0, 1, &m__cb);
	GraphicsEngine().GetD3DDeviceContext()->PSSetConstantBuffers(0, 1, &m__cb);
	GraphicsEngine().GetD3DDeviceContext()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	GraphicsEngine().GetD3DDeviceContext()->DrawIndexed(
		6,
		0,
		0
	);
}

//TODO : この名前なんとかする
void Sprite::Draww()
{
	auto d3dDeviceContext = GraphicsEngine().GetD3DDeviceContext();

	unsigned int vertexSize = sizeof(SVertex);	//頂点のサイズ。
	unsigned int offset = 0;
	d3dDeviceContext->IASetVertexBuffers(	//頂点バッファを設定。
		0,					//StartSlot番号。今は0でいい。
		1,					//バッファの数。今は1でいい。
		&m_vertexBuffer,	//頂点バッファ。
		&vertexSize,		//頂点のサイズ。
		&offset				//気にしなくてよい。
	);
	d3dDeviceContext->IASetIndexBuffer(	//インデックスバッファを設定。
		m_indexBuffer,			//インデックスバッファ。
		DXGI_FORMAT_R32_UINT,	//インデックスのフォーマット。
								//今回は32bitなので、DXGI_FORMAT_R32_UINTでいい。
		0						//オフセット0でいい。
	);
	//シェーダーを設定。
	d3dDeviceContext->VSSetShader(
		(ID3D11VertexShader*)m_vs.GetBody(),
		nullptr,
		0
	);
	d3dDeviceContext->PSSetShader(
		(ID3D11PixelShader*)m_pss.GetBody(),
		nullptr,
		0
	);
	d3dDeviceContext->IASetInputLayout(m_vs.GetInputLayout());
	//テクスチャを設定。
	d3dDeviceContext->PSSetShaderResources(0, 1, &m_texture);
	//サンプラステートを設定。
	d3dDeviceContext->PSSetSamplers(0, 1, &m_samplerState);
	ConstantBuffer cb;
	cb.WVP = m_world;
	cb.WVP.Mul(cb.WVP, MainCamera2D().GetViewMatrix());
	cb.WVP.Mul(cb.WVP, MainCamera2D().GetProjectionMatrix());
	cb.mulCol = m_mulCol;
	GraphicsEngine().GetD3DDeviceContext()->UpdateSubresource(m__cb, 0, NULL, &cb, 0, 0);
	GraphicsEngine().GetD3DDeviceContext()->VSSetConstantBuffers(0, 1, &m__cb);
	GraphicsEngine().GetD3DDeviceContext()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	GraphicsEngine().GetD3DDeviceContext()->DrawIndexed(
		6,
		0,
		0
	);
}