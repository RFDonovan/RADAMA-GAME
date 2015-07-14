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


            objectType = (static_cast<ObjectType*>(userData))->getObjectType();
            std::cout<<"after conversion\n";
            switch (objectType)
            {
            case ENTITY:
                //std::cout<<"mety";
            {
                std::cout<<"entityA\n";
                static_cast<Entity*>( userData )->startContact(contact->GetFixtureA(),contact->GetFixtureB());
            }
            case ITEM:
                //std::cout<<"mety";
            {
                std::cout<<"ItemA\n";
                static_cast<Item*>( userData )->collideWith(contact->GetFixtureB());

                void * userDataB = contact->GetFixtureB()->GetBody()->GetUserData();
                if(userDataB && ((ObjectType*)userDataB)->getObjectType() == ENTITY)
                {
                    (static_cast<Entity*>( userDataB ))->takeItem(static_cast<Item*>( userData ));///GIVE ITEM TO THE ENTITY
                }

            }
            break;
            case GROUND:
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
                (static_cast<Entity*>( userData ))->startContact(contact->GetFixtureB(),contact->GetFixtureA());
            }
            case ITEM:
                //std::cout<<"mety";
            {
                std::cout<<"ItemB\n";
                static_cast<Item*>( userData )->collideWith(contact->GetFixtureA());

                void * userDataB = contact->GetFixtureA()->GetBody()->GetUserData();
                if(userDataB && ((ObjectType*)userDataB)->getObjectType() == ENTITY)
                {
                    (static_cast<Entity*>( userDataB ))->takeItem(static_cast<Item*>( userData ));///GIVE ITEM TO THE ENTITY
                }
            }

            case GROUND:
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
                static_cast<Entity*>( userData )->endContact(contact->GetFixtureA(),contact->GetFixtureB());
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
                static_cast<Entity*>( userData )->endContact(contact->GetFixtureB(),contact->GetFixtureA());
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


        if(userDataA && ((ObjectType*)userDataA)->getClassName() == PROJECTILE)//&&fixture->GetBody()->GetLinearVelocity().x
        {
            //b2Body* arrowBody = fixtureA->GetBody();
            if(!((Projectile*)userDataA)->isFired())
                return;
            ((Projectile*)userDataA)->impactTo(fixtureA, fixtureB,(float)impulse->normalImpulses[0]);

            if(userDataB && ((ObjectType*)userDataB)->getObjectType() == ENTITY)///si le touchE est une entitE
            {
                if(userDataB && ((ObjectType*)userDataB)->getClassName() != PLAYER)///POUR EVITER LES COLLISIONS ENTRE ARME ET JOUEUR
                    ((Entity*)userDataB)->getHit(((Projectile*)userDataB)->damage,(float)impulse->normalImpulses[0]);
            }


            std::cout<<"////////////Contactlistener::postsolve fixtureA "<<std::endl;
            //std::cout<<"IMPULSE:"<<impulse->normalImpulses[0];
        }
        if(userDataB && ((ObjectType*)userDataB)->getClassName() == PROJECTILE)
        {
            if(!((Projectile*)userDataB)->isFired())
                return;
            //b2Body* arrowBody = fixtureB->GetBody();0
            ((Projectile*)userDataB)->impactTo(fixtureB, fixtureA,(float)impulse->normalImpulses[0]);

            if(userDataA && ((ObjectType*)userDataA)->getObjectType() == ENTITY)
            {
                if(userDataA && ((ObjectType*)userDataA)->getClassName() != PLAYER)///POUR EVITER LES COLLISIONS ENTRE ARME ET JOUEUR
                    ((Entity*)userDataA)->getHit(((Projectile*)userDataB)->damage,(float)impulse->normalImpulses[0]);

            }
            std::cout<<"////////////Contactlistener::postsolve fixtureB "<<std::endl;
            //std::cout<<"arrowbody trouve";
        }


    }
};
