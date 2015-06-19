#include "World.h"

World::World(sf::RenderWindow& window)
    : mWindow(window)
    , gravity(0.f,12.f)
//, gravity(0.f,9.8f)
    , p_world(gravity, true)
    , debugDrawInstance(window)
    , mWorldView(window.getDefaultView())
    , pauseLayer(sf::Vector2f(WINDOW_W, WINDOW_H))

{

    m_mouseJoint = NULL;
    window.setVerticalSyncEnabled(true);
    p_world.SetDebugDraw(&debugDrawInstance);

    debugDrawInstance.SetFlags(b2Draw::e_shapeBit|b2Draw::e_jointBit);


    loadTextures();
    buildScene();
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

void World::draw(sf::Time frameTime)
{

    fogShader.setParameter("time",clock.getElapsedTime().asSeconds());
    //fogShader.setParameter("mouse",400.f, 300.f);
    fogShader.setParameter("resolution",800.f, 600.f);
    //BG.setColor(sf::Color(0, 0, 0, 255));
    //mWindow.draw(BG, &fogShader);

    //BG.setColor(sf::Color(0, 0, 0, 200));

    ///mWindow.draw(BG);


    if(!paused)/// ******************************************************************>>>>PAUSE
    {
        p_world.Step(1/60.f,6,2);
        /// LES FONCTIONS QUI SONT EN DEHORS DU STEP : suppressions securisE des bodies ou changements des datas comme setActive
        sheduleRemove();
        ePlayer->stickAll();
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

///RENDU DU JOUEUR
    ePlayer->render(mWindow, frameTime, &Textures);

    for (int i = 0; i < grounds.size(); i++)
        grounds[i]->render(mWindow, &shader);



    for (int i = 0 ; i < humans.size() ; i++ )
    {

        ///teste une suppression d'Entité morts: ASSEZ BIEN!!!
        if(humans[i]->getY() > mWindow.getSize().y)
        {
            std::cout<< "ito suppr";
            listOfDeletedHuman.push_back(humans[i]);
            //delete humans[i];
            humans.erase(humans.begin()+i);
            continue;

        }
        else
        {
            humans[i]->render(mWindow, frameTime, &Textures);

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



void World::buildScene()
{
    /**prepare layers*/
    /**add background*/
    /**add player*/
    //createBox(p_world, 10, 10);


    //createGround(p_world, 40.f, 600.f);
    //if(loadLevel("exported1.xml") == -1)
      //  return;
    //level = new GameLevel(&p_world);
    //level->loadLevel("Resources/level.xml");

    xLoad = new XMLLoader(&p_world);
    xLoad->loadXML("Resources/level.xml");
    //xLoad->loadEntity("Resources/rectangle.xml");


    //createGround(p_world, 800.f, 500.f, 200.f,16.f);
    //createGround(p_world, 500.f, 500.f, 200.f,16.f);

    ePlayer = new Player(mWindow,&p_world, &Textures, 1.f , (float32)150, (float32)150, BOXSIZE_W, BOXSIZE_H);
    std::cout<<"creation d'une deuxieme entite";
    Human* e = new Human(mWindow,&p_world, &Textures, 1.f , (float32)400, (float32)200, BOXSIZE_W, BOXSIZE_H);
    humans.push_back(e);

    bodyData lefona1 = xLoad->loadXML("Resources/lefona.xml");
    ePlayer->loadWeapon(&lefona1);
    bodyData lefona2 = xLoad->loadXML("Resources/lefonaMiloko.xml");
    ePlayer->loadWeapon(&lefona2);



}
void World::rebuildScene()
{
    //level->clearAll();
    //level = nullptr;
    buildScene();
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

void World::createGround(b2World& world, float X, float Y, float W, float H)
{
    //int sizeSide = 10;
    Ground* g = new Ground(&world, &Textures, X, Y, W, H);
    grounds.push_back(g);
}

void World::createBox(b2World& world, int MouseX, int MouseY)
{


    b2BodyDef BodyDef;
    BodyDef.position.Set(MouseX/RATIO, MouseY/RATIO);
    BodyDef.type = b2_dynamicBody;
    b2Body* Body1 = world.CreateBody(&BodyDef);
    //b2Body *Body1;
    //Body1 = world.CreateBody(&BodyDef);

    b2PolygonShape Shape;
    Shape.SetAsBox((32.f/2)/RATIO, (32.f/2)/RATIO);
    b2FixtureDef FixtureDef;
    FixtureDef.density = 1.f;
    //FixtureDef.friction = 1.0f;
    FixtureDef.friction = 0.7f;
    FixtureDef.restitution = .3f;
    FixtureDef.shape = &Shape;



    Body1->CreateFixture(&FixtureDef);
}

void World::createEntity(b2World& world, int MouseX, int MouseY)
{
    Human* e = new Human(mWindow,&world, &Textures, 1.f , (float32)MouseX, (float32)MouseY, BOXSIZE_W, BOXSIZE_H);
    humans.push_back(e);
}

void World::pause()
{
    paused = true;
}

void World::resume()
{
    paused = false;
    std::cout<<"resume";
}

void World::sheduleRemove()
{
    for (int i = 0 ; i < listOfDeletedHuman.size() ; i++ )
    {
        delete listOfDeletedHuman[i];
        listOfDeletedHuman.erase(listOfDeletedHuman.begin()+i);


    }
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
		b2Body* body = callback.m_fixture->GetBody();
		b2MouseJointDef md;
		md.bodyA = m_groundBody;
		md.bodyB = body;
		md.target = p;
		md.maxForce = 1000.0f * body->GetMass();
		m_mouseJoint = (b2MouseJoint*)(&p_world)->CreateJoint(&md);
		body->SetAwake(true);
	}
	std::cout<<"posY:"<<p.y<<std::endl;
	std::cout<<"end MOUSEDOWN"<<std::endl;
}
void World::MouseUp()
{
    std::cout<<"start MOUSEUP"<<std::endl;
	if (m_mouseJoint)
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

	if (m_mouseJoint)
	{
		m_mouseJoint->SetTarget(p);
	}
	std::cout<<"end MOUSEMOVE"<<std::endl;
}


///*****************MOUSE TRICKS************************************

