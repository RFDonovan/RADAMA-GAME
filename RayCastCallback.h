#ifndef RAYCASTCALLBACK_H_INCLUDED
#define RAYCASTCALLBACK_H_INCLUDED
#include<Box2D/Box2D.h>
#include<typeinfo>
#include<iostream>
#include "Globals.h"
#include "GameLevel.hpp"
#include "Entity.hpp"
//#include "Player.h"

class RayCastCallback : public b2RayCastCallback
{
public:
    RayCastCallback()
    {
        m_hit = false;
        m_ground = false;
    }

    float32 ReportFixture(   b2Fixture* fixture, const b2Vec2& point, const b2Vec2& normal, float32 fraction)
    {
        b2Body* body = fixture->GetBody();

        void* userData = body->GetUserData();
        if (userData)
        {

            objectType = (static_cast<ObjectType*>(userData))->getObjectType();

            switch (objectType)
            {
            case ENTITY:
            {

                std::cout<<"EntitE vu ->";
                int eClass = (static_cast<ObjectType*>(userData))->getClassName();
                if(eClass==PLAYER)
                {

                    std::cout<<"Player vu \n";
                    void* fixtureData = fixture->GetUserData();
                    if(fixtureData)
                    {
                        std::cout<<(int)fixtureData<<"----------------*******-**-**-FIXTUREDATA \n";
                        if((int)fixtureData >  10000)
                            return fraction;  ///C EST UN ARME OU AUTRE CHOSES
                    }
                    m_hit = true;
                    m_point = point;
                    m_normal = normal;
                    m_body = body;
                }

                return fraction;
            }
            break;
            case GROUND:
                m_ground = true;
                return fraction;
                break;

            default:
                //return -1.0f;
                return fraction;
                break;
            }


        }
        std::cout<<"autres choses vu \n";
        m_ground = false;
        m_hit = false;
        m_point = point;
        m_normal = normal;
        m_body = body;

        //return -1.0f;
        return fraction;
    }
    int objectType;
    bool m_hit, m_ground;
    b2Vec2 m_point;
    b2Vec2 m_normal;
    b2Body* m_body;   //this is what I've added
};



#endif // RAYCASTCALLBACK_H_INCLUDED
