#pragma once
#include "../GameObjects/Car.h"
class GameCamera;
class Stage;
class CPlayer;
class IMFDXGIDeviceManager;
class IMFMediaSession;
class IMFSourceResolver;
class IMFActivate;
class IMFSample;
class ID3D11VideoDecoder;

class TestScene : public GameObject
{
public:
	bool Start() override;
	void Update() override;
	void OnDestroy() override;
private:
	CPlayer* g_pPlayer = nullptr;

	UINT token = 0;
	IMFDXGIDeviceManager* mp_dmanager = nullptr;
	IMFMediaSession* m_pSession = nullptr;
	IMFSourceResolver* m_pSourceResolver = nullptr;
	HANDLE handle;
	IMFSample* imfSample;
	ID3D11VideoDevice* videoDevice = nullptr;
	ID3D11VideoDecoder* videoDecoder;
	D3D11_TEX2D_VDOV tex2dvdov;
	ID3D11VideoDecoderOutputView* videoDecoderOutputView = nullptr;

	Car* m_myCar = nullptr;
	Stage* m_stage = nullptr;
	SkinModelRender* m_ground;
};

