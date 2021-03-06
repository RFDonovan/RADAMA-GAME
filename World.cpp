#include "World.h"

World::World(sf::RenderWindow& window)
    : mWindow(window)
    , gravity(0.f,12.f)
//, gravity(0.f,9.8f)
    , p_world(gravity)//, true)--2.2.0
    , p_world2(b2Vec2(0.f,0.f))
    , debugDrawInstance(window)
    , mWorldView(window.getDefaultView())
    , pauseLayer(sf::Vector2f(WINDOW_W, WINDOW_H))
    , levelPath("./Resources/L1/")
    , statInfo(mWindow)

{

    m_mouseJoint = NULL;
    window.setVerticalSyncEnabled(true);
    p_world.SetDebugDraw(&debugDrawInstance);
    p_world2.SetDebugDraw(&debugDrawInstance);

    ///debugDrawInstance.SetFlags(b2Draw::e_shapeBit|b2Draw::e_jointBit);


    loadTextures();
    buildScene(levelPath);
    pauseLayer.setOrigin(sf::Vector2f(WINDOW_W/2, WINDOW_H/2));
    Textures.loadFromFile("BG", "Resources/background1.png");
    BG = sf::Sprite(*Textures.getTexture("BG"));
    ///BG_pause = sf::Sprite(*Textures.getTexture(TextureHolder::Pause));
    BG.scale(2.6f,2.6f);
    BG.setPosition(-WINDOW_W/3, -WINDOW_W/3);

    p_world.SetContactListener(&CL_Instance);
    p_world.SetDestructionListener(&DL_Instance);


    if(!sf::Shader::isAvailable())
    {
        std::cout<< "SHADER NOT AVAILABLE!!!\n";

        exit(EXIT_FAILURE);
    }

    /**SHADER STUFF**/
//    Textures.loadFromFile("distortionMap", "Resources/Images/perlin_noise.jpg");
    if(!distortionMap.loadFromFile("Resources/Images/perlin_noise.jpg"/*"shade.png"*/))
        return;
    distortionMap.setRepeated(true);
    distortionMap.setSmooth(true);
    if (!shader.loadFromFile("heat_shader.vs", "heat_shader.fs"))
    {
        sf::err() << "Failed to load shader, exiting..." << std::endl;
        return;
    }

    shader.setParameter("currentTexture", sf::Shader::CurrentTexture);
    shader.setParameter("distortionMapTexture", distortionMap);
//    shader.setParameter("distortionMapTexture", *Textures.getTexture("distortionMap"));
    if(!fogShader.loadFromFile("fog_shader.fs", sf::Shader::Fragment))
    {
        sf::err() << "Failed to load shader, exiting..." << std::endl;
        return;
    }


    ///***********************************

//    xLoad2 = new XMLLoader(&p_world2);
//    xLoad2->loadXML("./pause.xml", "./");
//    exit(-1);

    b2BodyDef bodyDef;
    m_groundBody = p_world.CreateBody(&bodyDef);///pour le jointmouse

    /**TGUI STUFF*/
    {
        gui.setWindow(mWindow);
        gui.setGlobalFont("fonts/DejaVuSans.ttf");

        tgui::Button::Ptr btn(gui,"REPLAY");
        btn->load("widgets/Black.conf");
        btn->setTextSize(18);
        btn->setSize(85, 30);
//        btn->setPosition(100* i + 100, WINDOW_H - 50);
        btn->setPosition(WINDOW_W/2 - (85/2),WINDOW_H/2-(30/2));
        btn->setText("REPLAY");
        btn->bindCallback(tgui::Button::LeftMouseClicked);
        btn->setCallbackId(1);

        tgui::Button::Ptr btnX(gui,"EXIT");
        btnX->load("widgets/Black.conf");
        btnX->setTextSize(18);
        btnX->setSize(85, 30);
//        btn->setPosition(100* i + 100, WINDOW_H - 50);
        btnX->setPosition(WINDOW_W/2 - (85/2),WINDOW_H/2-(30/2)+50);
        btnX->setText("EXIT");
        btnX->bindCallback(tgui::Button::LeftMouseClicked);
        btnX->setCallbackId(2);
    }
}

void World::processInput(sf::Event e)
{
    if(GAME_OVER)
        gui.handleEvent(e);
    pauseMenu pM;

    if(!paused) /// ******************************************************************>>>>PAUSE
        if(!editMode)
        {
            //std::cout<<"passer la commande "<<std::endl;
            ePlayer->onCommand(e);
            //return;
        }



    if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
    {
        if(editMode)
            MouseMove(b2Vec2(getMousePos().x/RATIO, getMousePos().y/RATIO));
    }

    switch(e.type)
    {
    case sf::Event::MouseButtonPressed:
    {
        if(editMode)
            MouseDown(b2Vec2(getMousePos().x/RATIO, getMousePos().y/RATIO));

        //std::cout<<"positon mouse:" <<getMousePos().y<<std::endl;
    }
    break;
    case sf::Event::MouseButtonReleased:
    {
        if(editMode)
            MouseUp();
        if(paused)
        {

        }
    }
    break;
    case sf::Event::MouseWheelMoved:
        {
            if(e.mouseWheel.delta>0)
            {
                mWorldView.zoom(0.9f);
                mWorldView.setCenter(getMousePos());
            }
            if(e.mouseWheel.delta<0)
                mWorldView.zoom(1.1f);

        }
        std::cout << e.mouseWheel.delta << '\n';
    break;
    case sf::Event::KeyReleased:
        if(e.key.code == sf::Keyboard::Escape)
        {
            //paused = !paused;
            if(paused)
                resume();
            else
                pause();
        }
        if(e.key.code == sf::Keyboard::BackSpace)
        {
            rebuildScene();
        }
        if(e.key.code == sf::Keyboard::C)
        {
            debugDrawInstance.ClearFlags(b2Draw::e_shapeBit|b2Draw::e_jointBit);
        }
        if(e.key.code == sf::Keyboard::V)
        {
            debugDrawInstance.SetFlags(b2Draw::e_shapeBit|b2Draw::e_jointBit);
        }
        if(e.key.code == sf::Keyboard::E)
        {
            editMode = !editMode;
            std::cout<<"MODE EDITION: "<<editMode<<std::endl;

        }
        if(e.key.code == sf::Keyboard::A)
        {
            std::cout<<"deleting 10 01 0 10 1"<<std::endl;
            listOfDeletedHuman.push_back(humans[0]);
            humans.erase(humans.begin());
            std::cout<<"contenu de human"<<humans.size()<<std::endl;
            //delete humans[1];
        }
        if(e.key.code == sf::Keyboard::Return)
        {
            if(editMode)
                ePlayer->exportToXML("ePlayer.xml");

        }

        break;
        case sf::Event::MouseMoved :
            if (paused)
            {


            }
        break;

    }

    if (e.type == sf::Event::LostFocus)
        pause();

    if (e.type == sf::Event::GainedFocus)
        resume();
    if(paused)
    {
        nameToSprite["resume.png"]->setColor(sf::Color(255,255,255,255));
        nameToSprite["quit.png"]->setColor(sf::Color(255,255,255,255));
        nameToSprite["reload.png"]->setColor(sf::Color(255,255,255,255));
        pM = pauseMenu::NONE;

        if(mouseIsOnTheSprite(nameToSprite["resume.png"], getMousePos()))
        {
            nameToSprite["resume.png"]->setColor(sf::Color(255,255,255,150));
            pM = pauseMenu::RESUME;

        }
        if(mouseIsOnTheSprite(nameToSprite["quit.png"], getMousePos()))
        {
            nameToSprite["quit.png"]->setColor(sf::Color(255,255,255,150));
            pM = pauseMenu::QUIT;

        }
        if(mouseIsOnTheSprite(nameToSprite["reload.png"], getMousePos()))
        {
            nameToSprite["reload.png"]->setColor(sf::Color(255,255,255,150));
            pM = pauseMenu::RESTART;

        }

        if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
        {
            switch(pM)
            {
            case pauseMenu::RESUME:
                resume();
            break;
            case pauseMenu::RESTART:
                resume();
                rebuildScene();

            break;
            case pauseMenu::QUIT:
                exit(-1);
            break;
            default:
                break;
            }
        }
    }



}

bool World::mouseIsOnTheSprite(sf::Sprite* sp, sf::Vector2f mousePos)
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

void World::updateView(sf::Vector2f view)
{
    mWorldView.move(view);

}

void World::update()
{
    //*

    if(paused)/// ******************************************************************>>>>PAUSE
        return;

    sf::Vector2f playerPosition(0.0f,0.0f);

    if(ePlayer->m_life <= 0)
        GAME_OVER = true;

    ePlayer->processLogic();
    mWorldView.move(playerPosition);
    adaptViewToPlayer();

    statInfo.updateLife(ePlayer->m_life);
    statInfo.updateMana(ePlayer->m_mana);
    statInfo.updateProjectile(ePlayer->getProjectileCount());

    guiCallback();

}

void World::guiCallback()
{
    tgui::Callback callback;
        while (gui.pollCallback(callback))
        {
            if (callback.id == 1)
            {
                rebuildScene();
            }

            if (callback.id == 2)
                mWindow.close();
        }
}

void World::sticking()
{
    for (int i = 0; i < pList.size(); i++)
    {
        pList[i]->stickIt();
    }
}

void World::draw(sf::Time frameTime)
{

//    fogShader.setParameter("time",clock.getElapsedTime().asSeconds());
//    fogShader.setParameter("mouse",400.f, 300.f);
//    fogShader.setParameter("resolution",800.f, 600.f);
    //BG.setColor(sf::Color(0, 0, 0, 255));
//    mWindow.draw(BG, &fogShader);

    //BG.setColor(sf::Color(0, 0, 0, 200));

    mWindow.draw(BG);


    if(!paused)/// ******************************************************************>>>>PAUSE
    {
        sticking();///create joint for sticky projectile
        p_world.Step(1/60.f,6,2);

        /// LES FONCTIONS QUI SONT EN DEHORS DU STEP : suppressions securisE des bodies ou changements des datas comme setActive
        ///REMOVE JOINTS FIRST
        if((int)clock.getElapsedTime().asSeconds()%10==0)
            for (int j = 0 ; j < humans.size() ; j++ )
            {
                if(humans[j]->isDead())
                {
                    humans[j]->wipeJoints();
                }
            }
        ///----------------------
        sheduleRemove(clock.getElapsedTime().asSeconds());

        /// ----------------------------------------
        p_world.ClearForces();
        //mWindow.setMouseCursorVisible(false);

    }

    else
    {
        mWindow.setMouseCursorVisible(true);
        p_world2.Step(1/60.f,6,2);
    }

    mWindow.setView(mWorldView);
    ///LEVEL
    //level->render(mWindow, &shader);
    xLoad->render(mWindow, &shader);
    for (int i = 0; i < itemList.size(); i++)
    {
        if(itemList[i]->dejaPris)
        {
            itemList.erase(itemList.begin()+i);
        }
        else
            itemList[i]->render(mWindow, &shader);
    }


///RENDU DU JOUEUR
    ePlayer->render(mWindow, frameTime, &Textures, &shader);
    if(/*ePlayer->getY() > mWindow.getSize().y || */ePlayer->isDead())
    {
        if(!ePlayer->deleted)
        {
            ePlayer->doTheDead();
        }
    }
    for (int i = 0 ; i < humans.size() ; i++ )
    {
        humans[i]->render(mWindow, frameTime, &Textures, &shader);
        ///teste une suppression d'Entit� morts: ASSEZ BIEN!!!
        if(/*humans[i]->getY() > mWindow.getSize().y ||*/ humans[i]->isDead())
        {
            ///RENDER A DEAD VERSION
            if(!humans[i]->deleted)
                humans[i]->doTheDead();
            //humans[i]->wipeJoints();
            continue;

        }
        else
        {
            humans[i]->sense();
        }

        //std::cout<<"\n draw- after render\n";
    }

    ///RENDU DES STATS
    statInfo.render(frameTime, &shader);

    ///---------------

    p_world.DrawDebugData();

    if(paused)
    {


//        pauseLayer.setFillColor(sf::Color(0, 0, 0, 150));
        pauseLayer.setFillColor(sf::Color(0, 0, 0, 220));
        mWindow.draw(pauseLayer);
        //sf::Sprite::getTransform()
//        pauseLayer.setPosition(300,300);
//        nameToSprite = xLoad2->getNameToSpriteList();
//
//        std::cout<<
//        (nameToSprite["quit.png"])->getPosition().x
//        <<std::endl;
//        exit(-1);
        xLoad2->render(mWindow, &shader);

        distortionFactor = .01f;
        riseFactor = .5f;
        shader.setParameter("time", clock.getElapsedTime().asSeconds());
        shader.setParameter("distortionFactor", distortionFactor);
        shader.setParameter("riseFactor", riseFactor);
        ///mWindow.draw(BG_pause);

        p_world2.DrawDebugData();
//        mWindow.draw(BG_pause, &shader);

    }
    else
    {
        distortionFactor = .01f;
        riseFactor = .5f;
        shader.setParameter("time", clock.getElapsedTime().asSeconds());
        shader.setParameter("distortionFactor", distortionFactor);
        shader.setParameter("riseFactor", riseFactor);


        for( int i = 0; i< humans.size(); i++)
        {
            if(humans[i]->showFX)
            {
//                humans[i]->m_sensor->GetPosition().x*RATIO
                showFXon(humans[i]->m_sensor->GetPosition().x*RATIO,
                         humans[i]->m_sensor->GetPosition().y*RATIO
                         );
                         humans[i]->showFX = false;
            }
        }
        if(ePlayer->showFX)
        {
            showFXon(ePlayer->m_sensor->GetPosition().x*RATIO,
                         ePlayer->m_sensor->GetPosition().y*RATIO
                         );
            ePlayer->showFX = false;

        }
        drawFX(frameTime, &shader);
    }
    if(GAME_OVER)
    {
        pauseLayer.setFillColor(sf::Color(0, 0, 0, 100));
        mWindow.draw(pauseLayer);
        gui.draw();
    }

}

void World::drawFX(sf::Time frameTime, sf::Shader* shad)
{
    if(atkFXClock.getElapsedTime().asMilliseconds() < 800.f)
    {
        if(damageFXSprite.getColor().a>0&& (int)(atkFXClock.getElapsedTime().asMilliseconds()*255/800) > 150)
        {
            damageFXSprite.setColor(sf::Color(255,255,255,

                                              255 - (int)(atkFXClock.getElapsedTime().asMilliseconds()*255/800)
                                              ));
            damageFXSprite.setRotation(damageFXSprite.getRotation()+1);
            //damageFXSprite.setScale(damageFXSprite.getScale().x+ 0.005f, damageFXSprite.getScale().y+0.005f);
            damageFXSprite.setPosition(damageFXSprite.getPosition().x, damageFXSprite.getPosition().y - .3f);

        }
        mWindow.draw(damageFXSprite, shad);


    }

}

void World::showFXon(float x, float y)
{
    damageFXSprite.setPosition(x, y);
    damageFXSprite.setColor(sf::Color(255,255,255,255));
    float fxSize = 60.f;

//    damageFXSprite.scale(fxSize/damageFXSprite.getTexture()->getSize().x,
//                         fxSize/damageFXSprite.getTexture()->getSize().y
//                         );
    atkFXClock.restart();
}

void World::loadTextures()
{
    /**Load the animated sprites & more**/
//    Textures.loadFromFile(TextureHolder::Player, "pGGbv.png");
//    Textures.loadFromFile(TextureHolder::Ground, "ground.png");
//    Textures.loadFromFile(TextureHolder::Ground1, "ground1.png");
//    Textures.loadFromFile(TextureHolder::Ground2, "ground2.png");
//    Textures.loadFromFile(TextureHolder::Background1, "background.png");
//    Textures.loadFromFile(TextureHolder::Pause, "pause.png");
//    Textures.loadFromFile(TextureHolder::Fire, "fire.png");
//    Textures.loadFromFile(TextureHolder::Lefona, "lefona.png");

    //*/

    /*damageTexture FX*/

    Textures.loadFromFile("damage", "damage.png");
    damageFXSprite = sf::Sprite(*Textures.getTexture("damage"));
    float fxSize = 60.f;

    damageFXSprite.scale(fxSize/damageFXSprite.getTexture()->getSize().x,
                         fxSize/damageFXSprite.getTexture()->getSize().y
                         );
    damageFXSprite.setOrigin(Textures.getTexture("damage")->getSize().x/2, Textures.getTexture("damage")->getSize().x/2);

}



void World::buildScene(std::string CurrentDir)
{
    std::cout<<"\n**********************************INITIALISATION**************************************\n";
    std::cout<<"**************************************************************************************\n";

    loadInfo("Resources/config.xml");

    sf::FloatRect r(sf::Vector2f(0.f,0.f),
                              mWorldView.getSize()
               );

    //BG_pause.setPosition(sf::Vector2f(0.f,0.f));
    pauseLayer.setPosition(sf::Vector2f(0.f,0.f));
    mWorldView.reset(r);
    BG.setPosition(-WINDOW_W/3, -WINDOW_W/3);

    mWorldView.setCenter(WINDOW_W/2,WINDOW_H/2);
    ePlayer->setPosition(sf::Vector2f(WINDOW_W/2,WINDOW_H/2));

}


void World::loadInfo(std::string xmlCfg)
{
    AnimationDB  = new SpriteMapping("Resources/animationDB.xml");
//    smList.push_back(*AnimationDB);
    pugi::xml_document XMLDoc;
    if (!XMLDoc.load_file(xmlCfg.c_str()))
    {
        std::cout << "World::loadInfo -> error on loading "<<xmlCfg<< "\n";
        return;
    }
    pugi::xml_node levelNode = XMLDoc.child("RADAMA").child("level");

    std::stringstream ss;
    ss <<levelNode.attribute("path").as_string();
    std::string directory = ss.str();

    ///LOADING ALL ANIMATED SPRITES
    //loadSprites(directory+"spriteList.xml");

    ///LOADING GROUNDS
    xLoad = new XMLLoader(&p_world, &Textures);
    LevelObjectList = xLoad->loadXML(directory + "level.xml", directory);
    Ground * level = new Ground(&p_world, &LevelObjectList);


    ///LOADING ITEMS
    pugi::xml_node itemsNode = levelNode.child("items");
    for (pugi::xml_node node = itemsNode.first_child(); node ; node = node.next_sibling())
        ///Entities ITERATION
    {

        std::stringstream ss;
        ss <<node.attribute("image").as_string();
        std::string Ifilename = ss.str();

        Item* item = new Item(&p_world,
                              Ifilename,
                              (float32)node.attribute("x").as_float(),
                              -(float32)node.attribute("y").as_float(),
                              (int)node.attribute("mana").as_float(),
                              (int)node.attribute("life").as_float()
                              );
        itemList.push_back(item);
    }
    ///-------------
    ///LOADING PLAYER

    pugi::xml_node playerNode = levelNode.child("player");
        std::stringstream ss3;
        ss3 <<playerNode.attribute("file").as_string();
        std::string Pfilename = ss3.str();
    std::vector<bodyData> bDListP = xLoad->loadXML(directory + Pfilename, directory);
    std::map<std::string, b2Joint*> jMap = xLoad->GetCurrentJointMap();
    ePlayer = new Player(mWindow,&p_world, 1.f , &bDListP, &jMap, &AnimationDB->animationDB[playerNode.attribute("type").as_string()]);

    std::cout << "World::loadInfo -> playerjump infojump "<<playerNode.attribute("jump").as_string() <<spriteMap[playerNode.attribute("jump").as_string()]<<">>>>>>>>>>>>**************\n";



    ePlayer->setPosition(sf::Vector2f(
                                    playerNode.attribute("x").as_float(),
                                    playerNode.attribute("y").as_float()
                                    )
                       );

    ePlayer->addAnimations(&AnimationDB->animationDB[playerNode.attribute("type").as_string()]);

    ///LOADING OTHER ENTITIES
    std::cout << "World::loadInfo -> loading spritemap\n";
    pugi::xml_node entitiesNode = levelNode.child("enemies");

    std::cout << "World::loadInfo -> getting spritemap\n";
    for (pugi::xml_node node = entitiesNode.first_child(); node ; node = node.next_sibling())
        ///Entities ITERATION
    {

        std::stringstream ss2;
        ss2 <<node.attribute("file").as_string();
        std::string filename = ss2.str();
        ///ajoutez un / a la fin du path
        std::vector<bodyData> bDListH = xLoad->loadXML(directory + filename, directory);
        std::map<std::string, b2Joint*> jMapH = xLoad->GetCurrentJointMap();


        Human* e = new Human(mWindow,&p_world, 1.f
                             , &bDListH
                             , &jMapH
                             , &AnimationDB->animationDB[node.attribute("type").as_string()]
                             );
        e->setPosition(sf::Vector2f(
                                    node.attribute("x").as_float(),
                                    node.attribute("y").as_float()
                                    )
                       );
        humans.push_back(e);
    }

    ///--------------

    ///LOADING WEAPONS
    bDList.clear();
    pugi::xml_node weaponsNode = levelNode.child("weapons");
    for (pugi::xml_node node = weaponsNode.first_child(); node ; node = node.next_sibling())
        ///Entities ITERATION
    {

        std::stringstream ss2;
        ss2 <<node.attribute("file").as_string();
        std::string filename = ss2.str();
        bDList = xLoad->loadXML(directory+filename,directory+"/Weapons/");


            for (int i = 0; i < bDList.size(); i++)
            {
                if(strcmp(node.attribute("type").as_string(), "projectile") == 0)
                {
                    Projectile * p = new Projectile(&p_world, bDList[i]);
                    p->setPosition(sf::Vector2f(
                                    node.attribute("x").as_float(),
                                    node.attribute("y").as_float()
                                    )
                                    );
                    pList.push_back(p);
                }

            }
    }
    ///---------------


    ///loading pause
    //xLoad2->loadXML(std::string("pause.xml"), std::string(""));
    std::string directory2("./");
    xLoad2 = new XMLLoader(&p_world2, &Textures);
    xLoad2->loadXML(directory2 + "pause.xml", directory2);
    nameToSprite = xLoad2->getNameToSpriteList();

//    spriteRectList = xLoad2->getSpriteRectList();

   // exit(-1);

}

void World::rebuildScene()
{
    //level->clearAll();
    //level = nullptr;
    //p_world.
    ///CLEARING LISTS
    //WeaponList.clear();
    GAME_OVER = false;

    LevelObjectList.clear();
    humans.clear();
    listOfDeletedHuman.clear();
    pList.clear();
    itemList.clear();
    spriteMap.clear();
    ///--------------
    rebuild = true;
        ///BG_pause.setOrigin(BG_pause.getTexture()->getSize().x/2, BG_pause.getTexture()->getSize().y/2);
    ///BG_pause.setPosition(WINDOW_W/2, WINDOW_H/2);
//    BG_pause.setPosition(0.0f, 0.0f);
//    BG_pause.getTexture()->getSize().x/2
   // BG_pause.scale(1.2f,1.2f);
    //BG_pause.setPosition(sf::Vector2f(0.f,0.f));
    pauseLayer.setPosition(sf::Vector2f(0.f,0.f));
    statInfo.resetView();


}
/*//
void World::adaptViewToPlayer()
{

    if(ePlayer->getX() < WINDOW_W/2)
        return;


    try
    {
        b2Vec2 vel = ePlayer->getVelocity();
        //std::cout<< "vel x:"<<vel.x; //RA VO LAVA BE LE VELX DE MIPLANTE LE APP
        updateView(sf::Vector2f(vel.x/2,0));
        ///BG_pause.move(sf::Vector2f(vel.x/2,0));
        xLoad2->move(vel.x/2, 0.f);

        pauseLayer.move(sf::Vector2f(vel.x/2,0));
        ///BG.move(sf::Vector2f((vel.x)/2.5, 0));

        statInfo.adaptPosition(sf::Vector2f(vel.x/2,0));


    }
    catch(int e)
    {
        std::cout<< "ePlayer deleted";
        return;
    }

}
//*/
//*//
void World::adaptViewToPlayer()
{
    try
    {

//        //-----X
        b2Vec2 vel = ePlayer->getVelocity();
    ///ON DOIT SE REFERER A PARTIR DU VIEW PCQ LE RENDERWINDOW EST INFINI
//        if(std::abs(ePlayer->getX() - mWorldView.getCenter().x) <WINDOW_W/6)
//            vel.x = 0.f;
//        if(std::abs(ePlayer->getY() - mWorldView.getCenter().y) <WINDOW_H/6)
//            vel.y = 0.f;
//
//
//        if(ePlayer->getX() < WINDOW_W/2)
//            vel.x = 0.f;
//        if(ePlayer->getY() < WINDOW_H/3)
//            vel.y = 0.f;

        updateView(sf::Vector2f(vel.x/2,vel.y/2));
//        updateView(ePlayer->getPosition()-mWorldView.getCenter());
        xLoad2->move(vel.x/2, vel.y/2);
        BG.move(sf::Vector2f((vel.x)/2.5, (vel.y)/5));

        pauseLayer.setPosition(mWorldView.getCenter());

        statInfo.adaptPosition(sf::Vector2f(vel.x/2,vel.y/2));


    }
    catch(int e)
    {
        std::cout<< "ePlayer deleted";
        return;
    }
}
//*/

b2World& World::getWorld()
{
    return p_world;
}

sf::Vector2f World::getMousePos()
{

    return mWindow.mapPixelToCoords(sf::Mouse::getPosition(mWindow), mWindow.getView());
}
///>>>>>>>>>>>>>>>->>>>>>>>>>>>>>>>>>>>>>>>>>>>
///>>>>>>>>>CREATING THINGS>>>>>>>>>>



void World::pause()
{
    paused = true;
    ///APPLY FORCES ON EVERY BODY
    b2Body * bp = p_world2.GetBodyList();
    srand (time(NULL));
    p_world2.SetGravity(b2Vec2(1/2 * std::pow(-1 ,rand()%3),1/2 * std::pow(-1 ,rand()%5)));
    while(bp)
    {
        b2Body* tmpbp = bp;
        bp = bp->GetNext();
        tmpbp->ApplyAngularImpulse(change*2.f, true);
        change = std::pow(-1 ,rand()%3);
        tmpbp->ApplyLinearImpulse(b2Vec2(rand()%3*change, rand()%3*change), tmpbp->GetWorldCenter(), true);
    }
}

void World::resume()
{
    paused = false;
    std::cout<<"resume";
}

void World::sheduleRemove(float elapsedTime)
{

    if((int)elapsedTime%10 == 0)
        deletetime_restart = true;
    if(deletetime_restart)
    {
        std::cout<<"world::scheduleRemove ->declanchement du 10s"<<std::endl;
        for (int j = 0 ; j < humans.size() ; j++ )
        {
            if(humans[j]->isDead())
            {
                /// ///////////
                ///ATTENTION, LA SUPPRESSION DES JOINTS DOIT SE FAIRE EN DEHORS DE TOUT AUTRES CHOSES, Un ici et un autre DANS draw!!!
                /// //////////
                humans[j]->wipeJoints();
                ///Unstick all projectiles
                for (int i=0; i< pList.size(); i++)
                    {
                        if(pList[i]->stickOnEntity)
                            pList[i]->unStick();
                    }
                Item* item = new Item(&p_world,
                              "Resources/L1/lifefire.png",
                              humans[j]->getX(),
                              humans[j]->getY() - 100,
                              10.f,
                              5.f
                              );
                itemList.push_back(item);

                std::cout<<"world::scheduleRemove ->joint wiped"<<std::endl;
                delete humans[j];///SETACTIVE = FALSE
                std::cout<<"world::scheduleRemove ->human[j] deleted"<<std::endl;
                humans.erase(humans.begin()+j);
                std::cout<<"world::scheduleRemove ->human[j] erased from list"<<std::endl;
            }


        }
    }
    deletetime_restart = false;

    ///RECHARGER COMPLETEMENT TOUS LES SCENES
    if(rebuild)
    {
        for (unsigned int i = 0; i < humans.size(); i++)
        {
            delete humans[i];
            humans.erase(humans.begin()+i);
        }

        for (unsigned int i = 0; i < pList.size(); i++)
        {
            delete pList[i];
            pList.erase(pList.begin()+i);
        }

        b2Joint * j = p_world.GetJointList();
        while(j)
        {
            b2Joint* j1 = j;
            j = j->GetNext();
            p_world.DestroyJoint(j1);
        }
        ///VIDER TOUS LES BODIES
        b2Body * b = p_world.GetBodyList();
        while(b)
        {
            b2Body* b1 = b;
            b = b->GetNext();
            p_world.DestroyBody(b1);
        }

        b2Body * b2 = p_world2.GetBodyList();
        while(b2)
        {
            b2Body* b1 = b2;
            b2 = b2->GetNext();
            p_world2.DestroyBody(b1);
        }
        ///*********************
        ePlayer = nullptr;

        rebuild = false;
        m_mouseJoint = NULL;///MUST set to NULL
        b2BodyDef bodyDef;
        ///RECREER CELUI CI APRES LA DESTRUCTION
        m_groundBody = p_world.CreateBody(&bodyDef);
        buildScene(levelPath);

    }
    ///--------------------------------------


}
///MOUSE TRICKS************************************

void World::MouseDown(const b2Vec2& p)
{
    std::cout<<"start MOUSEDOWN"<<std::endl;
    m_mouseWorld = p;

    if (m_mouseJoint != NULL)
    {
        std::cout<<"MOUSEDOWN: mousejoint !null"<<std::endl;
        return;
    }
    else
    {
        // Make a small box.
        b2AABB aabb;
        b2Vec2 d;
        d.Set(0.001f, 0.001f);
        aabb.lowerBound = p - d;
        aabb.upperBound = p + d;

        // Query the world for overlapping shapes.
        QueryCallback callback(p);
        (&p_world)->QueryAABB(&callback, aabb);
        if (callback.m_fixture)
        {
            std::cout<<"Callback launching..."<<std::endl;
            b2Body* body = callback.m_fixture->GetBody();
            std::cout<<"get body...OK"<<std::endl;
            b2MouseJointDef md;
            md.bodyA = m_groundBody;
            md.bodyB = body;
            md.target = p;
            md.maxForce = 1000.0f * body->GetMass();
            std::cout<<"get mass...OK"<<std::endl;
            m_mouseJoint = (b2MouseJoint*)p_world.CreateJoint(&md);
            std::cout<<"create joint...OK"<<std::endl;
            //body->SetAwake(true);
        }
        std::cout<<"posY:"<<p.y<<std::endl;
        std::cout<<"end MOUSEDOWN"<<std::endl;
    }
}
void World::MouseUp()
{
    std::cout<<"start MOUSEUP"<<std::endl;
    if (m_mouseJoint !=  NULL)
    {
        std::cout<<"MOUSEUP destroy something.."<<std::endl;
        (&p_world)->DestroyJoint(m_mouseJoint);
        m_mouseJoint = NULL;
        std::cout<<"MOUSEUP destroyed something"<<std::endl;
    }
    std::cout<<"end MOUSEUP"<<std::endl;
}

void World::MouseMove(const b2Vec2& p)
{
    std::cout<<"start MOUSEMOVE"<<std::endl;
    m_mouseWorld = p;

    if (m_mouseJoint != NULL)
    {
        std::cout<<"setting target"<<std::endl;
        m_mouseJoint->SetTarget(p);
    }
    std::cout<<"end MOUSEMOVE"<<std::endl;
}


///*****************MOUSE TRICKS************************************

