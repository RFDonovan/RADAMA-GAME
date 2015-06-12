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
    myBodyDef.fixedRotation = true;
    myBodyDef.position.Set(x/RATIO, y/RATIO);

    m_body = world->CreateBody(&myBodyDef);

    m_body->SetUserData(this);
    //m_body->SetUserData(this);

    //set fixture:
    b2PolygonShape Shape;
    //Shape.SetAsBox((w/2)/RATIO, (h/2)/RATIO);
    Shape.SetAsBox((w/2)/RATIO, (h/4)/RATIO);
    b2FixtureDef FixtureDef;
    //FixtureDef.density = 0.5f;
    FixtureDef.density = 10.f;
    //FixtureDef.friction = 1.0f;
    FixtureDef.friction = 0.7f;
    FixtureDef.restitution = 0.f;
    FixtureDef.shape = &Shape;

    //attach to body:
    m_body->CreateFixture(&FixtureDef);
/// sensor

    b2Vec2 pos(0,(h/RATIO)/2);
    Shape.SetAsBox(((w-2)/8)/RATIO, (2/2)/RATIO, pos, 0);///10 PIXEL SUFFIT
    FixtureDef.isSensor = true;//pas de collision visible
    basFixture = m_body->CreateFixture(&FixtureDef);
    basFixture->SetUserData(this);
/// //////*/
    float rayon = w/2;
///bas du corps

    myBodyDef.type = b2_dynamicBody;
    myBodyDef.position.Set(x/RATIO, y/RATIO);
    m_legs = world->CreateBody(&myBodyDef);

    b2CircleShape cShape;
    cShape.m_p.Set(0,0);
    cShape.m_radius = (rayon/RATIO);
    b2FixtureDef cFixtureDef;
    cFixtureDef.isSensor = false;
    cFixtureDef.density = 10.f;
    cFixtureDef.friction = 1.f;
    cFixtureDef.restitution = 0.f;
    cFixtureDef.shape = &cShape;
    m_legs->CreateFixture(&cFixtureDef);
    b2RevoluteJointDef rJointDef;


      rJointDef.bodyA = m_body;
      rJointDef.bodyB = m_legs;
      rJointDef.collideConnected = false;
      rJointDef.localAnchorA = rJointDef.bodyA->GetLocalPoint( m_body->GetWorldPoint(b2Vec2(0, ((h/4)+rayon)/RATIO)) );
      rJointDef.localAnchorB = rJointDef.bodyB->GetLocalPoint( m_legs->GetWorldPoint(b2Vec2(0, 0)) );
      rJointDef.referenceAngle = rJointDef.bodyB->GetAngle() - rJointDef.bodyA->GetAngle();
      rJointDef.enableLimit = true;
      world->CreateJoint( &rJointDef );
/// ///////////

///tete

    myBodyDef.type = b2_dynamicBody;
    myBodyDef.position.Set(x/RATIO, y/RATIO);
    m_head = world->CreateBody(&myBodyDef);

    //cShape;
    cFixtureDef.density = 10.f;
    cShape.m_p.Set(0,0);
    cShape.m_radius = (rayon/RATIO);
    m_head->CreateFixture(&cFixtureDef);

      rJointDef.bodyA = m_body;
      rJointDef.bodyB = m_head;
      rJointDef.collideConnected = false;
      rJointDef.localAnchorA = rJointDef.bodyA->GetLocalPoint( m_body->GetWorldPoint(b2Vec2(0, -((h/4)+rayon)/RATIO)) );
      rJointDef.localAnchorB = rJointDef.bodyB->GetLocalPoint( m_head->GetWorldPoint(b2Vec2(0, 0)) );
      rJointDef.referenceAngle = rJointDef.bodyB->GetAngle() - rJointDef.bodyA->GetAngle();
      rJointDef.enableLimit = true;
      world->CreateJoint( &rJointDef );
/// ///////////


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

float Entity::getMass()
{
    return m_body->GetMass()+m_legs->GetMass()+m_head->GetMass();
}

bool Entity::isGrounded()
{
 return grounded;
}

void Entity::startContact(b2Fixture   *fixture)
{
    std::cout<< "CONTACT BEGIN";

    if(fixture == basFixture || fixture->GetBody() == m_legs)
    {
        std::cout<< "CONTACT++";
        grounded = true;
            nb_contacts++;
        std::cout<< "CONTACT++"<<nb_contacts;
        //std::cout<< "fixture ok";
    }
//nb_contacts++;

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
