#pragma once

#include "targetver.h"
#define WIN32_LEAN_AND_MEAN

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

#define MAX_LIGHT 10
#define MAX_BONE 256
#define MAX_FRAME 256
#define MAX_INSTANCE 128

#define CENTER Vector3(SCREEN_WIDTH >> 1, SCREEN_HEIGHT >> 1, 0.0f)

#define DEVICE Device::Get()->GetDevice()
#define DC Device::Get()->GetDeviceContext()

#define DELTA Timer::Get()->GetElapsedTime()
#define KEY Keyboard::Get()
#define SCENE SceneManager::Get()

#define CAM Environment::Get()->GetMainCamera()
#define DIALOG ImGuiFileDialog::Instance()

#define FOR(n) for(int i = 0 ; i < n ; i++)

#include <windows.h>
#include <vector>
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <string>
#include <queue>
#include <list>
#include <functional>
#include <algorithm>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <variant>
#include <iostream>
#include <random>

using namespace std;
using namespace DirectX;

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dxguid.lib")

//DirectXTex
#include <DirectXTex/DirectXTex.h>

#pragma comment(lib, "DirectXTex.lib")

//ImGui
#include <imgui.h>
#include <imgui_impl_dx11.h>
#include <imgui_impl_win32.h>
#include <ImGuiFileDialog.h>

#pragma comment(lib, "ImGui.lib")

//DirectWrite
#include <d2d1_2.h>
#include <dwrite.h>

#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dwrite.lib")

//Assimp
#include <Assimp/Importer.hpp>
#include <Assimp/scene.h>
#include <Assimp/postprocess.h>

#pragma comment(lib, "assimp-vc143-mtd.lib")

typedef XMMATRIX Matrix;
typedef XMFLOAT4 Float4;
typedef XMFLOAT3 Float3;
typedef XMFLOAT2 Float2;
typedef XMVECTOR Vector4;
typedef XMFLOAT4X4 Float4x4;

typedef function<void()> Event;
typedef function<void(void*)> ParamEvent;
typedef function<void(int)> IntParamEvent;

#include "random"
//Framework Head
#include "Framework/Math/Vector3.h"
#include "Framework/Math/Vector2.h"
#include "Framework/Math/GameMath.h"
#include "Framework/Math/Transform.h"

#include "Framework/Utility/Utility.h"
#include "Framework/Utility/Singleton.h"
#include "Framework/Utility/Timer.h"
#include "Framework/Utility/Keyboard.h"
#include "Framework/Utility/BinaryReader.h"
#include "Framework/Utility/BinaryWriter.h"
#include "Framework/Utility/Font.h"
#include "Framework/Utility/Observer.h"
#include "Framework/Utility/tinyxml2.h"

#include "Framework/Device/Device.h"

#include "Framework/Shader/Shader.h"
#include "Framework/Shader/VertexShader.h"
#include "Framework/Shader/PixelShader.h"

#include "Framework/Buffer/VertexBuffer.h"
#include "Framework/Buffer/IndexBuffer.h"
#include "Framework/Buffer/ConstBuffer.h"
#include "Framework/Buffer/GlobalBuffer.h"
#include "Framework/Buffer/VertexLayouts.h"

#include "Framework/Render/Texture.h"
#include "Framework/Render/Material.h"
#include "Framework/Render/Mesh.h"

#include "Framework/State/RasterizerState.h"
#include "Framework/State/SamplerState.h"
#include "Framework/State/BlendState.h"
#include "Framework/State/DepthStencilState.h"

#include "Framework/Environment/Camera.h"
#include "Framework/Environment/Environment.h"

#include "Framework/Model/ModelData.h"
#include "Framework/Model/ModelExporter.h"
#include "Framework/Model/ModelMesh.h"
#include "Framework/Model/Model.h"
#include "Framework/Model/ModelInstancing.h"
#include "Framework/Model/ModelClip.h"
#include "Framework/Model/ModelAnimator.h"

//GameObject Head
#include "Objects/Basic/GameObject.h"
#include "Objects/Basic/Cube.h"
#include "Objects/Basic/Sphere.h"
#include "Objects/Basic/Plane.h"
#include "Objects/Basic/Quad.h"

#include "Objects/Collider/Collider.h"
#include "Objects/Collider/SphereCollider.h"
#include "Objects/Collider/BoxCollider.h"
#include "Objects/Collider/CapsuleCollider.h"
#include "Objects/Collider/RectCollider.h"

#include "Objects/UI/Button.h"
#include "Objects/UI/ProgressBar.h"

#include "Objects/Landscape/Terrain.h"
#include "Objects/Landscape/Skybox.h"

#include "Objects/Manager/PoolingManager.h"

#include "Objects/Test/Planet.h"

#include "Objects/Shooting/Bullet.h"
#include "Objects/Shooting/BulletManager.h"
#include "Objects/Shooting/Enemy.h"
#include "Objects/Shooting/Player.h"

#include "Objects/MineCraft/Block.h"
#include "Objects/MineCraft/BlockManager.h"
#include "Objects/MineCraft/QuickSlotIcon.h"
#include "Objects/MineCraft/InventorySlot.h"
#include "Objects/MineCraft/Inventory.h"
#include "Objects/MineCraft/UIManager.h"

#include "Objects/Pacman/CubeMapEditor.h"
#include "Objects/Pacman/MapManager.h"
#include "Objects/Pacman/Hermit.h"
#include "Objects/Pacman/FloorManager.h"
#include "Objects/Pacman/ObjectManager.h"
#include "Objects/Pacman/OreManager.h"
#include "Objects/Pacman/Ore.h"
#include "Objects/Pacman/CilingManager.h"
#include "Objects/Pacman/OreInventory.h"
#include "Objects/Pacman/OreSlot.h"
#include "Objects/Pacman/ClickerUIManager.h"


//Manager Head
#include "Scenes/Scene.h"
#include "Managers/GameManager.h"
#include "Managers/SceneManager.h"

//Clicker

extern HWND hWnd;
extern Vector3 mousePos;