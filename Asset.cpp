#include "Asset.hpp"

Asset::Asset(sf::Sprite image, std::vector<sf::CircleShape> nodeList)
: aSprite(image)
, nodeList(nodeList)
{
    for(int i=0; i < nodeList.size(); i++)
    {
        nodeRatio.push_back(aSprite.getPosition()-nodeList[i].getPosition());
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
