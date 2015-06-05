#include "Player.h"

Player::Player(sf::RenderWindow& mWindow, b2World* world,TextureHolder* Textures, float radius, float32 x, float32 y, float w, float h)
: Entity(mWindow,world, Textures, radius, x, y, w, h)
, desiredVel(0)
{
    kind = Entity::Player;
    std::cout<< "creation*******";

    loadPlayerSprite(Textures);
    currentAnimation = &walkingAnimationLeft;///POUR LE STANDBY ANIMATION
}

void Player::loadPlayerSprite(TextureHolder* Textures)
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

void Player::render(sf::RenderWindow& mWindow, sf::Time frameTime, TextureHolder* Textures)
{

            //std::cout<< "rendu*******";
            /**GOD S HAND**/
/*
            sf::Vector2f tmousePos = mWindow.mapPixelToCoords(sf::Mouse::getPosition(mWindow), mWindow.getView());
            Shader->setParameter("time", clock.getElapsedTime().asSeconds());
            Shader->setParameter("distortionFactor", distortionFactor);
            Shader->setParameter("riseFactor", riseFactor);
            stuffSprite.setOrigin((stuffSprite.getTexture()->getSize().x/2), (stuffSprite.getTexture()->getSize().x/2));
            stuffSprite.setPosition(tmousePos.x-(stuffSprite.getTexture()->getSize().x*stuffSprite.getScale().x/2), tmousePos.y-(stuffSprite.getTexture()->getSize().y*stuffSprite.getScale().y/2));

            std::random_device rd;
            std::mt19937 mt(rd());
            std::uniform_real_distribution<double> dist(1, 10);
            stuffSprite.setRotation(stuffSprite.getRotation()+dist(mt));
            mWindow.draw(stuffSprite,Shader);
            /***************/
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
            //std::cout<< "\n******rendu ok*******";

//*/
}

void Player::onCommand(sf::Event e)
{
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

void Player::processLogic()
{
    if(kind == Entity::Player)
    {

    }
    vel = m_body->GetLinearVelocity();
    float velChange = desiredVel - vel.x;
    if (noKeyWasPressed)
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
        jump--;
    }


    if(noKeyWasPressed)
    {
        if(nb_contacts > 0)
            m_body->ApplyForce(b2Vec2(0, 150), m_body->GetWorldCenter());

        return;
    }


    if(nb_contacts>0)
        m_body->ApplyForce(b2Vec2(force, 0), m_body->GetWorldCenter());
    else if(vel.y < -1.0f) //|| vel.y > 2.0f)//vel.x != 0 &&
        m_body->ApplyForce(b2Vec2(force, 0), m_body->GetWorldCenter());

}



