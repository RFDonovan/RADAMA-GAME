#include "Player.h"

Player::Player(sf::RenderWindow& mWindow, b2World* world,TextureHolder* Textures, float radius, float32 x, float32 y, float w, float h)
: Entity(mWindow,world, Textures, radius, x, y, w, h)
, desiredVel(0)
{
    kind = Entity::Player;
    std::cout<< "creation*******";

    loadPlayerSprite(Textures);
    currentAnimation = &stopRight;///POUR LE STANDBY ANIMATION
    createWeapons();
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

    stopRight.setSpriteSheet(*texture);
    stopRight.addFrame(sf::IntRect(877, 323, 44, 148));
    stopLeft.setSpriteSheet(*texture);
    stopLeft.addFrame(sf::IntRect(891, 160, 43, 149));

    noKeyWasPressed = true;

    ///WEAPONS
    texture = Textures->getTexture(TextureHolder::Lefona);
    weaponSprite.setTexture(*texture);
    weaponSprite.setScale(sf::Vector2f(1.2f,1.f));
    texture->setSmooth(true);

}

void Player::render(sf::RenderWindow& mWindow, sf::Time frameTime, TextureHolder* Textures)
{



            /**GOD S HAND**/

            sf::Time frameTime1 = frameClock.restart();

            sf::Vector2i screenDimensions(800,600);

            //start animation:
            animatedSprite.play(*currentAnimation);

/// AJOUTER UNE ANIMATION EN FONCTION DE L'ACTUEL AU LIEU DE FAIRE UN STOP
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


            noKeyWasPressed = true;
            animatedSprite.update(frameTime1);


            ///emplacement:
            animatedSprite.setOrigin((BOXSIZE_W),(BOXSIZE_H/2));
            animatedSprite.setPosition(m_body->GetPosition().x * RATIO,
                                        m_body->GetPosition().y * RATIO);
            animatedSprite.setRotation(m_body->GetAngle() * 180/b2_pi);
            ///weapon render
            renderWeapons(mWindow);
            ///Draw:
            mWindow.draw(animatedSprite);

            //std::cout<< "\n******rendu ok*******";


//*/


}

void Player::onCommand(sf::Event e)
{
    if(sf::Keyboard::isKeyPressed(K_LEFT))
    {
        //currentAnimation = &walkingAnimationLeft;
        desiredVel = -5.0f;
        noKeyWasPressed = false;

    }
    if (sf::Keyboard::isKeyPressed(K_RIGHT))
    {
        //currentAnimation = &walkingAnimationRight;
        desiredVel = 5.0f;
        noKeyWasPressed = false;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
    {
        if(nb_contacts>0)
        {
            jump ++;
        }

    }
    if (sf::Mouse::isButtonPressed(sf::Mouse::Middle))
    {
        if(joint != nullptr)
        {
            p_world->DestroyJoint(joint);
            joint = nullptr;//pour eviter une repetition
        }

        mousePos = mWindow.mapPixelToCoords(sf::Mouse::getPosition(mWindow), mWindow.getView());
        int posX = (int)m_body->GetPosition().x * RATIO;
        int posY = (int)m_body->GetPosition().y * RATIO;

        //angle = std::atan(mousePos.y-posY/mousePos.x-posX);
        angle = std::atan2(mousePos.y-posY,mousePos.x-posX);


        weaponsMap[currentProjectile]->SetTransform(b2Vec2(m_body->GetPosition().x, m_body->GetPosition().y-3), angle);
        std::cout<<mousePos.x<<";"<<mousePos.y<<";"<<angle<<";\n";

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

        noKeyWasPressed = false;
    }

    switch(e.type)
    {
    case sf::Event::MouseButtonPressed:
        {

        mouseInit = mWindow.mapPixelToCoords(sf::Mouse::getPosition(mWindow), mWindow.getView());

        weaponsMap[currentProjectile]->SetLinearVelocity(b2Vec2(0,0));
        weaponsMap[currentProjectile]->SetActive(false);
        }
        break;
    case sf::Event::MouseButtonReleased:
        if(e.mouseButton.button == sf::Mouse::Middle)
            weaponsMap[currentProjectile]->SetActive(true);
            fire(currentProjectile);
        std::cout<< "End";
        break;
    case sf::Event::KeyReleased:
        if(e.key.code == sf::Keyboard::Space)
        {
            //jump++;
        }

        break;
    case sf::Event::MouseMoved:

        break;
    }


}

void Player::processLogic()
{


    vel = m_body->GetLinearVelocity();
    float velChange = desiredVel - vel.x;
    if (noKeyWasPressed)
        velChange = 0.f;

    float force = getMass() * velChange / (1/60.0);// f = mv/t
    if(jump>0)
    {
        force = getMass() * 4;
        if(velChange==0.0f)
            force = force;//*1.5;
        m_body->ApplyLinearImpulse(b2Vec2(velChange/2, -force), m_body->GetWorldCenter());
        jump--;
    }




    if(nb_contacts>0)
        m_body->ApplyForce(b2Vec2(force, 0), m_body->GetWorldCenter());
    else if(vel.y < -1.0f) //|| vel.y > 2.0f)//vel.x != 0 &&
        m_body->ApplyForce(b2Vec2(force, 0), m_body->GetWorldCenter());

}

void Player::attack()
{
    if(isAttacked)
        return;



}

void Player::fire(Projectile projectile)
{

    switch (projectile)
    {
    case lefona:
        {
            float angle = weaponsMap[Projectile::lefona]->GetAngle();
            float x,y;
            x = std::cos(angle) * 500;//cosinus*hypotenuse
            y = std::sin(angle) * 500;
            std::cout<< "lefona be!"<<projectile;
            //weaponsMap[currentProjectile]->ApplyLinearImpulse(b2Vec2(x, y), m_body->GetWorldCenter());
            weaponsMap[currentProjectile]->ApplyLinearImpulse(b2Vec2(x, y), weaponsMap[currentProjectile]->GetWorldPoint(b2Vec2(50.f/RATIO,0.f)));

        }

        break;

    default:
        break;
    }

    if(currentAnimation == &walkingAnimationRight)
    {

    }

}

void Player::createWeapons()
{
    b2Body* lefona;
    b2BodyDef myBodyDef;

    myBodyDef.type = b2_dynamicBody;
    myBodyDef.position.Set(0.f, 0.f);

    lefona = p_world->CreateBody(&myBodyDef);

    //set fixture:
    b2PolygonShape Shape;
    Shape.SetAsBox(100.f/RATIO, 2.f/RATIO);
    b2FixtureDef FixtureDef;
    FixtureDef.density = 0.f;
    //FixtureDef.density = 10.f;
    //FixtureDef.friction = 1.0f;
    FixtureDef.friction = 0.735f;
    //FixtureDef.restitution = 1.f;
    //FixtureDef.restitution = .5f;
    FixtureDef.shape = &Shape;

    //attach to body:
    lefona->CreateFixture(&FixtureDef);
    lefona->SetUserData((Entity*)this);

    //lefona->SetBullet(true);
    weaponsMap[Projectile::lefona] = lefona;
    //lefona->SetActive(false);

}

void Player::loadWeapon()
{

}

void Player::renderWeapons(sf::RenderWindow& mWindow)
{
    for(auto iterator : weaponsMap)
    {
        //std::cout<< iterator.first;
        b2Body* wBody = weaponsMap[iterator.first];
        weaponSprite.setOrigin(texture->getSize().x/2,texture->getSize().y/2);
        weaponSprite.setPosition(wBody->GetPosition().x * RATIO, wBody->GetPosition().y * RATIO);
        weaponSprite.setRotation(wBody->GetAngle()/ RADTODEG);
        mWindow.draw(weaponSprite);

    }
}

void Player::stickProjectile(b2Fixture* fixtureTarget)
{


        worldCoordsAnchorPoint =(weaponsMap[currentProjectile])->GetWorldPoint( b2Vec2(0.6f, 0) );
      weldJointDef.bodyA = fixtureTarget->GetBody();
      if (weldJointDef.bodyA == m_body||weldJointDef.bodyA == m_legs||weldJointDef.bodyA == m_head)
        return;


      weldJointDef.bodyB = weaponsMap[currentProjectile];
      weldJointDef.localAnchorA = weldJointDef.bodyA->GetLocalPoint( worldCoordsAnchorPoint );
      weldJointDef.localAnchorB = weldJointDef.bodyB->GetLocalPoint( worldCoordsAnchorPoint );
      weldJointDef.referenceAngle = weldJointDef.bodyB->GetAngle() - weldJointDef.bodyA->GetAngle();

      jointExist = true;
}

void Player::stickAll()
{
      if(jointExist)
      {
          if(joint != nullptr)
            {
                return;
            }
          joint = p_world->CreateJoint( &weldJointDef );
          jointExist = false;
      }


}



