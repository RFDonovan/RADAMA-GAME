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
        Other,
    };
     std::map<TexName, sf::Texture>  mTextureMap;
public:
    TextureHolder();
     void            loadFromFile(TexName type, std::string filename);
     sf::Texture*     getTexture(TexName type);


};


#endif // TEXTUREHOLDER_H_INCLUDED
