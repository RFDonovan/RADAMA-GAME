#include "LevelDesign.hpp"

LevelDesign::LevelDesign()
: mWindow(sf::VideoMode(WINDOW_W, WINDOW_H), "Level Designer", sf::Style::Close)
, mWorldView(mWindow.getDefaultView())
{


}


void LevelDesign::run()
{
    sf::Clock frameClock;
    //sf::Time timeSinceLastUpdate = sf::Time::Zero;
    while (mWindow.isOpen()){
        processInput();
        sf::Time frameTime = frameClock.restart();
        update();
        render(frameTime);
    }

}

void LevelDesign::processInput()
{
    sf::Event event;
    while (mWindow.pollEvent(event))
    {
        if(event.type == sf::Event::Closed)
            mWindow.close();

        basicInput(event);
        mouseInput(event);
    }

}

void LevelDesign::basicInput(sf::Event e)
{
    float unit = 20.f;
    if (sf::Keyboard::isKeyPressed(K_RIGHT))
    {
        mWorldView.move(sf::Vector2f(unit,0.f));
    }

    if (sf::Keyboard::isKeyPressed(K_LEFT))
    {
        mWorldView.move(sf::Vector2f(-unit,0));
    }

    if (sf::Keyboard::isKeyPressed(K_UP))
    {
        mWorldView.move(sf::Vector2f(0.f,-unit));
    }

    if (sf::Keyboard::isKeyPressed(K_DOWN))
    {
        mWorldView.move(sf::Vector2f(0.f,unit));
    }
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Add))
    {
        mWorldView.zoom(0.7f);
    }
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Subtract))
    {
        mWorldView.zoom(1.3f);
    }
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::O))
    {
        const char *filters[] = {"*.jpg","*.png"};

        //char const * lThePassword = tinyfd_inputBox( "a password box","your password will be revealed",NULL);
        const char *fn = tinyfd_openFileDialog("Open Image","",2,filters,"image files", 0);
        if(fn)
            imageList.push_back(loadImage(fn));
        std::cout <<"Loading of "<<fn << std::endl;
    }
    ///DELETING IMAGE OR NODE
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Delete))
    {
        if(vertexMode)
        {
            if(tmpNode != nullptr)
            {
                deleteNode(tmpNode);
                tmpNode = nullptr;
            }
        }
        else
        {
            if(tmpSprite != nullptr)
            {
                deleteImage(tmpSprite);
                tmpSprite = nullptr;
            }
        }
    }

    switch(e.type)
    {
        case sf::Event::KeyReleased:
            if(e.key.code == sf::Keyboard::V)///VERTEX EDIT
            {
                vertexMode = !vertexMode;
            }
        break;

    }
}

void LevelDesign::mouseInput(sf::Event e)
{

    switch(e.type)
    {
    case sf::Event::MouseMoved:
        {
            if(isMoving)
            {
                if(vertexMode)
                {
                    if(tmpNode != nullptr)
                    {
                        tmpNode->setPosition(getMousePos());
                    }
                }
                else
                    if(tmpSprite != nullptr)
                        tmpSprite->setPosition(getMousePos());
            }
        }
    break;
    case sf::Event::MouseButtonPressed:
        {
            if(e.mouseButton.button == sf::Mouse::Right)
            {
                if(vertexMode)
                {
                    for(int i=0 ; i < vertexList.size(); i++)
                    {
                        if(vertexList[i].getGlobalBounds().contains(getMousePos()))
                        {
                            isMoving = true;
                            tmpNode = &(vertexList[i]);
                            tmpNode->setFillColor(sf::Color(255,10,10));
                            break;
                        }
                    }
                }
                else
                {
                   for(int i=0 ; i < imageList.size(); i++)
                    {
    //                    sf::Sprite.getGlobalBounds().contains(getMousePos())
                        if(imageList[i].getGlobalBounds().contains(getMousePos()))
                        {
                            isMoving = true;
                            //imageList.erase(imageList.begin()+i);
                            tmpSprite = &(imageList[i]);
                            break;
                        }

                    }
                }

            }
        }
    break;
    case sf::Event::MouseButtonReleased:
        {
            if(tmpNode!=nullptr)
            {
                tmpNode->setFillColor(sf::Color(100, 250, 50));
            }
            tmpNode = nullptr;
            tmpSprite = nullptr;
            if(e.mouseButton.button == sf::Mouse::Right)
                isMoving = false;
            if(e.mouseButton.button == sf::Mouse::Left)
            {
                vertexList.push_back(createVertex(getMousePos()));
            }

        }
    break;
    }
}

sf::Vector2f LevelDesign::getMousePos()
{

    return mWindow.mapPixelToCoords(sf::Mouse::getPosition(mWindow), mWindow.getView());
}

void LevelDesign::update()
{

}

sf::Sprite LevelDesign::loadImage(std::string fn)
{
    textureHolder.loadFromFile(fn, fn);
    sf::Sprite s;
    s.setTexture(*(textureHolder.getTexture(fn)));
    s.setOrigin(textureHolder.getTexture(fn)->getSize().x/2,textureHolder.getTexture(fn)->getSize().y/2);
    s.setPosition(textureHolder.getTexture(fn)->getSize().x/2,textureHolder.getTexture(fn)->getSize().y/2);
    s.setColor(sf::Color(255,255,255,150));
    return s;
}

sf::CircleShape LevelDesign::createVertex(sf::Vector2f pos)
{
    sf::CircleShape shape(5);
    shape.setFillColor(sf::Color(100, 250, 50));
    shape.setOrigin(shape.getRadius(), shape.getRadius());
    shape.setPosition(pos);
    return shape;
}


//***DELETING THINGS***//
void LevelDesign::deleteNode(sf::CircleShape* node)
{
    for(int i=0 ; i < vertexList.size(); i++)
    {
        if(node == &vertexList[i])
            vertexList.erase(vertexList.begin()+i);
    }
}

void LevelDesign::deleteImage(sf::Sprite* image)
{
    for(int i=0 ; i < imageList.size(); i++)
    {
        if(image == &imageList[i])
            imageList.erase(imageList.begin()+i);
    }
}
//***END DELETING THINGS***//

void LevelDesign::render(sf::Time frameTime)
{
    mWindow.setView(mWorldView);

    if(vertexMode)
        mWindow.clear(sf::Color(50,50,50,50));
    else
        mWindow.clear(sf::Color::White);

    if(showImages)
        renderImages(frameTime);
    if(showVertex)
        renderVertex(frameTime);

    mWindow.display();
}

void LevelDesign::renderImages(sf::Time frameTime)
{
    for(int i=0 ; i < imageList.size(); i++)
    {
        mWindow.draw(imageList[i]);
    }
}

void LevelDesign::renderVertex(sf::Time frameTime)
{
    for(int i=0 ; i < vertexList.size(); i++)
    {
        if((i+1) < vertexList.size())
        {
            sf::Vertex line[] =
                {
                    sf::Vertex(vertexList[i].getPosition()),
                    sf::Vertex(vertexList[i+1].getPosition())
                };

            mWindow.draw(line, 2, sf::Lines);
        }
        else
        {
            sf::Vertex line[] =
                {
                    sf::Vertex(vertexList[i].getPosition()),
                    sf::Vertex(vertexList[0].getPosition())
                };

            mWindow.draw(line, 2, sf::Lines);
        }
        mWindow.draw(vertexList[i]);
    }
}

bool LevelDesign::mouseIsOnTheSprite(sf::Sprite* sp, sf::Vector2f mousePos)
{
    if(mousePos.x > sp->getPosition().x - sp->getOrigin().x
       &&
       mousePos.x < (sp->getPosition().x - sp->getOrigin().x + sp->getLocalBounds().width))

       {
           if(mousePos.y > sp->getPosition().y - sp->getOrigin().y
               &&
               mousePos.y < (sp->getPosition().y - sp->getOrigin().y + sp->getLocalBounds().height))
              return true;
       }
    return false;
}

