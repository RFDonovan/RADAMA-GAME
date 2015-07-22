#include "Human.hpp"

Human::Human(sf::RenderWindow& mWindow, b2World* world, TextureHolder* Textures, float radius, std::vector<bodyData> *bDList, std::map<std::string, b2Joint*> *jMap, std::map<std::string, Animation>* animationList)
    : Entity(mWindow, world, Textures,radius, bDList, jMap)
    //, desiredVel(0)
{
    animList = animationList;
    maxLife = 80;
    kind = Entity::Human;
    std::cout<< "creation*******";

    //loadSprite(Textures);
    walkingAnimationLeft = (*animList)["walkLeft"];
    walkingAnimationRight = (*animList)["walkRight"];
    stopRight = (*animList)["stopRight"];
    stopLeft = (*animList)["stopLeft"];
    currentAnimation = &stopLeft;///POUR LE STANDBY ANIMATION


    ///lifeSprite.setOrigin(lifeTex->getSize().x/2, lifeTex->getSize().y/2);
    ///deathSprite.setOrigin(deathTex->getSize().x/2, deathTex->getSize().y/2);

}

void Human::loadSprite(TextureHolder* Textures)
{
    texture = Textures->getTexture(TextureHolder::Player);
    /*
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
*/
    stopRight.setSpriteSheet(*texture);
    stopRight.addFrame(sf::IntRect(877, 323, 44, 148));
    stopLeft.setSpriteSheet(*texture);
    stopLeft.addFrame(sf::IntRect(891, 160, 43, 149));

}
/// ////////////////////// ///////////////////
/// ///////FINITE STATE MACHINE///////////////
/// ////////////////////// ///////////////////
void    Human::doNormalThings()
{
    std::cout<< "//////////HUMAN-> NORMAL MODE\n";
    visionSprite.setColor(sf::Color(50,255,50,200));

    //visionSprite.setRotation()
    if(fsmClock.getElapsedTime().asSeconds()<1.f)
    {
        say("Meuh...",1);
        return;
    }
    if((int)fsmClock.getElapsedTime().asSeconds()%15<1)
        say("..Te hatory za zany...");

    //rayRange = rayRange/RATIO;
    int changeDirection = -1;
    if(currentAnimation == &walkingAnimationLeft || currentAnimation == &stopLeft)
            changeDirection = -changeDirection;
    callback.m_hit=false;
    p_world->RayCast(&callback,m_head->GetPosition(), b2Vec2(m_head->GetPosition().x-(rayRange/RATIO*changeDirection),m_head->GetPosition().y));
    if(callback.m_hit)
    {
        std::cout<< "//////////HIT HIT HIT\n";
        resetFSM();
        fsm_shocked = true;
        targetBody = callback.m_body;
        say("!!",2);
    }
}
void    Human::doAlertThings()
{
    visionSprite.setColor(sf::Color(229,255,7,200));
    std::cout<< "//////////HUMAN-> ALERT MODE\n";
    if(fsmClock.getElapsedTime().asSeconds()<15.f)
        say("?!??",15);

    int changeDirection = -1;
    if(currentAnimation == &walkingAnimationLeft || currentAnimation == &stopLeft)
            changeDirection = -changeDirection;
    callback.m_hit=false;
    p_world->RayCast(&callback,m_head->GetPosition(), b2Vec2(m_head->GetPosition().x-(rayRange/RATIO*changeDirection),m_head->GetPosition().y));
    //if(fsmClock.getElapsedTime().asSeconds()<15.f)
    if(callback.m_hit)
    {

        resetFSM();
        fsm_hunting = true;
        targetBody = callback.m_body;
    }

    if((int)stateClock.getElapsedTime().asSeconds()>6)
    {
        resetFSM();
        fsm_normal = true;
    }

}
void    Human::doHuntingThings()
{

    std::cout<< "//////////HUMAN-> HUNTING MODE\n";
    if(fsmClock.getElapsedTime().asSeconds()<10.f)
        if(!fsm_attack)
            say("MARATRAKO ELA NIANY!!!",10);
    float secureDistance = 200.f;
    ///HUNTING BLOCK:
    {
        int o = 1;
        if(targetBody->GetPosition().x - m_head->GetPosition().x <0.f)
            o=-1;
        if(std::abs(targetBody->GetPosition().x - m_head->GetPosition().x) > secureDistance/RATIO)
        {
            visionSprite.setColor(sf::Color(255,7,7,200));
            goTo(b2Vec2(targetBody->GetPosition().x+(secureDistance*o),targetBody->GetPosition().y));
            fsm_attack = false;
            //commitLogic();
        }
        else///normalement on attaque
        {
            std::cout <<" GOTO---->ON Y EST"<<std::endl;
            resetFSM();///desiredVel 0
            fsm_attack = true;
            fsm_hunting = true;
        }

    }
    ///checker tous les X secondes si PLAYER est toujours devant
    std::cout<<(int)fsmClock.getElapsedTime().asSeconds()%4<<"<- elapsedtime%4"<<std::endl;
    //if((int)fsmClock.getElapsedTime().asSeconds()%6<=1)
    {

        int changeDirection = -1;
        if(currentAnimation == &walkingAnimationLeft || currentAnimation == &stopLeft)
                changeDirection = -changeDirection;
        callback.m_hit=false;
        p_world->RayCast(&callback,m_head->GetPosition(), b2Vec2(m_head->GetPosition().x-(rayRange/RATIO*changeDirection),m_head->GetPosition().y));
        std::cout<<"<- checking"<<std::endl;
        if(!callback.m_hit)
        {
            std::cout<< "//////////HIT HIT HIT\n";
            ///S IL NE LE VOIT PAS PENDANT UN CERTAINS TEMPS ON PASSE EN MODE ALERT
            if((int)stateClock.getElapsedTime().asSeconds()>6)
            {
                resetFSM();
                fsm_alert = true;
            }


        }
        else
        {
            std::cout<< "//////////m_hit = true\n";
            ///S IL LE VOIT ON RECOMMENCE LE COMPTE A REBOUR
            stateClock.restart();
        }
    }

}
void    Human::doShockedThings()
{
    visionSprite.setColor(sf::Color(229,255,7,20));
    std::cout<< "//////////HUMAN-> SHOCKED MODE\n";
    if(fsmClock.getElapsedTime().asSeconds()<1.f)
    {
        say("!!",1);
        return;
    }

    resetFSM();
    fsm_alert = true;
}

void    Human::doAttackThings()
{
    visionSprite.setColor(sf::Color(229,255,7,150));
    std::cout<< "//////////HUMAN-> ATTACK MODE\n";
    if((int)fsmClock.getElapsedTime().asSeconds()%8<2)
    {
        if(atkClock.getElapsedTime().asSeconds()>2.f)
            if(fixtureOnSensor != nullptr)
            {
                punchOn(fixtureOnSensor);
                atkClock.restart();
            }

        say("KTI KDA KDOUUUU!!!",1);
        return;
    }




}
void Human::punchOn(b2Fixture* fixt)
{
    void* userData = fixt->GetBody()->GetUserData();

    if(userData && ((ObjectType*)userData)->getObjectType() == ENTITY)///si le touchE est une entitE
    {
            ((Entity*)userData)->getHit(/*damage*/12.f,40.f);
            if(fixt->GetBody()->GetPosition().x - m_body->GetPosition().x <0)
                ((Entity*)userData)->applyForce(-5.f);
            else
                ((Entity*)userData)->applyForce(5.f);
    }
}
/// ////////////////////// ///////////////////
/// ////////////////////// ///////////////////
void Human::render(sf::RenderWindow& mWindow, sf::Time frameTime, TextureHolder* Textures, sf::Shader* shader)
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
            animatedSprite.setOrigin((animatedSprite.getAnimation()->getFrame(0).width/2),(BOXSIZE_H/2));

//            animatedSprite.sets
//            animatedSprite.setOrigin((currentAnimation->getSize().x),(currentAnimation->getSize().y/2));

            animatedSprite.setPosition(m_body->GetPosition().x * RATIO,
                                        m_body->GetPosition().y * RATIO);
            animatedSprite.setRotation(m_body->GetAngle() * 180/b2_pi);
            ///Draw:
            drawLife(mWindow);

            shader->setParameter("time", clock.getElapsedTime().asSeconds());
            shader->setParameter("distortionFactor", .002f);
            shader->setParameter("riseFactor", .3f);

            drawVision(mWindow);
            mWindow.draw(animatedSprite, shader);
//*/
}


