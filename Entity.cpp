#include "Entity.hpp"

Entity::Entity(b2World* world,TextureHolder* Textures, float radius, float32 x, float32 y, float w, float h)
: p_world(world)
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
    //FixtureDef.restitution = .3f;
    FixtureDef.shape = &Shape;

    //attach to body:
    m_body->CreateFixture(&FixtureDef);


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
/*
void Entity::loadPlayerSprite(TextureHolder* Textures)
{
    texture = Textures->getTexture(TextureHolder::Player);

    walkingAnimationLeft.setSpriteSheet(*texture);
    walkingAnimationLeft.addFrame(sf::IntRect(32, 32, 32, 32));
    walkingAnimationLeft.addFrame(sf::IntRect(64, 32, 32, 32));
    walkingAnimationLeft.addFrame(sf::IntRect(32, 32, 32, 32));
    walkingAnimationLeft.addFrame(sf::IntRect( 0, 32, 32, 32));

    //Animation walkingAnimationRight;
    walkingAnimationRight.setSpriteSheet(*texture);
    walkingAnimationRight.addFrame(sf::IntRect(32, 64, 32, 32));
    walkingAnimationRight.addFrame(sf::IntRect(64, 64, 32, 32));
    walkingAnimationRight.addFrame(sf::IntRect(32, 64, 32, 32));
    walkingAnimationRight.addFrame(sf::IntRect( 0, 64, 32, 32));
    noKeyWasPressed = true;
}
*/
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
            if (noKeyWasPressed)
            {

                animatedSprite.stop();
//                m_body->ApplyForceToCenter(b2Vec2(0.0f,0.f));
//                if(desiredVel>0)
//                    desiredVel = desiredVel - 0.2f;
//                if(desiredVel<0)
//                    desiredVel = desiredVel + 0.2f;

                desiredVel = 0;
            }
            noKeyWasPressed = true;
            animatedSprite.update(frameTime1);


            ///emplacement:
            animatedSprite.setOrigin((BOXSIZE_W/2),(BOXSIZE_H/2));
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
    if(!grounded)
        return;
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
        jump ++;
    }

    if(!grounded)
    {
        desiredVel = 0.f;
    }
}

void Entity::processLogic()
{
    vel = m_body->GetLinearVelocity();
    float velChange = desiredVel - vel.x;

    while(jump>0)
    {
        if(!grounded)
            break;
        m_body->ApplyLinearImpulse(b2Vec2(velChange, -5), m_body->GetWorldCenter());
        jump--;
    }


    if(noKeyWasPressed)
    {
        if(grounded)
            m_body->ApplyForce(b2Vec2(0, 150), m_body->GetWorldCenter());

        return;
    }


    //desiredVel = 0;

    if (noKeyWasPressed)
        velChange = -velChange;
    float force = m_body->GetMass() * velChange / (1/60.0);// f = mv/t
    m_body->ApplyForce(b2Vec2(force, 0), m_body->GetWorldCenter());
//*/
}

int Entity::getY()
{
    return (int)m_body->GetPosition().y * RATIO;
}

bool Entity::isGrounded()
{
 return grounded;
}

void Entity::startContact()
{
    std::cout<< "CONTACT BEGIN";
    grounded = true;
}
void Entity::endContact()
{
    std::cout<< "CONTACT END";
    grounded = false;
}


Entity::~Entity()
{
    p_world->DestroyBody(m_body);

    std::cout<< "fini";
}
