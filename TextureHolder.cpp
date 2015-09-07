#include "TextureHolder.h"

std::map<std::string, sf::Texture> TextureHolder::TextureMap;

TextureHolder::TextureHolder()
{

}

bool TextureHolder::exist(std::string filename)
{
    for (std::map<std::string, sf::Texture>::iterator it=TextureMap.begin(); it!=TextureMap.end(); ++it)
    {
        if(filename.compare(it->first) == 0)
            return true;
    }
    return false;
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
    if(exist(filename))
        return;
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
