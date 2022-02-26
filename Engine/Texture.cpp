#pragma region INCLUDE
#include "Texture.h"
#include "Ultilities.h"
#pragma endregion

namespace Engine
{

	Texture::Texture(
		ID3D10Texture2D* texture,
		ID3D10ShaderResourceView* rsview
	)
	{
		_texture = texture;
		_rsview = rsview;

		D3D10_TEXTURE2D_DESC desc;
		_texture->GetDesc(&desc);
		_width = desc.Width;
		_height = desc.Height;
	}

	Texture::~Texture()
	{
		if (_rsview != NULL)
		{
			_rsview->Release();
		}

		if (_texture != NULL)
		{
			_texture->Release();
		}
	}

	Textures::Textures(
		pGraphics graphics
	)
	{
		_graphics = graphics;
	}

	Textures::~Textures()
	{
		_graphics = nullptr;
	}

	void Textures::Load(
		unsigned int id, 
		std::wstring sourcePath
	)
	{
		if (_textures.find(id) != _textures.end())
		{
			DebugOut(L"[Engine] Texture id is already used: %d.\n", id);
			return;
		}

		auto texture = LoadTexture(sourcePath);
		if (texture == nullptr)
		{
			return;
		}

		AddTexture(id, texture);
	}

	pTexture Textures::LoadTexture(
		std::wstring sourcePath
	)
	{
		HRESULT result;
		ID3D10Resource* pD3D10Resource = NULL;
		ID3D10Texture2D* tex = NULL;

		D3DX10_IMAGE_INFO imageInfo;
		result = D3DX10GetImageInfoFromFile(sourcePath.c_str(), NULL, &imageInfo, NULL);
		if (FAILED(result))
		{
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
			_graphics->GetDevice(),
			sourcePath.c_str(),
			&info,
			NULL,
			&pD3D10Resource,
			NULL);
		if (FAILED(result))
		{
			DebugOut(L"[Engine] Texture load failed: %s\n", sourcePath.c_str());
			return nullptr;
		}

		pD3D10Resource->QueryInterface(__uuidof(ID3D10Texture2D), (LPVOID*)&tex);
		pD3D10Resource->Release();
		if (!tex)
		{
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
		result = _graphics->GetDevice()->CreateShaderResourceView(tex, &SRVDesc, &gSpriteTextureRV);
		if (FAILED(result))
		{
			DebugOut(L"[Engine] Texture load failed: %s\n", sourcePath.c_str());
			return nullptr;
		}

		DebugOut(L"[Engine] Texture loaded: %s\n", sourcePath.c_str());
		return new Texture(tex, gSpriteTextureRV);
	}

	void Textures::AddTexture(
		unsigned int id, 
		pTexture texture
	)
	{
		_textures[id] = texture;
	}

	pTexture Textures::GetTexture(
		unsigned int id
	)
	{
		if (_textures.find(id) == _textures.end())
		{
			DebugOut(L"[Engine] Texture not found: %d\n", id);
			return nullptr;
		}

		return _textures[id];
	}

	void Textures::Clean()
	{
		for (auto texture : _textures)
		{
			delete texture.second;
			texture.second = nullptr;
		}
		_textures.clear();
	}

}