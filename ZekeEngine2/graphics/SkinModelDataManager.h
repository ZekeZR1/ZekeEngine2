#pragma once
#include <map>

class Skeleton;
/*!
*@brief	スキンモデルデータマネージャー。。
*@FlyweightパターンのFlyweightFactoryクラス、。
*/
class SkinModelDataManager
{
public:
	/*!
	*@brief	モデルをロード。
	*@param[in]	filePath	ファイルパス。
	*@param[in]	skeleton	スケルトン。
	*/
	DirectX::Model* Load(const wchar_t* filePath, const Skeleton& sketon, const char*,const char*, const wchar_t* normalMap, const wchar_t* specularMap);
	/*!
	*@brief	モデルデータを全開放。
	*/
	void Release();

private:
	std::map<
		std::wstring,					
		std::unique_ptr<DirectX::Model>	
	> m_directXModelMap;
};

extern SkinModelDataManager g_skinModelDataManager;

