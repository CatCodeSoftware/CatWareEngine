#pragma once

#include <unordered_map>

#include "CatWare/Graphics/Renderer/Texture.h"

namespace CatWare
{
	class TextureAsset
	{
	public:
		~TextureAsset( );

		Rendering::Texture2D* texture = nullptr;
		Rendering::TextureFilter filter;
		std::string path;

		Int16 refs = 0;

		void Load( );
		void Unload( );
	};

	class TextureRef
	{
	public:
		TextureRef( );
		TextureRef( std::string path );
		~TextureRef( );

		Rendering::Texture2D* Get( );

		inline std::string GetPath( ) { return path; }

	private:
		std::string path;
	};

	class TextureManager
	{
		friend class TextureRef;
	public:
		static void DoCleanup( );
		static void AddTexture( std::string path, Rendering::TextureFilter filter );

	private:
		static inline std::unordered_map<std::string, TextureAsset*> textures;

		static void AddRef( TextureRef* ref );
		static void ReleaseRef( TextureRef* ref );
		static Rendering::Texture2D* GetTexture( std::string path );
	};
}