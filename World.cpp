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
}

void World::processInput(sf::Event e)
{
    /*if(sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
        paused = !paused;*/

    switch(e.type)
    {
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

        break;
    }

    if (e.type == sf::Event::LostFocus)
        pause();

    if (e.type == sf::Event::GainedFocus)
        resume();

    if(!paused) /// ******************************************************************>>>>PAUSE
        /*for (int i = 0 ;i < entities.size() ; i++ )
        {
            if(entities[i]->kind == Entity::Player)
                ((Player*)entities[i])->onCommand(e);

        }*/
        ePlayer->onCommand(e);
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

    /*
    for (int i = 0 ;i < entities.size() ; i++ )
    {
        if(entities[i]->kind == Entity::Player)
            ((Player*)entities[i])->processLogic();
        //entities[i]->processLogic(mWindow);

    }
    //*/
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
    mWindow.draw(BG);

    if(!paused)/// ******************************************************************>>>>PAUSE
    {
        p_world.Step(1/60.f,6,2);
        ePlayer->stickAll();
        p_world.ClearForces();
        mWindow.setMouseCursorVisible(false);

    }

    else
    {
        mWindow.setMouseCursorVisible(true);
    }

    mWindow.setView(mWorldView);
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
            delete humans[i];
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
        //




        /*
                //renderTexture.clear(sf::Color(0, 0, 0, 150));

                //renderTexture.draw(BG_pause);
                //renderTexture.draw(object);
                //renderTexture.display();
        */
        //mWindow.draw(sprite, &shader);
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


    createGround(p_world, 40.f, 600.f);
    //if(loadLevel("exported1.xml") == -1)
      //  return;
    GameLevel level(&p_world);
    //level.loadLevel("exported1.xml");

    createGround(p_world, 800.f, 500.f, 200.f,16.f);
    createGround(p_world, 500.f, 500.f, 200.f,16.f);

    ePlayer = new Player(mWindow,&p_world, &Textures, 1.f , (float32)150, (float32)150, BOXSIZE_W, BOXSIZE_H);
    std::cout<<"creation d'une deuxieme entite";
    Human* e = new Human(mWindow,&p_world, &Textures, 1.f , (float32)400, (float32)200, BOXSIZE_W, BOXSIZE_H-20);
    humans.push_back(e);



}

void World::adaptViewToPlayer()
{
    /**player always at 2/3 of scren*/
    /**except at the begining or at the end of the level*/

    if(ePlayer->getX() < WINDOW_W/3)
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
/*
int World::loadLevel(std::string filename)
{
    pugi::xml_document doc;
    if (!doc.load_file(filename.c_str()))
    {
        std::cout << "error on loading "<<filename<< "\n";
        return -1;
    }

    pugi::xml_node bodiesNode = doc.child("box2d").child("bodies");

    for (pugi::xml_node node = bodiesNode.first_child(); node ; node = node.next_sibling())
        ///BODIES LEVEL ITERATION
    {
        std::cout<< node.name()<<"-----------\n";
        createBody(node, node.child("fixtures"));

    }
    return 1;
}

void World::createBody(pugi::xml_node body, pugi::xml_node fixtures)
{
    std::map<std::string , int> shapeMap;
    std::map<std::string, b2BodyType> bodyType;

    bodyType["dynamic"] = b2_dynamicBody;
    bodyType["static"] = b2_staticBody;
    bodyType["kinematic"] = b2_kinematicBody;


    /// BODY DEFINITION
    b2Body * mBody;
    b2BodyDef myBodyDef;
    myBodyDef.type = bodyType[body.attribute("type").as_string()];
    std::cout<<"x:"<<(float32)body.attribute("x").as_float()/RATIO<<"y:"<<(float32) body.attribute("y").as_float()/RATIO;
    myBodyDef.position.Set((float32)body.attribute("x").as_float()/RATIO,-(float32) body.attribute("y").as_float()/RATIO);
    mBody = p_world.CreateBody(&myBodyDef);

///*
    for (pugi::xml_node nodeSon = fixtures.first_child(); nodeSon ; nodeSon = nodeSon.next_sibling() )
        ///FIXTURES LEVEL ITERATION
    {

        std::cout<< "--------"<<nodeSon.name()<<"-----------\n";
        b2EdgeShape Shape;

        b2FixtureDef FixtureDef;
        //FixtureDef.density = 0.5f;
        FixtureDef.density = 10.f;//nodeSon.attribute("density").as_float();
        //FixtureDef.friction = 1.0f;
        FixtureDef.friction = nodeSon.attribute("friction").as_float();
        //FixtureDef.restitution = 0.f;
        FixtureDef.shape = &Shape;
///*
        for(pugi::xml_node vertice = nodeSon.first_child(); vertice; vertice = vertice.next_sibling())
            ///PARCOURS DES VERTEX
        {
            Shape.Set(b2Vec2((float32)vertice.attribute("x").as_int()/RATIO,-(float32)vertice.attribute("y").as_int()/RATIO),b2Vec2((float32)vertice.next_sibling().attribute("x").as_int()/RATIO,-(float32)vertice.next_sibling().attribute("y").as_int()/RATIO));
            std::cout<<"coord"<<(float32)vertice.attribute("x").as_float()/RATIO<< ","<<vertice.attribute("y").as_int()/RATIO<<" ; "<<vertice.next_sibling().attribute("x").as_int()/RATIO<<","<<vertice.next_sibling().attribute("y").as_int()/RATIO<<"\n";
            mBody->CreateFixture(&FixtureDef);
        }
        //mBody->SetUserData(grounds[0]);

    }///
    mBody->SetUserData(grounds[0]);
}
*/


