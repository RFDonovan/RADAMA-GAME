#include "Entity.hpp"

Entity::Entity(sf::RenderWindow& mWindow, b2World* world,TextureHolder* Textures, float radius, float32 x, float32 y, float w, float h)
: p_world(world)
, mWindow(mWindow)
, textureHolder(Textures)
,animatedSprite(sf::seconds(0.08), true, false)
{
    m_body = NULL;
    m_radius = radius;
    //set dynamic body:
    b2BodyDef myBodyDef;
    myBodyDef.type = b2_dynamicBody;
    myBodyDef.position.Set(x/RATIO, y/RATIO);
    m_body = world->CreateBody(&myBodyDef);

    m_body->SetUserData(this);
    //m_body->SetUserData(this);

    //set fixture:
    b2PolygonShape Shape;
    Shape.SetAsBox((w/2)/RATIO, (h/2)/RATIO);
    b2FixtureDef FixtureDef;
    //FixtureDef.density = 0.5f;
    //FixtureDef.density = 1.f;
    //FixtureDef.friction = 1.0f;
    FixtureDef.friction = 0.7f;
    //FixtureDef.restitution = .5f;
    FixtureDef.shape = &Shape;

    //attach to body:
    m_body->CreateFixture(&FixtureDef);

    b2Vec2 pos(0,(h/RATIO)/2);
    Shape.SetAsBox(((w-2)/2)/RATIO, (10/2)/RATIO, pos, 0);///1 PIXEL SUFFIT
    //FixtureDef.restitution = 1.f;
    FixtureDef.isSensor = true;
    basFixture = m_body->CreateFixture(&FixtureDef);
    basFixture->SetUserData(this);
    //currentAnimation = &walkingAnimationLeft;///POUR LE STANDBY ANIMATION
    //Entity::EntityLists.push_back(this);
//EntityLists.push_back(this);
}
/*
void Entity::processLogic()
{
}
*/
int Entity::getY()
{
    return (int)m_body->GetPosition().y * RATIO;
}

int Entity::getX()
{
    return (int)m_body->GetPosition().x * RATIO;
}

b2Vec2 Entity::getVelocity()
{
    return m_body->GetLinearVelocity();
}

bool Entity::isGrounded()
{
 return grounded;
}

void Entity::startContact(b2Fixture   *fixture)
{
    std::cout<< "CONTACT BEGIN";

    if(fixture == basFixture)
    {
        std::cout<< "CONTACT++";
        grounded = true;
            nb_contacts++;
        std::cout<< "CONTACT++"<<nb_contacts;
        //std::cout<< "fixture ok";
    }


}
void Entity::endContact(b2Fixture   *fixture)
{
    std::cout<< "CONTACT END";
    if(fixture == basFixture)
    {
        grounded = false;
        nb_contacts--;
        std::cout<< "CONTACT--"<<nb_contacts;
        //std::cout<< "fixture ko";
    }


}


Entity::~Entity()
{
    p_world->DestroyBody(m_body);

    std::cout<< "fini";
}
