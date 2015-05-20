#ifndef TEXTUREHOLDER_H_INCLUDED
#define TEXTUREHOLDER_H_INCLUDED
#include<map>
#include<string>
class TextureHolder
{
public:
    TextureHolder();
private:
    std::map<std::string, std::string>  mTextureMap;
};


#endif // TEXTUREHOLDER_H_INCLUDED
