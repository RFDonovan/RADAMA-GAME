#ifndef PLAYER_H_INCLUDED
#define PLAYER_H_INCLUDED
#include "Entity.hpp"
#include "Structures.hpp"

class Player: public Entity
{

public:


    //animation control
    enum Projectile{
        lefona,
        vato,
        afo,
        sokina,
    };

    sf::Sprite stuffSprite, weaponSprite;
    sf::Shader *Shader;

    bool noKeyWasPressed = true;
    float desiredVel;
    float jumpLimit = 5.f;

    std::vector<b2Body*>    weapons;
    std::map<int, b2Body*>  weaponsMap;///map <PROJECTILEREF, BODY>
    std::map<std::string, int>  nameToWeapon; ///map <NOM, PROJECTILEREF>
    std::map<int, std::string>  weaponToName;
    //Projectile currentProjectile = Projectile::lefona;
    int currentProjectile = 0;
    int identificationArme = 20000;///fixture->setUserData((void*)(identificationArme + numero))pour identifier le numero d'un arme, on soustrait l'object data avec celui ci
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



public:
            Player(sf::RenderWindow& mWindow, b2World* world, TextureHolder* Textures, float radius, float32 x, float32 y, float w, float h);
    void    loadPlayerSprite(TextureHolder* Textures);
    void    processLogic();
    void    processLogic(sf::RenderWindow& mWindow);
    void    render(sf::RenderWindow& mWindow,sf::Time frameTime, TextureHolder* Textures);
    void    renderWeapons(sf::RenderWindow& mWindow);
    void    onCommand(sf::Event e);
    void    attack();
    void    fire(int projectile);

    void    createWeapons();
    void    loadWeapon(bodyData* data);
    int     getClassName(){return PLAYER;}
    void    stickProjectile( int projectile, b2Fixture* fixtureTarget);
    void    impactTo(b2Fixture* fixtureSource, b2Fixture* fixtureTarget, float impulse);
    void    stickAll();
    void    jointDestroyer();

    void    clearAll();

};


#endif // PLAYER_H_INCLUDED
