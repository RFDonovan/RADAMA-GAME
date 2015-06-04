#include "Entity.hpp"

Entity::Entity(sf::RenderWindow& mWindow, b2World* world,TextureHolder* Textures, float radius, float32 x, float32 y, float w, float h)
: p_world(world)
, mWindow(mWindow)
, desiredVel(0)
,animatedSprite(sf::seconds(0.08), true, false)
{
    kind = Entity::Player;

    m_body = NULL;
    m_radius = radius;

    //set dynamic body:
    b2BodyDef myBodyDef;
    myBodyDef.type = b2_dynamicBody;
    myBodyDef.position.Set(x/RATIO, y/RATIO);
    m_body = world->CreateBody(&myBodyDef);

    m_body->SetUserData(this);

    //set fixture:
    b2PolygonShape Shape;
    Shape.SetAsBox((w/2)/RATIO, (h/2)/RATIO);
    b2FixtureDef FixtureDef;
    //FixtureDef.density = 0.5f;
    //FixtureDef.density = 1.f;
    //FixtureDef.friction = 1.0f;
    FixtureDef.friction = 0.7f;
    //FixtureDef.restitution = .5f;
    FixtureDef.shape = &Shape;

    //attach to body:
    m_body->CreateFixture(&FixtureDef);

    b2Vec2 pos(0,(h/RATIO)/2);
    Shape.SetAsBox(((w-2)/2)/RATIO, (1/2)/RATIO, pos, 0);///1 PIXEL SUFFIT
    //FixtureDef.restitution = 1.f;
    basFixture = m_body->CreateFixture(&FixtureDef);


    loadPlayerSprite(Textures);
    currentAnimation = &walkingAnimationLeft;///POUR LE STANDBY ANIMATION
}

void Entity::loadPlayerSprite(TextureHolder* Textures)
{
    texture = Textures->getTexture(TextureHolder::Player);

    walkingAnimationLeft.setSpriteSheet(*texture);
    walkingAnimationLeft.addFrame(sf::IntRect(9, 163, 74, 149));
    walkingAnimationLeft.addFrame(sf::IntRect(107, 162, 74, 150));
    walkingAnimationLeft.addFrame(sf::IntRect(210, 163, 68, 149));
    walkingAnimationLeft.addFrame(sf::IntRect( 319, 160, 42, 152));
    walkingAnimationLeft.addFrame(sf::IntRect( 413, 160, 44, 153));
    walkingAnimationLeft.addFrame(sf::IntRect( 494, 161, 57, 150));
    walkingAnimationLeft.addFrame(sf::IntRect(578, 161, 74, 149));
    walkingAnimationLeft.addFrame(sf::IntRect(683, 161, 73, 149));
    walkingAnimationLeft.addFrame(sf::IntRect( 785, 162, 68, 147));
    walkingAnimationLeft.addFrame(sf::IntRect(891, 160, 43, 149));
    walkingAnimationLeft.addFrame(sf::IntRect(986, 161, 39, 147));
    walkingAnimationLeft.addFrame(sf::IntRect(1062, 162, 64, 146));

    //Animation walkingAnimationRight;
    walkingAnimationRight.setSpriteSheet(*texture);
    walkingAnimationRight.addFrame(sf::IntRect(10, 324, 75, 148));
    walkingAnimationRight.addFrame(sf::IntRect(100, 325, 76, 150));
    walkingAnimationRight.addFrame(sf::IntRect(196, 325, 68, 149));
    walkingAnimationRight.addFrame(sf::IntRect( 304, 323, 41, 152));
    walkingAnimationRight.addFrame(sf::IntRect( 399, 323, 44, 152));
    walkingAnimationRight.addFrame(sf::IntRect( 496, 324, 57, 150));
    walkingAnimationRight.addFrame(sf::IntRect(583, 323, 77, 150));
    walkingAnimationRight.addFrame(sf::IntRect(668, 325, 78, 147));
    walkingAnimationRight.addFrame(sf::IntRect( 764, 323, 71, 149));
    walkingAnimationRight.addFrame(sf::IntRect(877, 323, 44, 148));
    walkingAnimationRight.addFrame(sf::IntRect(976, 324, 39, 147));
    walkingAnimationRight.addFrame(sf::IntRect(1067, 323, 63, 148));

    noKeyWasPressed = true;
}

void Entity::render(sf::RenderWindow& mWindow, sf::Time frameTime, TextureHolder* Textures)
{
//*
    switch(kind){
    case Entity::Player:
        {
            /*Animation* */

            //loadPlayerSprite(Textures);

            sf::Time frameTime1 = frameClock.restart();

            sf::Vector2i screenDimensions(800,600);

            //start animation:
            animatedSprite.play(*currentAnimation);
            if (noKeyWasPressed || nb_contacts<=0)
            {
/// AJOUTER UNE ANIMATION EN FONCTION DE L'ACTUEL AU LIEU DE FAIRE UN STOP

                animatedSprite.stop();

                //desiredVel = 0;
            }

            noKeyWasPressed = true;
            animatedSprite.update(frameTime1);


            ///emplacement:
            animatedSprite.setOrigin((BOXSIZE_W),(BOXSIZE_H/2));
            animatedSprite.setPosition(m_body->GetPosition().x * RATIO,
                                        m_body->GetPosition().y * RATIO);
            animatedSprite.setRotation(m_body->GetAngle() * 180/b2_pi);
            ///Draw:
            mWindow.draw(animatedSprite);

        }

        break;
    case Entity::Other:
        {
/** TODO (dno#1#05/27/15):
Ajouter des actions pour les autres types d'entitEs autre que player */
        }
        break;
    default:
        break;

    }
//*/
}

void Entity::onCommand(sf::Event e)
{
    if(kind != Entity::Player)
        return;
    //if(nb_contacts<=0)
        //return;
    //std::cout<< "ON COMMAND RUNNING";

    if(sf::Keyboard::isKeyPressed(K_LEFT))
    {
        currentAnimation = &walkingAnimationLeft;
        desiredVel = -5.0f;
        noKeyWasPressed = false;

    }
    if (sf::Keyboard::isKeyPressed(K_RIGHT))
    {
        currentAnimation = &walkingAnimationRight;
        desiredVel = 5.0f;
        noKeyWasPressed = false;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
    {
        if(nb_contacts>0)
            jump ++;
    }

    if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
    {
        //std::cout<< "click";
        mousePos = mWindow.mapPixelToCoords(sf::Mouse::getPosition(mWindow), mWindow.getView());
        playerPos = animatedSprite.getPosition();
        desiredVel = (mousePos.x - mouseInit.x )/20.f;

        //PROPRIETE DE PYTHAGORE
        //float force = std::pow(mouseInit.x - mousePos.x, 2.0f) + std::pow(mouseInit.y - mousePos.y, 2.0f);
        //force = std::sqrt(force);
        velocityForce = sf::Vector2f();

        if (desiredVel > velocityLimit)
            desiredVel = velocityLimit;
        if (desiredVel <- velocityLimit)
            desiredVel = -velocityLimit;

        if(mousePos.x > playerPos.x)
        {
            if (desiredVel > 0)
                currentAnimation = &walkingAnimationRight;
        }

        else
            if(mousePos.x < playerPos.x)
                if(desiredVel < 0)
                    currentAnimation = &walkingAnimationLeft;


        noKeyWasPressed = false;
        //sf::Vector2i newPosition((int)(e.MouseButtonEvent.x),(int)e.MouseButtonEvent.x);
        //sf::Mouse::setPosition(newPosition, mWindow);
    }

    switch(e.type)
    {
    case sf::Event::MouseButtonPressed:
        mouseInit = mWindow.mapPixelToCoords(sf::Mouse::getPosition(mWindow), mWindow.getView());
        break;
    case sf::Event::MouseButtonReleased:
        std::cout<< "End";
        break;
    case sf::Event::KeyReleased:
        if(e.key.code == sf::Keyboard::Space)
        {
            //jump++;
        }

        break;
    }

    if(nb_contacts<=0)
    {
        //qdesiredVel = 0.f;
    }

}

void Entity::processLogic()
{
    vel = m_body->GetLinearVelocity();
    float velChange = desiredVel - vel.x;
    if (noKeyWasPressed)
        //velChange = -velChange;
        velChange = 0.f;

    float force = m_body->GetMass() * velChange / (1/60.0);// f = mv/t
    while(jump>0)
    {
        if(nb_contacts<=0)
            break;
        force = m_body->GetMass() * 5;
        if(velChange==0.0f)
            force = force*1.5;
        m_body->ApplyLinearImpulse(b2Vec2(velChange/2, -force), m_body->GetWorldCenter());
        /*force = m_body->GetMass() * (mouseInit.y - mousePos.y)/RATIO;
        //std::cout<<"force:"<< force;
        if (force > jumpLimit)
            force = jumpLimit;
        m_body->ApplyLinearImpulse(b2Vec2(velChange/2, -force), m_body->GetWorldCenter());*/
        jump--;
    }


    if(noKeyWasPressed)
    {
        if(nb_contacts > 0)
            m_body->ApplyForce(b2Vec2(0, 150), m_body->GetWorldCenter());

        return;
    }


    //desiredVel = 0;

    if(nb_contacts>0)
        m_body->ApplyForce(b2Vec2(force, 0), m_body->GetWorldCenter());
    else if(vel.y < -1.0f) //|| vel.y > 2.0f)//vel.x != 0 &&
        m_body->ApplyForce(b2Vec2(force, 0), m_body->GetWorldCenter());
//*/
}


int Entity::getY()
{
    return (int)m_body->GetPosition().y * RATIO;
}

int Entity::getX()
{
    return (int)m_body->GetPosition().x * RATIO;
}

b2Vec2 Entity::getVelocity()
{
    return m_body->GetLinearVelocity();
}

bool Entity::isGrounded()
{
 return grounded;
}

void Entity::startContact(b2Fixture   *fixture)
{
    std::cout<< "CONTACT BEGIN";

    if(fixture == basFixture)
    {
        grounded = true;
        nb_contacts++;
        //std::cout<< "fixture ok";
    }


}
void Entity::endContact(b2Fixture   *fixture)
{
    std::cout<< "CONTACT END";
    if(fixture == basFixture)
    {
        grounded = false;
        nb_contacts--;
        //std::cout<< "fixture ko";
    }


}


Entity::~Entity()
{
    p_world->DestroyBody(m_body);

    std::cout<< "fini";
}
