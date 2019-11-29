#include "stdafx.h"
#include "TestScene.h"
#include "../GameObjects/GameCamera.h"
#include "../GameObjects/Stage.h"
#include "..//..//ZekeEngine2/graphics/Media/MediaPlayer.h"
#include <mfapi.h>
#include <mfidl.h>
#include <mfreadwrite.h>
#include <Mferror.h>
#include <evr.h>
#include <mfobjects.h>

bool TestScene::Start() {
	m_stage = NewGO<Stage>(0);
	m_myCar = NewGO<Car>(0, "MyCar");
	m_myCar->ResetCar(CVector3::Zero());

	MainCamera().SetPosition({ -2,2,4 });
	MainCamera().SetTarget(CVector3::Zero());
	MainCamera().Update();

	IGameObjectManager().SetShadowCameraPosAndTarget(
		{ m_myCar->GetPosition().x,m_myCar->GetPosition().y + 50,m_myCar->GetPosition().z },
		{ m_myCar->GetPosition().x,m_myCar->GetPosition().y,m_myCar->GetPosition().z }
	);

	//auto wnd = FindWindow(NULL, "Game");
	//CPlayer::CreateInstance(wnd, wnd, &g_pPlayer);


	HRESULT hr = MFStartup(MF_VERSION);
	//TODO : error check
	MFCreateDXGIDeviceManager(&token, &mp_dmanager);
	mp_dmanager->ResetDevice(GraphicsEngine().GetD3DDevice(), token);

	MFCreateMediaSession(NULL, &m_pSession);
	MFCreateSourceResolver(&m_pSourceResolver);

	MF_OBJECT_TYPE ObjectType = MF_OBJECT_INVALID;
	IUnknown* pSource = NULL;

	hr = m_pSourceResolver->CreateObjectFromURL(
		L"Assets/video/rltest.mov",                       // URL of the source.
		MF_RESOLUTION_MEDIASOURCE,  // Create a source object.
		NULL,                       // Optional property store.
		&ObjectType,        // Receives the created object type. 
		&pSource            // Receives a pointer to the media source.
	);

	if (FAILED(hr))
	{
		printf("failed\n");
	}

	//create MF Transform
	IMFTransform* ppDecoder = nullptr;
	{
		UINT32 num_activates = 0; // will be 1
		IMFActivate** activate = nullptr;
		MFT_REGISTER_TYPE_INFO info = { 0 };
		info.guidMajorType = MFMediaType_Video;
		info.guidSubtype = MFMediaType_Audio;

		//TODO : check
		hr = MFTEnumEx(
			MFT_CATEGORY_VIDEO_DECODER,
			MFT_ENUM_FLAG_SYNCMFT | MFT_ENUM_FLAG_LOCALMFT | MFT_ENUM_FLAG_SORTANDFILTER,
			//&info,      // Input type
			NULL,      // Input type
			NULL,       // Output type
			&activate,
			&num_activates
		);

		if (SUCCEEDED(hr) && num_activates == 0)
		{
			hr = MF_E_TOPO_CODEC_NOT_FOUND;
		}

		if (SUCCEEDED(hr))
		{
			hr = activate[0]->ActivateObject(IID_PPV_ARGS(&ppDecoder));
		}

		for (UINT32 i = 0; i < num_activates; i++)
		{
			activate[i]->Release();
		}
		CoTaskMemFree(activate);
	}

	//Set Type
	{
		IMFMediaType* mt_iyuv;
		MFCreateMediaType(&mt_iyuv);
		mt_iyuv->SetGUID(MF_MT_MAJOR_TYPE, MFMediaType_Video);
		mt_iyuv->SetGUID(MF_MT_SUBTYPE, MFVideoFormat_IYUV);
		mt_iyuv->SetUINT32(MF_MT_DEFAULT_STRIDE, 1280);
		MFSetAttributeRatio((IMFAttributes*)mt_iyuv, MF_MT_FRAME_RATE, 24, 1);
		//MFSetAttributeRatio((IMFAttributes*)mt_iyuv, MF_SA_D3D11_AWARE, 24, 1);
		MFSetAttributeSize((IMFAttributes*)mt_iyuv, MF_MT_FRAME_SIZE, 1280, 720);
		mt_iyuv->SetUINT32(MF_MT_INTERLACE_MODE, MFVideoInterlace_Progressive);
		mt_iyuv->SetUINT32(MF_MT_ALL_SAMPLES_INDEPENDENT, TRUE);
		MFSetAttributeRatio((IMFAttributes*)mt_iyuv, MF_MT_PIXEL_ASPECT_RATIO, 1, 1);

		IMFMediaType* mt_argb32;
		MFCreateMediaType(&mt_argb32);
		mt_argb32->SetGUID(MF_MT_MAJOR_TYPE, MFMediaType_Video);
		mt_argb32->SetGUID(MF_MT_SUBTYPE, MFVideoFormat_ARGB32);
		mt_argb32->SetUINT32(MF_MT_DEFAULT_STRIDE, 1280 * 4);
		MFSetAttributeRatio((IMFAttributes*)mt_argb32, MF_MT_FRAME_RATE, 24, 1);
		MFSetAttributeSize((IMFAttributes*)mt_argb32, MF_MT_FRAME_SIZE, 1280, 720);
		mt_argb32->SetUINT32(MF_MT_INTERLACE_MODE, MFVideoInterlace_Progressive);
		mt_argb32->SetUINT32(MF_MT_ALL_SAMPLES_INDEPENDENT, TRUE);
		MFSetAttributeRatio((IMFAttributes*)mt_argb32, MF_MT_PIXEL_ASPECT_RATIO, 1, 1);

		ppDecoder->SetInputType(0, mt_iyuv, 0);
		ppDecoder->SetOutputType(0, mt_argb32, 0);

	}

	//IMFAttributes* at;
	//ppDecoder->GetAttributes(&at);

	//Find a Decoder Configuration
	hr = mp_dmanager->OpenDeviceHandle(&handle);

	if (!FAILED(hr)) {
		//ID3D11VideoDevice* videoDevice;
		void* vpvideoDevice;
		mp_dmanager->GetVideoService(handle, IID_ID3D11VideoDevice, &vpvideoDevice);
		videoDevice = (ID3D11VideoDevice*)vpvideoDevice;
		auto pc = videoDevice->GetVideoDecoderProfileCount();
		printf("Video Decoder Profile Count %d\n", pc);

		GUID finalId;
		for (int i = 0; i < pc; i++) {
			GUID id;
			videoDevice->GetVideoDecoderProfile(i, &id);
			if (id == D3D11_DECODER_PROFILE_MPEG2_MOCOMP ||
				id == D3D11_DECODER_PROFILE_MPEG2_IDCT ||
				id == D3D11_DECODER_PROFILE_MPEG2_VLD) {
				printf("mituketaaa\n");
				BOOL br;
				videoDevice->CheckVideoDecoderFormat(&id, DXGI_FORMAT_NV12, &br);
				if (br)
					finalId = id;
				break;
			}
		}

		UINT configCount;
		D3D11_VIDEO_DECODER_DESC desc;
		desc.Guid = finalId;
		desc.SampleHeight = 1280;
		desc.SampleHeight = 720;
		desc.OutputFormat = DXGI_FORMAT_NV12;
		videoDevice->GetVideoDecoderConfigCount(&desc, &configCount);
		printf("config count %d \n", configCount);

		//TODO : configがないんだけど！？
		D3D11_VIDEO_DECODER_CONFIG config;
		videoDevice->GetVideoDecoderConfig(&desc, 0, &config);
		//ppDecoder->GetOutputAvailableType(0, 0, 0);

		//GetImmediateContext : D3DDeviceContext
	//TODO : Fall back

	//Allocating Uncompressed Buffers
		{
			D3D11_TEXTURE2D_DESC texDesc;
			ZeroMemory(&texDesc, sizeof(texDesc));
			texDesc.Width = (UINT)FRAME_BUFFER_W;
			texDesc.Height = (UINT)FRAME_BUFFER_H;
			texDesc.MipLevels = 1;
			texDesc.ArraySize = 1;
			texDesc.Format = DXGI_FORMAT_NV12;
			texDesc.SampleDesc.Count = 1;
			texDesc.SampleDesc.Quality = 0;
			texDesc.Usage = D3D11_USAGE_DEFAULT;
			texDesc.BindFlags = D3D11_BIND_DECODER;
			texDesc.CPUAccessFlags = 0;
			texDesc.MiscFlags = 0;

			//TODO : release 
			ID3D11Texture2D* texture2d = NULL;
			GraphicsEngine().GetD3DDevice()->CreateTexture2D(&texDesc, NULL, &texture2d);

			D3D11_VIDEO_DECODER_OUTPUT_VIEW_DESC vdovdesc;
			vdovdesc.DecodeProfile = MFMediaType_Video;
			vdovdesc.Texture2D = tex2dvdov;
			vdovdesc.ViewDimension = D3D11_VDOV_DIMENSION_TEXTURE2D;
			if (texture2d != NULL)
				videoDevice->CreateVideoDecoderOutputView(texture2d, &vdovdesc, &videoDecoderOutputView);


			IMFMediaBuffer* imfm;
			MFCreateDXGISurfaceBuffer(IID_ID3D11Texture2D, m_pSourceResolver, 0, false, &imfm);

			//TODO : ナニコレ
			MFCreateVideoSampleFromSurface(0, &imfSample);

			//Decoding
			//videoDevice->CreateVideoDecoder(&desc, &config, &videoDecoder);
			videoDevice->CreateVideoDecoder(&desc, NULL, &videoDecoder); //応急処置、ちゃんとコンフィグ見つけてどうぞ


		}
	}

	//MFT_MESSAGE_SET_D3D_MANAGER

	//MFTEnumEx(MFT_CATEGORY_VIDEO_DECODER, MFT_ENUM_FLAG_SYNCMFT, NULL, NULL, &activate, &num_activates);
	//m_pIMFActivate->ActivateObject();

	//printf("NUM ACTIVATES %d\n", num_activates);
	//for (int i = 0; i < num_activates; i++) {
	//	//activate[i]->get
	//	hr = activate[0]->ActivateObject(IID_PPV_ARGS(&mft));
	//}
	////activate[0]->ActivateObject(__uuidof(IMFTransform), (void**)& mft);
	////SAFE_RELEASE(activates[0]);
	//delete[] activate;
	//CoTaskMemFree(activate);

	////CoCreateInstance();
	//DWORD        dwInputStreamID;
	//DWORD        dwTypeIndex;
	//IMFMediaType* ppType;

	//IMFTransform::GetInputAvailableType(dwInputStreamID,dwTypeIndex,ppType);
	return true;
}

void TestScene::OnDestroy() {
	DeleteGO(m_stage);
	DeleteGO(m_myCar);


	MFShutdown();
	if (!mp_dmanager) {
		delete mp_dmanager;
	}

	if (!m_pSession)
		delete m_pSession;

	if (!m_pSourceResolver)
		delete m_pSourceResolver;

	//if (g_pPlayer)
	//{
	//	g_pPlayer->Shutdown();
	//	SafeRelease(&g_pPlayer);
	//}
}

void TestScene::Update() {
	mp_dmanager->TestDevice(handle);
	//imfSample->SetAllocator();
	ID3D11VideoContext* videoContext = nullptr;
	GraphicsEngine().GetD3DDeviceContext()->QueryInterface<ID3D11VideoContext>(&videoContext);
	videoContext->DecoderBeginFrame(videoDecoder, videoDecoderOutputView, 0, NULL);

	void* buffer;
	UINT bufSize = 1280 * 720;
	videoContext->GetDecoderBuffer(videoDecoder,
		D3D11_VIDEO_DECODER_BUFFER_TYPE::D3D11_VIDEO_DECODER_BUFFER_BITSTREAM,
		&bufSize,
		&buffer
	);

	videoContext->Release();

	D3D11_VIDEO_DECODER_BUFFER_DESC vdbufferDesc;
	videoContext->SubmitDecoderBuffers(videoDecoder, 1, &vdbufferDesc);

	videoContext->DecoderEndFrame(videoDecoder);
}