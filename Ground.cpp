#include "Ground.hpp"

Ground::Ground(b2World* world,std::vector<bodyData> *bDList)
{
    for (int i = 0; i< bDList->size(); i++)
    {
        (*bDList)[i].body->SetUserData(this);

        for (b2Fixture* f = (*bDList)[i].body->GetFixtureList(); f; f = f->GetNext())
        {
            f->SetUserData((void*)identificationGround + i);
        }
    }
}

void    Ground::startContact(b2Fixture   *fixture)
{
    //KTSAK
}
void    Ground::endContact(b2Fixture   *fixture)
{

}

Ground::~Ground()
{

}
