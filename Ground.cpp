#include "Ground.hpp"

Ground::Ground(b2World* world, TextureHolder* Textures, float32 x, float32 y, float w, float h)
: p_world(world)
, groundPosition(x, y)
, groundSize(w, h)
{
    kind = Type::Default;

    b2BodyDef BodyDef;
    BodyDef.position.Set(x/RATIO+(w/2/RATIO), y/RATIO);
    BodyDef.type = b2_staticBody;
    m_body = world->CreateBody(&BodyDef);

    b2PolygonShape Shape;
    Shape.SetAsBox((w/2)/RATIO, (h/2)/RATIO);

    b2FixtureDef FixtureDef;
    FixtureDef.density = 1.f;

    FixtureDef.shape = &Shape;
    m_body->CreateFixture(&FixtureDef);


    m_body->SetUserData(this);
    loadSprites(Textures);
}

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

void Ground::loadSprites(TextureHolder* Textures)
{
    switch (kind)
    {
    case Type::Default:
        Texture = Textures->getTexture(TextureHolder::Ground);
        Tex1 = Textures->getTexture(TextureHolder::Ground1);
        Tex2 = Textures->getTexture(TextureHolder::Ground2);
        break;

    default:
        Texture = Textures->getTexture(TextureHolder::Ground);
        Tex1 = Textures->getTexture(TextureHolder::Ground1);
        Tex2 = Textures->getTexture(TextureHolder::Ground2);
        break;
    }
    //Texture->setSmooth(true);
    //Texture->setRepeated(true);

    //groundSprite.setTextureRect(new sf::Rect(groundPosition,groundSize));
    groundSprite.setTexture(*Texture);
    gs1.setTexture(*Tex1);
    gs2.setTexture(*Tex2);

    sf::IntRect i(0,0,(int)groundSize.x, Texture->getSize().y);
    groundSprite.setTextureRect(i);
    Texture->setRepeated(true);

}

void Ground::render(sf::RenderWindow& mWindow, sf::Shader* shader)
{
    ///groundSprite.setScale(groundSize.x/Texture->getSize().x,1);
    //groundSprite.setOrigin(Texture->getSize().x/2, Texture->getSize().y/1.5f);
    groundSprite.setOrigin(groundSize.x/2, Texture->getSize().y/1.3);///LE 1.3 EST UN BRICOLAGE A MAIN POUR AJUSTER LA TEXTURE DU SOL
    groundSprite.setPosition(m_body->GetPosition().x * RATIO,
                             m_body->GetPosition().y * RATIO);
    ///PUISQU IL N Y A PAS DE SETORIGIN, ON ADAPTE LA POSITION COMME LE SETORIGIN PRECEDENT:
    gs1.setPosition(groundSprite.getPosition().x - groundSize.x/2 - Tex1->getSize().x, groundSprite.getPosition().y - Tex1->getSize().y/1.35);// - Texture->getSize().y/1.3);
    gs2.setPosition(groundSprite.getPosition().x + groundSize.x/2, groundSprite.getPosition().y - Tex1->getSize().y/1.35);


    distortionFactor = .01f;
    riseFactor = .5f;
    shader->setParameter("time", clock.getElapsedTime().asSeconds());
    shader->setParameter("distortionFactor", distortionFactor);
    shader->setParameter("riseFactor", riseFactor);

    mWindow.draw(groundSprite, shader);
    {
        distortionFactor = .1f;
        riseFactor = .5f;
        shader->setParameter("time", clock.getElapsedTime().asSeconds());
        shader->setParameter("distortionFactor", distortionFactor);
        shader->setParameter("riseFactor", riseFactor);
    }
    mWindow.draw(gs1, shader);
    mWindow.draw(gs2, shader);

    ///POUR UN TEXTURE D EAU, ON BOOSTE LA DISTORTIONFACTOR A .5F ET ON REMPLACE GS1 ET GS2 PAR AUTRE CHOSE (UN RECIPIENT PAR EXEMPLE)
    ///PCQ CA MARCHE PAS BIEN SI LA TEXTURE EST PETITE
}

int     Ground::getY()
{
    return (int)m_body->GetPosition().y * RATIO;
}
int     Ground::getX()
{
    return (int)m_body->GetPosition().x * RATIO;
}

int     Ground::getW()
{
    return (int)groundSize.x;
}
int     Ground::getH()
{
    return (int)groundSize.y;
}

void    Ground::startContact(b2Fixture   *fixture)
{

}
void    Ground::endContact(b2Fixture   *fixture)
{

}

Ground::~Ground()
{

}
