#include "Assets.h"

namespace CatWare
{
    AssetManager<TextureAsset, Rendering::Texture2D> Assets::textures;
    AssetManager<SoundAsset, Sound> Assets::sounds;
}