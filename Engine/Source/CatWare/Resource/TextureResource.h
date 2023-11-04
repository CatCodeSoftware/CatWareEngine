#pragma once

#include <unordered_map>

#include "CatWare/Core.h"
#include "CatWare/Graphics/Renderer/Texture.h"

namespace CatWare
{
	/* Internal class that uses a refrence counting system that
	   Automatically unloads the texture if unused

	   TODO:
		- Improve the refrence counting method
		- Make this type of autocleanup available for diffrent resources
	*/
	class TextureResource
	{
	public:
		TextureResource( std::string filePath, Rendering::TextureFilter textureFilter );
		~TextureResource( );

		Rendering::Texture2D* GetTexture( );

		void Load( );
		void Unload( );

		void ResetRefrenceCount( );
		unsigned int GetRefrenceCount( );

	private:
		Rendering::Texture2D* texture = nullptr;
		Rendering::TextureFilter textureFilter;

		unsigned int refrenceCount = 0;

		bool isLoaded = false;
		bool dontRemove = false;

		std::string filePath;
	};

	class CATWARE_API TextureManager
	{
	public:
		// Adds a texture to the registry, returns if adding was succesful 
		static bool	AddTexture( std::string	name, std::string path, Rendering::TextureFilter textureFilter = Rendering::TextureFilter::LINEAR );

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