#include "Asset.hpp"

Asset::Asset(sf::Sprite image, std::vector<sf::CircleShape> nodeList,std::string filename)
: aSprite(image)
, nodeList(nodeList)
, path(filename)
{
    for(int i=0; i < nodeList.size(); i++)
    {
        nodeRatio.push_back(aSprite.getPosition()-nodeList[i].getPosition());
    }
    const char *filters[] = {"*.xml"};
    const char *fn = tinyfd_saveFileDialog("Save as", "", 1, filters,"XML File");
    std::stringstream ss;
    ss<<fn;

    exportToXML(ss.str());

    std::stringstream ssName;
    ssName<<PathFindFileName(filename.c_str());

    name = ssName.str();
}

Asset::Asset(std::string filename)
{
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


    aSprite.setTexture(*textureHolder.getTexture("Aname"));
    aSprite.setOrigin(textureHolder.getTexture("Aname")->getSize().x/2, textureHolder.getTexture("Aname")->getSize().y/2);
    aSprite.setScale(imageN.attribute("scaleX").as_float(),imageN.attribute("scaleY").as_float());
    aSprite.setPosition(imageN.attribute("x").as_float(),imageN.attribute("y").as_float());

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
    if(pinned)
    {
        aSprite.setColor(sf::Color(55,55,55,150));
    }
    else
    {
        aSprite.setColor(sf::Color(255,255,255,150));
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


