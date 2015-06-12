#include "GameLevel.hpp"

GameLevel::GameLevel(b2World* world)
: p_world(world)
{
    /*kind = Type::Default;
    std::cout<< "OBJTYPE=" << getObjectType();

    b2Body * mBody;
    b2BodyDef BodyDef;
    BodyDef.position.Set(5/RATIO,500/RATIO);
    BodyDef.type = b2_staticBody;
    mBody = world->CreateBody(&BodyDef);

    b2PolygonShape Shape;
    Shape.SetAsBox(600/RATIO, 2/RATIO);

    b2FixtureDef FixtureDef;
    FixtureDef.density = 1.f;

    FixtureDef.shape = &Shape;
    mBody->CreateFixture(&FixtureDef);


    mBody->SetUserData(this);
    mBody->GetUserData();*/
}
///*
int GameLevel::loadLevel(std::string filename)
{
    pugi::xml_document XMLDocument;
    if (!XMLDocument.load_file(filename.c_str()))
    {
        std::cout << "error on loading "<<filename<< "\n";
        return -1;
    }

    pugi::xml_node bodiesNode = XMLDocument.child("box2d").child("bodies");

    for (pugi::xml_node node = bodiesNode.first_child(); node ; node = node.next_sibling())
        ///BODIES LEVEL ITERATION
    {
        std::cout<< node.name()<<"-----------\n";
        createBody(node, node.child("fixtures"));

    }
    return 1;
}

void GameLevel::createBody(pugi::xml_node body, pugi::xml_node fixtures)
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
    mBody = p_world->CreateBody(&myBodyDef);
    //mBody->SetUserData(this);
    b2EdgeShape Shape;
    b2FixtureDef FixtureDef;

    /*FixtureDef.shape = &Shape;
    Shape.Set(b2Vec2(-50, -5),b2Vec2(50,-5));
    b2Fixture * fix = mBody->CreateFixture(&FixtureDef);
    fix->SetUserData(this);
    mBody->SetUserData(this);*/

//*
    for (pugi::xml_node nodeSon = fixtures.first_child(); nodeSon ; nodeSon = nodeSon.next_sibling() )
        ///FIXTURES LEVEL ITERATION
    {

        std::cout<< "--------"<<nodeSon.name()<<"-----------\n";
        //b2EdgeShape Shape;
        //b2Shape Shape;
        std::string shapeType(nodeSon.attribute("shapeType").as_string());
        //if(shapeType.compare("edgeShape") == 0)



        FixtureDef.density = nodeSon.attribute("density").as_float();
        //FixtureDef.friction = 1.0f;
        FixtureDef.friction = nodeSon.attribute("friction").as_float();
        //FixtureDef.restitution = 0.f;
        FixtureDef.shape = &Shape;
///*
        for(pugi::xml_node vertice = nodeSon.first_child(); vertice; vertice = vertice.next_sibling())
            ///PARCOURS DES VERTEX
        {
            Shape.Set(
                        b2Vec2((float32)vertice.attribute("x").as_int()/RATIO,
                               -(float32)vertice.attribute("y").as_int()/RATIO)
                      ,b2Vec2((float32)vertice.next_sibling().attribute("x").as_int()/RATIO,
                              -(float32)vertice.next_sibling().attribute("y").as_int()/RATIO));
            /*Shape.m_vertex0.Set((vertice.attribute("x").as_int()/RATIO)-1, -(float32)vertice.attribute("y").as_int()/RATIO);
            Shape.m_vertex0.Set((vertice.next_sibling().attribute("x").as_int()/RATIO ) -1, -(float32)vertice.next_sibling().attribute("y").as_int()/RATIO);
            Shape.m_hasVertex0 = true;
            Shape.m_hasVertex3 = true;*/

            std::cout<<"coord"<<(float32)vertice.attribute("x").as_float()/RATIO<< ","<<vertice.attribute("y").as_int()/RATIO<<" ; "<<vertice.next_sibling().attribute("x").as_int()/RATIO<<","<<vertice.next_sibling().attribute("y").as_int()/RATIO<<"\n";
            FixtureDef.shape = &Shape;
            b2Fixture* fixture = mBody->CreateFixture(&FixtureDef);

            fixture->SetUserData(this);
        }
        //mBody->SetUserData(this);

    }///
    mBody->SetUserData(this);
}
//*/
void GameLevel::clearAll()
{

}

void    GameLevel::render(sf::RenderWindow& mWindow)
{

}
void    GameLevel::render(sf::RenderWindow& mWindow, sf::Shader* shader)
{

}

void    GameLevel::startContact(b2Fixture   *fixture)
{

}
void    GameLevel::endContact(b2Fixture   *fixture)
{

}

GameLevel::~GameLevel()
{

}
