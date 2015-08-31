#include "LevelDesign.hpp"

LevelDesign::LevelDesign()
: mWindow(sf::VideoMode(WINDOW_W, WINDOW_H), "Level Designer", sf::Style::Close)
, mWorldView(mWindow.getDefaultView())
, gui(mWindow)
{
    createGUI();

    if (!MyFont.loadFromFile("Resources/CHIZZ___.ttf"))
        {
            // Error...
        }
    t.setFont(MyFont);
    t.setColor(sf::Color(0,0,255,100));


}

void LevelDesign::createGUI()
{
    gui.setGlobalFont("fonts/DejaVuSans.ttf");
    tgui::Button::Ptr button(gui);

    tgui::Label::Ptr labelUsername(gui);
    labelUsername->setText("Username:");
    labelUsername->setPosition(200, 100);
    labelUsername->setTextColor(sf::Color::Red);




    button->load("widgets/Black.conf");
    button->setSize(260, 60);
    button->setPosition(270, 440);
    button->setText("EJDKJSF");
    button->bindCallback(tgui::Button::LeftMouseClicked);
    button->setCallbackId(1);
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
        /*TGUI RENDER*/
        gui.draw();
        /*************/
        mWindow.display();
    }

}

void LevelDesign::tguiEventHandler()
{
    tgui::Callback callback;
    while (gui.pollCallback(callback))
    {
        // Make sure tha callback comes from the button
        if (callback.id == 1)
        {
            exit(-1);

        }
    }
}

void LevelDesign::processInput()
{
    sf::Event event;
    tguiEventHandler();

    while (mWindow.pollEvent(event))
    {
        if(event.type == sf::Event::Closed)
            mWindow.close();

        if(!gui.handleEvent(event))
            //SEULEMENT SI LES TGUI NE SONT PAS CONCERNE
        {
            basicInput(event);
            mouseInput(event);
        }

    }


}

void LevelDesign::basicInput(sf::Event e)
{
    ///VIEW CONTROL: UP, DOWN, LEFT, RIGHT, ZOOM
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
        if (!sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) ||
            !sf::Keyboard::isKeyPressed(sf::Keyboard::RControl))
            mWorldView.move(sf::Vector2f(0.f,unit));
    }
    ///RESET VIEW
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::BackSpace))
    {
        mWorldView.setCenter(WINDOW_W/2,WINDOW_H/2);
    }
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Add))
    {
        if(isMoving)
        {
            if(tmpSprite != nullptr)
            {
                tmpSprite->setScale(tmpSprite->getScale()+sf::Vector2f(.05f,.05f));
            }
        }
        else
            mWorldView.zoom(0.7f);
    }
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Subtract))
    {
        if(isMoving)
        {
            if(tmpSprite != nullptr)
            {
                tmpSprite->setScale(tmpSprite->getScale()-sf::Vector2f(.05f,.05f));
            }
        }
        else
            mWorldView.zoom(1.3f);
    }



    ///CREATE AN ASSET FROM CURRENT IMAGE AND NODE
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Return))
    {
        if(showAssets)
        {

        }
        else
            if(imageList.size()>0)
                createAsset();
    }
    ///OPEN IMAGE FILE
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::O))
    {
        if(showAssets)
        {
            const char *filters[] = {"*.asset","*.xml"};
            const char *fn = tinyfd_openFileDialog("Open Assets","",2,filters,"Asset files", 1);

            if(fn)
            {
                std::vector<std::string> pathList;
                pathList = splitPerso(fn, "|");

                for(int i=0; i<pathList.size(); i++)
                {
                    std::cout <<"Loading of "<<pathList[i] << std::endl;
                    assetList.push_back(Asset(pathList[i]));
                }
            }

        }
        else
        {
            const char *filters[] = {"*.jpg","*.png"};
            const char *fn = tinyfd_openFileDialog("Open Image","",2,filters,"image files", 0);
            if(fn)
            {
                imageList.push_back(loadImage(fn));
                filenameList.push_back(fn);
            }

            std::cout <<"Loading of "<<fn << std::endl;
        }

    }
    ///DELETING IMAGE OR NODE
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Delete))
    {
        if(showAssets)
        {
            if(tmpAsset != nullptr)
            {
                deleteAsset(tmpAsset);
                tmpAsset = nullptr;
            }
        }
        else
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
            if(e.key.code == sf::Keyboard::A)///VERTEX EDIT
            {
                showAssets = !showAssets;
            }
            ///PINNING OR UNPINNING ASSET
            if(e.key.code == sf::Keyboard::P)
            {
                if(tmpAsset!=nullptr)
                {
                    tmpAsset->pinned = !(tmpAsset->pinned);
//                    if(tmpAsset->pinned)
//                    {
//                        downZIndex(tmpAsset);
//                        tmpAsset = nullptr;
//                    }

                }
            }
            ///DUPLICATE ASSET
            if(e.key.code == sf::Keyboard::Return)
            {
                if(showAssets)
                {
                    if(tmpAsset != nullptr)
                    {
        //                Asset a = *tmpAsset;
                        assetList.push_back(*tmpAsset);
                    }
                }
            }
            ///SAVE LEVEL
            if(e.key.code == sf::Keyboard::Slash)
            {
                if(showAssets)
                {
                    saveLevel();
                }

            }
            ///SAVE ASSETS
            if(e.key.code == sf::Keyboard::S)
            {
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)|| sf::Keyboard::isKeyPressed(sf::Keyboard::RControl))
                {
                    if(showAssets)
                        saveAssets();
                }
            }


        break;


    }
}

void LevelDesign::createAsset()
{
    assetList.push_back(Asset(imageList[0],vertexList,filenameList[0]));
    vertexList.clear();
    imageList.clear();
    filenameList.clear();
    tmpNode = nullptr;
    tmpSprite = nullptr;
}

void LevelDesign::mouseInput(sf::Event e)
{

    switch(e.type)
    {
    case sf::Event::MouseMoved:
        {
            if(isMoving)
            {
                ///MANIPULATION DES ASSETS
                if(showAssets)
                {
                    if(tmpAsset != nullptr)//ajouter if pinned
                        if(!tmpAsset->isPinned())
                            tmpAsset->setPosition(getMousePos());
                }
                else ///--------AUTRES
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
        }
    break;
    case sf::Event::MouseButtonPressed:
        {
            if(e.mouseButton.button == sf::Mouse::Right)
            {
                ///MANIPULATION DES ASSETS
                if(showAssets)
                {

                    for (int i=0; i < assetList.size(); i++)
                    {
                        if(assetList[i].getGlobalBounds().contains(getMousePos()))
                        {
                            isMoving = true;
                            tmpAsset = &(assetList[i]);
                            break;
                        }
                    }
                }
                else///--------AUTRES
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
        }
    break;
    case sf::Event::MouseButtonReleased:
        {
            ///MANIPULATION DES ASSETS
            if(showAssets)
            {
                if(e.mouseButton.button == sf::Mouse::Right)
                {
                    tmpAsset = nullptr;
                    isMoving = false;
                }


            }
            else///--------AUTRES
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


        }
    break;
    case sf::Event::MouseWheelMoved:
        {
            if(e.mouseWheel.delta>0)
                mWorldView.zoom(0.7f);
            if(e.mouseWheel.delta<0)
                mWorldView.zoom(1.3f);

        }
        std::cout << e.mouseWheel.delta << '\n';
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

//    s.setPosition(textureHolder.getTexture(fn)->getSize().x/2,textureHolder.getTexture(fn)->getSize().y/2);
    s.setPosition(mWorldView.getCenter().x,mWorldView.getCenter().y);
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

void LevelDesign::deleteAsset(Asset* asset)
{
    for(int i=0 ; i < assetList.size(); i++)
    {
        if(asset == &assetList[i])
            assetList.erase(assetList.begin()+i);
    }
}

void LevelDesign::downZIndex(Asset* asset)
{
    for(int i=0 ; i < assetList.size(); i++)
    {
        if(asset == &assetList[i])
        {
//            assetList.insert(assetList.begin()+0, *asset);
            assetList.push_back(Asset(asset->aSprite, asset->nodeList, asset->path));
            assetList.erase(assetList.begin()+i);
        }

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

    if(showAssets)
    {
        mWindow.clear(sf::Color::White);
        mWindow.clear(sf::Color(10,100,10,50));
        renderAssets(frameTime);
    }
    else
    {
        if(showImages)
            renderImages(frameTime);
        if(showVertex)
            renderVertex(frameTime);
    }

    /**AXIS LINE*/
    sf::RectangleShape line(sf::Vector2f(5000, 2));
    line.setFillColor(sf::Color(255,0,0,200));
    line.setPosition(sf::Vector2f(.0f,.0f));
    line.setRotation(0);
    mWindow.draw(line);
    line.setRotation(90);
    mWindow.draw(line);
    for(int i=0; i < 50; i++)
    {
        std::stringstream ss;
        ss<<"-"<<i<<"00-";

        t.setCharacterSize(20);
        t.setString(ss.str());
        t.setPosition(i*100, 10.f);

        mWindow.draw(t);
    }
    /************/



//    mWindow.display();
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


void LevelDesign::renderAssets(sf::Time frameTime)
{
    for(int i=0; i < assetList.size(); i++)
    {
        assetList[i].render(mWindow);
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

//*****************
//*SAVE LEVEL
//*****************

void LevelDesign::saveLevel()
{

    const char *filters[] = {"*.xml"};
    const char *fn = tinyfd_saveFileDialog("Save as", "", 1, filters,"XML File");
    std::stringstream ss;
    ss<<fn;


    pugi::xml_document doc;

    pugi::xml_node box2d = doc.append_child("box2d");
    pugi::xml_node bodies = box2d.append_child("bodies");
    pugi::xml_node images = box2d.append_child("images");


    ///CREATE BODIES
    for(int i=0; i< assetList.size(); i++)
    {
        //*********body**********
        pugi::xml_node bodyN = bodies.append_child("body");
        bodyN.append_attribute("name") = i;
        bodyN.append_attribute("type") = "static";
        bodyN.append_attribute("x") = assetList[i].getPosition().x;
        bodyN.append_attribute("y") = -assetList[i].getPosition().y;
        bodyN.append_attribute("image") = assetList[i].name.c_str();
        //*********end body**********

        //*********fixtures**********
        pugi::xml_node fixturesN = bodyN.append_child("fixtures");
        pugi::xml_node fixtureN = fixturesN.append_child("fixture");
        fixtureN.append_attribute("name") = i;
        fixtureN.append_attribute("restitution") = "0";
        fixtureN.append_attribute("friction") = "0";
        fixtureN.append_attribute("isSensor") = "false";
        fixtureN.append_attribute("shapeType") = "polygonShape";
        fixtureN.append_attribute("categoryBits") = "0x0003";
        fixtureN.append_attribute("maskBits") = "0x0002";
        for(int j = 0; j < assetList[i].nodeRatio.size(); j++)
        {
            pugi::xml_node vertexN = fixtureN.append_child("vertex");
            vertexN.append_attribute("x") = -assetList[i].nodeRatio[j].x;
            vertexN.append_attribute("y") = -assetList[i].nodeRatio[j].y;
        }
        //*********end fixtures**********

        //*********Images**********
        pugi::xml_node imageN = images.append_child("image");
        imageN.append_attribute("name") = assetList[i].name.c_str();
        imageN.append_attribute("path") = assetList[i].name.c_str();
        imageN.append_attribute("x") = assetList[i].getPosition().x;
        imageN.append_attribute("y") = -assetList[i].getPosition().y;
        imageN.append_attribute("scaleX") = assetList[i].aSprite.getScale().x;
        imageN.append_attribute("scaleY") = assetList[i].aSprite.getScale().y;
        //*********end Images**********

    }

    doc.save_file(ss.str().c_str());
}


void LevelDesign::saveAssets()
{
    const char *filters[] = {"*.asset"};
    const char *fn = tinyfd_saveFileDialog("Save all assets as", "", 1, filters,"Asset Files");
    for(int i=0; i< assetList.size(); i++)
    {
        std::stringstream ss;
        ss<<fn<<i<<".asset";
        assetList[i].exportToXML(ss.str());
    }
}


