#pragma once

#include <unordered_map>

#include "CatWare/Core.h"
#include "CatWare/Graphics/Renderer/Texture.h"

namespace CatWare
{
	class TextureResource
	{
	public:
		TextureResource( std::string filePath );
		~TextureResource( );

		Rendering::Texture2D* GetTexture( );

		void Load( );
		void Unload( );

		void ResetRefrenceCount( );
	private:
		Rendering::Texture2D* texture = nullptr;

		unsigned int refrenceCount = 0;

		bool isLoaded = false;
		bool dontRemove = false;

		std::string filePath;
	};

	class CATWARE_API TextureManager
	{
	public:
		// Adds a texture to the registry, returns if adding was succesful 
		static bool	AddTexture( std::string	name, std::string path );

		// Removes a texture from the registry, returns is removing was succesful
		static bool RemoveTexture( std::string name );

		// Returns a Texture2D pointer, NOT a TextureResource
		static Rendering::Texture2D* GetTexture( std::string name );

		static void CleanUpTextures( );

		static void RemoveEverything( );

	private:
		static std::unordered_map<std::string, TextureResource*> textureRegistry;
	};
}