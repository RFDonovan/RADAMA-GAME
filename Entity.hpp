#ifndef ENTITY_HPP_INCLUDED
#define ENTITY_HPP_INCLUDED
#include <random>
#include<SFML/Graphics.hpp>
#include<Box2D/Box2D.h>
#include<iostream>
#include<cmath>
#include<vector>
#include<sstream>
#include "Globals.h"
#include "AnimatedSprite.hpp"
#include "TextureHolder.h"
#include "Structures.hpp"
#include "RayCastCallback.h"
#include "Projectile.hpp"
#include "Item.hpp"

#include "pugixml.hpp"

struct bodyUserData {
      int bodyKind;
  };

class Entity : public ObjectType{

public:
    enum Type{
        Player,
        Human,
        Other,
    };
    //class member variables

    struct jointStruct{
        b2Joint* joint;
        std::string bodyA;
        std::string bodyB;
    };

    b2Body* m_body, *m_legs, *m_head, *m_sensor, *m_sensorL, *m_sensorR;
    float   m_radius;
    Type    kind;
    std::vector<b2Joint *>      jointList;
    std::vector<jointStruct>      jointBodyList;
    std::map<std::string, b2Body*> bodyList;

    sf::RenderWindow& mWindow;
    sf::Clock noRestartClock;

    //animation control
    sf::Texture* texture;
    TextureHolder* textureHolder;
    TextureHolder Textures;

    bool grounded = false;
    int  nb_contacts = 0;
    b2World *p_world;
    b2Vec2 vel;
    b2Fixture   *basFixture;
    int jump = 0;

    ///ANIMATION
    sf::Clock frameClock;
    Animation *currentAnimation;
    Animation walkingAnimationLeft, walkingAnimationRight, stopLeft, stopRight, jumpLeft, jumpRight, shiftRight, shiftLeft;
    Animation atkLeft, atkRight;
    AnimatedSprite animatedSprite;
    std::map<std::string, Animation>* animList;
    float max_width = 0;
    sf::Vector2f mouseInit, mousePos, playerPos, velocityForce;

    float desiredVel;
    float velocityLimit = 5.f;
    float jumpLimit = 5.f;
    sf::Clock jumpClock;

    ///SPRINT MAX
    sf::Clock shiftClock;
    bool isShifted = false;
    int lastTime = 0;

    sf::Sprite shiftSprite;

    sf::Clock clock;

    ///FINITE STATE MACHINE
    RayCastCallback callback;
    b2Body* targetBody;
    sf::Clock fsmClock,stateClock;
    bool fsm_hunting = false;
    bool fsm_attack = false;
    bool fsm_alert = false;
    bool fsm_shocked = false;
    bool fsm_normal = true;
    ///SENSOR
    sf::Clock visionClock;
    float rayRange = 300.f;
    sf::Sprite visionSprite;

    bool showFX = false;


    ///LIFE HANDLER
    int m_life = 80; // sur 100
    int m_mana = 100; // sur 100
    int maxLife;
    bool deleted = false;

    sf::Clock lifeClock;
//    sf::Texture *lifeTex, *deathTex;
    sf::Sprite lifeSprite, deathSprite;

    sf::Clock textClock;
    sf::Text Text;
    sf::Font MyFont;
    bool haveToSpeak = false;
    int textDelay=5;

    ///WEAPON TAKING
    bool        isWeaponDispo = false;
    Projectile* weaponDispo;

    ///ATTACK DEF INTERACTION
    bool isAttacked = false;
    int defense = 0;
    b2Fixture*  fixtureOnSensor;
    bool isAttacking = false;
    sf::Clock atkClock, atackedClock;

public:
            Entity(sf::RenderWindow& mWindow, b2World* world, float radius, std::vector<bodyData> *bDList, std::map<std::string, b2Joint*> *jMap);
    //void    loadPlayerSprite(TextureHolder* Textures);
    //void    processLogic();
    //void    processLogic(sf::RenderWindow& mWindow);
    //void    render(sf::RenderWindow& mWindow,sf::Time frameTime, TextureHolder* Textures){}
    //void    onCommand(sf::Event e);
    int     getY();
    int     getX();
    void    setPosition(sf::Vector2f position);

    bool    isGrounded();
    b2Vec2  getVelocity();
    float   getMass();

    void    startContact(b2Fixture   *fixture, b2Fixture   *fixtureB);
    void    endContact(b2Fixture   *fixture, b2Fixture   *fixtureB);

    //void    attachStuff(sf::Shader* shader, TextureHolder::TexName tex);

    int     getObjectType(){return ENTITY;}

    void    wipeJoints();

    void    exportToXML(std::string filename);
    void    addBodyNode(pugi::xml_node parent, std::string name, b2Body* body);
    void    addJointNode(pugi::xml_node parent, std::string name, jointStruct* jStruct);

    void    sense();
    void    jumpOnObstacle();
    void    goTo(b2Vec2);
    void    commitLogic();
    void    getHit(int damage, float impulse);
    void    applyForce(float f);
    bool    isDead();
    void    doTheDead();
    void    takeItem(Item* item);


    ///FINITE STATE MACHINE
    virtual void    doNormalThings()=0;
    virtual void    doAlertThings()=0;
    virtual void    doHuntingThings()=0;
    virtual void    doAttackThings()=0;
    virtual void    doShockedThings()=0;
    void    resetFSM();
    ///DRAWING SOMETHING
    void    drawVision(sf::RenderWindow& mWindow);
    void    drawLife(sf::RenderWindow& mWindow);
    void    speak(sf::RenderWindow& mWindow);
    void    say(std::string,int delay=5);

    ///ATTACK RANGE
    virtual void    addAttackRange() = 0;
    b2Vec2  getSensorPosition();

            ~Entity();
};

#endif // ENTITY_HPP_INCLUDED
