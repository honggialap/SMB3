#pragma once
#ifndef __TEXTURE_H__
#define __TEXTURE_H__

#pragma region INCLUDE
#include "Graphics.h"
#include <unordered_map>
#pragma endregion

namespace Engine
{

	struct Texture
	{
		ID3D10Texture2D* _texture;
		ID3D10ShaderResourceView* _rsview;
		int _width = 0;
		int _height = 0;

		Texture(
			ID3D10Texture2D* texture,
			ID3D10ShaderResourceView* rsview
		);
		~Texture();
	};
	typedef Texture* pTexture;

	class Textures
	{
	private:
		pGraphics _graphics;
		std::unordered_map<unsigned int, pTexture> _textures;

	public:
		Textures(
			pGraphics graphics
		);
		~Textures();

		void Load(
			unsigned int id,
			std::wstring sourcePath
		);
		pTexture LoadTexture(
			std::wstring sourcePath
		);
		void AddTexture(
			unsigned int id, 
			pTexture texture
		);
		pTexture GetTexture(
			unsigned int id
		);
		void Clean();
	};
	typedef Textures* pTextures;

}

#endif // !__TEXTURE_H__
