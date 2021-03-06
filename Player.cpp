#include "Player.h"

Player::Player(sf::RenderWindow& mWindow, b2World* world, float radius, std::vector<bodyData> *bDList, std::map<std::string, b2Joint*> *jMap, std::map<std::string, Animation>* animationList)
    : Entity(mWindow, world,radius, bDList, jMap)
    //, desiredVel(0)
{
    animList = animationList;
    maxLife = 100;
    m_life = 10;
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


//    lifeTex = new sf::Texture();
//    deathTex = new sf::Texture();
//    lifeTex->loadFromFile("life.png");

    Textures.loadFromFile("lifeBar", "Resources/Images/life.png");
    Textures.loadFromFile("deathBar", "Resources/Images/death.png");
//    lifeSprite.setTexture(*lifeTex);
    lifeSprite.setTexture(*Textures.getTexture("lifeBar"));

//    deathTex->loadFromFile("death.png");
//    deathSprite.setTexture(*deathTex);
    lifeSprite.setTexture(*Textures.getTexture("deathBar"));
    deathSprite.setColor(sf::Color(0, 0, 0, 150));
    ///lifeSprite.setOrigin(lifeTex->getSize().x/2, lifeTex->getSize().y/2);
    ///deathSprite.setOrigin(deathTex->getSize().x/2, deathTex->getSize().y/2);



}

void Player::addAnimations(std::map<std::string, Animation>* animationList)
{
    stopRight = (*animationList)["stopRight"];
    stopLeft = (*animationList)["stopLeft"];
    addJumpSprite(animationList);
    addShiftSprite(animationList);
    addAtkSprite(animationList);
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

    Textures.loadFromFile("shiftPower", "Resources/Images/shiftPower.png");

    shiftSprite.setTexture(*Textures.getTexture("shiftPower"));
    shiftSprite.setOrigin(Textures.getTexture("shiftPower")->getSize().x/2, Textures.getTexture("shiftPower")->getSize().y/2);
    shiftSprite.setScale(-1.f,1.2f);
}

void Player::addAtkSprite(std::map<std::string, Animation>* animationList)
{
    atkRight = (*animationList)["atkRight"];
    atkLeft = (*animationList)["atkLeft"];
}

/// //////////////

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
            if(currentAnimation == &walkingAnimationLeft || currentAnimation == &atkLeft || currentAnimation == &stopLeft)
            {
                currentAnimation = &jumpLeft;
            }

            if(currentAnimation == &walkingAnimationRight || currentAnimation == &atkRight || currentAnimation == &stopRight)
            {
                currentAnimation = &jumpRight;
                //rotate 45%
            }

        }
    else///QUAND IL NE BOUGE PLUS OU BOUGE PETIT
    {
        if(currentAnimation == &walkingAnimationLeft||currentAnimation == &jumpLeft || currentAnimation == &atkLeft)
            currentAnimation = &stopLeft;
        if(currentAnimation == &walkingAnimationRight||currentAnimation == &jumpRight || currentAnimation == &atkRight)
            currentAnimation = &stopRight;
    }

    if(isAttacking)
    {
        //if(m_body->GetAngle() != -45.f*b2_pi)
            //m_body->SetTransform(m_body->GetPosition(), 10);
        if(m_sensor == m_sensorL)
            currentAnimation = &atkLeft;
        else
            currentAnimation = &atkRight;
    }

    if(isDead())
        if(m_sensor == m_sensorL)
            currentAnimation = &stopLeft;
        else
            currentAnimation = &stopRight;

    noKeyWasPressed = true;
    animatedSprite.update(frameTime1);


    ///emplacement:
    //animatedSprite.setOrigin((BOXSIZE_W),(BOXSIZE_H/2));
    animatedSprite.setOrigin((animatedSprite.getAnimation()->getFrame(0).width/2),(BOXSIZE_H/2));
    animatedSprite.setPosition(
                               m_body->GetPosition().x * RATIO,
                               m_body->GetPosition().y * RATIO
                               );
    if(
       currentAnimation == &stopLeft || currentAnimation == &stopRight
       )
    {
        animatedSprite.setOrigin((animatedSprite.getAnimation()->getFrame(0).width/2),(animatedSprite.getAnimation()->getFrame(0).height));
        animatedSprite.setScale(0.9f, 0.9f);
        animatedSprite.setPosition(
                               m_body->GetPosition().x * RATIO,
//                               m_body->GetPosition().y * RATIO
                               (m_legs->GetPosition().y)*RATIO + 18
                               );
    }
    else
    {
        animatedSprite.setScale(1.f, 1.f);
    }


    animatedSprite.setRotation(m_body->GetAngle() * 180/b2_pi);

    ///JUST SPRITE ANIMATION BLOCK
    {
            if(isAttacking)
        {
            if(currentAnimation==&atkLeft)
                animatedSprite.setRotation(animatedSprite.getRotation()-10.f);
            if(currentAnimation==&atkRight)
                animatedSprite.setRotation(animatedSprite.getRotation()+10.f);
            animatedSprite.setPosition(animatedSprite.getPosition().x, animatedSprite.getPosition().y+5);
        }

        if(nb_contacts<=0 && !isDead())
        {
            if((int)jumpClock.getElapsedTime().asMilliseconds()>300 &&(int)jumpClock.getElapsedTime().asMilliseconds() < 360+300)
            {
                if(currentAnimation==&jumpRight || currentAnimation == &shiftRight)
                    animatedSprite.setRotation(animatedSprite.getRotation()
                                           +
                                           (int)jumpClock.getElapsedTime().asMilliseconds()-300
                                           );
                if(currentAnimation==&jumpLeft || currentAnimation == &shiftLeft)
                    animatedSprite.setRotation(animatedSprite.getRotation()
                                           -
                                           (int)jumpClock.getElapsedTime().asMilliseconds()-300
                                           );
            }


        }
        else
            jumpClock.restart();

    }///JUST SPRITE ANIMATION BLOCK

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
        shiftSprite.setColor(sf::Color(255,255,255,155+m_mana));
        mWindow.draw(shiftSprite, shader);
    }


    //std::cout<< "\n******rendu ok*******";


//*/


}

void Player::onCommand(sf::Event e)
{
    if(isDead())
        return;

    if(atkClock.getElapsedTime().asMilliseconds()>300.0f)
        isAttacking = false;
    if(!isShifted)
    {
        int realTime = (int)shiftClock.getElapsedTime().asSeconds()%4;
        if(lastTime != realTime)
        {
            //shifter = shifter + 1;
            if (m_mana<100)
                m_mana = m_mana + 5;
            else
                m_mana = 100;
            lastTime = realTime;
        }
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
    {
            if(m_mana>0)
            {
                isShifted = true;
                int realTime = (int)shiftClock.getElapsedTime().asSeconds()%4;
                if(lastTime != realTime)
                {
                    //shifter = shifter - 1;
                    if (m_mana>=5)
                        m_mana = m_mana - 20;
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
        if(e.mouseButton.button == sf::Mouse::Left)
        {
            isAttacking =true;
            atkClock.restart();
        }
    }
    break;
    case sf::Event::MouseButtonReleased:
        if(e.mouseButton.button == sf::Mouse::Left)
        {
            if(fixtureOnSensor != nullptr)
            {
                attackOn(fixtureOnSensor);
                //isAttacking = false;
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

    showFX = true;

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
    ((Projectile*)prData)->setVisible(true);
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
    weaponDispo->setVisible(false);

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

