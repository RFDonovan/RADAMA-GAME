#include "Entity.hpp"

Entity::Entity(b2World* world, float radius, float32 x, float32 y)
{
    m_body = NULL;
    m_radius = radius;

    //set dynamic body:
    b2BodyDef myBodyDef;
    myBodyDef.type = b2_dynamicBody;
    myBodyDef.position.Set(x/RATIO, y/RATIO);
    m_body = world->CreateBody(&myBodyDef);

    //set fixture:
    b2PolygonShape Shape;
    Shape.SetAsBox((32.f/2)/RATIO, (32.f/2)/RATIO);
    b2FixtureDef FixtureDef;
    FixtureDef.density = 1.f;
    //FixtureDef.friction = 1.0f;
    FixtureDef.friction = 0.7f;
    FixtureDef.restitution = .3f;
    FixtureDef.shape = &Shape;

    //attach to body:
    m_body->CreateFixture(&FixtureDef);
}

void Entity::render()
{

}
