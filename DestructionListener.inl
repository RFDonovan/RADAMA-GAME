#include<Box2D/Box2D.h>
#include<typeinfo>
#include<iostream>
#include "Globals.h"
#include "GameLevel.hpp"
#include "Entity.hpp"

class MyDestructionListener : public b2DestructionListener
{
    void SayGoodbye(b2Joint* joint)
    {

        std::cout<<"UN JOINT VA ETRE SUPPRIMEE"<<std::endl;
        void * uData = joint->GetUserData();
        if(!uData)
            return;
//        if(uData && ((ObjectType*)uData)->getClassName() == PROJECTILE)
//            {
//
//                ((Projectile*)uData)->unStick();
//            }
            //std::cout<<"UN JOINT "<<(int)uData<<"VA ETRE SUPPRIMEE"<<std::endl;
            //*
        if((int)uData == (JOINTRANGE + 2))
        {
            std::cout<<"UN JOINT 2 VA ETRE SUPPRIMEE"<<std::endl;
            void * uDataB = joint->GetBodyB()->GetUserData();
            if(uDataB && ((ObjectType*)uDataB)->getClassName() == PROJECTILE)
            {

                ((Projectile*)uDataB)->unStick();
                //((Projectile*)uDataB)->safeUnstick = true;
            }
        }
        if((int)uData == (JOINTRANGE + 3))
        {
            std::cout<<"UN JOINT 3 VA ETRE SUPPRIMEE"<<std::endl;
            void * uDataA = joint->GetBodyB()->GetUserData();
            if(uDataA && ((ObjectType*)uDataA)->getClassName() == PROJECTILE)
            {
                std::cout<<"DestructionListener::sayGoodbye -> c est un projectile"<<std::endl;

                ((Projectile*)uDataA)->unStick();
                //((Projectile*)uDataB)->safeUnstick = true;
            }
            std::cout<<"DestructionListener::sayGoodbye -> WIPE OK"<<std::endl;
        }
        if((int)uData == (JOINTRANGE + 1))
        {
            void * uDataB = joint->GetBodyA()->GetUserData();
            ((Entity*)uDataB)->wipeJoints();
            std::cout<<"UN JOINT 1 VA ETRE SUPPRIMEE ";//<<((Player*)uDataB)->getObjectType()<<std::endl;

        }
        //*/
        joint->GetBodyB()->GetWorld()->DestroyJoint(joint);
        std::cout<<"DestructionListener::sayGoodbye -> Direct WIPE"<<std::endl;
    }
    void SayGoodbye(b2Fixture *fixture)
    {
        //fixture =  nullptr;
    }
};
