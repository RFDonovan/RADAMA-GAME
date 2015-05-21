#include "World.h"

World::World(sf::RenderWindow& window)
: mWindow(window)
, gravity(0.f,9.8f)
, p_world(gravity, true)
, debugDrawInstance(window)
, mWorldView(window.getDefaultView())
//, mPlayerPosition(mWorldView.getSize().x/2.f, )
{
    window.setVerticalSyncEnabled(true);
    p_world.SetDebugDraw(&debugDrawInstance);

    debugDrawInstance.SetFlags(b2Draw::e_shapeBit);

    loadTextures();
    buildScene();

    //mWorldView.setCenter(mPlayerPosition);

}

void World::processInput(sf::Event e)
{
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        {
            std::cout<<mWorldView.getCenter().x;
            std::cout<<mWorldView.getCenter().y;
            updateView(sf::Vector2f(-2, 0));
        }
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        {
            std::cout<<mWorldView.getCenter().x;
            std::cout<<mWorldView.getCenter().y;
            updateView(sf::Vector2f(2, 0));
        }
}

void World::updateView(sf::Vector2f view)
{
    mWorldView.move(view);
}

void World::update()
{
    //*
    sf::Vector2f playerPosition(0.0f,0.0f);
    mWorldView.move(playerPosition);
    adaptViewToPlayer();
//*/
    ///depiler commandes
    /*while(!mCommandQueue.isEmpty())
        player.onCommand(mCommandQueue.pop(), dt);
    */

}

void World::draw()
{

    p_world.Step(1/60.f,6,2);
    p_world.ClearForces();
    p_world.DrawDebugData();

    mWindow.setView(mWorldView);

    for (int i = 0 ;i < entities.size() ; i++ )
        entities[i]->render(mWindow, &Textures);
    /*mWindow.draw(sprites);*/
    /*mWincow.draw(player)*/

    /*
    for (b2Body* BodyIterator = p_world.GetBodyList();
    BodyIterator; BodyIterator = BodyIterator->GetNext() )
    {
        if(BodyIterator->GetType() == b2_staticBody)
        {
            GroundSprite.setTexture(t_ground);
            GroundSprite.setScale(.5f,0.5f);
            GroundSprite.setOrigin(400.f, 8.f);

            GroundSprite.setPosition(BodyIterator->GetPosition().x * RATIO,
                                     BodyIterator->GetPosition().y * RATIO);

            GroundSprite.setRotation(BodyIterator->GetAngle() * 180/b2_pi);
            mWindow.draw(GroundSprite);
        }
        else
        {

        }

    }*/


}

void World::loadTextures()
{
    /**Load the animated sprites & more**/
    //mTextures.load(Textues::Radama, "/path/to/tex")
    //t_ground.loadFromFile("ground.png");
    //*
    Textures.loadFromFile(TextureHolder::Player, "player.png");
    Textures.loadFromFile(TextureHolder::Ground1, "ground.png");
    //*/

}



void World::buildScene()
{
    /**prepare layers*/
    /**add background*/
    /**add player*/
    //createBox(p_world, 10, 10);
    createGround(p_world, 400.f, 500.f);

}

void World::adaptViewToPlayer()
{
    /**player always at 2/3 of scren*/
    /**except at the begining or at the end of the level*/
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

void World::createGround(b2World& world, float X, float Y)
{
    b2BodyDef BodyDef;
    BodyDef.position.Set(X/RATIO, Y/RATIO);
    BodyDef.type = b2_staticBody;
    b2Body* Body = world.CreateBody(&BodyDef);

    b2PolygonShape Shape;
    Shape.SetAsBox((800.f/2)/RATIO, (16.f/2)/RATIO);

    b2FixtureDef FixtureDef;
    FixtureDef.density = 1.f;

    FixtureDef.shape = &Shape;
    Body->CreateFixture(&FixtureDef);
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
    Entity* e = new Entity(&world, 1.f , (float32)MouseX, (float32)MouseY);
    entities.push_back(e);
}
