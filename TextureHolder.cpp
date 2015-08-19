#include "TextureHolder.h"

TextureHolder::TextureHolder()
{

}

void TextureHolder::loadFromFile(TexName type, std::string filename)
{
    sf::Texture tex;
    if ( !tex.loadFromFile(filename) )
        std::cout << "Failed to load player spritesheet!" << std::endl;
    tex.setSmooth(true);
    mTextureMap[type] = tex;

}

void TextureHolder::loadFromFile(std::string id, std::string filename)
{
    sf::Texture tex;
    if ( !tex.loadFromFile(filename) )
        std::cout << "Failed to load player spritesheet!" << std::endl;
    tex.setSmooth(true);
    TextureMap[id] = tex;

}

sf::Texture* TextureHolder::getTexture(TexName type)
{
    return &mTextureMap[type];
}

sf::Texture* TextureHolder::getTexture(std::string id)
{
    return &TextureMap[id];
}
