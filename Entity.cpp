#include "Entity.hpp"
/*
void Entity::processLogic()
{
}
*/

Entity::Entity(sf::RenderWindow& mWindow, b2World* world, TextureHolder* Textures, float radius, std::vector<bodyData> *bDList, std::map<std::string, b2Joint*> *jMap)
: p_world(world)
, mWindow(mWindow)
, textureHolder(Textures)
, animatedSprite(sf::seconds(0.08), true, false)
,fixtureOnSensor(nullptr)
{

    visionTex = new sf::Texture();
    visionTex->loadFromFile("Resources/vision.png");
    visionSprite.setTexture(*visionTex);
    visionSprite.setOrigin(0,visionTex->getSize().y/2);
    visionSprite.setColor(sf::Color(229,255,7,100));

    lifeTex = new sf::Texture();
    deathTex = new sf::Texture();
    lifeTex->loadFromFile("life.png");
    lifeSprite.setTexture(*lifeTex);
    deathTex->loadFromFile("death.png");
    deathSprite.setTexture(*deathTex);
    deathSprite.setColor(sf::Color(0, 0, 0, 150));

    if (!MyFont.loadFromFile("Resources/CHIZZ___.ttf"))
    {
        // Error...
    }

    Text.setFont(MyFont);
    Text.setCharacterSize(20);
    Text.setString("Andrana");
    //Text.setFont(sf::Font::GetDefaultFont());
    Text.setColor(sf::Color::Cyan);

//    m_body->SetUserData(this);
//    m_legs->SetUserData(this);
//    m_head->SetUserData(this);
//    basFixture->SetUserData(this);

    for (int i = 0; i< bDList->size(); i++)
    {
        std::cout<<i<<"----bdlist\n";
        std::string bodyName("m_body");
        std::string headName("m_head");
        std::string legsName("m_legs");
        std::string sensorName("m_sensor");



        if(bodyName.compare((*bDList)[i].name)==0)
        {
            std::cout<<i<<"----body\n";
            m_body = (*bDList)[i].body;
            basFixture = nullptr;
            std::map<std::string, b2Fixture*> fixMap = (*bDList)[i].mapFixture;
            //std::cout<<test.size()<<"---SIZE -fixture list\n";
            std::string basFixtureName("basFixture");
            for (auto f : fixMap)
            {
                if(basFixtureName.compare(f.first)==0)
                {
                    std::cout<<fixMap[f.first]<<"---->basFixture trouve\n";
                    //(fixMap[f.first])->SetUserData(this);
                    //f.second->SetUserData((void*)1);
                    basFixture = (b2Fixture*)fixMap[f.first];
                    basFixture->SetUserData(this);
                }
            }
        }else if(headName.compare((*bDList)[i].name)==0)
        {
            std::cout<<i<<"----head\n";
            m_head = (*bDList)[i].body;
        }else if(legsName.compare((*bDList)[i].name)==0)
        {
            std::cout<<i<<"----legs\n";
            m_legs = (*bDList)[i].body;
        }else if(sensorName.compare((*bDList)[i].name)==0)
        {
            std::cout<<i<<"----sensor\n";
            m_sensor = (*bDList)[i].body;
        }

        (*bDList)[i].body->SetUserData(this);
    }

    for (auto j : *jMap)
    {
        std::cout<<"----jMap\n";
        j.second->SetUserData((void*)(JOINTRANGE + 1));
        jointList.push_back(j.second);
    }


    std::cout<<"\n\n\n\n------------------------------------------------------"<<std::endl;
    //basFixture->SetUserData(this);
    std::cout<<"\n\n\n\n------------------------DEUXIEME CONSTRUCTEUR------------------------------"<<std::endl;

    std::cout<<"\n\n\n\n-----bDList-"<<bDList->size()<<std::endl;
    std::cout<<"\n\n\n\n-----jMap-"<<jMap->size()<<std::endl;

    std::cout<<"\n\n\n\n------------------------------------------------------"<<std::endl;

    say("HOOLY SHIIIIT!!");
}

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
        std::stringstream ss;
        ss<<"JointName"<<i;


        addJointNode(joints, ss.str(), &jointBodyList[i]);
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
    m_bodyNode.append_attribute("fixedRotation") = body->IsFixedRotation();
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
          fixture.append_attribute("categoryBits") = f->GetFilterData().categoryBits;
          fixture.append_attribute("maskBits") = f->GetFilterData().maskBits;
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

void Entity::addJointNode(pugi::xml_node parent, std::string name, jointStruct* jStruct)
{
    std::cout<<"++++++++++++++CREATING XML JOINT\n";
    b2Joint* j = jStruct->joint;
    pugi::xml_node joint = parent.append_child("joint");
    joint.append_attribute("name") = name.c_str();
    std::cout<<"++++++++++++++ADDING BODY A\n";
    joint.append_attribute("bodyA") = jStruct->bodyA.c_str();
    std::cout<<"++++++++++++++ADDING BODY B\n";
    joint.append_attribute("bodyB") = jStruct->bodyB.c_str();
    joint.append_attribute("collideConnected") = j->GetCollideConnected();
    joint.append_attribute("Ax") = "";
    joint.append_attribute("Ay") = "";
    joint.append_attribute("Bx") = "";
    joint.append_attribute("By") = "";
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


void Entity::setPosition(sf::Vector2f position)
{
    m_body->SetTransform(b2Vec2(position.x/RATIO,position.y/RATIO),m_body->GetAngle());
//    m_head->SetTransform(b2Vec2(position.x/RATIO,position.y/RATIO),m_head->GetAngle());
//    m_legs->SetTransform(b2Vec2(position.x/RATIO,position.y/RATIO),m_legs->GetAngle());
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

void Entity::startContact(b2Fixture   *fixture, b2Fixture   *fixtureB)
{
    std::cout<< "CONTACT BEGIN";
    if(fixture->GetBody() == m_sensor)
    {
        std::cout<< "-------CONTACT SENSOR BEGIN\n";
        fixtureOnSensor = fixtureB;
    }
    if((int)fixtureB->GetUserData()>20000 && (int)fixtureB->GetUserData()<30000)
    {
        Projectile* p = (Projectile*)(fixtureB->GetBody()->GetUserData());
        isWeaponDispo = p->isDispo();
        std::cout<< "CONTACT BEGIN NAME: "<</*p->getBodyData()->name*/(int)fixtureB->GetUserData()<<std::endl;
        if(isWeaponDispo)
            weaponDispo = p;
        //std::cout<< "CONTACT WITH PROJECTILE!!!!!!!!!!!!!!!!!!!!!!!!!!!\n";
    }

    if(fixture == basFixture)
    {
        /*if((int)fixtureB->GetUserData()>20000)
        {
            nb_contacts = 1;
            return;
        }*/
        std::cout<< "CONTACT++";
        grounded = true;
        if(!fixtureB->IsSensor())
            nb_contacts++;
        std::cout<< "CONTACT++"<<nb_contacts;
        //std::cout<< "fixture ok";
    }
//nb_contacts++;

}
void Entity::endContact(b2Fixture   *fixture, b2Fixture   *fixtureB)
{

    std::cout<< "CONTACT END";
    if(fixture->GetBody() == m_sensor)
    {
        std::cout<< "-------CONTACT SENSOR END\n";
        fixtureOnSensor=nullptr;
    }
    if((int)fixtureB->GetUserData()>20000 && (int)fixtureB->GetUserData()<30000)
    {
        isWeaponDispo = false;
        std::cout<< "Entity::endContact-> END CONTACT WITH PROJECTILE!!!!!!!!!!!!!!!!!!!!!!!!!!!\n";
    }
    if(fixture == basFixture)
    {
        /*if((int)fixtureB->GetUserData()>20000)
        {
            nb_contacts = 0;
            return;
        }*/
        grounded = false;
        if(!fixtureB->IsSensor())
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
    std::cout<<"Entity::wipeJoints ->jointList vidE"<<std::endl;
}

void Entity::resetFSM()
{
    fsm_normal = false;
    fsm_shocked = false;
    fsm_alert = false;
    ///normalement hunting et attack doit etre combinE sinon le player peut disparaitre des yeux de l'ennemie pendant qu'il attaque
    fsm_hunting = false;
    fsm_attack = false;
    fsmClock.restart();
    stateClock.restart();
    desiredVel = 0.f;
}

void Entity::sense()
{
    if(fsm_normal)
        doNormalThings();
    if(fsm_shocked)
        doShockedThings();
    if(fsm_alert)
        doAlertThings();
    if(fsm_attack)
        doAttackThings();
    if(fsm_hunting)
        doHuntingThings();

    commitLogic();

    /*
    float rayRange = 300.f/RATIO;

    int changeDirection = -1;
    if(currentAnimation == &walkingAnimationLeft || currentAnimation == &stopLeft)
            changeDirection = -changeDirection;

    //jumpOnObstacle();
    float secureDistance = 200.f;
    if(fsm_hunting)
    {
        int o = 1;
        if(targetBody->GetPosition().x - m_body->GetPosition().x <0.f)
            o=-1;
        if(std::abs(targetBody->GetPosition().x - m_body->GetPosition().x) > secureDistance/RATIO)
        {
            goTo(b2Vec2(targetBody->GetPosition().x+(secureDistance*o),targetBody->GetPosition().y));
            commitLogic();
        }

        return;
    }

//    RayCastCallback callback;

    p_world->RayCast(&callback,m_body->GetPosition(), b2Vec2(m_body->GetPosition().x-(rayRange*changeDirection),m_body->GetPosition().y));
    if(callback.m_hit)
    {
        std::cout<< "//////////HIT HIT HIT\n";
        fsm_hunting = true;
        targetBody = callback.m_body;
        say("!!",2);
    }
*/
}

void Entity::jumpOnObstacle()
{
    float rayRange = 10.f;
    int changeDirection = -1;

    if(currentAnimation == &walkingAnimationLeft || currentAnimation == &stopLeft)
            changeDirection = -changeDirection;

    RayCastCallback callback;
    p_world->RayCast(&callback,
                     m_body->GetPosition(),
                     b2Vec2(m_body->GetPosition().x-(rayRange*changeDirection),
                            m_body->GetPosition().y+rayRange
                            )
                     );
    if(callback.m_ground)///TERRE DIRECTEMENT
    {
            //std::cout<< "/////////////////////////////////////Terre devant\n";
    }else///VIDE OU QUELQUES CHOSES D AUTRE
    {
        if(std::abs(m_body->GetLinearVelocity().x)>1.f)
            if(nb_contacts>0)
                        jump ++;
                    else
                        jump --;
        //std::cout<< "/////////////////////////////////////Vide devant\n";
    }
}

void Entity::goTo(b2Vec2 newPos)
{
//find path to a position

    if(std::abs(m_body->GetPosition().x-newPos.x)<=10.f/RATIO)
    {
        return;
    }
    else
    {
        if(m_body->GetPosition().x-newPos.x>0.f)
            desiredVel = -velocityLimit;
        else
            desiredVel = velocityLimit;
    }

//    if(m_body->GetPosition().x-newPos.x > 10.f/RATIO)
//        desiredVel = -velocityLimit;
//    else if(m_body->GetPosition().x-newPos.x < -10.f/RATIO)
//        desiredVel = velocityLimit;
//    else
//        desiredVel = .0f;
//
    jumpOnObstacle();

}


void Entity::doTheDead()
{
    //if(m_life>0)///vider la vie actuel
        m_life = 0;

    m_body->SetFixedRotation(false);
    m_legs->SetFixedRotation(false);
    m_head->SetFixedRotation(false);
    m_sensor->SetFixedRotation(false);
    b2Filter filter;
    for (auto b : bodyList)
    {
        for (b2Fixture* f = (b.second)->GetFixtureList(); f; f = f->GetNext())
        {
            filter = f->GetFilterData();
            //filter.categoryBits = 0x0001;
            filter.maskBits = 0x0002;
            f->SetFilterData(filter);
            f->SetFriction(1.0);
        }
    }

    /*for (b2Fixture* f = m_body->GetFixtureList(); f; f = f->GetNext())
    {
        filter = f->GetFilterData();
        filter.categoryBits = 0;
        filter.maskBits = 0;
        f->SetFilterData(filter);
    }
    for (b2Fixture* f = m_head->GetFixtureList(); f; f = f->GetNext())
    {
        f->SetFilterData(filter);
    }
    for (b2Fixture* f = m_head->GetFixtureList(); f; f = f->GetNext())
    {
        f->SetFilterData(filter);
    }*/
    std::cout<<"Entity::doTheDead -> OK"<<std::endl;
}

void Entity::commitLogic()
{
    if(isDead())
        return;

    vel = m_body->GetLinearVelocity();
    float velChange = desiredVel - vel.x;
//    if (!fsm_hunting)
//        velChange = 0.f;

    float force = getMass() * velChange / (1/60.0);// f = mv/t
    if(jump>0)
    {
        force = getMass() * 4;
        if(velChange==0.0f)
            force = force;//*1.5;
        m_body->ApplyLinearImpulse(b2Vec2(velChange/2, -force), m_body->GetWorldCenter(), true);
        jump--;
    }




    if(nb_contacts>0)
        m_body->ApplyForce(b2Vec2(force, 0), m_body->GetWorldCenter(), true);
    else if(vel.y < -1.0f) //|| vel.y > 2.0f)//vel.x != 0 &&
        m_body->ApplyForce(b2Vec2(force, 0), m_body->GetWorldCenter(), true);

}

void Entity::getHit(int damage, float impulse)
{
    float degat = 10;
    if(impulse >= 40)
        degat = (float)damage;

    if(degat<defense)
        degat = 0;
    else
        degat = degat-defense;

    //std::cout<<"TOUCHEEEEEEEEEEEEEEEEEEEEEE avec impulse = "<<impulse<<std::endl;
    if(m_life>0)
        m_life = m_life - degat;
    if(m_life<0)
        m_life = 0;

    //std::cout<<"m_life = "<<m_life<<std::endl;
}

void Entity::applyForce(float f)
{
    m_body->ApplyLinearImpulse(b2Vec2(-f*10, -f*10), m_body->GetWorldCenter(), true);
    //m_body->ApplyLinearImpulse(b2Vec2(-f*100, 0.f), m_body->GetWorldCenter(), true);

}

bool Entity::isDead()
{
    if(m_life <= 0)
    {
        say("Aaaaaaaaah!!!!!...", 5);
        return true;
    }

    else
        return false;
}

void Entity::takeItem(Item* item)
{
    std::cout<< "+++++++++++++++++++++Item taken \n";
    if(item->dejaPris)
        return;
    m_life = m_life + item->bonusLife;
    item->dejaPris = true;
    //m_mana = m_mana + item->bonusMana;
    if(m_life>maxLife)
        m_life = maxLife;

}


void Entity::drawLife(sf::RenderWindow& mWindow)
{
    lifeSprite.setColor(sf::Color(255-(m_life*255/100), 255-180+(m_life*180/100), 100, 255));

    if(m_life<30)
    {
        if(lifeClock.getElapsedTime().asMilliseconds()<=50)
            deathSprite.setColor(sf::Color(100, 0, 0, 150));
        if(lifeClock.getElapsedTime().asMilliseconds()>=100)
            deathSprite.setColor(sf::Color(0, 0, 0, 150));
        if(lifeClock.getElapsedTime().asMilliseconds()>=200)
            lifeClock.restart();
    }
    else
        deathSprite.setColor(sf::Color(0, 0, 0, 150));

    deathSprite.setPosition(animatedSprite.getPosition().x - deathTex->getSize().x/2,
                     animatedSprite.getPosition().y - 100);
    mWindow.draw(deathSprite);

    lifeSprite.setPosition(animatedSprite.getPosition().x - lifeTex->getSize().x/2,
                     animatedSprite.getPosition().y - 100+1);

    lifeSprite.setScale(m_life/(float)maxLife,1.f);

    mWindow.draw(lifeSprite);
    speak(mWindow);
}

void Entity::drawVision(sf::RenderWindow& mWindow)
{
    if(isDead())
        return;
    if(currentAnimation == &walkingAnimationLeft||currentAnimation == &stopLeft)
    {
        if(visionSprite.getScale().x> -1.f)
            if((int)visionClock.getElapsedTime().asMilliseconds()%20<20)
                visionSprite.setScale(visionSprite.getScale().x-0.2f,1.f);

        //visionSprite.setScale(-1.f,1.f);
    }

    if(currentAnimation == &walkingAnimationRight||currentAnimation == &stopRight)
    {
        if(visionSprite.getScale().x< 1.f)
            if((int)visionClock.getElapsedTime().asMilliseconds()%20<20)
                visionSprite.setScale(visionSprite.getScale().x+0.2f,1.f);
    }
    if((int)visionClock.getElapsedTime().asSeconds()%4<1)
        if((int)visionClock.getElapsedTime().asMilliseconds()%1000<500)
        {
            visionSprite.setRotation(visionSprite.getRotation()+.02f);
            visionSprite.setScale(visionSprite.getScale().x,visionSprite.getScale().y+.02f);
        }
        else
        {
            visionSprite.setScale(visionSprite.getScale().x,visionSprite.getScale().y-.02f);
            visionSprite.setRotation(visionSprite.getRotation()-.02f);
        }



    visionSprite.setPosition(m_head->GetPosition().x * RATIO,
                             m_head->GetPosition().y * RATIO);
    mWindow.draw(visionSprite);
}

void    Entity::speak(sf::RenderWindow& mWindow)
{


    //Text.setRotation(90.f);
    //Text.setScale(2.f, 2.f);
    Text.setPosition(animatedSprite.getPosition().x - lifeTex->getSize().x/2,
                     animatedSprite.getPosition().y - 150+1);
    //Text.move(100.f, 200.f);
    if(haveToSpeak)
    {

        if(textClock.getElapsedTime().asSeconds()<textDelay)///faire disparaitre apres 10s
        {

            Text.setColor(sf::Color(255,255,255,55));
            mWindow.draw(Text);
            Text.setPosition(animatedSprite.getPosition().x - lifeTex->getSize().x/2-2,
                     animatedSprite.getPosition().y - 150-1);
            Text.setColor(sf::Color(0,0,0,255));
            mWindow.draw(Text);
        }

        else
        {
            haveToSpeak=false;
            textClock.restart();
        }

    }


}

void    Entity::say(std::string text, int delay)
{
    textDelay = delay;
    Text.setString(text);
    haveToSpeak = true;
    textClock.restart();
}

Entity::~Entity()
{
    std::cout<<"Entity::~Entity -> DELETING ENTITY"<<std::endl;
    wipeJoints();
    std::cout<<"Entity::~Entity -> DELETING body"<<std::endl;
    p_world->DestroyBody(m_body);
    std::cout<<"Entity::~Entity -> DELETING Legs"<<std::endl;
    p_world->DestroyBody(m_legs);
    std::cout<<"Entity::~Entity -> DELETING head"<<std::endl;
    p_world->DestroyBody(m_head);
    p_world->DestroyBody(m_sensor);
//
//    m_body->SetActive(false);
//    m_legs->SetActive(false);
//    m_head->SetActive(false);
    std::cout<<"Entity::~Entity -> DELETING ENTITY DONE"<<std::endl;
    deleted = true;
}


