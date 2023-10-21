#include "TextureResource.h"

#include "CatWare/Utils/Log.h"

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
		Unload( );
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
		CW_ENGINE_LOG->Info( "Loading texture from %s", filePath.c_str( ) );

		texture = Rendering::Texture2D::Create( filePath );

		if ( texture == nullptr )
			return;

		isLoaded = true;
	}
	void TextureResource::Unload( )
	{
		if ( isLoaded )
		{
			CW_ENGINE_LOG->Info( "Unloading texture %s", filePath.c_str( ) );

			delete texture;
		}

		isLoaded = false;
	}

	void TextureResource::ResetRefrenceCount( )
	{
		refrenceCount = 0;
	}

	unsigned int TextureResource::GetRefrenceCount( )
	{
		return refrenceCount;
	}

	// -----------------------------------------
	// TextureManager --------------------------
	// -----------------------------------------
	std::unordered_map<std::string, TextureResource*> TextureManager::textureRegistry;

	bool TextureManager::AddTexture( std::string name, std::string path )
	{
		TextureResource* textureResource = new TextureResource( path );

		textureRegistry.insert( { name, textureResource } );

		CW_ENGINE_LOG->Info( "Adding texture from %s as %s", path.c_str( ), name.c_str( ) );

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
		for ( auto keyVal : textureRegistry )
		{
			TextureResource* resource = keyVal.second;

			if ( resource->GetRefrenceCount( ) == 0 )
			{
				resource->Unload( );
			}

			resource->ResetRefrenceCount( );
		}
	}

	void TextureManager::RemoveEverything( )
	{
		for ( auto keyValPair : textureRegistry )
		{
			delete keyValPair.second;
		}

		textureRegistry.clear( );
	}
}