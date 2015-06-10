#ifndef PLAYER_H_INCLUDED
#define PLAYER_H_INCLUDED
#include "Entity.hpp"

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
    std::map<Projectile, b2Body*>  weaponsMap;
    Projectile currentProjectile = Projectile::lefona;
    std::vector<b2Body*>    stickingProjectile;
    float angle;

    b2RevoluteJointDef weldJointDef;
    b2Vec2 worldCoordsAnchorPoint;
    b2Joint * joint = nullptr;
    bool jointExist = false;

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
    void    fire(Projectile projectile);

    void    createWeapons();
    int     getClassName(){return PLAYER;}
    void    stickProjectile(b2Fixture* fixtureTarget);
    void    stickAll();

};


#endif // PLAYER_H_INCLUDED
