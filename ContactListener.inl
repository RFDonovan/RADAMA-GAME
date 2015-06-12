#include<Box2D/Box2D.h>
#include<typeinfo>
#include "Globals.h"
#include "GameLevel.hpp"
#include "Entity.hpp"


static std::vector<Entity*>    EntityLists;

class ContactListener : public b2ContactListener
{
    void BeginContact(b2Contact* contact)
    {
        std::cout<<"contact exist\n";

        void * userData = contact->GetFixtureA()->GetBody()->GetUserData();
        int objectType;

        if(userData)
        {
std::cout<<"userdata exist\n";

//            if(contact->GetFixtureA()->GetBody()->GetType() == b2_staticBody)
//            {
//                (static_cast<ObjectType*>(userData))->getObjectType();
//                std::cout <<"got ya!!!";
//            }
            //int objectType = ((ObjectType*)userData)->getObjectType();
            //int objectType = ((ObjectType*)userData)->getObjectType();


            objectType = (static_cast<ObjectType*>(userData))->getObjectType();
            std::cout<<"after conversion\n";
            switch (objectType)
            {
            case PLAYER:
                //std::cout<<"mety";
            {
                std::cout<<"entityA\n";
                static_cast<Entity*>( userData )->startContact(contact->GetFixtureA());
            }
            break;
            case GROUND:
                {
                    int classname = (static_cast<ObjectType*>(userData))->getClassName();
                    userData = contact->GetFixtureB()->GetBody()->GetUserData();
                    if(classname == HUMAN)
                    {

                    }
                    if(classname == PLAYER)
                    {
                        userData = contact->GetFixtureB()->GetBody()->GetUserData();
                        static_cast<Entity*>( userData )->startContact(contact->GetFixtureB());
                    }
                    std::cout<<"----->GROUND A\n";
                }
                break;
            default:
                std::cout<<"latsaka ato am DEFAULT A\n";
                std::cout<<"ObjectType = "<< objectType<<"\n" ;
                break;
            }
        }

        userData = contact->GetFixtureB()->GetBody()->GetUserData();
        if(userData)
        {

            //objectType = ((ObjectType*)userData)->getObjectType();
            objectType = static_cast<ObjectType*>(userData)->getObjectType();
            switch (objectType)
            {
            case ENTITY:
                //std::cout<<"mety";
            {
                std::cout<<"entityB\n";
                (static_cast<Entity*>( userData ))->startContact(contact->GetFixtureB());
            }
            case GROUND:
                {
                    int classname = (static_cast<ObjectType*>(userData))->getClassName();
                    userData = contact->GetFixtureA()->GetBody()->GetUserData();
                    if(classname == HUMAN)
                    {

                    }
                    if(classname == PLAYER)
                    {
                        userData = contact->GetFixtureA()->GetBody()->GetUserData();
                        static_cast<Entity*>( userData )->startContact(contact->GetFixtureA());
                    }
                    std::cout<<"----->GROUND B\n";
                }
            break;
            default:
                std::cout<<"latsaka ato am DEFAULT B\n";
                break;
            }
        }
//*/
    }

    void EndContact(b2Contact* contact)
    {
        int objectType;
        void * userData = contact->GetFixtureA()->GetBody()->GetUserData();
        if(userData)
        {


            //int objectType = ((ObjectType*)userData)->getObjectType();
            objectType = static_cast<ObjectType*>(userData)->getObjectType();
            switch (objectType)
            {
            case ENTITY:
                //std::cout<<"mety";
            {
                static_cast<Entity*>( userData )->endContact(contact->GetFixtureA());
            }
            break;
            default:
                break;
            }

        }
        userData = contact->GetFixtureB()->GetBody()->GetUserData();
        if(userData)
        {


            objectType = ((ObjectType*)userData)->getObjectType();
            //objectType = static_cast<ObjectType*>(userData)->getObjectType();
            switch (objectType)
            {
            case ENTITY:
                //std::cout<<"mety";
            {
                static_cast<Entity*>( userData )->endContact(contact->GetFixtureB());
            }
            break;
            default:
                break;
            }
        }
        //*/
    }


    void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse)
    {
        b2Fixture* fixtureA = contact->GetFixtureA();
        b2Fixture* fixtureB = contact->GetFixtureB();

        //fixture with user data is a target, other fixture is arrow or apple
        void * userDataA = contact->GetFixtureA()->GetBody()->GetUserData();
        void * userDataB = contact->GetFixtureB()->GetBody()->GetUserData();


        if(userDataA && ((ObjectType*)userDataA)->getClassName() == PLAYER)//&&fixture->GetBody()->GetLinearVelocity().x
        {
            //b2Body* arrowBody = fixtureA->GetBody();
            if(fixtureA->GetFriction() == 0.735f)
                ((Player*)userDataA)->stickProjectile(fixtureB);
            //std::cout<<"arrowbody trouve";
        }
        if(userDataB && ((ObjectType*)userDataB)->getClassName() == PLAYER)
        {
            //b2Body* arrowBody = fixtureB->GetBody();
            if(fixtureB->GetFriction() == 0.735f)
                ((Player*)userDataB)->stickProjectile(fixtureA);
            //std::cout<<"arrowbody trouve";
        }


    }
};
