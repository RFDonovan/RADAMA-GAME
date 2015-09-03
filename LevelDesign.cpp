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


    tgui::Label::Ptr labelMousePos(gui, "MousePos");
    labelMousePos->setText("[;]");
    labelMousePos->setPosition(WINDOW_W - 120, listMenu2.size()*50+100);
    labelMousePos->setTextColor(sf::Color::Red);
    labelMousePos->setTextSize(12);


    for (int i=0; i< listMenu2.size(); i++)
    {
        tgui::Button::Ptr btn(gui,listMenu2[i]+"LM2");
        btn->load("widgets/Black.conf");
        btn->setTextSize(12);
        btn->setSize(85, 30);
//        btn->setPosition(100* i + 100, WINDOW_H - 50);
        btn->setPosition(WINDOW_W - 120, 50* i + 100);
        btn->setText(listMenu2[i]);
        btn->bindCallback(tgui::Button::LeftMouseClicked);
        btn->setCallbackId(200+i);
        btn->hide();
    }

    for (int i=0; i< listMenu1.size(); i++)
    {
        tgui::Button::Ptr btn(gui,listMenu1[i]+"LM1");
        btn->load("widgets/Black.conf");
        btn->setTextSize(12);
        btn->setSize(85, 30);
//        btn->setPosition(100* i + 100, WINDOW_H - 50);
        btn->setPosition(WINDOW_W - 120, 50* i + 100);
        btn->setText(listMenu1[i]);
        btn->bindCallback(tgui::Button::LeftMouseClicked);
        btn->setCallbackId(100+i);

    }

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
//            exit(-1);
//            gui.get("Clear")->hide();

        }
        switch(callback.id)
        {
        case LOAD_IMAGE://LOAD image
            {
//                saveAsset(Asset(imageList[0],vertexList,filenameList[0]));

                loadFiles();
            }
        break;

//        case 100://SAVE image
//            {
////                saveAsset(Asset(imageList[0],vertexList,filenameList[0]));
//            }
//        break;

        case 101://COMMIT: create asset from this
            {
                createAsset();
                toggleAssetMode();
            }
        break;
        case 102://CLEAR ALL NODES AND ASSETS
            {
//                mWorldView.setCenter(WINDOW_W/2,WINDOW_H/2);
                vertexList.clear();
                imageList.clear();
                filenameList.clear();
                tmpNode = nullptr;
                tmpSprite = nullptr;
            }
        break;
        case 103://SWITCH VERTEX MODE
            {
                vertexMode = !vertexMode;
            }
        break;
        case 104://LEVEL EDITOR
            {
                toggleAssetMode();
            }
        break;
        case 200://IMPORT assets
            {
                loadFiles(
                          tinyfd_messageBox("PROJECT or ASSET?", "YES: Open a project file \nNO: Open an Asset","yesno","warning",0)
                          );
            }
        break;
        case 201://EXPORT as level
            {
                if(showAssets)
                {
                    saveLevel();
                }
            }
        break;
        case 202://SAVE
            {
                if(showAssets)
                {

                    if(tinyfd_messageBox("PROJECT or ASSET?", "YES: Save as project file \nNO: Save Assets","yesno","warning",0))
                    {
                        saveProject();
                    }
                    else
                    {
                        saveAssets();
                    }

                }

            }
        break;
        case 203://RESET ALL
            {
                if(tinyfd_messageBox("CLEAR ALL", "T'es SUR?!","yesno","warning",0))
                {
                    assetList.clear();
                    _assetList.clear();
                    assetIDList.clear();
                }

            }
        break;
        case 204://ASSET CREATOR
            {
                toggleAssetMode();

            }
        break;

        }
    }
}
void LevelDesign::toggleAssetMode()
{
    if(showAssets)
    {
        for (int i=0; i< listMenu1.size(); i++)
        {
            gui.get(listMenu1[i]+"LM1")->show();
        }
        showAssets = false;
        for (int i=0; i< listMenu2.size(); i++)
        {
            gui.get(listMenu2[i]+"LM2")->hide();
        }
    }
    else
    {
        for (int i=0; i< listMenu1.size(); i++)
        {
            gui.get(listMenu1[i]+"LM1")->hide();
        }
        showAssets = true;
        for (int i=0; i< listMenu2.size(); i++)
        {
            gui.get(listMenu2[i]+"LM2")->show();
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
        zoomIN();
    }
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Subtract))
    {
        zoomOUT();
    }
    ///OPEN IMAGE FILE
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::O))
    {
        loadFiles();
    }
    ///DELETING IMAGE OR NODE
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Delete))
    {
        if(showAssets)
        {
//            if(tmpAsset != nullptr)
//            {
//                deleteAsset(tmpAsset);
//                tmpAsset = nullptr;
//            }
//            for(int i=0; i < selectedAsset.size(); i++)
//            {
//                deleteAsset(selectedAsset[i]);
//            }
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
                if(showAssets)
                {
                    if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) || sf::Keyboard::isKeyPressed(sf::Keyboard::RControl))
                    {
                        selectAllA();
                    }
                    else
                        showAssets = !showAssets;
                }
                else
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
//                    if(tmpAsset != nullptr)
//                    {
//        //                Asset a = *tmpAsset;
//                        assetList.push_back(*tmpAsset);
//                    }
                    duplicateSelectedA();
                }
                else///CREATE AN ASSET FROM CURRENT IMAGE AND NODES
                    createAsset();


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
                    {
                        saveAssets();
                    }

                }
            }
            ///DELETE ASSETS
            if(e.key.code == sf::Keyboard::Delete)
            {
                if(showAssets)
                {
                    deleteAsset(nullptr);
                    if(tmpAsset != nullptr)
                    {
                        deleteAsset(tmpAsset);
                        tmpAsset = nullptr;
                    }
                }
            }
            ///EDIT ASSETS
            if(e.key.code == sf::Keyboard::E)
            {
                if(showAssets)
                {
                    if(tmpAsset != nullptr)
                    {
                        vertexList.clear();
                        imageList.clear();
                        filenameList.clear();
                        tmpNode = nullptr;
                        tmpSprite = nullptr;

                        vertexList = tmpAsset->nodeList;
                        imageList.push_back(tmpAsset->aSprite);
                        filenameList.push_back(tmpAsset->path);
                        toggleAssetMode();
                    }
                }
            }

        break;


    }
}

void LevelDesign::zoomIN()
{
    if(isMoving)
        {
            if(tmpSprite != nullptr)
            {
                tmpSprite->setScale(tmpSprite->getScale()+sf::Vector2f(.05f,.05f));
            }
            if(tmpAsset != nullptr)
            {
//                tmpAsset->shrink(); MDR
                if(tmpAsset->zIndex < 2)
                    tmpAsset->zIndex++;
                else
                    tmpAsset->zIndex = 0;
            }
        }
        else
            mWorldView.zoom(0.7f);
}
void LevelDesign::zoomOUT()
{
    if(isMoving)
        {
            if(tmpSprite != nullptr)
            {
                tmpSprite->setScale(tmpSprite->getScale()-sf::Vector2f(.05f,.05f));
            }
            if(tmpAsset != nullptr)
            {
//                tmpAsset->shrink(); MDR
                if(tmpAsset->zIndex > 0)
                    tmpAsset->zIndex--;
                else
                    tmpAsset->zIndex = 2;
            }
        }
        else
            mWorldView.zoom(1.3f);
}

void LevelDesign::openProject(std::string filename)
{

    pugi::xml_document          XMLDocument;
    if (!XMLDocument.load_file(filename.c_str()))
    {
        std::cout << "error on loading Asset: "<<filename<< "\n";
    }
    pugi::xml_node assetsN = XMLDocument.child("Assets");
    pugi::xml_node dupliN = XMLDocument.child("Copies");
    for (pugi::xml_node node = assetsN.first_child(); node ; node = node.next_sibling())
    {
        _assetList.push_back(Asset(node));
        assetList.push_back(_assetList[_assetList.size()-1]);
        assetIDList.push_back(node.attribute("id").as_int());
    }
    for (pugi::xml_node node = dupliN.first_child(); node ; node = node.next_sibling())
    {
        for(int i=0; i<_assetList.size(); i++)
        {
            if(_assetList[i]._id == node.attribute("from").as_int())
            {
                assetList.push_back(_assetList[i]);
                assetList[assetList.size()-1].setPosition(
                                                          sf::Vector2f(node.attribute("x").as_float(),node.attribute("y").as_float())
                                                          );
            }
        }
    }
}

void LevelDesign::loadFiles(int choice)
{
    if(showAssets && choice != 3)
        {
            const char *filters[] = {"*.asset"};
            const char *fn = tinyfd_openFileDialog("Open Assets","",1,filters,"Asset files", 1);

            if(fn)
            {
                std::vector<std::string> pathList;
                pathList = splitPerso(fn, "|");

                for(int i=0; i<pathList.size(); i++)
                {
                    std::cout <<"Loading of "<<pathList[i] << std::endl;

                    if (choice == 1) //YES
                    {
                        openProject(pathList[i]);
                    }
                    else
                    {
                        _assetList.push_back(Asset(pathList[i]));
                        assetList.push_back(_assetList[_assetList.size()-1]);////
                    }


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
void LevelDesign::createAsset()
{
    if(imageList.size()<=0)
        return;
//    std::cout<<"creating asset...."<<std::endl;
//    Asset a(imageList[0],vertexList,filenameList[0]);
//    std::cout<<"pushing the asset...."<<std::endl;
//    assetList.push_back(a);
    assetList.push_back(Asset(imageList[0],vertexList,filenameList[0]));
//    std::cout<<"clearing all...."<<std::endl;
    vertexList.clear();
    imageList.clear();
    filenameList.clear();
    tmpNode = nullptr;
    tmpSprite = nullptr;
//    std::cout<<"done!!"<<std::endl;
}

void LevelDesign::selectAsset(Asset* asset)
{
    asset->selected = true;
}
void LevelDesign::unselectAllA()
{
    for(int i=0; i < assetList.size(); i++)
    {
        assetList[i].selected = false;
    }
}
void LevelDesign::selectAllA()
{
    for(int i=0; i < assetList.size(); i++)
    {
        assetList[i].selected = true;
    }
}

void LevelDesign::mouseInput(sf::Event e)
{

    switch(e.type)
    {
    case sf::Event::MouseMoved:
        {
            if(showAssets)
            {
                if(selectOn)
                {
                    selectRectangle.setPosition(clickPos);
                    selectRectangle.setSize(sf::Vector2f(getMousePos().x - clickPos.x, getMousePos().y - clickPos.y));

                }

            }

            if(moveView)
            {
                mWorldView.move(clickPos - getMousePos());
            }
            if(isMoving)
            {
                ///MANIPULATION DES ASSETS
                if(showAssets)
                {
//                    if(tmpAsset != nullptr)//ajouter if pinned
//                        if(!tmpAsset->isPinned())
//                            tmpAsset->setPosition(getMousePos());
                    if(tmpAsset != nullptr)
                    {
                        moveSelectedA(getMousePos()-clickPos);
                    }
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
            if(e.mouseButton.button == sf::Mouse::Left)
            {
                if(showAssets)
                {

//                    selectedAsset.clear();
                    selectOn = true;
                    clickPos = getMousePos();
                    if(!sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)||!sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) )
                        unselectAllA();
                }
            }
            if(e.mouseButton.button == sf::Mouse::Right)
            {
                ///MANIPULATION DES ASSETS
                if(showAssets)
                {
                    clickPos = getMousePos();
                    for (int i=0; i < assetList.size(); i++)
                    {
                        if(assetList[i].getGlobalBounds().contains(getMousePos()))
                        {
                            isMoving = true;
                            tmpAsset = &(assetList[i]);
                            if(!sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)||!sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) )
                                unselectAllA();
                            if(!tmpAsset->isPinned())
                                tmpAsset->selected = true;

                            getAssetsRatio();
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
            if(e.mouseButton.button == sf::Mouse::Middle)
            {
                clickPos = getMousePos();
                moveView = true;
            }
        }
    break;
    case sf::Event::MouseButtonReleased:
        {
            if(e.mouseButton.button == sf::Mouse::Left)
            {
                if(showAssets)
                {
                    if(selectOn)
                    {

                        for (int i=0; i < assetList.size(); i++)
                        {
//                            std::cout<<"rect Lb:"<<selectRectangle.getGlobalBounds().top<<selectRectangle.getGlobalBounds().left<<selectRectangle.getGlobalBounds().width<<selectRectangle.getGlobalBounds().height
//                            <<"\t pos asset:"<<assetList[i].getPosition().x<<assetList[i].getPosition().y<<std::endl;
                            if(selectRectangle.getGlobalBounds().contains(assetList[i].getPosition() ))
                            {
                                assetList[i].selected = !assetList[i].selected;
                                //selectedAsset.push_back(&assetList[i]);
                                //selectedAsset[selectedAsset.size()-1]->pinned = true;
                            }
                        }

                        selectRectangle.setSize(sf::Vector2f(0.f,0.f));
                        selectOn = false;
                    }

                }
            }
            if(e.mouseButton.button == sf::Mouse::Middle)
            {
                moveView = false;
            }
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
                    if(vertexMode)
                        vertexList.push_back(createVertex(getMousePos()));
                }
            }


        }
    break;
    case sf::Event::MouseWheelMoved:
        {
            if(e.mouseWheel.delta>0)
                zoomIN();
            if(e.mouseWheel.delta<0)
                zoomOUT();

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
//    for(int i=0 ; i < assetList.size(); i++)
//    {
//        if(assetList[i].selected)
//        {
//            assetList.erase(assetList.begin()+i);
//        }
//
//    }
    for ( std::vector<Asset>::iterator it = assetList.begin(); it != assetList.end(); )
    {
        if((*it).selected )
           {
              it = assetList.erase(it);
           }
           else {
              ++it;
           }
    }

}

void LevelDesign::getAssetsRatio()
{
    assetRatio.clear();
    for(int i = 0; i<assetList.size(); i++)
    {
        if(assetList[i].selected)
            assetRatio.push_back(assetList[i].getPosition());
    }
}

void LevelDesign::moveSelectedA(sf::Vector2f pos)
{
    int i = 0;
    for ( std::vector<Asset>::iterator it = assetList.begin(); it != assetList.end(); it++)
    {
        if((*it).selected )
        {
            (*it).setPosition(pos + assetRatio[i]);

            i++;
        }
    }
}

void LevelDesign::duplicateSelectedA()
{
//    for ( std::vector<Asset>::iterator it = assetList.begin(); it != assetList.end(); it++)
//    {
//        if((*it).selected )
//        {
////            assetList.push_back((*it));
//            (*it).selected = false;
//        }
//    }
    for(int i = 0; i<assetList.size(); i++)
    {
        if(assetList[i].selected)
        {
            assetList[i].selected = false;
            assetList.push_back(assetList[i]);
            assetList[i].selected = true;
        }

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
    sf::RectangleShape line(sf::Vector2f(5000, 10));
    if(showAssets)
    {
        line.setFillColor(sf::Color(255,100,100,200));
        t.setColor(sf::Color::Black);
    }
    else
        if(!vertexMode)
        {
            line.setFillColor(sf::Color(255,0,0,200));
            t.setColor(sf::Color(0,0,255,100));
        }

        else
        {
            line.setFillColor(sf::Color(255,255,255,200));
            t.setColor(sf::Color::White);
        }

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
        t.setOrigin(t.getLocalBounds().width/2,t.getLocalBounds().height/2);
        t.setPosition(i*100, 10.f);

        mWindow.draw(t);
    }

    for(int i=0; i < 50; i++)
    {
        std::stringstream ss;
        ss<<"-"<<i<<"00-";

        t.setCharacterSize(20);
        t.setString(ss.str());
        t.setOrigin(t.getLocalBounds().width/2,t.getLocalBounds().height/2);
        t.setPosition(10.f, i*100);

        mWindow.draw(t);
    }
    /************/

    if(selectOn)
        mWindow.draw(selectRectangle);
    renderGUI(frameTime);
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
    for (int z = 0; z <= 2; z++) // zIndex
    {
        for(int i=0; i < assetList.size(); i++)
        {
            if(assetList[i].zIndex == z)
                assetList[i].render(mWindow);
        }
    }
}

void LevelDesign::renderGUI(sf::Time frameTime)
{
    std::stringstream ss;
    ss<<"x: "<<(int)getMousePos().x<<", y:"<<(int)getMousePos().y;
    tgui::Label::Ptr labelMousePos = gui.get("MousePos");
    labelMousePos->setText(ss.str());
    if(showAssets)
        labelMousePos->setTextColor(sf::Color::White);
    else
        labelMousePos->setTextColor(sf::Color::Red);
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
    for (int z = 0; z <= 2; z++) // zIndex
    {
        for(int i=0; i< assetList.size(); i++)
        {
            if(assetList[i].zIndex != z)
                continue;
            std::stringstream ss;
            ss<<i;

            if(assetList[i].nodeRatio.size() <3)
                continue;
            //*********body**********
            pugi::xml_node bodyN = bodies.append_child("body");
            bodyN.append_attribute("name") = i;
            bodyN.append_attribute("type") = "static";
            bodyN.append_attribute("x") = assetList[i].getPosition().x;
            bodyN.append_attribute("y") = -assetList[i].getPosition().y;
            bodyN.append_attribute("image") = (assetList[i].name+ss.str()).c_str();
            //*********end body**********

            //*********fixtures**********
            pugi::xml_node fixturesN = bodyN.append_child("fixtures");
            pugi::xml_node fixtureN = fixturesN.append_child("fixture");
            fixtureN.append_attribute("name") = i;
            fixtureN.append_attribute("restitution") = "0";
            fixtureN.append_attribute("friction") = "1";
            fixtureN.append_attribute("isSensor") = "false";
            fixtureN.append_attribute("shapeType") = "polygonShape";
            fixtureN.append_attribute("categoryBits") = "0x0003";
            fixtureN.append_attribute("maskBits") = "0x0002";
            bool isInf8sup3 = false;
            std::vector<int> Vx;
            std::vector<int> Vy;


            for(int j = 0; j < assetList[i].nodeRatio.size(); j++)
            {
                pugi::xml_node vertexN = fixtureN.append_child("vertex");
                vertexN.append_attribute("x") = -assetList[i].nodeRatio[j].x;
                vertexN.append_attribute("y") = -assetList[i].nodeRatio[j].y;
                if(j%7 == 0 && assetList[i].nodeRatio.size() > 8)
                {
                    if(j>0)
                    {
                        isInf8sup3 = true;
                        if((assetList[i].nodeRatio.size() - j) > 3 )
                        {
                            fixtureN = fixturesN.append_child("fixture");
                            fixtureN.append_attribute("name") = i;
                            fixtureN.append_attribute("restitution") = "0";
                            fixtureN.append_attribute("friction") = "1";
                            fixtureN.append_attribute("isSensor") = "false";
                            fixtureN.append_attribute("shapeType") = "polygonShape";
                            fixtureN.append_attribute("categoryBits") = "0x0003";
                            fixtureN.append_attribute("maskBits") = "0x0002";
                            pugi::xml_node vertexNj = fixtureN.append_child("vertex");
                            vertexNj.append_attribute("x") = -assetList[i].nodeRatio[j].x;
                            vertexNj.append_attribute("y") = -assetList[i].nodeRatio[j].y;
                        }
//                        else
//                        {
//                            Vx.push_back(-assetList[i].nodeRatio[j].x);
//                            Vy.push_back(-assetList[i].nodeRatio[j].y);
//                        }

                    }



                    Vx.push_back(-assetList[i].nodeRatio[j].x);
                    Vy.push_back(-assetList[i].nodeRatio[j].y);
                }
//                if(isInf8sup3 && j == assetList[i].nodeRatio.size()-1)
//                {
//                    Vx.push_back(-assetList[i].nodeRatio[j].x);
//                    Vy.push_back(-assetList[i].nodeRatio[j].y);
//                }
            }
            if(isInf8sup3)
            {
                Vx.push_back(-assetList[i].nodeRatio[assetList[i].nodeRatio.size()-1].x);
                Vy.push_back(-assetList[i].nodeRatio[assetList[i].nodeRatio.size()-1].y);

                fixtureN = fixturesN.append_child("fixture");
                fixtureN.append_attribute("name") = i;
                fixtureN.append_attribute("restitution") = "0";
                fixtureN.append_attribute("friction") = "1";
                fixtureN.append_attribute("isSensor") = "false";
                fixtureN.append_attribute("shapeType") = "polygonShape";
                fixtureN.append_attribute("categoryBits") = "0x0003";
                fixtureN.append_attribute("maskBits") = "0x0002";
                for(int k =0; k<Vx.size(); k++)
                {

                    pugi::xml_node vertexNj = fixtureN.append_child("vertex");
                    vertexNj.append_attribute("x") = Vx[k];
                    vertexNj.append_attribute("y") = Vy[k];
                }


            }
            //*********end fixtures**********

            //*********Images**********
            pugi::xml_node imageN = images.append_child("image");
            imageN.append_attribute("name") = (assetList[i].name+ss.str()).c_str();
            imageN.append_attribute("path") = assetList[i].name.c_str();
            imageN.append_attribute("x") = assetList[i].getPosition().x;
            imageN.append_attribute("y") = -assetList[i].getPosition().y;
            imageN.append_attribute("scaleX") = assetList[i].aSprite.getScale().x;
            imageN.append_attribute("scaleY") = assetList[i].aSprite.getScale().y;
            //*********end Images**********

        }
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

void LevelDesign::saveProject()
{
    const char *filters[] = {"*.asset"};
    const char *fn = tinyfd_saveFileDialog("Save all assets as", "", 1, filters,"Asset Files");

    pugi::xml_document doc;
    pugi::xml_node assetN = doc.append_child("Assets");
    pugi::xml_node dupliN = doc.append_child("Copies");
    assetIDList.clear();
    for(int i=0; i< assetList.size(); i++)
    {
        if(!assetIDExist(assetList[i]._id))
        {
            assetList[i].addAssetNode(assetN);
            assetIDList.push_back(assetList[i]._id);
        }
        else
        {
            assetList[i].addDupliNode(dupliN);
        }

//        std::stringstream ss;
//        ss<<fn<<i<<".asset";
//        assetList[i].exportToXML(ss.str());
    }

    std::stringstream ss;
    ss<<fn<<".asset";
    doc.save_file(ss.str().c_str());
}

bool LevelDesign::assetIDExist(int id)
{
    for(int i=0; i<assetIDList.size(); i++)
    {
        if(assetIDList[i] == id)
            return true;
    }
    return false;
}

void LevelDesign::saveAsset(Asset a)
{
    const char *filters[] = {"*.asset"};
    const char *fn = tinyfd_saveFileDialog("Save Asset as", "", 1, filters,"Asset File");
    std::stringstream ss;
    ss<<fn;

    a.exportToXML(ss.str());
}


