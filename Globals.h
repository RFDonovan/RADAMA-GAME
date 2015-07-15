#ifndef GLOBALS_H_INCLUDED
#define GLOBALS_H_INCLUDED
// 1 meter (box2d) is more or less 64 pixels (sfml)
#define RATIO 30.0f
#define PIXELS_PER_METER RATIO
// 64 pixels (sfml) are more or less 1 meter (box2d)
#define UNRATIO (1.0F/RATIO)
#define METERS_PER_PIXEL UNRATIO
//formula to convert radians to degrees = (radians * (pi/180))
#define RADTODEG (b2_pi / 180.0)

#define WINDOW_W    1024//800
#define WINDOW_H    768//600

#define BOXSIZE_W 36.f //74
#define BOXSIZE_H 150.f

#define K_LEFT sf::Keyboard::Q
#define K_RIGHT sf::Keyboard::D

/*
enum objectType{
    Ground,
    Entity,
    Projectile,
};
*/
#define ENTITY 1
#define GROUND 2
#define PLAYER 5
#define HUMAN 4
#define PROJECTILE 3
#define ITEM 6
#define LEVEL

#define JOINTRANGE 30000

class ObjectType
{
public:

    int hardness = 100;
    virtual int getObjectType() = 0;
    virtual int getClassName() = 0;
};


#endif // GLOBALS_H_INCLUDED
