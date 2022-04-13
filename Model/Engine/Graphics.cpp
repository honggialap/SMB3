#pragma region INCLUDE
#include "Graphics.h"
#include "Ultilities.h"
#pragma endregion

#pragma region Texture

CTexture::CTexture(
	ID3D10Texture2D* texture, 
	ID3D10ShaderResourceView* rsview
) {
	_texture = texture;
	_rsview = rsview;

	D3D10_TEXTURE2D_DESC desc;
	_texture->GetDesc(&desc);
	_width = desc.Width;
	_height = desc.Height;
}

CTexture::~CTexture() {
	if (_rsview) {
		_rsview->Release();
	}

	if (_texture) {
		_texture->Release();
	}
}

#pragma endregion

#pragma region Direct3D

bool CGraphics::CreateSwapChain(
	HWND hWnd
) {
	RECT rect;
	GetClientRect(hWnd, &rect);
	_backBufferWidth = rect.right + 1;
	_backBufferHeight = rect.bottom + 1;

	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));
	swapChainDesc.BufferCount = 1;
	swapChainDesc.BufferDesc.Width = _backBufferWidth;
	swapChainDesc.BufferDesc.Height = _backBufferHeight;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.OutputWindow = hWnd;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.Windowed = TRUE;

	HRESULT result = D3D10CreateDeviceAndSwapChain(
		NULL,
		D3D10_DRIVER_TYPE_REFERENCE,
		NULL,
		0,
		D3D10_SDK_VERSION,
		&swapChainDesc,
		&_swapChain,
		&_device
	);
	if (result != S_OK) {
		DebugOut(L"[Engine] Create D3D10 Device failed.\n");
		return false;
	}

	return true;
}

bool CGraphics::CreateRenderTargetView() {
	ID3D10Texture2D* pBackBuffer;
	HRESULT result = _swapChain->GetBuffer(
		0,
		__uuidof(ID3D10Texture2D),
		(LPVOID*)&pBackBuffer
	);
	if (result != S_OK) {
		DebugOut(L"[Engine] Get D3D10 Back buffer failed.\n");
		return false;
	}

	result = _device->CreateRenderTargetView(
		pBackBuffer,
		NULL,
		&_renderTargetView
	);
	pBackBuffer->Release();
	if (result != S_OK) {
		DebugOut(L"[Engine] Create D3D10 Render target view failed.\n");
		return false;
	}

	_device->OMSetRenderTargets(
		1,
		&_renderTargetView,
		NULL
	);

	return true;
}

bool CGraphics::CreateSpriteHandler() {
	D3D10_VIEWPORT viewPort;
	viewPort.Width = _backBufferWidth;
	viewPort.Height = _backBufferHeight;
	viewPort.MinDepth = 0.0f;
	viewPort.MaxDepth = 1.0f;
	viewPort.TopLeftX = 0;
	viewPort.TopLeftY = 0;
	_device->RSSetViewports(1, &viewPort);

	D3D10_SAMPLER_DESC desc;
	desc.Filter = D3D10_FILTER_MIN_MAG_POINT_MIP_LINEAR;
	desc.AddressU = D3D10_TEXTURE_ADDRESS_CLAMP;
	desc.AddressV = D3D10_TEXTURE_ADDRESS_CLAMP;
	desc.AddressW = D3D10_TEXTURE_ADDRESS_CLAMP;
	desc.MipLODBias = 0;
	desc.MaxAnisotropy = 1;
	desc.ComparisonFunc = D3D10_COMPARISON_NEVER;
	desc.BorderColor[0] = 1.0f;
	desc.BorderColor[1] = 1.0f;
	desc.BorderColor[2] = 1.0f;
	desc.BorderColor[3] = 1.0f;
	desc.MinLOD = -FLT_MAX;
	desc.MaxLOD = FLT_MAX;

	_device->CreateSamplerState(&desc, &this->_pointSamplerState);
	_device->VSSetSamplers(0, 1, &_pointSamplerState);
	_device->GSSetSamplers(0, 1, &_pointSamplerState);
	_device->PSSetSamplers(0, 1, &_pointSamplerState);

	HRESULT result = D3DX10CreateSprite(
		_device,
		0,
		&_spriteHandler
	);
	if (result != S_OK) {
		DebugOut(L"[Engine] Create D3D10 Sprite handler failed.\n");
		return false;
	}

	D3DXMATRIX matProjection;
	D3DXMatrixOrthoOffCenterLH(
		&matProjection,
		(float)viewPort.TopLeftX,
		(float)viewPort.Width,
		(float)viewPort.TopLeftY,
		(float)viewPort.Height,
		0.1f,
		10
	);

	result = _spriteHandler->SetProjectionTransform(&matProjection);
	if (result != S_OK) {
		DebugOut(L"[Engine] Set D3D10 viewport failed.\n");
		return false;
	}

	return true;
}

bool CGraphics::CreateBlendState() {
	D3D10_BLEND_DESC StateDesc;
	ZeroMemory(&StateDesc, sizeof(D3D10_BLEND_DESC));
	StateDesc.AlphaToCoverageEnable = FALSE;
	StateDesc.BlendEnable[0] = TRUE;
	StateDesc.SrcBlend = D3D10_BLEND_SRC_ALPHA;
	StateDesc.DestBlend = D3D10_BLEND_INV_SRC_ALPHA;
	StateDesc.BlendOp = D3D10_BLEND_OP_ADD;
	StateDesc.SrcBlendAlpha = D3D10_BLEND_ZERO;
	StateDesc.DestBlendAlpha = D3D10_BLEND_ZERO;
	StateDesc.BlendOpAlpha = D3D10_BLEND_OP_ADD;
	StateDesc.RenderTargetWriteMask[0] = D3D10_COLOR_WRITE_ENABLE_ALL;

	HRESULT result = _device->CreateBlendState(
		&StateDesc,
		&_blendStateAlpha
	);
	if (result != S_OK) {
		DebugOut(L"[Engine] Create D3D10 Blend state failed.\n");
		return false;
	}

	return true;
}

bool CGraphics::Initialize(
	HWND hWnd
) {
	if (!CreateSwapChain(hWnd)) {
		return false;
	}

	if (!CreateRenderTargetView()) {
		return false;
	}

	if (!CreateSpriteHandler()) {
		return false;
	}

	if (!CreateBlendState()) {
		return false;
	}

	return true;
}

void CGraphics::Shutdown() {
	for (auto texture : _textures) {
		delete texture.second;
		texture.second = nullptr;
	}
	_textures.clear();

	if (_blendStateAlpha) {
		_blendStateAlpha->Release();
	}

	if (_spriteHandler) {
		_spriteHandler->Release();
	}

	if (_renderTargetView) {
		_renderTargetView->Release();
	}

	if (_swapChain) {
		_swapChain->Release();
	}

	if (_device) {
		_device->Release();
	}
}

#pragma endregion

#pragma region Textures Database

pTexture CGraphics::LoadTextureFromFile(
	std::wstring sourcePath
) {
	ID3D10Resource* pD3D10Resource = NULL;
	ID3D10Texture2D* tex = NULL;

	D3DX10_IMAGE_INFO imageInfo;
	HRESULT result = D3DX10GetImageInfoFromFile(
		sourcePath.c_str(),
		NULL,
		&imageInfo,
		NULL
	);
	if (FAILED(result)) {
		DebugOut(L"[Engine] Texture load failed: %s\n", sourcePath.c_str());
		return nullptr;
	}

	D3DX10_IMAGE_LOAD_INFO info;
	ZeroMemory(&info, sizeof(D3DX10_IMAGE_LOAD_INFO));
	info.Width = imageInfo.Width;
	info.Height = imageInfo.Height;
	info.Depth = imageInfo.Depth;
	info.FirstMipLevel = 0;
	info.MipLevels = 1;
	info.Usage = D3D10_USAGE_DEFAULT;
	info.BindFlags = D3DX10_DEFAULT;
	info.CpuAccessFlags = D3DX10_DEFAULT;
	info.MiscFlags = D3DX10_DEFAULT;
	info.Format = imageInfo.Format;
	info.Filter = D3DX10_FILTER_NONE;
	info.MipFilter = D3DX10_DEFAULT;
	info.pSrcInfo = &imageInfo;

	result = D3DX10CreateTextureFromFile(
		_device,
		sourcePath.c_str(),
		&info,
		NULL,
		&pD3D10Resource,
		NULL
	);
	if (FAILED(result)) {
		DebugOut(L"[Engine] Texture load failed: %s\n", sourcePath.c_str());
		return nullptr;
	}

	pD3D10Resource->QueryInterface(
		__uuidof(ID3D10Texture2D),
		(LPVOID*)&tex
	);
	pD3D10Resource->Release();
	if (!tex) {
		DebugOut(L"[Engine] Texture load failed: %s\n", sourcePath.c_str());
		return nullptr;
	}

	D3D10_TEXTURE2D_DESC desc;
	tex->GetDesc(&desc);

	D3D10_SHADER_RESOURCE_VIEW_DESC SRVDesc;
	ZeroMemory(&SRVDesc, sizeof(SRVDesc));
	SRVDesc.Format = desc.Format;
	SRVDesc.ViewDimension = D3D10_SRV_DIMENSION_TEXTURE2D;
	SRVDesc.Texture2D.MipLevels = desc.MipLevels;

	ID3D10ShaderResourceView* gSpriteTextureRV = NULL;
	result = _device->CreateShaderResourceView(
		tex,
		&SRVDesc,
		&gSpriteTextureRV
	);
	if (FAILED(result)) {
		DebugOut(L"[Engine] Texture load failed: %s\n", sourcePath.c_str());
		return nullptr;
	}

	DebugOut(L"[Engine] Texture loaded: %s\n", sourcePath.c_str());
	return new CTexture(tex, gSpriteTextureRV);
}

void CGraphics::LoadTexture(
	unsigned int id,
	std::wstring sourcePath
) {
	if (_textures.find(id) != _textures.end()) {
		DebugOut(L"[Engine] Texture ID is already existed: %d.\n", id);
		return;
	}

	auto texture = LoadTextureFromFile(sourcePath);
	if (texture == nullptr) {
		return;
	}

	_textures[id] = texture;
}

pTexture CGraphics::GetTexture(
	unsigned int id
) {
	if (_textures.find(id) == _textures.end()) {
		DebugOut(L"[Engine] Texture not found: %d\n", id);
		return nullptr;
	}

	return _textures[id];
}

#pragma endregion