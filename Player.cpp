#include "Player.h"

Player::Player(sf::RenderWindow& mWindow, b2World* world, TextureHolder* Textures, float radius, std::vector<bodyData> *bDList, std::map<std::string, b2Joint*> *jMap, std::map<std::string, Animation>* animationList)
    : Entity(mWindow, world, Textures,radius, bDList, jMap)
    //, desiredVel(0)
{
    animList = animationList;
    maxLife = 100;
    m_life = 50;
    desiredVel = 0;
    kind = Entity::Player;
    std::cout<< "creation*******";

    //loadPlayerSprite(Textures);

    walkingAnimationLeft = (*animList)["walkLeft"];
    walkingAnimationRight = (*animList)["walkRight"];
    stopRight = (*animList)["stopRight"];
    stopLeft = (*animList)["stopLeft"];

    currentAnimation = &stopRight;

    currentProjectile = weaponsMap.size() - 1;//nameToWeapon["lefona"];


    lifeTex = new sf::Texture();
    deathTex = new sf::Texture();
    lifeTex->loadFromFile("life.png");
    lifeSprite.setTexture(*lifeTex);

    deathTex->loadFromFile("death.png");
    deathSprite.setTexture(*deathTex);
    deathSprite.setColor(sf::Color(0, 0, 0, 150));
    ///lifeSprite.setOrigin(lifeTex->getSize().x/2, lifeTex->getSize().y/2);
    ///deathSprite.setOrigin(deathTex->getSize().x/2, deathTex->getSize().y/2);



}

void Player::addJumpSprite(std::map<std::string, Animation>* animationList)
{
    jumpRight = (*animationList)["jumpRight"];
    jumpLeft = (*animationList)["jumpLeft"];
//    (*animList)["jumpRight"] = (*animationList)["jumpRight"];
//    (*animList)["jumpLeft"] = (*animationList)["jumpLeft"];
//
//    jumpRight = (*animList)["jumpRight"];
//    jumpLeft = (*animList)["jumpLeft"];
}

void Player::addShiftSprite(std::map<std::string, Animation>* animationList)
{
    shiftRight = (*animationList)["shiftRight"];
    shiftLeft = (*animationList)["shiftLeft"];

    shiftTexture.loadFromFile("Resources/shiftPower.png");
    shiftSprite.setTexture(shiftTexture);
    shiftSprite.setOrigin(shiftTexture.getSize().x/2, shiftTexture.getSize().y/2);
    shiftSprite.setScale(-1.f,1.2f);
//    (*animList)["jumpRight"] = (*animationList)["jumpRight"];
//    (*animList)["jumpLeft"] = (*animationList)["jumpLeft"];
//
//    jumpRight = (*animList)["jumpRight"];
//    jumpLeft = (*animList)["jumpLeft"];
}

/// //////////////

void Player::loadPlayerSprite(TextureHolder* Textures)
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

    noKeyWasPressed = true;

    ///WEAPONS
    texture = Textures->getTexture(TextureHolder::Lefona);
    weaponSprite.setTexture(*texture);
    weaponSprite.setScale(sf::Vector2f(1.2f,1.f));
    texture->setSmooth(true);

}

void Player::render(sf::RenderWindow& mWindow, sf::Time frameTime, TextureHolder* Textures, sf::Shader* shader)
{



    /**GOD S HAND**/

    sf::Time frameTime1 = frameClock.restart();

    sf::Vector2i screenDimensions(800,600);

    //start animation:
    ///animatedSprite.setFrameTime(sf::seconds(0.01));
    animatedSprite.play(*currentAnimation);

/// AJOUTER UNE ANIMATION EN FONCTION DE L'ACTUEL AU LIEU DE FAIRE UN STOP
    if(std::abs(getVelocity().x)>1&& !isDead())///SI IL BOUGE PLUS QUE NECESSAIRE---------POUR EVITER LE TREMBLEMENT DES SPRITES
        if(nb_contacts>0)
            if (getVelocity().x>0)
                if(!isShifted)
                    currentAnimation = &walkingAnimationRight;
                else
                    currentAnimation = &shiftRight;
            else if (getVelocity().x<0)
                if(!isShifted)
                    currentAnimation = &walkingAnimationLeft;
                else
                    currentAnimation = &shiftLeft;
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
                currentAnimation = &jumpLeft;
            if(currentAnimation == &walkingAnimationRight)
                currentAnimation = &jumpRight;
        }
    else///QUAND IL NE BOUGE PLUS OU BOUGE PETIT
    {
        if(currentAnimation == &walkingAnimationLeft||currentAnimation == &jumpLeft)
            currentAnimation = &stopLeft;
        if(currentAnimation == &walkingAnimationRight||currentAnimation == &jumpRight)
            currentAnimation = &stopRight;
    }

    noKeyWasPressed = true;
    animatedSprite.update(frameTime1);


    ///emplacement:
    //animatedSprite.setOrigin((BOXSIZE_W),(BOXSIZE_H/2));
    animatedSprite.setOrigin((animatedSprite.getAnimation()->getFrame(0).width/2),(BOXSIZE_H/2));
    animatedSprite.setPosition(m_body->GetPosition().x * RATIO,
                               m_body->GetPosition().y * RATIO);
    animatedSprite.setRotation(m_body->GetAngle() * 180/b2_pi);


    //drawLife(mWindow);
    ///weapon render
    renderWeapons(mWindow);
    ///Draw:
    mWindow.draw(animatedSprite);


    if(isShifted && !isDead())
    {

        shader->setParameter("time", clock.getElapsedTime().asSeconds());
            shader->setParameter("distortionFactor", .02f);
            shader->setParameter("riseFactor", 1.3f);
        shiftSprite.setPosition(m_body->GetPosition().x * RATIO,
                               m_body->GetPosition().y * RATIO);
        if(getVelocity().x<0)
            shiftSprite.setScale(-1.f,1.f);
        else
            shiftSprite.setScale(1.f,1.f);
        mWindow.draw(shiftSprite, shader);
    }


    //std::cout<< "\n******rendu ok*******";


//*/


}

void Player::onCommand(sf::Event e)
{
    if(isDead())
        return;

    if(!isShifted)
    {
        int realTime = (int)shiftClock.getElapsedTime().asSeconds()%4;
        if(lastTime != realTime)
        {
            shifter = shifter + 1;
            lastTime = realTime;
        }
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
    {
            if(shifter>0)
            {
                isShifted = true;
                int realTime = (int)shiftClock.getElapsedTime().asSeconds()%4;
                if(lastTime != realTime)
                {
                    shifter = shifter - 1;
                    lastTime = realTime;
                }
            }
            else
            {
                isShifted = false;
            }

    }else
    {
        isShifted = false;
    }


    if(sf::Keyboard::isKeyPressed(K_LEFT))
    {
        //currentAnimation = &walkingAnimationLeft;
        if (isShifted)
            desiredVel = -15.0f;
        else
            desiredVel = -5.0f;
        noKeyWasPressed = false;

    }


    if (sf::Keyboard::isKeyPressed(K_RIGHT))
    {
        //currentAnimation = &walkingAnimationRight;
        if (isShifted)
            desiredVel = 15.0f;
        else
            desiredVel = 5.0f;
        noKeyWasPressed = false;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::T))///T to TAKE
    {
        if(isWeaponDispo)
        {
            takeWeapon();
        }

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
        if(weaponsMap.size() <=0 )
                return;
        std::cout<<"PLAYER::IsButtonPressed avant tout\n";
        void* ptile = weaponsMap[currentProjectile]->GetUserData();
        std::cout<<"PLAYER::IsButtonPressed apres GetUserData\n";
        ((Projectile*)ptile)->unStick();
        ((Projectile*)ptile)->fired = true; ///ARMER LE PROJECTILE POUR FAIRE MAL
        std::cout<<"PLAYER::IsButtonPressed apres unStick\n";
        mousePos = mWindow.mapPixelToCoords(sf::Mouse::getPosition(mWindow), mWindow.getView());
        int posX = (int)m_body->GetPosition().x * RATIO;
        int posY = (int)m_body->GetPosition().y * RATIO;


        ///FA MANINONA NO ERREUR REFA DEBARASSE LEIZ----resolu

        //angle = std::atan(mousePos.y-posY/mousePos.x-posX);
        angle = std::atan2(mousePos.y-posY,mousePos.x-posX);


        weaponsMap[currentProjectile]->SetTransform(b2Vec2(m_body->GetPosition().x, m_body->GetPosition().y-3), angle);
        std::cout<<mousePos.x<<";"<<mousePos.y<<";"<<angle<<";\n";

    }

    if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
    {

        /*/std::cout<< "click";
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
        */
    }

    switch(e.type)
    {
    case sf::Event::MouseButtonPressed:
    {

        if(e.mouseButton.button == sf::Mouse::Middle)
        {
            //mouseInit = mWindow.mapPixelToCoords(sf::Mouse::getPosition(mWindow), mWindow.getView());

            weaponsMap[currentProjectile]->SetLinearVelocity(b2Vec2(0,0));
            weaponsMap[currentProjectile]->SetActive(false);
        }
    }
    break;
    case sf::Event::MouseButtonReleased:
        if(e.mouseButton.button == sf::Mouse::Left)
        {
            if(fixtureOnSensor != nullptr)
            {
                attackOn(fixtureOnSensor);
            }
        }
        if(weaponsMap.size() <=0 )
                return;
        if(e.mouseButton.button == sf::Mouse::Middle)
        {
            weaponsMap[currentProjectile]->SetActive(true);
            fire(currentProjectile);
        }

        break;

    case sf::Event::KeyPressed:
//        if(e.key.code == sf::Keyboard::LShift)
//        {
//            isShifted = true;
//        }
        break;
    case sf::Event::KeyReleased:
        if(e.key.code == sf::Keyboard::Space)
        {
            //jump++;
        }
//        if(e.key.code == sf::Keyboard::LShift)
//        {
//            isShifted = false;
//        }
        break;
    case sf::Event::MouseMoved:

        break;
    case sf::Event::MouseWheelMoved:
    {
        if (sf::Mouse::isButtonPressed(sf::Mouse::Middle))
            return;
        if(weaponsMap.size() <=0 )
            return;
        if(e.mouseWheel.delta>0)
        {
            currentProjectile = nextProjectile();
//                        std::cout<<std::endl<< "UP to"<<currentProjectile<<std::endl;
//            if(currentProjectile<weaponsMap.size()-1)
//                currentProjectile++;
//            else
//                currentProjectile = 0;
        }else
        {
            currentProjectile = prevProjectile();
//            std::cout<< "Down";
//            if(currentProjectile > 0)
//                currentProjectile--;
//            else
//                currentProjectile = weaponsMap.size() - 1;
        }
        std::cout<< "ARME UTILISE: "<<weaponToName[currentProjectile]<<std::endl;
    }
    break;
    }


}

void Player::attackOn(b2Fixture* fixt)
{
    void* userData = fixt->GetBody()->GetUserData();

    if(userData && ((ObjectType*)userData)->getObjectType() == ENTITY)///si le touchE est une entitE
    {
        if((Entity*)userData == this)
            return;
            ((Entity*)userData)->getHit(/*damage*/12.f,40.f);
            if(fixt->GetBody()->GetPosition().x - m_body->GetPosition().x <0)
                ((Entity*)userData)->applyForce(-50.f);
            else
                ((Entity*)userData)->applyForce(200.f);
    }
}
int Player::nextProjectile()
{
    bool trouve = false;
    for(auto& it: weaponsMap)
    {
        if(trouve)
            return it.first;
        if(currentProjectile == it.first)
        trouve = true;
    }
    return currentProjectile;
}
int Player::prevProjectile()
{
    int prev = currentProjectile;
    for(auto& it: weaponsMap)
    {
        if(currentProjectile == it.first)
            return prev;
        prev = it.first;
    }
    return currentProjectile;
}


void Player::processLogic()
{

    if(isDead())
        return;
    if(currentAnimation == &walkingAnimationLeft||currentAnimation==&stopLeft)
        m_sensor = m_sensorL;
    if(currentAnimation == &walkingAnimationRight||currentAnimation==&stopRight)
        m_sensor = m_sensorR;
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
        m_body->ApplyLinearImpulse(b2Vec2(velChange/2, -force), m_body->GetWorldCenter(), true);
        jump--;
    }




    if(nb_contacts>0)
        m_body->ApplyForce(b2Vec2(force, 0), m_body->GetWorldCenter(), true);
    else if(vel.y < -1.0f) //|| vel.y > 2.0f)//vel.x != 0 &&
        m_body->ApplyForce(b2Vec2(force, 0), m_body->GetWorldCenter(), true);

}

void Player::fire(int projectile)
{

    switch (projectile)
    {

    break;

    default:
        {
            float angle = weaponsMap[currentProjectile]->GetAngle();
        float x,y;
        x = std::cos(angle) * 500;//cosinus*hypotenuse
        y = std::sin(angle) * 500;
        std::cout<< "lefona be!"<<projectile;
        //weaponsMap[currentProjectile]->ApplyLinearImpulse(b2Vec2(x, y), m_body->GetWorldCenter());
        weaponsMap[currentProjectile]->ApplyLinearImpulse(b2Vec2(x, y), weaponsMap[currentProjectile]->GetWorldPoint(b2Vec2(50.f/RATIO,0.f)), true);

        }
        break;
    }

    if(currentAnimation == &walkingAnimationRight)
    {

    }

    ///DROP IT
    void * prData = weaponsMap[currentProjectile]->GetUserData();
    ((Projectile*)prData)->dejaPris = false;
    //std::cout<<"PLAYER::fire weaponsMap.size = "<<weaponsMap.size()<<std::endl;
    weaponsMap.erase(currentProjectile);
    ///on utilise ceci car les key dans weaponsMap ne sont pas forcement continue
    for(auto& it: weaponsMap)
    {
        currentProjectile = it.first;
    }
    //std::cout<<"PLAYER::fire currentprojectile = "<<currentProjectile<<std::endl;
    //std::cout<<"PLAYER::fire weaponsMap.size = "<<weaponsMap.size()<<std::endl;

}

void Player::takeWeapon()
{
    weaponDispo->unStick();
    loadWeapon(weaponDispo->getBodyData());
    weaponDispo->dejaPris = true;

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
    FixtureDef.density = 1.f;
    //FixtureDef.friction = 1.0f;
    FixtureDef.friction = 0.735f;
    //FixtureDef.restitution = 1.f;
    //FixtureDef.restitution = .5f;
    FixtureDef.shape = &Shape;

    //attach to body:
    b2Fixture* f = lefona->CreateFixture(&FixtureDef);

    lefona->SetUserData((Entity*)this);

    //lefona->SetBullet(true);
    int numero = weaponsMap.size();
    weaponsMap[numero] = lefona;
    nameToWeapon["lefona"] = numero;
    weaponToName[numero] = "lefona";
    f->SetUserData((void*)(identificationArme + numero));
    //lefona

}

void    Player::loadWeapon(bodyData* data)
{
    std::cout<<"**************LOADING DATA*************"<<std::endl;
    if(data==nullptr)
    {
        return;
    }
    //data->body->SetUserData((Entity*)this);
    data->body->SetActive(false);
    int numero = weaponsMap.size();
    weaponsMap[numero] = data->body;
    nameToWeapon[data->name] = numero;
    weaponToName[numero] = data->name;
    for (b2Fixture* f = data->body->GetFixtureList(); f; f = f->GetNext())
    {
        f->SetUserData((void*)identificationArme + numero);
    }

    std::cout<<"**************data*************"<<std::endl;
    std::cout<<"name: "<<data->name<<std::endl;
    std::cout<<"***************************"<<std::endl;
    currentProjectile = numero;
}

void Player::renderWeapons(sf::RenderWindow& mWindow)
{
    return;
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


void Player::clearAll()
{
    std::vector<b2Body*>    weapons;
    std::map<int, b2Body*>  weaponsMap;///map <PROJECTILEREF, BODY>
    std::map<std::string, int>  nameToWeapon; ///map <NOM, PROJECTILEREF>
    std::map<int, std::string>  weaponToName;
    weaponsMap.clear();
    nameToWeapon.clear();
    weaponToName.clear();
    for (int i = 0; i < weapons.size(); i++)
    {
        p_world->DestroyBody(weapons[i]);
        weapons[i] = nullptr;
        weapons.erase(weapons.begin()+i);
    }

}

