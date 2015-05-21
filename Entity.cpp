#include "Entity.hpp"

Entity::Entity(b2World* world, float radius, float32 x, float32 y)
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
}

void Entity::render(sf::RenderWindow& mWindow, TextureHolder* Textures)
{
//*
    //Type c = Entity::Player;
    switch(kind){
    case Entity::Player:
        {
            //free(texture);
            texture = Textures->getTexture(TextureHolder::Player);
            Animation walkingAnimationLeft;
            walkingAnimationLeft.setSpriteSheet(*texture);
            walkingAnimationLeft.addFrame(sf::IntRect(32, 32, 32, 32));
            walkingAnimationLeft.addFrame(sf::IntRect(64, 32, 32, 32));
            walkingAnimationLeft.addFrame(sf::IntRect(32, 32, 32, 32));
            walkingAnimationLeft.addFrame(sf::IntRect( 0, 32, 32, 32));

            Animation walkingAnimationRight;
            walkingAnimationRight.setSpriteSheet(*texture);
            walkingAnimationRight.addFrame(sf::IntRect(32, 64, 32, 32));
            walkingAnimationRight.addFrame(sf::IntRect(64, 64, 32, 32));
            walkingAnimationRight.addFrame(sf::IntRect(32, 64, 32, 32));
            walkingAnimationRight.addFrame(sf::IntRect( 0, 64, 32, 32));

            Animation* currentAnimation = &walkingAnimationLeft;

            /// set up AnimatedSprite
            sf::Vector2i screenDimensions(800,600);
            AnimatedSprite animatedSprite(sf::seconds(0.2), true, false);

            animatedSprite.setPosition(sf::Vector2f(screenDimensions / 2));

            //start animation:
            animatedSprite.play(*currentAnimation);
            if (noKeyWasPressed)
            {
                animatedSprite.stop();
                //Body1->ApplyForceToCenter(b2Vec2(0.0f,0.f));
            }
            noKeyWasPressed = true;

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

void onCommand(sf::Event e)
{

}
