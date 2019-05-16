#include "stdafx.h"
#include "SkinModelDataManager.h"
#include "SkinModelEffect.h"
#include "Skelton.h"

SkinModelDataManager g_skinModelDataManager;

DirectX::Model* SkinModelDataManager::Load(const wchar_t* filePath, const Skeleton& skeleton, const char* psmain, const char* vsmain, const wchar_t* normalMap, const wchar_t* specularMap)
{
	DirectX::Model* retModel = NULL;
	//ボーンを発見したときのコールバック関数。
	auto onFindBone = [&](
		const wchar_t* boneName,
		const VSD3DStarter::Bone* bone,
		std::vector<int>& localBoneIDtoGlobalBoneIDTbl
		)
	{
		int globalBoneID = skeleton.FindBoneID(boneName);
		if (globalBoneID == -1) {
			return;
		}
		localBoneIDtoGlobalBoneIDTbl.push_back(globalBoneID);
	};
	//マップに登録されているか調べる。
	auto it = m_directXModelMap.find(filePath);
	if (it == m_directXModelMap.end()) {
		//未登録なので、新規でロードする。
		//エフェクトファクトリ。
		SkinModelEffectFactory effectFactory(GraphicsEngine().GetD3DDevice(), psmain, vsmain, normalMap, specularMap);
		//テクスチャがあるフォルダを設定する。
		effectFactory.SetDirectory(L"Assets/modelData");
		//CMOファイルのロード。
		auto model = DirectX::Model::CreateFromCMO(	
			GraphicsEngine().GetD3DDevice(),
			filePath,								
			effectFactory,						
			false,										
			false,
			onFindBone
		);
		retModel = model.get();
		//新規なのでマップに登録する。
		m_directXModelMap.insert({ filePath, std::move(model) });
	}
	else {
		//登録されているので、読み込み済みのデータを取得。
		retModel = it->second.get();
	}
	return retModel;
}

void SkinModelDataManager::Release()
{
	//mapを空にする。
	m_directXModelMap.clear();
}