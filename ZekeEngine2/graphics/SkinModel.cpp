#include "stdafx.h"
#include "SkinModel.h"
#include "SkinModelDataManager.h"
#include "SkinModelEffect.h"

SkinModel::~SkinModel()
{
	if (m_cb != nullptr) {
		//定数バッファを解放。
		m_cb->Release();
	}
	if (m_samplerState != nullptr) {
		//サンプラステートを解放。
		m_samplerState->Release();
	}
}

void SkinModel::Init(const wchar_t* filePath, EnFbxUpAxis enFbxUpAxis,
	const char* entryPS, const char* entryVS,
	const wchar_t* normalMap, const wchar_t* specularMap)
{
	m_psmain = entryPS;
	m_vsmain = entryVS;
	//スケルトンのデータを読み込む。

	InitSkeleton(filePath);

	//定数バッファの作成。
	InitConstantBuffer();

	//サンプラステートの初期化。
	InitSamplerState();

	//ディレクションライトの初期化
	InitDirectionLight();
	if (normalMap) {
		m_hasNormalMap = true;
	}
	if (specularMap)
		m_hasSpecularMap = true;
	//SkinModelDataManagerを使用してCMOファイルのロード。
	m_modelDx = g_skinModelDataManager.Load(filePath, m_skeleton, m_psmain, m_vsmain, normalMap, specularMap);
	m_enFbxUpAxis = enFbxUpAxis;
}
void SkinModel::InitSkeleton(const wchar_t* filePath)
{
	//スケルトンのデータを読み込む。
	//cmoファイルの拡張子をtksに変更する。
	std::wstring skeletonFilePath = filePath;
	//文字列から.cmoファイル始まる場所を検索。
	int pos = (int)skeletonFilePath.find(L".cmo");
	//.cmoファイルを.tksに置き換える。
	skeletonFilePath.replace(pos, 4, L".tks");
	//tksファイルをロードする。
	bool result = m_skeleton.Load(skeletonFilePath.c_str());
	if (result == false) {
		//assert(false);
		//スケルトンが読み込みに失敗した。
#ifdef _DEBUG
		char message[256];
		//sprintf_s(message, "tksファイルの読み込みに失敗しました。%ls\n", skeletonFilePath.c_str());
		OutputDebugStringA(message);
#endif
	}
	else {
		if (strcmp(m_vsmain, "VSMain") == 0) {
			m_vsmain = "VSMainSkin";
		}
	}
}
void SkinModel::InitConstantBuffer()
{
	//作成するバッファのサイズをsizeof演算子で求める。
	int bufferSize = sizeof(SVSConstantBuffer);
	//どんなバッファを作成するのかをせてbufferDescに設定する。
	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));				//０でクリア。
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;						//バッファで想定されている、読み込みおよび書き込み方法。
	bufferDesc.ByteWidth = (((bufferSize - 1) / 16) + 1) * 16;	//バッファは16バイトアライメントになっている必要がある。
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;			//バッファをどのようなパイプラインにバインドするかを指定する。
																//定数バッファにバインドするので、D3D11_BIND_CONSTANT_BUFFERを指定する。
	bufferDesc.CPUAccessFlags = 0;
	GraphicsEngine().GetD3DDevice()->CreateBuffer(&bufferDesc, NULL, &m_cb);
}

void SkinModel::InitDirectionLight() {
	for (int i = 0; i < NUM_DIRECTION_LIG; i++) {
		m_DirLight[i] = m_defDir;
		m_DirCol[i] = m_defCol;
	}
}

void SkinModel::InitSamplerState()
{
	//テクスチャのサンプリング方法を指定するためのサンプラステートを作成。
	D3D11_SAMPLER_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	desc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	desc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	GraphicsEngine().GetD3DDevice()->CreateSamplerState(&desc, &m_samplerState);
}
void SkinModel::UpdateWorldMatrix(CVector3 position, CQuaternion rotation, CVector3 scale)
{
	//3dsMaxと軸を合わせるためのバイアス。
	CMatrix mBias = CMatrix::Identity();
	if (m_enFbxUpAxis == enFbxUpAxisZ) {
		//Z-up
		mBias.MakeRotationX(CMath::PI * -0.5f);
	}
	CMatrix transMatrix, rotMatrix, scaleMatrix;
	//平行移動行列を作成する。
	transMatrix.MakeTranslation(position);
	//回転行列を作成する。
	rotMatrix.MakeRotationFromQuaternion(rotation);
	//拡大行列を作成する。
	scaleMatrix.MakeScaling(scale);
	//ワールド行列を作成する。
	//拡大×回転×平行移動の順番で乗算する
	m_worldMatrix.Mul(mBias, scaleMatrix);
	m_worldMatrix.Mul(m_worldMatrix, rotMatrix);
	m_worldMatrix.Mul(m_worldMatrix, transMatrix);

	//スケルトンの更新。
	m_skeleton.Update(m_worldMatrix);
}

void SkinModel::Draw(CMatrix viewMatrix, CMatrix projMatrix)
{
	DirectX::CommonStates state(GraphicsEngine().GetD3DDevice());

	ID3D11DeviceContext* d3dDeviceContext = GraphicsEngine().GetD3DDeviceContext();

	//定数バッファの内容を更新。
	SVSConstantBuffer vsCb;
	vsCb.mWorld = m_worldMatrix;
	vsCb.mProj = projMatrix;
	vsCb.mView = viewMatrix;
	for (int i = 0; i < NUM_DIRECTION_LIG; i++) {
		vsCb.mCol[i] = m_DirCol[i];
		vsCb.mDir[i] = m_DirLight[i];
	}
	d3dDeviceContext->UpdateSubresource(m_cb, 0, nullptr, &vsCb, 0, 0);
	//定数バッファをGPUに転送。
	d3dDeviceContext->VSSetConstantBuffers(0, 1, &m_cb);
	d3dDeviceContext->PSSetConstantBuffers(0, 1, &m_cb);
	//サンプラステートを設定。
	d3dDeviceContext->PSSetSamplers(0, 1, &m_samplerState);
	//ボーン行列をGPUに転送。
	m_skeleton.SendBoneMatrixArrayToGPU();
	//描画。
	m_modelDx->Draw(
		d3dDeviceContext,
		state,
		m_worldMatrix,
		viewMatrix,
		projMatrix
	);
}

void SkinModel::Draw(EnRenderMode renderMode, CMatrix viewMatrix, CMatrix projMatrix)
{
	auto deviceContext = GraphicsEngine().GetD3DDeviceContext();
	DirectX::CommonStates state(GraphicsEngine().GetD3DDevice());

	//auto shadowMap = g_game->GetShadowMap();
	auto shadowMap = IGameObjectManager().GetShadowMap();
	//定数バッファを更新。
	SVSConstantBuffer modelFxCb;
	modelFxCb.mWorld = m_worldMatrix;
	modelFxCb.mProj = projMatrix;
	modelFxCb.mView = viewMatrix;

	for (int i = 0; i < NUM_DIRECTION_LIG; i++) {
		modelFxCb.mCol[i] = m_DirCol[i];
		modelFxCb.mDir[i] = m_DirLight[i];
	}
	modelFxCb.eyePos = MainCamera().GetPosition();
	modelFxCb.specPow = m_specPow;
	//todo ライトカメラのビュー、プロジェクション行列を送る。
	modelFxCb.mLightProj = shadowMap->GetLightProjMatrix();
	modelFxCb.mLightView = shadowMap->GetLighViewMatrix();
	if (m_isShadowReciever) {
		modelFxCb.isShadowReciever = 1;
	}
	else {
		modelFxCb.isShadowReciever = 0;
	}
	if (m_hasNormalMap) {
		modelFxCb.hasNormalMap = 1;
	}
	else {
		modelFxCb.hasNormalMap = 0;
	}
	if (m_hasSpecularMap) {
		modelFxCb.hasSpecularMap = 1;
	}
	else {
		modelFxCb.hasSpecularMap = 0;
	}
	modelFxCb.ambientLight = GraphicsEngine().GetAmbientLight();
	deviceContext->UpdateSubresource(m_cb, 0, nullptr, &modelFxCb, 0, 0);
	//ライト用の定数バッファを更新。
	//deviceContext->UpdateSubresource(m_lightCb, 0, nullptr, &m_dirLight, 0, 0);

	//定数バッファをシェーダースロットに設定。
	deviceContext->VSSetConstantBuffers(0, 1, &m_cb);
	deviceContext->PSSetConstantBuffers(0, 1, &m_cb);
	//deviceContext->PSSetConstantBuffers(1, 1, &m_lightCb);
	//サンプラステートを設定する。
	deviceContext->PSSetSamplers(0, 1, &m_samplerState);
	//エフェクトにクエリを行う。
	m_modelDx->UpdateEffects([&](DirectX::IEffect* material) {
		auto modelMaterial = reinterpret_cast<ModelEffect*>(material);
		modelMaterial->SetRenderMode(renderMode);
		modelMaterial->SetShadoMapSRV(m_shadowMapSRV);
		});
	m_skeleton.SendBoneMatrixArrayToGPU();
	m_modelDx->Draw(
		deviceContext,
		state,
		m_worldMatrix,
		MainCamera().GetViewMatrix(),
		MainCamera().GetProjectionMatrix()
	);
}

void SkinModel::Draw()
{

	CMatrix viewMatrix = MainCamera().GetViewMatrix();
	CMatrix projMatrix = MainCamera().GetProjectionMatrix();

	DirectX::CommonStates state(GraphicsEngine().GetD3DDevice());

	ID3D11DeviceContext* d3dDeviceContext = GraphicsEngine().GetD3DDeviceContext();

	//定数バッファの内容を更新。
	SVSConstantBuffer vsCb;
	vsCb.mWorld = m_worldMatrix;
	vsCb.mProj = projMatrix;
	vsCb.mView = viewMatrix;
	for (int i = 0; i < NUM_DIRECTION_LIG; i++) {
		vsCb.mCol[i] = m_DirCol[i];
		vsCb.mDir[i] = m_DirLight[i];
	}
	vsCb.eyePos = MainCamera().GetPosition();
	vsCb.specPow = m_specPow;
	d3dDeviceContext->UpdateSubresource(m_cb, 0, nullptr, &vsCb, 0, 0);
	//定数バッファをGPUに転送。
	d3dDeviceContext->VSSetConstantBuffers(0, 1, &m_cb);
	d3dDeviceContext->PSSetConstantBuffers(0, 1, &m_cb);
	//サンプラステートを設定。
	d3dDeviceContext->PSSetSamplers(0, 1, &m_samplerState);
	//ボーン行列をGPUに転送。
	m_skeleton.SendBoneMatrixArrayToGPU();
	//描画。
	m_modelDx->Draw(
		d3dDeviceContext,
		state,
		m_worldMatrix,
		viewMatrix,
		projMatrix
	);
}

void SkinModel::FindVertexPosition(std::function<void(CVector3* pos)> func) {
	FindMesh([&](const auto& mesh) {
		ID3D11DeviceContext* deviceContext = GraphicsEngine().GetD3DDeviceContext();
		{
			D3D11_MAPPED_SUBRESOURCE subresource;
			HRESULT hr = deviceContext->Map(mesh->vertexBuffer.Get(), 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &subresource);
			if (FAILED(hr)) {
				return;
			}
			D3D11_BUFFER_DESC bufferDesc;
			mesh->vertexBuffer->GetDesc(&bufferDesc);
			int vertexCount = bufferDesc.ByteWidth / mesh->vertexStride;
			char* pData = reinterpret_cast<char*>(subresource.pData);
			CVector3* pos;
			for (int i = 0; i < vertexCount; i++) {
				//pos = (CVector3*)pData;
				pos = reinterpret_cast<CVector3*>(pData);
				func(pos);
				pData += mesh->vertexStride;
			}
			deviceContext->Unmap(mesh->vertexBuffer.Get(), 0);
		}
		});
}