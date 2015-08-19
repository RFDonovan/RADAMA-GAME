#ifndef TEXTUREHOLDER_H_INCLUDED
#define TEXTUREHOLDER_H_INCLUDED
#include<SFML/Graphics.hpp>
#include<iostream>
#include<map>
#include<string>
class TextureHolder
{
public:
    enum TexName
    {
        Player,
        Ground,
        Ground1,
        Ground2,
        Background1,
        Pause,

        Fire,
        Lefona,

        Other,
    };
     std::map<TexName, sf::Texture>  mTextureMap;
     std::map<std::string, sf::Texture> TextureMap;
public:
    TextureHolder();
     void            loadFromFile(TexName type, std::string filename);
     void            loadFromFile(std::string id, std::string filename);
     sf::Texture*     getTexture(TexName type);
     sf::Texture*     getTexture(std::string id);


};


#endif // TEXTUREHOLDER_H_INCLUDED
