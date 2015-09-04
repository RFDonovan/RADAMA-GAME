#include "Asset.hpp"
int Asset::AssetID = 0;
Asset::Asset(sf::Sprite image, std::vector<sf::CircleShape> nodeList,std::string filename)
: aSprite(image)
, nodeList(nodeList)
, path(filename)
{
    _id = AssetID++;
    std::cout << "Asset:: nodelist....\n";
    for(int i=0; i < nodeList.size(); i++)
    {
        nodeRatio.push_back(aSprite.getPosition()-nodeList[i].getPosition());
    }
//    const char *filters[] = {"*.xml"};
//    const char *fn = tinyfd_saveFileDialog("Save as", "", 1, filters,"XML File");
//    std::stringstream ss;
//    ss<<fn;
//
//    exportToXML(ss.str());

    std::stringstream ssName;
    ssName<<PathFindFileName(filename.c_str());

    name = ssName.str();
    std::cout << "Asset:: n "<<_id<<" done\n";
    textureHolder.add("Aname", *image.getTexture());
}

Asset::Asset(std::string filename)
{
    _id = AssetID++;
    pugi::xml_document          XMLDocument;
    if (!XMLDocument.load_file(filename.c_str()))
    {
        std::cout << "error on loading Asset: "<<filename<< "\n";
//        return;
    }
///LOAD IMAGE

    pugi::xml_node imageN = XMLDocument.child("Asset").child("image");
    textureHolder.loadFromFile("Aname",imageN.attribute("path").as_string());

    std::stringstream ssName;
    ssName<<PathFindFileName(imageN.attribute("path").as_string());

    name = ssName.str();
    path = imageN.attribute("path").as_string();


    aSprite.setTexture(*textureHolder.getTexture("Aname"));
    aSprite.setOrigin(textureHolder.getTexture("Aname")->getSize().x/2, textureHolder.getTexture("Aname")->getSize().y/2);
    aSprite.setScale(imageN.attribute("scaleX").as_float(),imageN.attribute("scaleY").as_float());
    aSprite.setPosition(imageN.attribute("x").as_float(),imageN.attribute("y").as_float());
    zIndex = imageN.attribute("z").as_int();

///LOAD NODES
    pugi::xml_node nodesN = XMLDocument.child("Asset").child("nodes");
    for (pugi::xml_node node = nodesN.first_child(); node ; node = node.next_sibling())
    {
        sf::CircleShape cShape(5);
        cShape.setFillColor(sf::Color(100, 250, 50));
        cShape.setOrigin(cShape.getRadius(), cShape.getRadius());
        cShape.setPosition(
                           node.attribute("x").as_int(),
                           node.attribute("y").as_int()
                           );
        std::cout<<"x:"<<node.attribute("x").as_int()<<std::endl;
        nodeList.push_back(cShape);
        nodeRatio.push_back(aSprite.getPosition()-cShape.getPosition());
    }


}

Asset::Asset(pugi::xml_node parent)
{

///LOAD IMAGE
    _id = parent.attribute("id").as_int();

    pugi::xml_node imageN = parent.child("image");
    textureHolder.loadFromFile("Aname",imageN.attribute("path").as_string());

    std::stringstream ssName;
    ssName<<PathFindFileName(imageN.attribute("path").as_string());

    name = ssName.str();
    path = imageN.attribute("path").as_string();


    aSprite.setTexture(*textureHolder.getTexture("Aname"));
    aSprite.setOrigin(textureHolder.getTexture("Aname")->getSize().x/2, textureHolder.getTexture("Aname")->getSize().y/2);
    aSprite.setScale(imageN.attribute("scaleX").as_float(),imageN.attribute("scaleY").as_float());
    aSprite.setPosition(imageN.attribute("x").as_float(),imageN.attribute("y").as_float());
    zIndex = imageN.attribute("z").as_int();
    if(strcmp(imageN.attribute("isActive").value(), "") != 0)
        isActive = imageN.attribute("isActive").as_bool();
    else
        isActive = true;

///LOAD NODES
    pugi::xml_node nodesN = parent.child("nodes");
    for (pugi::xml_node node = nodesN.first_child(); node ; node = node.next_sibling())
    {
        sf::CircleShape cShape(5);
        cShape.setFillColor(sf::Color(100, 250, 50));
        cShape.setOrigin(cShape.getRadius(), cShape.getRadius());
        cShape.setPosition(
                           node.attribute("x").as_int(),
                           node.attribute("y").as_int()
                           );
        std::cout<<"x:"<<node.attribute("x").as_int()<<std::endl;
        nodeList.push_back(cShape);
        nodeRatio.push_back(aSprite.getPosition()-cShape.getPosition());
    }


}



void  Asset::setPosition(sf::Vector2f pos)
{
    aSprite.setPosition(pos);

    for(int i=0; i < nodeList.size(); i++)
    {
        nodeList[i].setPosition(aSprite.getPosition()-nodeRatio[i]);
    }
}

void  Asset::render(sf::RenderWindow& mWindow)
{
    if(deleted)
        return;
    aSprite.setTexture(*textureHolder.getTexture("Aname"));
    if(pinned)
    {
        aSprite.setColor(sf::Color(55,55,55,150));
    }
    else
    {
        aSprite.setColor(sf::Color(255,255,255,250));
    }
    mWindow.draw(aSprite);
    for(int i=0; i < nodeList.size(); i++)
    {
        if((i+1) < nodeList.size())
        {
            sf::Vertex line[] =
                {
                    sf::Vertex(nodeList[i].getPosition()),
                    sf::Vertex(nodeList[i+1].getPosition())
                };

            mWindow.draw(line, 2, sf::Lines);
        }
        else
        {
            sf::Vertex line[] =
                {
                    sf::Vertex(nodeList[i].getPosition(),sf::Color::Blue),
                    sf::Vertex(nodeList[0].getPosition(),sf::Color::Blue)
                };


            mWindow.draw(line, 2, sf::Lines);
        }
        if(selected)
        {
            nodeList[i].setFillColor(sf::Color::Red);
        }
        else
        {
            nodeList[i].setFillColor(sf::Color(100, 250, 50));
        }
        mWindow.draw(nodeList[i]);
    }
}

void  Asset::exportToXML(std::string filename)
{
    pugi::xml_document doc;
    pugi::xml_node assetN = doc.append_child("Asset");
    pugi::xml_node nodesN = assetN.append_child("nodes");
    pugi::xml_node imageN = assetN.append_child("image");

////IMAGE

    imageN.append_attribute("x") = aSprite.getPosition().x;
    imageN.append_attribute("y") = aSprite.getPosition().y;
    imageN.append_attribute("z") = zIndex;

    imageN.append_attribute("scaleX") = aSprite.getScale().x;
    imageN.append_attribute("scaleY") = aSprite.getScale().y;
    imageN.append_attribute("path") = path.c_str();
////NODES
    for(int i=0; i < nodeList.size(); i++)
    {
        pugi::xml_node nodeN = nodesN.append_child("node");
        nodeN.append_attribute("x") = nodeList[i].getPosition().x;
        nodeN.append_attribute("y") = nodeList[i].getPosition().y;
//        nodeN.append_attribute("x") = nodeRatio[i].x;
//        nodeN.append_attribute("y") = nodeRatio[i].y;
    }

    doc.save_file(filename.c_str());

}

void  Asset::addAssetNode(pugi::xml_node parent)
{
    pugi::xml_node assetN = parent.append_child("Asset");

    std::stringstream ss;
    ss<<""<<_id;
    assetN.append_attribute("id") = ss.str().c_str();
    assetN.append_attribute("isActive") = isActive;

    pugi::xml_node nodesN = assetN.append_child("nodes");
    pugi::xml_node imageN = assetN.append_child("image");

////IMAGE

    imageN.append_attribute("x") = aSprite.getPosition().x;
    imageN.append_attribute("y") = aSprite.getPosition().y;
    imageN.append_attribute("z") = zIndex;

    imageN.append_attribute("scaleX") = aSprite.getScale().x;
    imageN.append_attribute("scaleY") = aSprite.getScale().y;
    imageN.append_attribute("path") = path.c_str();
////NODES
    for(int i=0; i < nodeList.size(); i++)
    {
        pugi::xml_node nodeN = nodesN.append_child("node");
        nodeN.append_attribute("x") = nodeList[i].getPosition().x;
        nodeN.append_attribute("y") = nodeList[i].getPosition().y;
//        nodeN.append_attribute("x") = nodeRatio[i].x;
//        nodeN.append_attribute("y") = nodeRatio[i].y;
    }

}

void  Asset::addDupliNode(pugi::xml_node parent)
{
    pugi::xml_node assetN = parent.append_child("Copy");
    std::stringstream ss;
    ss<<""<<_id;
    assetN.append_attribute("from") = ss.str().c_str();

    assetN.append_attribute("x") = aSprite.getPosition().x;
    assetN.append_attribute("y") = aSprite.getPosition().y;
    assetN.append_attribute("z") = zIndex;
}

void  Asset::shrink()
{
    for(int i=0; i < nodeRatio.size(); i++)
    {
//        if(nodeRatio[i].x>aSprite.getPosition().x)
//            nodeRatio[i].x = nodeRatio[i].x -1.f;
//        if(nodeRatio[i].x<aSprite.getPosition().x)
//            nodeRatio[i].x = nodeRatio[i].x +1.f;
//        if(nodeRatio[i].y>aSprite.getPosition().y)
//            nodeRatio[i].y = nodeRatio[i].y -1.f;
//        if(nodeRatio[i].y<aSprite.getPosition().y)
//            nodeRatio[i].y = nodeRatio[i].y +1.f;

        nodeRatio[i] = nodeRatio[i] - sf::Vector2f(1.f,1.f);
        if(aSprite.getPosition().x>nodeList[i].getPosition().x)
            nodeList[i].setPosition(nodeList[i].getPosition() - sf::Vector2f(1.f,0.f));
        if(aSprite.getPosition().x<nodeList[i].getPosition().x)
            nodeList[i].setPosition(nodeList[i].getPosition() + sf::Vector2f(1.f,0.f));

        if(aSprite.getPosition().y>nodeList[i].getPosition().y)
            nodeList[i].setPosition(nodeList[i].getPosition() - sf::Vector2f(0.f,1.f));
        if(aSprite.getPosition().y<nodeList[i].getPosition().y)
            nodeList[i].setPosition(nodeList[i].getPosition() + sf::Vector2f(0.f,1.f));

        nodeRatio[i] = aSprite.getPosition()-nodeList[i].getPosition();
    }
}

void  Asset::expand()
{

}

Asset::~Asset()
{
    std::cout<<"Destruct asset"<<std::endl;
//    deleted = true;
}
