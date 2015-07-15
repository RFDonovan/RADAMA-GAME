#include "Human.hpp"

Human::Human(sf::RenderWindow& mWindow, b2World* world, TextureHolder* Textures, float radius, std::vector<bodyData> *bDList, std::map<std::string, b2Joint*> *jMap)
    : Entity(mWindow, world, Textures,radius, bDList, jMap)
    //, desiredVel(0)
{
    maxLife = 80;
    kind = Entity::Human;
    std::cout<< "creation*******";

    loadSprite(Textures);
    currentAnimation = &stopLeft;///POUR LE STANDBY ANIMATION


    ///lifeSprite.setOrigin(lifeTex->getSize().x/2, lifeTex->getSize().y/2);
    ///deathSprite.setOrigin(deathTex->getSize().x/2, deathTex->getSize().y/2);

}

void Human::loadSprite(TextureHolder* Textures)
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

    stopRight.setSpriteSheet(*texture);
    stopRight.addFrame(sf::IntRect(877, 323, 44, 148));
    stopLeft.setSpriteSheet(*texture);
    stopLeft.addFrame(sf::IntRect(891, 160, 43, 149));

}

void Human::render(sf::RenderWindow& mWindow, sf::Time frameTime, TextureHolder* Textures)
{

            sf::Time frameTime1 = frameClock.restart();

            sf::Vector2i screenDimensions(800,600);

            //start animation:
            if(std::abs(getVelocity().x)>1)///SI IL BOUGE PLUS QUE NECESSAIRE---------POUR EVITER LE TREMBLEMENT DES SPRITES
            if(nb_contacts>0)
                if (getVelocity().x>0)
                    currentAnimation = &walkingAnimationRight;
                else if (getVelocity().x<0)
                    currentAnimation = &walkingAnimationLeft;
                else
                {
                    if(currentAnimation == &walkingAnimationLeft)
                        currentAnimation = &stopLeft;
                    if(currentAnimation == &walkingAnimationRight)
                        currentAnimation = &stopRight;
                }
            else///QUAND IL SAUTE
                {
                    if(currentAnimation == &walkingAnimationLeft)
                        currentAnimation = &stopLeft;
                    if(currentAnimation == &walkingAnimationRight)
                        currentAnimation = &stopRight;
                }
            else///QUAND IL NE BOUGE PLUS OU BOUGE PETIT
                {
                    if(currentAnimation == &walkingAnimationLeft)
                        currentAnimation = &stopLeft;
                    if(currentAnimation == &walkingAnimationRight)
                        currentAnimation = &stopRight;
                }

            if(!isDead())
                animatedSprite.play(*currentAnimation);

            animatedSprite.update(frameTime1);


            ///emplacement:
            animatedSprite.setOrigin((BOXSIZE_W),(BOXSIZE_H/2));
            animatedSprite.setPosition(m_body->GetPosition().x * RATIO,
                                        m_body->GetPosition().y * RATIO);
            animatedSprite.setRotation(m_body->GetAngle() * 180/b2_pi);
            ///Draw:
            drawLife(mWindow);
            mWindow.draw(animatedSprite);

//*/
}


