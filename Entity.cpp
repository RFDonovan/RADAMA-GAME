#include "Entity.hpp"

Entity::Entity(b2World* world,TextureHolder* Textures, float radius, float32 x, float32 y)
: p_world(world)
, desiredVel(0)
{
    kind = Entity::Player;

    m_body = NULL;
    m_radius = radius;

    //set dynamic body:
    b2BodyDef myBodyDef;
    myBodyDef.type = b2_dynamicBody;
    myBodyDef.position.Set(x/RATIO, y/RATIO);
    m_body = world->CreateBody(&myBodyDef);

    //set fixture:
    b2PolygonShape Shape;
    Shape.SetAsBox((32.f/2)/RATIO, (32.f/2)/RATIO);
    b2FixtureDef FixtureDef;
    FixtureDef.density = 1.f;
    //FixtureDef.friction = 1.0f;
    FixtureDef.friction = 0.7f;
    FixtureDef.restitution = .3f;
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
}

void Entity::render(sf::RenderWindow& mWindow, sf::Time frameTime, TextureHolder* Textures)
{
//*
    switch(kind){
    case Entity::Player:
        {
            /*Animation* */

            //loadPlayerSprite(Textures);

            ///currentAnimation = &walkingAnimationLeft;///POUR LE STANDBY ANIMATION
            /// set up AnimatedSprite
            sf::Vector2i screenDimensions(800,600);
            AnimatedSprite animatedSprite(sf::seconds(0.2), true, false);

            animatedSprite.setPosition(sf::Vector2f(screenDimensions / 2));

            //start animation:
            animatedSprite.play(*currentAnimation);
            if (noKeyWasPressed)
            {

                animatedSprite.stop();
                m_body->ApplyForceToCenter(b2Vec2(0.0f,0.f));
            }
            noKeyWasPressed = true;
            animatedSprite.update(frameTime);
            ///emplacement:
            animatedSprite.setOrigin(16,16);
            animatedSprite.setPosition(m_body->GetPosition().x * RATIO,
                                        m_body->GetPosition().y * RATIO);
            animatedSprite.setRotation(m_body->GetAngle() * 180/b2_pi);
            ///Draw:
            mWindow.draw(animatedSprite);

        }

        break;
    case Entity::Other:
        {

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
    //std::cout<< "ON COMMAND RUNNING";

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
    {
        currentAnimation = &walkingAnimationLeft;
        desiredVel = -5.0f;
        noKeyWasPressed = false;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
    {
        currentAnimation = &walkingAnimationRight;
        desiredVel = 5.0f;
        noKeyWasPressed = false;
    }

}

void Entity::processLogic()
{
    b2Vec2 vel = m_body->GetLinearVelocity();
    //desiredVel = 0;
    float velChange = desiredVel - vel.x;
    float force = m_body->GetMass() * velChange / (1/60.0);// f = mv/t
    m_body->ApplyForce(b2Vec2(force, 0), m_body->GetWorldCenter());

}

int Entity::getY()
{
    return (int)m_body->GetPosition().y * RATIO;
}

Entity::~Entity()
{
    p_world->DestroyBody(m_body);

    std::cout<< "fini";
}
