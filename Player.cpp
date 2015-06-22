#include "Player.h"

Player::Player(sf::RenderWindow& mWindow, b2World* world,TextureHolder* Textures, float radius, float32 x, float32 y, float w, float h)
    : Entity(mWindow,world, Textures, radius, x, y, w, h)
    , desiredVel(0)
{
    kind = Entity::Player;
    std::cout<< "creation*******";

    loadPlayerSprite(Textures);
    currentAnimation = &stopRight;///POUR LE STANDBY ANIMATION
    ///createWeapons(); je vais desactiver celui ci

    currentProjectile = nameToWeapon["lefona"];
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
        if(joint !=nullptr)
        {
            std::cout<<"destroying joint 1...\n";
            if(!jointsAlreadyDestroyed)
                p_world->DestroyJoint(joint);

            joint = nullptr;//pour eviter une repetition
            std::cout<<"joint 1 destroyed\n";
        }
        if(joint2 != nullptr)
        {
            std::cout<<"destroying joint 2...\n";
            if(!jointsAlreadyDestroyed)
                p_world->DestroyJoint(joint2);
            joint2 = nullptr;
            std::cout<<"joint 2 destroyed\n";
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
    case sf::Event::MouseWheelMoved:
    {
        if (sf::Mouse::isButtonPressed(sf::Mouse::Middle))
            return;
        if(e.mouseWheel.delta>0)
        {
            std::cout<< "UP";
            if(currentProjectile<weaponsMap.size()-1)
                currentProjectile++;
            else
                currentProjectile = 0;
        }else
        {
            std::cout<< "Down";
            if(currentProjectile > 0)
                currentProjectile--;
            else
                currentProjectile = weaponsMap.size() - 1;
        }
        std::cout<< "ARME UTILISE: "<<weaponToName[currentProjectile]<<std::endl;
    }
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

void Player::fire(int projectile)
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
        {
            float angle = weaponsMap[currentProjectile]->GetAngle();
        float x,y;
        x = std::cos(angle) * 500;//cosinus*hypotenuse
        y = std::sin(angle) * 500;
        std::cout<< "lefona be!"<<projectile;
        //weaponsMap[currentProjectile]->ApplyLinearImpulse(b2Vec2(x, y), m_body->GetWorldCenter());
        weaponsMap[currentProjectile]->ApplyLinearImpulse(b2Vec2(x, y), weaponsMap[currentProjectile]->GetWorldPoint(b2Vec2(50.f/RATIO,0.f)));

        }
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
    f->SetUserData((void*)identificationArme + numero);
    //lefona->SetActive(false);

}

void    Player::loadWeapon(bodyData* data)
{
    if(data==nullptr)
    {
        return;
    }
    data->body->SetUserData((Entity*)this);

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


void    Player::impactTo(b2Fixture* fixtureSource, b2Fixture* fixtureTarget, float impulse)
{
    void* fixData = fixtureSource->GetUserData();
    if(! fixData)
    {
        //std::cout<<"fixture trouvE+++++++++++: "<< weaponToName[(int)fixData-identificationArme] <<std::endl;
        return;
    }
    if(weaponToName[(int)fixData-identificationArme].compare("lefona")==0
       || weaponToName[(int)fixData-identificationArme].compare("lefonaMiloko")==0)
       if(impulse > 40)///d'habitude c'est >50
            stickProjectile((int)fixData-identificationArme, fixtureTarget);

}
void Player::stickProjectile(int projectile,b2Fixture* fixtureTarget)
{


    worldCoordsAnchorPoint =(weaponsMap[projectile])->GetWorldPoint( b2Vec2(2.6f, 0.f) );
    weldJointDef.bodyA = fixtureTarget->GetBody();
    if (weldJointDef.bodyA == m_body||weldJointDef.bodyA == m_legs||weldJointDef.bodyA == m_head)
        return;


    weldJointDef.bodyB = weaponsMap[projectile];
    weldJointDef.localAnchorA = weldJointDef.bodyA->GetLocalPoint( worldCoordsAnchorPoint );
    weldJointDef.localAnchorB = weldJointDef.bodyB->GetLocalPoint( worldCoordsAnchorPoint );
    weldJointDef.referenceAngle = weldJointDef.bodyB->GetAngle() - weldJointDef.bodyA->GetAngle();
    ///SECOND
    worldCoordsAnchorPoint =(weaponsMap[projectile])->GetWorldPoint( b2Vec2(2.f, 6.f) );
    weldJointDef1.bodyA = fixtureTarget->GetBody();
    if (weldJointDef.bodyA == m_body||weldJointDef.bodyA == m_legs||weldJointDef.bodyA == m_head)
        return;


    weldJointDef1.bodyB = weaponsMap[projectile];
    weldJointDef1.localAnchorA = weldJointDef.bodyA->GetLocalPoint( worldCoordsAnchorPoint );
    weldJointDef1.localAnchorB = weldJointDef.bodyB->GetLocalPoint( worldCoordsAnchorPoint );
    weldJointDef1.referenceAngle = weldJointDef.bodyB->GetAngle() - weldJointDef.bodyA->GetAngle();

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
        joint2 = p_world->CreateJoint( &weldJointDef1 );
        joint->SetUserData((void*)(JOINTRANGE + 2));
        joint2->SetUserData((void*)(JOINTRANGE + 3));

        jointExist = false;
        jointsAlreadyDestroyed = false;
    }


}

void Player::jointDestroyer()
{
    std::cout<<"GOOOOOT YAAAAA!!!"<<std::endl;
    jointsAlreadyDestroyed = true;
    //jointExist =  true;
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

