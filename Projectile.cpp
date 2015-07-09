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
}



void    Projectile::impactTo(b2Fixture* fixtureSource, b2Fixture* fixtureTarget, float impulse)
{
       if(impulse > 40)///d'habitude c'est >50
            stickProjectile(fixtureTarget);

}


void Projectile::stickProjectile(b2Fixture* fixtureTarget)
{
    worldCoordsAnchorPoint = data->body->GetWorldPoint( b2Vec2(2.6f, 0.f) );
    weldJointDef.bodyA = fixtureTarget->GetBody();

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
}


void Projectile::stickIt()
{
    if(!canStick)
        return;
    if(jointExist)
        return;

    if(joint != nullptr)
    {
        return;
    }
    joint = p_world->CreateJoint( &weldJointDef );
    joint2 = p_world->CreateJoint( &weldJointDef1 );
    joint->SetUserData((void*)(JOINTRANGE + 2));
    joint2->SetUserData((void*)(JOINTRANGE + 3));
    canStick = false;
    jointExist = true;
}

void Projectile::unStick()
{
    if(joint != nullptr)
    {
        p_world->DestroyJoint(joint);
        joint = nullptr;
    }
    if(joint2 != nullptr)
    {
        p_world->DestroyJoint(joint2);
        joint2 = nullptr;
    }
    jointExist = false;
}

Projectile::~Projectile()
{
    p_world->DestroyBody(data->body);
    free(data);
}
