#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN

#include <WinSock2.h>
#include <windows.h>
#include <d3d11.h>
#include <D3Dcompiler.h>
#include <stdio.h>
#include <cstdlib>
#include <vector>
#include <list>
#include <queue>
#include <algorithm>
#include <map>
#include <wincodec.h>
#include <memory>
#include <wincodecsdk.h>
#include <Xinput.h>
#include <xaudio2.h>
#include <x3daudio.h>
#include <xaudio2fx.h>
#include <functional>
#include <cstring>
#include <string>

#define  MRT_MAX  8
const float FRAME_BUFFER_W = 1280.0f;
const float FRAME_BUFFER_H = 720.0f;
static const int MAX_BONE = 512;

#include "DirectXTK/Inc/Model.h"
#include "DirectXTK/Inc/Effects.h"	
#include "DirectXTK/Inc/CommonStates.h"
#include "DirectXTK/Inc/SpriteFont.h"
#include "DirectXTK/Inc/DDSTextureLoader.h"
#include "DirectXTK/Inc/SpriteBatch.h"
#include "DirectXTK\Inc\Audio.h"

#include "bulletPhysics/src/btBulletDynamicsCommon.h"
#include "bulletPhysics/src/btBulletCollisionCommon.h"
#include	 "bulletPhysics/src/BulletCollision/CollisionDispatch/btGhostObject.h"

#include "EffekseerRuntime130/src/include/Effekseer.h"
#include "EffekseerRuntime130/src/EffekseerRendererDX11/EffekseerRendererDX11.h"

#include "math/Vector.h"
#include "math/Matrix.h"
#include "graphics/GraphicsEngine.h"
#include "graphics/Sprite/Sprite.h"
#include "graphics\animation\Animation.h"
#include "graphics/animation/AnimationClip.h"
#include "math/Easing.h"
#include "window\Window.h"
#include "physics/Physics.h"
#include "HID/Pad.h"

//TODO : photon include
//#include "Common-cpp/inc/Common.h"
//#include "LoadBalancing-cpp/inc/Client.h"
//#include "Network/NetworkLogic.h"
//#include "../Game/Online/NetworkLogic.h"
//#include "Network/Console.h"
//#include "../Game/Online/Console.h"

#include "Engine.h"
#include "gameObject/GameObject.h"
#include "GameObject/GameObjectManager.h"
#include "graphics/Skelton.h"
#include "graphics/SkinModel.h"
#include "graphics/SkinModelRender.h"
#include "physics/PhysicsStaticObject.h"
#include "graphics/SkinModelDummy.h"
#include "physics/SpriteBoxCollider.h"
#include "graphics/SpriteRender.h"
#include "graphics/font/CFONT.h"
#include "graphics/font/FontRender.h"
#include "graphics/animation/Animation.h"
#include "graphics/animation/AnimationClip.h"
#include "graphics/Camera.h"
#include "graphics/Effect/CEffectEngine.h"
#include "graphics/Effect/CEffect.h"
#include "HID/Mouse.h"
#include "time/GameTime.h"
#include "graphics/ShadowMap.h"
#include "graphics//Primitive.h"
#include "sound\CSound.h"
