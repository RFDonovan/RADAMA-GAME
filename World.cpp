#include "World.h"

World::World(sf::RenderWindow& window)
    : mWindow(window)
    , gravity(0.f,12.f)
//, gravity(0.f,9.8f)
    , p_world(gravity)//, true)--2.2.0
    , debugDrawInstance(window)
    , mWorldView(window.getDefaultView())
    , pauseLayer(sf::Vector2f(WINDOW_W, WINDOW_H))
    , levelPath("./Resources/L1/")

{

    m_mouseJoint = NULL;
    window.setVerticalSyncEnabled(true);
    p_world.SetDebugDraw(&debugDrawInstance);

    debugDrawInstance.SetFlags(b2Draw::e_shapeBit|b2Draw::e_jointBit);


    loadTextures();
    buildScene(levelPath);
    BG = sf::Sprite(*Textures.getTexture(TextureHolder::Background1));
    BG_pause = sf::Sprite(*Textures.getTexture(TextureHolder::Pause));
    BG.scale(.6f,.6f);
    BG_pause.setPosition(0.0f, 0.0f);
    //BG_pause.scale(.9f,.9f);

    p_world.SetContactListener(&CL_Instance);
    p_world.SetDestructionListener(&DL_Instance);


    if(!sf::Shader::isAvailable())
    {
        std::cout<< "SHADER NOT AVAILABLE!!!\n";

        exit(EXIT_FAILURE);
    }

    /**SHADER STUFF**/
    if(!distortionMap.loadFromFile("perlin_noise.jpg"/*"shade.png"*/))
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
    if(!fogShader.loadFromFile("fog_shader.fs", sf::Shader::Fragment))
    {
        sf::err() << "Failed to load shader, exiting..." << std::endl;
        return;
    }


    ///***********************************
    b2BodyDef bodyDef;
    m_groundBody = p_world.CreateBody(&bodyDef);
}

void World::processInput(sf::Event e)
{
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
    }
    break;
    case sf::Event::KeyReleased:
        if(e.key.code == sf::Keyboard::Escape)
        {
            paused = !paused;
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

    }

    if (e.type == sf::Event::LostFocus)
        pause();

    if (e.type == sf::Event::GainedFocus)
        resume();

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


    ePlayer->processLogic();
    mWorldView.move(playerPosition);
    adaptViewToPlayer();


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

    fogShader.setParameter("time",clock.getElapsedTime().asSeconds());
    //fogShader.setParameter("mouse",400.f, 300.f);
    fogShader.setParameter("resolution",800.f, 600.f);
    //BG.setColor(sf::Color(0, 0, 0, 255));
    //mWindow.draw(BG, &fogShader);

    //BG.setColor(sf::Color(0, 0, 0, 200));

    mWindow.draw(BG);


    if(!paused)/// ******************************************************************>>>>PAUSE
    {
        sticking();///create joint for sticky projectile
        p_world.Step(1/60.f,6,2);
        /// LES FONCTIONS QUI SONT EN DEHORS DU STEP : suppressions securisE des bodies ou changements des datas comme setActive

        //ePlayer->stickAll();
        ///maskAndCategoryBit(clock.getElapsedTime().asSeconds());
        sheduleRemove(clock.getElapsedTime().asSeconds());

        /// ----------------------------------------
        p_world.ClearForces();
        //mWindow.setMouseCursorVisible(false);

    }

    else
    {
        mWindow.setMouseCursorVisible(true);
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
    ePlayer->render(mWindow, frameTime, &Textures);

    for (int i = 0 ; i < humans.size() ; i++ )
    {
        humans[i]->render(mWindow, frameTime, &Textures, &shader);
        ///teste une suppression d'Entité morts: ASSEZ BIEN!!!
        if(humans[i]->getY() > mWindow.getSize().y || humans[i]->isDead())
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

    p_world.DrawDebugData();
    if(paused)
    {

        pauseLayer.setFillColor(sf::Color(0, 0, 0, 150));

        mWindow.draw(pauseLayer);
        distortionFactor = .01f;
        riseFactor = .5f;
        shader.setParameter("time", clock.getElapsedTime().asSeconds());
        shader.setParameter("distortionFactor", distortionFactor);
        shader.setParameter("riseFactor", riseFactor);
        mWindow.draw(BG_pause, &shader);

    }

    ///je teste si ce truck change la distorsion pixelshader--->oui
    //clock.restart();
    //std::cout<<"\n\t\t\t\t\t\t\t\t\t\tTIME: "<<(int)clock.getElapsedTime().asSeconds()%7<<std::endl;


}

void World::loadTextures()
{
    /**Load the animated sprites & more**/
    Textures.loadFromFile(TextureHolder::Player, "pGGbv.png");
    Textures.loadFromFile(TextureHolder::Ground, "ground.png");
    Textures.loadFromFile(TextureHolder::Ground1, "ground1.png");
    Textures.loadFromFile(TextureHolder::Ground2, "ground2.png");
    Textures.loadFromFile(TextureHolder::Background1, "background.png");
    Textures.loadFromFile(TextureHolder::Pause, "pause.png");
    Textures.loadFromFile(TextureHolder::Fire, "fire.png");
    Textures.loadFromFile(TextureHolder::Lefona, "lefona.png");

    //*/

}



void World::buildScene(std::string CurrentDir)
{
    std::cout<<"\n**********************************INITIALISATION**************************************\n";
    std::cout<<"**************************************************************************************\n";

    loadInfo("Resources/config.xml");

    sf::FloatRect r(sf::Vector2f(0.f,0.f),
                              mWorldView.getSize()
               );

    BG_pause.setPosition(sf::Vector2f(0.f,0.f));
    pauseLayer.setPosition(sf::Vector2f(0.f,0.f));
    mWorldView.reset(r);

}

void World::loadSprites(std::string listFile)
{
    pugi::xml_document XMLDoc;
    if (!XMLDoc.load_file(listFile.c_str()))
    {
        std::cout << "World::loadSprites -> error on loading "<<listFile<< "\n";
        return;
    }
    pugi::xml_node spritesNode = XMLDoc.child("RADAMA").child("spriteList");
    for (pugi::xml_node node = spritesNode.first_child(); node ; node = node.next_sibling())
        ///LOADING ALL SPRITES TO A HASHTABLE
    {
        SpriteMapping* ps_map = new SpriteMapping();
        ps_map->loadXML(node.attribute("file").as_string());
        spriteMap[node.attribute("name").as_string()] = ps_map;
        std::cout << "World::loadSprites -> loading "<<node.attribute("name").as_string() <<spriteMap[node.attribute("name").as_string()]<< ps_map<<">>>>>>>>>>>>**************\n";
    }
    //exit(-1);
}

void World::loadInfo(std::string xmlCfg)
{
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

    xLoad = new XMLLoader(&p_world);
    LevelObjectList = xLoad->loadXML(directory + "level.xml", directory);
    Ground * level = new Ground(&p_world, &LevelObjectList);

    loadSprites(directory+"spriteList.xml");
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
    SpriteMapping* ps_map = new SpriteMapping();
    ps_map->loadXML(playerNode.attribute("sprite").as_string());
        std::stringstream ss3;
        ss3 <<playerNode.attribute("file").as_string();
        std::string Pfilename = ss3.str();
    std::vector<bodyData> bDListP = xLoad->loadXML(directory + Pfilename, directory);
    std::map<std::string, b2Joint*> jMap = xLoad->GetCurrentJointMap();
    ePlayer = new Player(mWindow,&p_world, &Textures, 1.f , &bDListP, &jMap, ps_map->getAnimationList());
    ePlayer->setPosition(sf::Vector2f(
                                    playerNode.attribute("x").as_float(),
                                    playerNode.attribute("y").as_float()
                                    )
                       );
    ///---------------

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


        Human* e = new Human(mWindow,&p_world, &Textures, 1.f
                             , &bDListH
                             , &jMapH
                             , (spriteMap[node.attribute("type").as_string()])->getAnimationList()
                             );
        e->setPosition(sf::Vector2f(
                                    node.attribute("x").as_float(),
                                    node.attribute("y").as_float()
                                    )
                       );
        humans.push_back(e);
        std::cout << "World::loadInfo -> "<<spriteMap[node.attribute("type").as_string()]<<std::endl;
    }

    //exit(-1);
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
                    pList.push_back(new Projectile(&p_world, bDList[i]));
            }
    }
    ///---------------
}

void World::rebuildScene()
{
    //level->clearAll();
    //level = nullptr;
    //p_world.
    ///CLEARING LISTS
    //WeaponList.clear();
    LevelObjectList.clear();
    humans.clear();
    listOfDeletedHuman.clear();
    pList.clear();
    itemList.clear();
    ///--------------
    rebuild = true;

}

void World::adaptViewToPlayer()
{
    /**player always at 2/3 of scren*/
    /**except at the begining or at the end of the level*/

    if(ePlayer->getX() < WINDOW_W/2)
        return;


    try
    {
        b2Vec2 vel = ePlayer->getVelocity();
        //std::cout<< "vel x:"<<vel.x; //RA VO LAVA BE LE VELX DE MIPLANTE LE APP
        updateView(sf::Vector2f(vel.x/2,0));
        BG_pause.move(sf::Vector2f(vel.x/2,0));
        pauseLayer.move(sf::Vector2f(vel.x/2,0));
        BG.move(sf::Vector2f((vel.x)/2.5, 0));


    }
    catch(int e)
    {
        std::cout<< "ePlayer deleted";
        return;
    }
//*/
}

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
}

void World::resume()
{
    paused = false;
    std::cout<<"resume";
}

void World::sheduleRemove(float elapsedTime)
{
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

        b2Body * b = p_world.GetBodyList();
        while(b)
        {
            b2Body* b1 = b;
            b = b->GetNext();
            p_world.DestroyBody(b1);
        }
        ePlayer = nullptr;

        rebuild = false;
        m_mouseJoint = NULL;///MUST set to NULL
        b2BodyDef bodyDef;
        ///RECREER CELUI CI APRES LA DESTRUCTION
        m_groundBody = p_world.CreateBody(&bodyDef);
        buildScene(levelPath);

    }
    ///--------------------------------------


    if((int)elapsedTime%10 == 0)
        deletetime_restart = true;
    if(deletetime_restart)
    {
        std::cout<<"world::scheduleRemove ->declanchement du 10s"<<std::endl;
        for (int j = 0 ; j < humans.size() ; j++ )
        {
            if(humans[j]->isDead())
            {
                Item* item = new Item(&p_world,
                              "Resources/L1/lifefire.png",
                              humans[j]->getX(),
                              humans[j]->getY() - 100,
                              10.f,
                              5.f
                              );
                ///Unstick all projectiles
                for (int i=0; i< pList.size(); i++)
                    {
                        if(pList[i]->stickOnEntity)
                            pList[i]->unStick();
                    }
                itemList.push_back(item);
                std::cout<<"world::scheduleRemove ->gonna wipe joint one more time"<<std::endl;
                humans[j]->wipeJoints();
                std::cout<<"world::scheduleRemove ->joint wiped"<<std::endl;
                delete humans[j];///SETACTIVE = FALSE
                std::cout<<"world::scheduleRemove ->human[j] deleted"<<std::endl;
                humans.erase(humans.begin()+j);
                std::cout<<"world::scheduleRemove ->human[j] erased from list"<<std::endl;
            }


        }
    }
    deletetime_restart = false;
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

