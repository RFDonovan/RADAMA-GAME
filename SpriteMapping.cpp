#include "SpriteMapping.hpp"

SpriteMapping::SpriteMapping()
{

}

void SpriteMapping::loadXML(std::string XMLFile)
{
    if (!XMLDocument.load_file(XMLFile.c_str()))
    {
        std::cout << "SpriteMapping::loadXML -> error on loading "<<XMLFile<< "\n";
    }
    pugi::xml_node textureNode = XMLDocument.child("SpriteMapping").child("Texture");
    pugi::xml_node spritesNode = XMLDocument.child("SpriteMapping").child("Sprites");
    pugi::xml_node animationsNode = XMLDocument.child("SpriteMapping").child("Animations");

    std::cout << "SpriteMapping::loadXML -> loading texture ...\n";
    std::stringstream ss;
    ss<<textureNode.attribute("Path").as_string();
    std::cout << "SpriteMapping::loadXML -> loading texture ..."<<ss.str()<<"\n";

    if ( !SpriteSheet.loadFromFile(ss.str()) )
        std::cout << "SpriteMapping::loadXML -> loading texture ERROR!\n";
    std::cout << "SpriteMapping::loadXML -> loading Sprites ...\n";
    for(pugi::xml_node node = spritesNode.first_child(); node; node = node.next_sibling())
        ///SPRITES
    {
        spriteList[node.attribute("Id").as_int()]= getSpriteInfo(node);
    }
    std::cout << "SpriteMapping::loadXML -> loading animations ...\n";
    for(pugi::xml_node node = animationsNode.first_child(); node; node = node.next_sibling())
        ///ANIMATIONS
    {
        createAnimation(node);
    }
    std::cout << "SpriteMapping::loadXML -> loading Finished\n";

}

SpriteInfo  SpriteMapping::getSpriteInfo(pugi::xml_node spriteNode)
{
    SpriteInfo s_info;
    std::stringstream ss;
    ss<<spriteNode.attribute("Name").as_string();
    s_info.name = ss.str();
    s_info.Id = spriteNode.attribute("Id").as_int();
    s_info.x = spriteNode.child("Coordinates").child("X").text().as_int();
    s_info.y = spriteNode.child("Coordinates").child("Y").text().as_int();
    s_info.w = spriteNode.child("Coordinates").child("Width").text().as_int();
    s_info.h = spriteNode.child("Coordinates").child("Height").text().as_int();


    return s_info;
}

void SpriteMapping::createAnimation(pugi::xml_node animationNode)
{

    std::stringstream ss;
    ss<<animationNode.attribute("Name").as_string();

    Animation anim;
    anim.setSpriteSheet(SpriteSheet);

    pugi::xml_node framesNode = animationNode.child("Frames");

    for(pugi::xml_node node = framesNode.first_child(); node; node = node.next_sibling())
    {
        SpriteInfo s_info = spriteList[node.attribute("SpriteId").as_int()];

        std::cout << "--SpriteMapping::getSpriteInfo -> s_info name:"<< s_info.name<<std::endl;
        std::cout << "--SpriteMapping::getSpriteInfo -> s_info i:"<< s_info.Id<<std::endl;
        std::cout << "--SpriteMapping::getSpriteInfo -> s_info x:"<< s_info.x<<std::endl;
        std::cout << "--SpriteMapping::getSpriteInfo -> s_info y:"<< s_info.y<<std::endl;
        std::cout << "--SpriteMapping::getSpriteInfo -> s_info w:"<< s_info.w<<std::endl;
        std::cout << "--SpriteMapping::getSpriteInfo -> s_info h:"<< s_info.h<<std::endl;

        anim.addFrame(sf::IntRect(s_info.x
                                  ,s_info.y
                                  ,s_info.w
                                  ,s_info.h
                                  )
                      );

    }

    animationList[ss.str()]=anim;

}

std::map<std::string, Animation>* SpriteMapping::getAnimationList()
{
    std::cout << "SpriteMapping::getAnimationList -> getting animationlist\n";
    return &animationList;
}
