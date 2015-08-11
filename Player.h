#ifndef PLAYER_H_INCLUDED
#define PLAYER_H_INCLUDED
#include "Entity.hpp"
#include "Structures.hpp"
#include "Item.hpp"

class Player: public Entity
{

public:

    //animation control

    sf::Sprite stuffSprite, weaponSprite;

    sf::Shader *Shader;

    bool noKeyWasPressed = true;
//    float desiredVel;
    float jumpLimit = 5.f;

    std::vector<b2Body*>    weapons;
    std::map<int, b2Body*>  weaponsMap;///map <PROJECTILEREF, BODY>
    std::map<std::string, int>  nameToWeapon; ///map <NOM, PROJECTILEREF>
    std::map<int, std::string>  weaponToName;
    //Projectile currentProjectile = Projectile::lefona;
    int currentProjectile = 0;
    int identificationArme = 20001;///fixture->setUserData((void*)(identificationArme + numero))pour identifier le numero d'un arme, on soustrait l'object data avec celui ci
    //std::vector<b2Body*>    stickingProjectile;
    float angle;

    b2RevoluteJointDef weldJointDef, weldJointDef1;
    b2Vec2 worldCoordsAnchorPoint;
    b2Joint * joint = nullptr;
    b2Joint * joint2 = nullptr;
    bool jointExist = false;
    bool jointsAlreadyDestroyed = false;

    float distortionFactor = .2f;
    float riseFactor = .5f;
    sf::Clock clock;

    ///SPRINT MAX
    sf::Clock shiftClock;
    bool isShifted = false;
    int lastTime = 0;

    sf::Texture shiftTexture;
    sf::Sprite shiftSprite;
     ///red and green

public:
            Player(sf::RenderWindow& mWindow, b2World* world, TextureHolder* Textures, float radius, std::vector<bodyData> *bDList, std::map<std::string, b2Joint*> *jMap, std::map<std::string, Animation>* animationList);
    void    loadPlayerSprite(TextureHolder* Textures);
    void    addJumpSprite(std::map<std::string, Animation>* animationList);
    void    addShiftSprite(std::map<std::string, Animation>* animationList);
    void    processLogic();
    void    processLogic(sf::RenderWindow& mWindow);
    void    render(sf::RenderWindow& mWindow,sf::Time frameTime, TextureHolder* Textures, sf::Shader* shader);
    void    renderWeapons(sf::RenderWindow& mWindow);

    //void    drawLife(sf::RenderWindow& mWindow);

    void    onCommand(sf::Event e);
    void    attackOn(b2Fixture* fixt);
    void    fire(int projectile);

    void    createWeapons();
    void    loadWeapon(bodyData* data);
    int     getClassName(){return PLAYER;}
    void    takeWeapon();
    int     nextProjectile();
    int     prevProjectile();

    void    clearAll();

    ///FINITE STATE MACHINE
    void    doNormalThings(){}
    void    doAlertThings(){}
    void    doHuntingThings(){}
    void    doAttackThings(){}
    void    doShockedThings(){}

    ///ATK RANGE
    void    addAttackRange(){}

};


#endif // PLAYER_H_INCLUDED
