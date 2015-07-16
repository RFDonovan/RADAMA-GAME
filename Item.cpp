#include "Item.hpp"

Item::Item(b2World* world, std::string filename, float x, float y, int mana, int life)
: p_world(world)
{
///set as sensor

    m_body = NULL;
    //set dynamic body:
    b2BodyDef myBodyDef;
    myBodyDef.type = b2_staticBody;
    myBodyDef.fixedRotation = true;
    myBodyDef.position.Set(x/RATIO, y/RATIO);

    m_body = world->CreateBody(&myBodyDef);
    m_body->SetUserData(this);
    //m_body->SetUserData(this);

    //set fixture:
    b2PolygonShape Shape;
    //Shape.SetAsBox((w/2)/RATIO, (h/2)/RATIO);
    Shape.SetAsBox((50/4)/RATIO, (50/2)/RATIO);
    b2FixtureDef FixtureDef;
    //FixtureDef.density = 0.5f;
    FixtureDef.density = 10.f;
    //FixtureDef.friction = 1.0f;
    FixtureDef.friction = 1.f;
    FixtureDef.restitution = 0.f;
    FixtureDef.isSensor = true;
    //FixtureDef.filter.categoryBits = 0x0001;
    //FixtureDef.filter.maskBits = 0x0001;
    //m_body->SetActive(false);
    FixtureDef.shape = &Shape;

    //attach to body:
    b2Fixture* f = m_body->CreateFixture(&FixtureDef);
    f->SetUserData((void*)identificationItem);
    /// ////////////////
    itemTexture.loadFromFile(filename);
    itemSprite.setTexture(itemTexture);
    itemSprite.setOrigin(itemTexture.getSize().x/2, itemTexture.getSize().y/2);
    itemSprite.scale(.2f,.2f);
    if(mana>0)
        itemSprite.setColor(sf::Color(46, 189, 243, 255));

    if(life>0)
        itemSprite.setColor(sf::Color(139, 245, 44, 255));
    if(life>0&&mana>0)
        itemSprite.setColor(sf::Color(255, 255, 255, 255));

}

void Item::render(sf::RenderWindow& mWindow, sf::Shader* shader)
{

    itemSprite.setPosition(m_body->GetPosition().x * RATIO,
                             m_body->GetPosition().y * RATIO);

    shader->setParameter("time", clock.getElapsedTime().asSeconds());
    shader->setParameter("distortionFactor", .2f);
    shader->setParameter("riseFactor", .5f);

    mWindow.draw(itemSprite, shader);

}

void Item::collideWith(b2Fixture* fixture)
{
    //setPosition
}
