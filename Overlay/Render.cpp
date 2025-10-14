#pragma warning (disable : 4995)
#define STB_IMAGE_IMPLEMENTATION
#define NOMINMAX
#include <Overlay/Render.h>
#include <Utility/Globals.h>
#include <Utility/ConfigManager.h>
#include "Utility/Vectors.h"
#include "../cache/cache.h"
#include <iostream>
#include <sstream>
#include <iomanip>

#include "ImGUI/imgui.h"
#include "ImGUI/imgui_impl_win32.h"
#include "ImGUI/imgui_impl_dx11.h"
#include <d3d11.h>

#include "ImGUI/D3DX11tex.h"
#include "ImGUI/stb_image.h"
#include <algorithm>
#include <resource.h>
#include <DMALibrary/Memory/Memory.h>

#pragma comment(lib, "Winmm.lib")

void DrawSidebar();

//DRAWING SECTION
//TODO Move Drawbox from CFMANAGER
//draw box decide color  and thickness based on teamid

void DrawLine(ImVec2 start, ImVec2 end, ImColor color, float thickness = 1.5f) {
	ImGui::GetForegroundDrawList()->AddLine(start, end, color, thickness);
}



void DrawBox(FVector2D position, float width, float height, ImColor color, float thickness = 1.0f, int boxStyle = 0) {
	ImVec2 tl(position.X - width / 2, position.Y - height / 2);
	ImVec2 br(position.X + width / 2, position.Y + height / 2);
	if (boxStyle == 0) { //corner box
		float lineW = width / 4;
		float lineH = height / 4;
		//top left
		ImGui::GetForegroundDrawList()->AddLine(tl, ImVec2(tl.x + lineW, tl.y), color, thickness);
		ImGui::GetForegroundDrawList()->AddLine(tl, ImVec2(tl.x, tl.y + lineH), color, thickness);
		//top right
		ImGui::GetForegroundDrawList()->AddLine(ImVec2(br.x - lineW, tl.y), ImVec2(br.x, tl.y), color, thickness);
		ImGui::GetForegroundDrawList()->AddLine(ImVec2(br.x, tl.y), ImVec2(br.x, tl.y + lineH), color, thickness);
		//bottom left
		ImGui::GetForegroundDrawList()->AddLine(ImVec2(tl.x, br.y - lineH), ImVec2(tl.x, br.y), color, thickness);
		ImGui::GetForegroundDrawList()->AddLine(ImVec2(tl.x, br.y), ImVec2(tl.x + lineW, br.y), color, thickness);
		//bottom right
		ImGui::GetForegroundDrawList()->AddLine(ImVec2(br.x - lineW, br.y), ImVec2(br.x, br.y), color, thickness);
		ImGui::GetForegroundDrawList()->AddLine(ImVec2(br.x, br.y - lineH), ImVec2(br.x, br.y), color, thickness);
	}
	else { //full box
		ImGui::GetForegroundDrawList()->AddRect(tl, br, color, 0.0f, 0, thickness);
	}
}
void DrawHeadCircle(FVector2D position, float radius, ImColor color, float thickness = 3.0f) {
	ImVec2 center(position.X, position.Y - radius / 2);
	
	ImGui::GetForegroundDrawList()->AddCircle(center, radius / 2, color, 20, thickness);

}
void DrawTraceline(FVector2D start, FVector2D end, ImColor color, float thickness = 1.0f) {
	ImGui::GetForegroundDrawList()->AddLine(ImVec2(start.X, start.Y), ImVec2(end.X, end.Y + 20), color, thickness);
}
void DrawDistanceText(FVector2D position, float distance, ImColor color)
{

	float adjustedDistance = distance - 4.0f;
	if (adjustedDistance < 0.0f) adjustedDistance = 0.0f;

	int meters = static_cast<int>(std::lround(adjustedDistance));
	std::string txt = "[" + std::to_string(meters) + "M]";

	ImVec2 ts = ImGui::CalcTextSize(txt.c_str());
	constexpr float verticalOffset = 1.0f;
	ImVec2 tp(position.X - ts.x / 2.0f, position.Y + verticalOffset);
	ImGui::GetForegroundDrawList()->AddText(Config.logoFont, 16.0f, ImVec2(tp.x + 1, tp.y + 1), ImColor(0, 0, 0, 150), txt.c_str());
	ImGui::GetForegroundDrawList()->AddText(Config.logoFont, 16.0f, tp, color, txt.c_str());
}
void DrawHealthBar(FVector2D center, float health, float width = 6.0f, float height = 60.0f)
{
	float pct = std::clamp(health / 150.0f, 0.0f, 1.0f);

	ImColor fillColor;
	if (pct > 0.66f) {
		fillColor = ImColor(0, 220, 0, 220);
	}
	else if (pct > 0.33f) {
		fillColor = ImColor(255, 165, 0, 220); 
	}
	else {
		fillColor = ImColor(220, 20, 20, 220);
	}
	ImVec2 tl(center.X - width / 2.0f, center.Y - height / 2.0f);
	ImVec2 br(center.X + width / 2.0f, center.Y + height / 2.0f);
	ImGui::GetForegroundDrawList()->AddRectFilled(tl, br, ImColor(0, 0, 0, 140));

	float filledTopY = tl.y + height * (1.0f - pct);
	ImGui::GetForegroundDrawList()->AddRectFilled(ImVec2(tl.x, filledTopY), ImVec2(br.x, br.y), fillColor);

	ImGui::GetForegroundDrawList()->AddRect(tl, br, ImColor(255, 255, 255, 200));
	ImGui::GetForegroundDrawList()->AddRect(ImVec2(tl.x + 1.0f, tl.y + 1.0f),
		ImVec2(br.x - 1.0f, br.y - 1.0f),
		ImColor(0, 0, 0, 60));
}
void DrawNames(FVector2D position, const std::string& name, ImColor color)
{

	ImVec2 ts = Config.headerFont->CalcTextSizeA(15.0f, FLT_MAX, 0.0f, name.c_str());
	ImVec2 tp(position.X + 4, position.Y - ts.y / 2 - 20);

	ImGui::GetForegroundDrawList()->AddText(Config.logoFont, 18.0f, ImVec2(tp.x + 1, tp.y + 1), ImColor(0, 0, 0, 150), name.c_str());
	ImGui::GetForegroundDrawList()->AddText(Config.logoFont, 18.0f, tp, color, name.c_str());
}
void DrawItemName(FVector2D position, const std::string& name, ImVec4 color)
{
	ImU32 col = ImGui::GetColorU32(color);
	ImU32 shadow = ImGui::GetColorU32(ImVec4(0, 0, 0, 0.6f));
	ImVec2 ts = ImGui::CalcTextSize(name.c_str());
	ImVec2 tp(position.X - ts.x / 2, position.Y - 30);
	ImGui::GetForegroundDrawList()->AddText(Config.logoFont, 18.0f, ImVec2(tp.x + 1, tp.y + 1), shadow, name.c_str());
	ImGui::GetForegroundDrawList()->AddText(Config.logoFont, 18.0f, tp, col, name.c_str());
}

void DrawItemBox(FVector2D position, float width, float height, ImVec4 color, float thickness = 1.0f) {
	ImU32 col = ImGui::GetColorU32(color);
	ImVec2 tl(position.X - width / 2, position.Y - height / 2);
	ImVec2 br(position.X + width / 2, position.Y + height / 2);
	ImGui::GetForegroundDrawList()->AddRect(tl, br, col, 0.0f, 0, thickness);
}
void DrawItemDistanceText(FVector2D position, float distance, ImColor color)
{
	std::ostringstream ss;
	ss << std::fixed << std::setprecision(1) << distance << "m";
	std::string txt = ss.str();
	ImVec2 ts = ImGui::CalcTextSize(txt.c_str());
	ImVec2 tp(position.X - ts.x / 2, position.Y + 8);
	ImGui::GetForegroundDrawList()->AddText(Config.logoFont, 18.0f, ImVec2(tp.x + 1, tp.y + 1), ImColor(0, 0, 0, 150), txt.c_str());
	ImGui::GetForegroundDrawList()->AddText(Config.logoFont, 18.0f, tp, color, txt.c_str());
}

inline void DrawSkeleton(const _PlayerCache& player, ImColor color, float thickness = 2.0f) {
	if (player.bonePtr == 0 || !player.valid) return;

	ImDrawList* drawList = ImGui::GetForegroundDrawList();

	// Helper lambdas
	auto V = [](const FVector2D& v) -> ImVec2 { return ImVec2(v.X, v.Y); };
	auto IsValid = [](const FVector2D& v) -> bool { return v.X != 0.0f || v.Y != 0.0f; };

	// Draw with distance validation (from reference code)
	auto DrawLine = [&](const FVector& from3D, const FVector& to3D, const FVector2D& fromW2S, const FVector2D& toW2S) -> void {
		if (!IsValid(fromW2S) || !IsValid(toW2S)) return;

		// Reference code validation checks
		if (distance_3d(player.pos, from3D) > 118.0f) return;
		if (distance_3d(player.pos, to3D) > 118.0f) return;
		if (distance_3d(from3D, to3D) > 39.0f) return;

		drawList->AddLine(V(fromW2S), V(toW2S), color, thickness);
		};

	// Draw skeleton with 3D bone positions for validation
	DrawLine(player.head, player.neck, player.headW2S, player.neckW2S);
	DrawLine(player.neck, player.chest, player.neckW2S, player.chestW2S);
	DrawLine(player.chest, player.pelvis, player.chestW2S, player.pelvisW2S);

	// Left arm
	DrawLine(player.chest, player.leftShoulder, player.chestW2S, player.leftShoulderW2S);
	DrawLine(player.leftShoulder, player.leftElbow, player.leftShoulderW2S, player.leftElbowW2S);
	DrawLine(player.leftElbow, player.leftHand, player.leftElbowW2S, player.leftHandW2S);

	// Right arm
	DrawLine(player.chest, player.rightShoulder, player.chestW2S, player.rightShoulderW2S);
	DrawLine(player.rightShoulder, player.rightElbow, player.rightShoulderW2S, player.rightElbowW2S);
	DrawLine(player.rightElbow, player.rightHand, player.rightElbowW2S, player.rightHandW2S);

	// Left leg
	DrawLine(player.pelvis, player.leftThigh, player.pelvisW2S, player.leftThighW2S);
	DrawLine(player.leftThigh, player.leftKnee, player.leftThighW2S, player.leftKneeW2S);
	DrawLine(player.leftKnee, player.leftAnkle, player.leftKneeW2S, player.leftAnkleW2S);
	DrawLine(player.leftAnkle, player.leftFoot, player.leftAnkleW2S, player.leftFootW2S);

	// Right leg
	DrawLine(player.pelvis, player.rightThigh, player.pelvisW2S, player.rightThighW2S);
	DrawLine(player.rightThigh, player.rightKnee, player.rightThighW2S, player.rightKneeW2S);
	DrawLine(player.rightKnee, player.rightAnkle, player.rightKneeW2S, player.rightAnkleW2S);
	DrawLine(player.rightAnkle, player.rightFoot, player.rightAnkleW2S, player.rightFootW2S);
}

void Render::Loop() {
	ImDrawList* draw = ImGui::GetForegroundDrawList();
	if (!draw) return;

	if (Config.fov > 0 && Config.EnableAimbot) {
		draw->AddCircle(
			ImVec2(Globals::screenWidth / 2, Globals::screenHeight / 2),
			Config.fov,
			ImColor(Config.ColorFOV),
			50,
			Config.ItemThickness
		);
	}

	std::lock_guard<std::mutex> lock(cache.cacheMutex);

	if (cache.RenderPlayerCount == 0) return;

	const int screenW = Globals::screenWidth;
	const int screenH = Globals::screenHeight;

	for (int i = 0; i < cache.RenderPlayerCount; i++) {
		const auto& player = cache.RenderPlayers[i];

		if (player.headW2S.IsZeroVector()) continue;
		if (player.RootW2S.IsZeroVector()) continue;

		//screen bounds check
 
		if (player.headW2S.Y < 0 || player.headW2S.Y > screenH) continue;
		if (player.headW2S.X < 0 || player.headW2S.X > screenW) continue;
		if (player.RootW2S.Y < 0 || player.RootW2S.Y > screenH) continue;
		if (player.RootW2S.X < 0 || player.RootW2S.X > screenW) continue;

		if (player.distance > Config.maxDistance) continue;

		if (player.visible == false && Config.VisibleOnly) continue;

		if (!Config.TeamCheck && player.teamId == cache.LocalPlayer.teamId) {
			continue;
		}

		 

		if (player.NameEnt.health < 1) continue;

		float boxHeight = abs(player.headW2S.Y - player.RootW2S.Y);
		float boxWidth = boxHeight * 0.55f;

		if (boxHeight < 5.0f || boxHeight > 500.0f) continue;
		if (boxWidth < 3.0f || boxWidth > 300.0f) continue;


		bool isTeammate = (player.teamId == cache.LocalPlayer.teamId);
		ImColor boxColor = isTeammate ? ImColor(Config.ColorTeamBox) : ImColor(Config.ColorEnemyBox);
		ImColor nameColor = isTeammate ? ImColor(Config.ColorTeamName) : ImColor(Config.ColorEnemyName);


		if (Config.ESPEnabled && Config.BoxESP) {

			 
			DrawBox(
				FVector2D(player.headW2S.X, (player.headW2S.Y + player.RootW2S.Y) / 2),
				boxWidth, boxHeight,
				boxColor,
				Config.PlayerThickness,
				Config.BoxStyle
			);
		}

		if (Config.DrawNames && player.NameEnt.name[0] != 0) {
			DrawNames(
				FVector2D(player.headW2S.X, player.headW2S.Y - 12),
				std::string(player.NameEnt.name),
				nameColor
			);
		}

		if (Config.DrawHP && player.NameEnt.health > 0) {
			float boxCenterY = (player.headW2S.Y + player.RootW2S.Y) / 2.0f;
			float boxLeft = player.headW2S.X - (boxWidth / 2.0f);
			const float barWidth = 6.0f;
			const float gap = 6.0f;
			FVector2D barCenter;
			barCenter.X = boxLeft - gap - (barWidth / 2.0f);
			barCenter.Y = boxCenterY;
			DrawHealthBar(barCenter, (float)player.NameEnt.health, barWidth, boxHeight);
		}


		if (Config.DrawHeadCircle) {
			DrawHeadCircle(
				FVector2D(player.headW2S.X, player.headW2S.Y),
				boxWidth / 2,
				ImColor(Config.ColorHead),
				Config.PlayerThickness
			);
		}

		if (Config.DrawBones) {
			DrawSkeleton(player, ImColor(Config.ColorBones), Config.PlayerThickness);
		}

		if (Config.DrawTraceline) {
			FVector2D start{ static_cast<float>(Globals::screenWidth) * 0.5f, static_cast<float>(Globals::screenHeight) };
			ImColor traceColor = ImColor(Config.ColorLines);
			DrawTraceline(start, FVector2D(player.RootW2S.X, player.RootW2S.Y), traceColor, Config.ItemThickness);
		}


		if (Config.DistanceESP) {
			DrawDistanceText(
				FVector2D(player.RootW2S.X, player.RootW2S.Y),
				(float)player.distance,
				ImColor(Config.ColorDistance)
			);
		}
	}
	 
	 
}



static ID3D11Device* g_pd3dDevice = nullptr;
static ID3D11DeviceContext* g_pd3dDeviceContext = nullptr;
static IDXGISwapChain* g_pSwapChain = nullptr;
static bool                     g_SwapChainOccluded = false;
static UINT                     g_ResizeWidth = 0, g_ResizeHeight = 0;
static ID3D11RenderTargetView* g_mainRenderTargetView = nullptr;

void CreateRenderTarget()
{
	ID3D11Texture2D* pBackBuffer;
	g_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
	g_pd3dDevice->CreateRenderTargetView(pBackBuffer, nullptr, &g_mainRenderTargetView);
	pBackBuffer->Release();
}

bool CreateDeviceD3D(HWND hWnd)
{
	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.BufferCount = 2;
	sd.BufferDesc.Width = 0;
	sd.BufferDesc.Height = 0;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 0;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = hWnd;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = TRUE;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	UINT createDeviceFlags = 0;
	D3D_FEATURE_LEVEL featureLevel;
	const D3D_FEATURE_LEVEL featureLevelArray[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0, };
	HRESULT res = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &featureLevel, &g_pd3dDeviceContext);
	if (res == DXGI_ERROR_UNSUPPORTED)
		res = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_WARP, nullptr, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &featureLevel, &g_pd3dDeviceContext);
	if (res != S_OK)
		return false;

	CreateRenderTarget();
	return true;
}

void CleanupRenderTarget()
{
	if (g_mainRenderTargetView) { g_mainRenderTargetView->Release(); g_mainRenderTargetView = nullptr; }
}

void CleanupDeviceD3D()
{
	CleanupRenderTarget();
	if (g_pSwapChain) { g_pSwapChain->Release(); g_pSwapChain = nullptr; }
	if (g_pd3dDeviceContext) { g_pd3dDeviceContext->Release(); g_pd3dDeviceContext = nullptr; }
	if (g_pd3dDevice) { g_pd3dDevice->Release(); g_pd3dDevice = nullptr; }
}

inline std::vector<unsigned char> LoadResourceData(int resourceId)
{
	HRSRC hRes = FindResource(
		nullptr,
		MAKEINTRESOURCE(resourceId),
		RT_RCDATA);

	if (!hRes) {
		std::cout << "FindResource failed for ID " << resourceId
			<< ", GetLastError = " << GetLastError() << "\n";
		return {};
	}


	HGLOBAL hGlob = LoadResource(nullptr, hRes);
	if (!hGlob) return {};

	DWORD size = SizeofResource(nullptr, hRes);
	const void* p = LockResource(hGlob);
	if (!p || size == 0) return {};


	const unsigned char* bytes = static_cast<const unsigned char*>(p);
	return std::vector<unsigned char>(bytes, bytes + size);
}

inline  ImFont* LoadFontFromResource(int resourceId, float fontSize, ImGuiIO& io) {
	std::vector<unsigned char> fontData = LoadResourceData(resourceId);
	if (fontData.empty()) return nullptr;


	void* fontMemory = IM_ALLOC(fontData.size());
	memcpy(fontMemory, fontData.data(), fontData.size());

	ImFontConfig config;
	config.FontDataOwnedByAtlas = true;

	return io.Fonts->AddFontFromMemoryTTF(fontMemory, fontData.size(), fontSize, &config);
}

inline  bool LoadTextureFromFile(const char* file_name, ID3D11ShaderResourceView** out_srv, int* out_width, int* out_height)
{
	FILE* f = fopen(file_name, "rb");
	if (f == NULL)
		return false;

	fseek(f, 0, SEEK_END);
	size_t file_size = (size_t)ftell(f);
	if (file_size == -1) {
		fclose(f);
		return false;
	}

	fseek(f, 0, SEEK_SET);
	void* file_data = IM_ALLOC(file_size);
	fread(file_data, 1, file_size, f);
	fclose(f);


	int image_width = 0;
	int image_height = 0;
	unsigned char* image_data = stbi_load_from_memory((const unsigned char*)file_data, (int)file_size, &image_width, &image_height, NULL, 4);
	IM_FREE(file_data);

	if (image_data == NULL)
		return false;


	D3D11_TEXTURE2D_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.Width = image_width;
	desc.Height = image_height;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.SampleDesc.Count = 1;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = 0;

	ID3D11Texture2D* pTexture = NULL;
	D3D11_SUBRESOURCE_DATA subResource;
	subResource.pSysMem = image_data;
	subResource.SysMemPitch = desc.Width * 4;
	subResource.SysMemSlicePitch = 0;

	HRESULT hr = g_pd3dDevice->CreateTexture2D(&desc, &subResource, &pTexture);
	if (FAILED(hr)) {
		stbi_image_free(image_data);
		return false;
	}


	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(srvDesc));
	srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = desc.MipLevels;
	srvDesc.Texture2D.MostDetailedMip = 0;

	hr = g_pd3dDevice->CreateShaderResourceView(pTexture, &srvDesc, out_srv);
	pTexture->Release();

	*out_width = image_width;
	*out_height = image_height;
	stbi_image_free(image_data);

	return SUCCEEDED(hr);
}

inline  bool LoadTextureFromResource(int resourceId, ID3D11ShaderResourceView** out_srv, int* out_width, int* out_height) {
	std::vector<unsigned char> imageData = LoadResourceData(resourceId);
	if (imageData.empty()) return false;


	int image_width = 0;
	int image_height = 0;
	unsigned char* decoded_data = stbi_load_from_memory(imageData.data(), imageData.size(), &image_width, &image_height, NULL, 4);

	if (decoded_data == NULL) return false;


	D3D11_TEXTURE2D_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.Width = image_width;
	desc.Height = image_height;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.SampleDesc.Count = 1;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = 0;

	ID3D11Texture2D* pTexture = NULL;
	D3D11_SUBRESOURCE_DATA subResource;
	subResource.pSysMem = decoded_data;
	subResource.SysMemPitch = desc.Width * 4;
	subResource.SysMemSlicePitch = 0;

	HRESULT hr = g_pd3dDevice->CreateTexture2D(&desc, &subResource, &pTexture);
	if (FAILED(hr)) {
		stbi_image_free(decoded_data);
		return false;
	}


	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(srvDesc));
	srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = desc.MipLevels;
	srvDesc.Texture2D.MostDetailedMip = 0;

	hr = g_pd3dDevice->CreateShaderResourceView(pTexture, &srvDesc, out_srv);
	pTexture->Release();

	*out_width = image_width;
	*out_height = image_height;
	stbi_image_free(decoded_data);

	return SUCCEEDED(hr);
}

LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);


void SetOverlayMode(HWND hwnd, int mode, bool menuVisible) {
	LONG style = GetWindowLong(hwnd, GWL_EXSTYLE);
	style |= WS_EX_TOOLWINDOW;

	bool transparent = (mode == 0);
	MARGINS m = transparent ? MARGINS{ -1,-1,-1,-1 } : MARGINS{ 0,0,0,0 };
	DwmExtendFrameIntoClientArea(hwnd, &m);

	if (transparent) {
		style |= WS_EX_LAYERED | WS_EX_TOPMOST;
		if (!menuVisible) style |= WS_EX_TRANSPARENT | WS_EX_NOACTIVATE;
		else style &= ~(WS_EX_TRANSPARENT | WS_EX_NOACTIVATE);
		SetLayeredWindowAttributes(hwnd, 0, 255, LWA_COLORKEY | LWA_ALPHA);
		SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | (menuVisible ? 0 : SWP_NOACTIVATE));
	}
	else {
		style &= ~(WS_EX_LAYERED | WS_EX_TOPMOST);
		if (menuVisible) style &= ~(WS_EX_TRANSPARENT | WS_EX_NOACTIVATE);
		else style |= WS_EX_TRANSPARENT | WS_EX_NOACTIVATE;
		SetWindowPos(hwnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | (menuVisible ? 0 : SWP_NOACTIVATE));
	}

	SetWindowLong(hwnd, GWL_EXSTYLE, style);
}

void SetOverlayMode2(HWND hwnd, int overlayMode, bool showMenu)
{
	LONG style = GetWindowLong(hwnd, GWL_EXSTYLE);

	style |= WS_EX_TOOLWINDOW;

	if (overlayMode == 0) {

		MARGINS margins = { -1, -1, -1, -1 };
		DwmExtendFrameIntoClientArea(hwnd, &margins);

		if (showMenu) {
			style &= ~WS_EX_LAYERED;
			style &= ~WS_EX_TRANSPARENT; 
			style &= ~WS_EX_NOACTIVATE;

			SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, 0, 0,
				SWP_NOMOVE | SWP_NOSIZE);
		}
		else {
			style |= WS_EX_LAYERED;
			style |= WS_EX_TRANSPARENT;
			style |= WS_EX_NOACTIVATE;

			SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, 0, 0,
				SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
		}
	}
	else {

		style &= ~WS_EX_LAYERED; 

		MARGINS margins = { 0, 0, 0, 0 };
		DwmExtendFrameIntoClientArea(hwnd, &margins);

		if (showMenu) {

			style &= ~WS_EX_TRANSPARENT;
			style &= ~WS_EX_NOACTIVATE;

			SetWindowPos(hwnd, HWND_NOTOPMOST, 0, 0, 0, 0,
				SWP_NOMOVE | SWP_NOSIZE);
		}
		else {
			style |= WS_EX_TRANSPARENT; 
			style |= WS_EX_NOACTIVATE; 

			SetWindowPos(hwnd, HWND_NOTOPMOST, 0, 0, 0, 0,
				SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
		}
	}

	SetWindowLong(hwnd, GWL_EXSTYLE, style);
}

void SwitchToMonitor(HWND hwnd, int idx) {
	DISPLAY_DEVICE dd{ sizeof(dd) }; DEVMODE dm{ sizeof(dm) };
	EnumDisplayDevices(NULL, idx, &dd, 0) && EnumDisplaySettings(dd.DeviceName, ENUM_CURRENT_SETTINGS, &dm) &&
		SetWindowPos(hwnd, HWND_TOPMOST, dm.dmPosition.x, dm.dmPosition.y, dm.dmPelsWidth, dm.dmPelsHeight, SWP_NOACTIVATE);
}

 

void Render::Init() {
	WNDCLASSEXW wc = { sizeof(wc), CS_CLASSDC, WndProc, 0L, 0L,
		GetModuleHandle(nullptr), nullptr, nullptr, nullptr, nullptr, L"Overlay", nullptr };
	RegisterClassExW(&wc);

	_PlayerCache localplayer;
	Globals::screenWidth = GetSystemMetrics(SM_CXSCREEN);
	Globals::screenHeight = GetSystemMetrics(SM_CYSCREEN);

	HWND hwnd = CreateWindowExW(WS_EX_TOOLWINDOW | WS_EX_TOPMOST,
		wc.lpszClassName, L"Overlay", WS_POPUP,
		0, 0, Globals::screenWidth,Globals::screenHeight,
		nullptr, nullptr, wc.hInstance, nullptr);

	Config.overlayHWND = hwnd;

	BOOL compositionEnabled = FALSE;
	DwmSetWindowAttribute(Config.overlayHWND, DWMWA_EXCLUDED_FROM_PEEK, &compositionEnabled, sizeof(compositionEnabled));
	DwmSetWindowAttribute(Config.overlayHWND, DWMWA_DISALLOW_PEEK, &compositionEnabled, sizeof(compositionEnabled));

	SetOverlayMode2(Config.overlayHWND, Config.overlayMode, Config.sidebarOpen);

	if (!CreateDeviceD3D(hwnd)) {
		CleanupDeviceD3D();
		UnregisterClassW(wc.lpszClassName, wc.hInstance);
		return;
	}

 
	::ShowWindow(hwnd, SW_SHOWDEFAULT);
	::UpdateWindow(hwnd);

 
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;    
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
	 
	 
	Config.logoFont = LoadFontFromResource(IDR_FONT1, 32.0f, io); 
	Config.headerFont = LoadFontFromResource(IDR_FONT2, 22.0f, io); 
	Config.regularFont = LoadFontFromResource(IDR_FONT3, 19.0f, io);  

 
	if (!Config.logoFont) Config.logoFont = io.Fonts->AddFontDefault();
	if (!Config.headerFont) Config.headerFont = io.Fonts->AddFontDefault();
	if (!Config.regularFont) Config.regularFont = io.Fonts->AddFontDefault();

	 
	ID3D11ShaderResourceView* my_texture = NULL;
	int my_image_width = 0;
	int my_image_height = 0;
	bool ret = LoadTextureFromResource(IDR_LOGO_PNG, &my_texture, &my_image_width, &my_image_height);


	if (ret) {
		Config.logoTexture = (ImTextureID)my_texture;
		Config.logoWidth = my_image_width;
		Config.logoHeight = my_image_height;

	}
	else {
		std::cout << "Failed to load logo texture!" << std::endl;
		Config.logoTexture = 0;
		Config.logoWidth = 0;
		Config.logoHeight = 0;
	}

 
	ImGui::StyleColorsDark();
	 

	 
	ImGui_ImplWin32_Init(hwnd);
	ImGui_ImplDX11_Init(g_pd3dDevice, g_pd3dDeviceContext);


	std::string defaultConfig = ConfigManager::GetDefaultConfig();
	if (ConfigManager::LoadConfig(defaultConfig)) {
		std::cout << "[Config] Auto-loaded: " << defaultConfig << std::endl;
	} else {
		std::cout << "[Config] No default config found, using program defaults" << std::endl;
	}

	bool show_demo_window = true;
	bool show_another_window = false;
	 
 
	bool done = false;
	while (!done && Config.running)
	{

		MSG msg;
		while (::PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE))
		{
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
			if (msg.message == WM_QUIT)
				done = true;
		}
		if (done || !Config.running)
			break;

		
		if (g_SwapChainOccluded && g_pSwapChain->Present(0, DXGI_PRESENT_TEST) == DXGI_STATUS_OCCLUDED)
		{
			::Sleep(10);
			continue;
		}
		g_SwapChainOccluded = false;

		 
		if (g_ResizeWidth != 0 && g_ResizeHeight != 0)
		{
			CleanupRenderTarget();
			g_pSwapChain->ResizeBuffers(0, g_ResizeWidth, g_ResizeHeight, DXGI_FORMAT_UNKNOWN, 0);
			g_ResizeWidth = g_ResizeHeight = 0;
			CreateRenderTarget();
		}

		 
		
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		 

		 

		Render::Loop();

		DrawSidebar();

		ImGui::Render();

		ImVec4 clear_color;
		if (Config.overlayMode == 0) {clear_color = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
		}
		else {
			
			clear_color = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
		}
		 
		const float clear_color_with_alpha[4] = { clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w };
		g_pd3dDeviceContext->OMSetRenderTargets(1, &g_mainRenderTargetView, nullptr);
		g_pd3dDeviceContext->ClearRenderTargetView(g_mainRenderTargetView, clear_color_with_alpha);
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

		
		HRESULT hr = g_pSwapChain->Present(Config.vsync == 1 ? 1 : 0, 0); 
		if (!Config.vsync && Config.fps_limit > 0) {
			static auto lastFrameTime = std::chrono::high_resolution_clock::now();

			auto currentTime = std::chrono::high_resolution_clock::now();
			double elapsedTime = std::chrono::duration<double>(currentTime - lastFrameTime).count();

			const double frameTime = 1.0 / (double)Config.fps_limit;
			if (elapsedTime < frameTime) {
				DWORD sleepTime = static_cast<DWORD>((frameTime - elapsedTime) * 1000);

				timeBeginPeriod(1);
				Sleep(sleepTime);
				timeEndPeriod(1);
			}

			lastFrameTime = std::chrono::high_resolution_clock::now();
		}

		
		g_SwapChainOccluded = (hr == DXGI_STATUS_OCCLUDED);
	}

 
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	CleanupDeviceD3D();
	::DestroyWindow(hwnd);
	::UnregisterClassW(wc.lpszClassName, wc.hInstance);

	return;
}


extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
		return true;

	switch (msg)
	{
	case WM_SIZE:
		if (wParam == SIZE_MINIMIZED)
			return 0;
		g_ResizeWidth = (UINT)LOWORD(lParam);
		g_ResizeHeight = (UINT)HIWORD(lParam);
		return 0;
	case WM_SYSCOMMAND:
		if ((wParam & 0xfff0) == SC_KEYMENU)
			return 0;
		break;
	case WM_DESTROY:
		::PostQuitMessage(0);
		return 0;
	}
	return ::DefWindowProcW(hWnd, msg, wParam, lParam);
}