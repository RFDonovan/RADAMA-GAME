#include "Projectile.hpp"

Projectile::Projectile(b2World* world, bodyData dataa)
: p_world(world)
, realData(dataa)
{
    std::cout<<"**************LOADING DATA*************"<<std::endl;
    data = &realData;
    if(data==nullptr)
    {
        return;
    }
    data->body->SetUserData((Projectile*)this);
    data->body->SetActive(true);
    std::string name = data->name;
    for (b2Fixture* f = data->body->GetFixtureList(); f; f = f->GetNext())
    {
        f->SetUserData((void*)identificationArme);
    }

    std::cout<<"**************data*************"<<std::endl;
    std::cout<<"name: "<<data->name<<std::endl;
    std::cout<<"***************************"<<std::endl;
    //data->sprite->setColor(sf::Color::Black);
}



void    Projectile::impactTo(b2Fixture* fixtureSource, b2Fixture* fixtureTarget, float impulse)
{
       if(impulse > 40)///d'habitude c'est >50
            stickProjectile(fixtureTarget);

    fired = false;

}


void Projectile::stickProjectile(b2Fixture* fixtureTarget)
{
    worldCoordsAnchorPoint = data->body->GetWorldPoint( b2Vec2(2.6f, 0.f) );
    weldJointDef.bodyA = fixtureTarget->GetBody();

    void * userDataA = fixtureTarget->GetBody()->GetUserData();
    if(userDataA && ((ObjectType*)userDataA)->getObjectType() == ENTITY)
        stickOnEntity = true;

    weldJointDef.bodyB = data->body;
    weldJointDef.localAnchorA = weldJointDef.bodyA->GetLocalPoint( worldCoordsAnchorPoint );
    weldJointDef.localAnchorB = weldJointDef.bodyB->GetLocalPoint( worldCoordsAnchorPoint );
    weldJointDef.referenceAngle = weldJointDef.bodyB->GetAngle() - weldJointDef.bodyA->GetAngle();
    ///SECOND
    worldCoordsAnchorPoint =data->body->GetWorldPoint( b2Vec2(2.f, 6.f) );
    weldJointDef1.bodyA = fixtureTarget->GetBody();

    weldJointDef1.bodyB = data->body;
    weldJointDef1.localAnchorA = weldJointDef.bodyA->GetLocalPoint( worldCoordsAnchorPoint );
    weldJointDef1.localAnchorB = weldJointDef.bodyB->GetLocalPoint( worldCoordsAnchorPoint );
    weldJointDef1.referenceAngle = weldJointDef.bodyB->GetAngle() - weldJointDef.bodyA->GetAngle();
    canStick = true;
    fired = true;
}


void Projectile::stickIt()
{
//    if(!fired)
//        return;
    /*//PUISQUE CETTE FONCTION EST APPELLE JUSTE APRES LE STEP, JE VAIS METTRE ICI

    /*if(fired)
        realData.body->SetAwake(true);
    else
        realData.body->SetAwake(false);*/
    ///*-------------------------*

   /// std::cout<<"Projectile::Sticking  ->Start"<<std::endl;
    if(!canStick)
        return;
    if(jointExist)
        return;

    if(joint != nullptr || joint2 != nullptr)
    {
        return;
    }
    joint = p_world->CreateJoint( &weldJointDef );
    joint2 = p_world->CreateJoint( &weldJointDef1 );
    joint->SetUserData((void*)(JOINTRANGE + 2));
    joint2->SetUserData((void*)(JOINTRANGE + 3));
//    joint->SetUserData(this);
//    joint2->SetUserData(this);
    canStick = false;
    jointExist = true;
    fired = false;
   /// std::cout<<"Projectile::Sticking  ->End"<<std::endl;

}

void Projectile::unStick()
{
    if(joint != nullptr)
    {
        p_world->DestroyJoint(joint);
        joint = nullptr;
        std::cout<<"Projectile::unStick ->joint supprimE"<<std::endl;
    }
    if(joint2 != nullptr)
    {
        p_world->DestroyJoint(joint2);
        joint2 = nullptr;
        std::cout<<"Projectile::unStick ->joint2 supprimE"<<std::endl;
    }
    jointExist = false;
    fired = false;
    canStick = false;

    stickOnEntity = false;
    std::cout<<"Projectile::unStick ->all joints supprimE"<<std::endl;
}

void Projectile::setVisible(bool visible)
{
    if(!visible)
        realData.sprite->setColor(sf::Color(0,0,0,50));
    else
        realData.sprite->setColor(sf::Color(255,255,255,255));
}

Projectile::~Projectile()
{
    p_world->DestroyBody(data->body);
    free(data);
}
