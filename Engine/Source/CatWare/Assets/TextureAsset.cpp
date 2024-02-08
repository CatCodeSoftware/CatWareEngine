#include "TextureAsset.h"


#include "CatWare/Error.h"
#include "CatWare/Log.h"

namespace CatWare
{
	TextureAsset::~TextureAsset( )
	{
		Unload( );
	}

	void TextureAsset::Load( )
	{
		this->texture = Rendering::Texture2D::Create( path, filter );
		CW_ENGINE_LOG->Info( "Loading texture %s", path.c_str( ) );
	}

	void TextureAsset::Unload( )
	{
		delete texture;
		texture = nullptr;
		CW_ENGINE_LOG->Info( "Unloading texture %s", path.c_str( ) );
	}

	TextureRef::TextureRef( )
	{
		path = "";
	}

	TextureRef::TextureRef( std::string path )
	{
		this->path = path;
		TextureManager::AddRef( this );
	}

	TextureRef::~TextureRef( )
	{
		if ( path != "" )
			TextureManager::ReleaseRef( this );
	}

	Rendering::Texture2D* TextureRef::Get( )
	{
	 	return TextureManager::GetTexture( path );
	}


	void TextureManager::AddTexture( std::string path, Rendering::TextureFilter filter )
	{
		TextureAsset* asset = new TextureAsset;
		asset->path = path;
		asset->filter = filter;

		textures.insert( { path, asset } );
	}


	Rendering::Texture2D* TextureManager::GetTexture( std::string path )
	{
		if ( textures.find( path ) == textures.end( ) )
			AddTexture( path, Rendering::TextureFilter::LINEAR );

		TextureAsset* asset = textures.find( path )->second;

		if ( asset->texture == nullptr )
			asset->Load( );

		return asset->texture;
	}

	void TextureManager::AddRef( TextureRef* ref )
	{
		if ( textures.find( ref->GetPath( ) ) == textures.end( ) )
			CW_ABORT( "Attempted to refrence texture " + ref->GetPath( ) + " that does not exist" );

		TextureAsset* asset = textures[ref->GetPath( )];

		asset->refs++;

		if ( asset->texture == nullptr )
			asset->Load( );
	}

	void TextureManager::ReleaseRef( TextureRef* ref )
	{
		if ( textures.find( ref->GetPath( ) ) == textures.end( ) )
			CW_ABORT( "Attempted to release refrence to texture " + ref->GetPath( ) + " that does not exist" );

		TextureAsset* asset = textures[ref->GetPath( )];
		asset->refs--;

		if ( asset->refs <= 0 )
		{
			asset->refs = 0;
		}
	}

	void TextureManager::DoCleanup( )
	{
		for ( auto& [ key, value ] : textures )
		{
			// CW_ENGINE_LOG->Info( "%d", value->refs );

			if ( value->texture != nullptr )
				if ( value->refs <= 0 )
					value->Unload( );
		}
	}
} // namespace CatWare
