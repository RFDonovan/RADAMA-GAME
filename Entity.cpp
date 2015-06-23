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
    FixtureDef.friction = 0.f;
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
    m_legs->SetUserData(this);


      rJointDef.bodyA = m_body;
      rJointDef.bodyB = m_legs;
      rJointDef.collideConnected = false;
      rJointDef.localAnchorA = rJointDef.bodyA->GetLocalPoint( m_body->GetWorldPoint(b2Vec2(0, ((h/4)+rayon)/RATIO)) );
      rJointDef.localAnchorB = rJointDef.bodyB->GetLocalPoint( m_legs->GetWorldPoint(b2Vec2(0, 0)) );
      rJointDef.referenceAngle = rJointDef.bodyB->GetAngle() - rJointDef.bodyA->GetAngle();
      rJointDef.enableLimit = true;
      b2Joint * j = world->CreateJoint( &rJointDef );
      j->SetUserData((void*)(JOINTRANGE + 1));
      jointList.push_back(j);

      jointStruct js1;
      js1.joint = j;
      js1.bodyA = "m_body";
      js1.bodyB = "m_legs";

      jointBodyList.push_back(js1);
/// ///////////

///tete

    myBodyDef.type = b2_dynamicBody;
    myBodyDef.position.Set(x/RATIO, y/RATIO);
    m_head = world->CreateBody(&myBodyDef);
    m_head->SetUserData(this);

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
      b2Joint * j1 = world->CreateJoint( &rJointDef );
      j1->SetUserData((void*)(JOINTRANGE + 1));
      jointList.push_back(j1);

      jointStruct js;
      js.joint = j1;
      js.bodyA = "m_body";
      js.bodyB = "m_head";

      jointBodyList.push_back(js);


/// ///////////
    bodyList["m_body"] = m_body;
    bodyList["m_head"] = m_head;
    bodyList["m_legs"] = m_legs;

}
/*
void Entity::processLogic()
{
}
*/

void Entity::exportToXML(std::string filename)
{
    pugi::xml_document doc;
    pugi::xml_node box2d = doc.append_child("box2d");
    pugi::xml_node bodies = box2d.append_child("bodies");
    pugi::xml_node joints = box2d.append_child("joints");
    pugi::xml_node images = box2d.append_child("images");
    ///M_BODY
    for (auto b : bodyList)
    {
        addBodyNode(bodies, b.first, b.second);
    }
    for (int i = 0; i< jointBodyList.size(); i++)
    {

        addJointNode(joints, "Jointname", jointBodyList[i]);
    }


    doc.save_file(filename.c_str());



}

void Entity::addBodyNode(pugi::xml_node parent, std::string name, b2Body* body)
{
    pugi::xml_node m_bodyNode = parent.append_child("body");
    m_bodyNode.append_attribute("name") = name.c_str();
    m_bodyNode.append_attribute("x") = body->GetPosition().x*RATIO;
    m_bodyNode.append_attribute("y") = body->GetPosition().y*RATIO;
    m_bodyNode.append_attribute("type") = "dynamic";
    m_bodyNode.append_attribute("bullet") = body->IsBullet();
    m_bodyNode.append_attribute("image") = "null";
    /// /FIXTURE
    pugi::xml_node fixtures = m_bodyNode.append_child("fixtures");
    int i = 0;
    for (b2Fixture* f = body->GetFixtureList(); f; f = f->GetNext())
      {
          i++;
          pugi::xml_node fixture = fixtures.append_child("fixture");
          fixture.append_attribute("name") = i;
          fixture.append_attribute("restitution") = f->GetRestitution();
          fixture.append_attribute("density") = f->GetDensity();
          fixture.append_attribute("friction") = f->GetFriction();
          fixture.append_attribute("isSensor") = f->IsSensor();
          if(f->GetType() == b2Shape::e_circle)
          {
              b2CircleShape* circleShape = (b2CircleShape*)f->GetShape();
              fixture.append_attribute("shapeType") = "circleShape";
              fixture.append_attribute("circleRadius") = circleShape->m_radius*RATIO;
          }
          else if(f->GetType() == b2Shape::e_polygon)
                  {
                      b2PolygonShape* polyShape = (b2PolygonShape*)f->GetShape();
                      fixture.append_attribute("shapeType") = "polygonShape";
                      for(int i = 0; i< polyShape->GetVertexCount(); i++)
                      {
                          pugi::xml_node vertex = fixture.append_child("vertex");
                          vertex.append_attribute("x") = polyShape->GetVertex(i).x*RATIO;
                          vertex.append_attribute("y") = polyShape->GetVertex(i).y*RATIO;
                      }

                  }

      }
}

void Entity::addJointNode(pugi::xml_node parent, std::string name, jointStruct jStruct)
{
    std::cout<<"++++++++++++++CREATING XML JOINT\n";
    b2Joint* j = jStruct.joint;
    pugi::xml_node joint = parent.append_child("joint");
    joint.append_attribute("name") = name.c_str();
    std::cout<<"++++++++++++++ADDING BODY A\n";
    joint.append_attribute("bodyA") = jStruct.bodyA.c_str();
    std::cout<<"++++++++++++++ADDING BODY B\n";
    joint.append_attribute("bodyB") = jStruct.bodyB.c_str();
    joint.append_attribute("collideConnected") = j->GetCollideConnected();
    std::cout<<"++++++++++++++COMPARE\n";
    if (j->GetType() == b2JointType::e_weldJoint)
    {
        joint.append_attribute("type") = "WeldJoint";
    }
    else if(j->GetType() == b2JointType::e_revoluteJoint)
            {
                b2RevoluteJoint* rj = (b2RevoluteJoint*) j;
                joint.append_attribute("type") = "RevoluteJoint";
                joint.append_attribute("enableLimit") = rj->IsLimitEnabled();
                joint.append_attribute("enableMotor") = rj->IsMotorEnabled();

            }
}
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

    if(fixture == basFixture)
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

void Entity::wipeJoints()
{
    for (int i = 0; i< jointList.size(); i++)
    {
        p_world->DestroyJoint(jointList[i]);
        jointList.erase(jointList.begin()+i);
    }
}

Entity::~Entity()
{
    wipeJoints();
    p_world->DestroyBody(m_body);
    p_world->DestroyBody(m_legs);
    p_world->DestroyBody(m_head);

    std::cout<< "fini";
}
