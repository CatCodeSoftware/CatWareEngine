#include "TextureResource.h"

namespace CatWare
{
	// -----------------------------------------
	// TextureResource -------------------------
	// -----------------------------------------
	TextureResource::TextureResource( std::string filePath )
	{
		this->dontRemove = false;
		this->filePath = filePath;

		isLoaded = false;
		refrenceCount = false;
	}

	TextureResource::~TextureResource( )
	{
		delete texture;
	}

	Rendering::Texture2D* TextureResource::GetTexture( )
	{
		if ( !isLoaded )
			Load( );

		refrenceCount++;

		return texture;
	}
	void TextureResource::Load( )
	{
		texture = Rendering::Texture2D::Create( filePath );

		if ( texture == nullptr )
			return;

		isLoaded = true;
	}
	void TextureResource::Unload( )
	{
		delete texture;
		isLoaded = false;
	}

	void TextureResource::ResetRefrenceCount( )
	{
		refrenceCount = 0;
	}

	// -----------------------------------------
	// TextureManager --------------------------
	// -----------------------------------------
	std::unordered_map<std::string, TextureResource*> TextureManager::textureRegistry;

	bool TextureManager::AddTexture( std::string name, std::string path )
	{
		TextureResource* textureResource = new TextureResource( path );

		textureRegistry.insert( { name, textureResource } );

		return true;
	}

	bool TextureManager::RemoveTexture( std::string name )
	{
		auto keyVal = textureRegistry.find( name );
		delete keyVal->second;
		
		textureRegistry.erase( name );

		return true;
	}

	Rendering::Texture2D* TextureManager::GetTexture( std::string name )
	{
		auto keyVal = textureRegistry.find( name );
		return keyVal->second->GetTexture( );
	}

	void TextureManager::CleanUpTextures( )
	{
		// i don't know what the criteria for what textures to dump should be here - PT
	}
}