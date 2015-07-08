#include "Projectile.hpp"

Projectile::Projectile(bodyData* data)
: data(data)
{
    std::cout<<"**************LOADING DATA*************"<<std::endl;
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
