#ifndef DEBUGDRAW_HPP_INCLUDED
#define DEBUGDRAW_HPP_INCLUDED

#include <Box2D/Box2D.h>
#include <iostream>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include "Globals.h"
class DebugDraw : public b2Draw
{
public:
    DebugDraw(sf::RenderWindow &window);
    virtual ~DebugDraw();
    void DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color);
    void DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color);
    void DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color);
    void DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color);
    void DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color);
    ///void DrawParticles(const b2Vec2 *centers, float32 radius, const b2ParticleColor *colors, int32 count){}

    void DrawTransform(const b2Transform& xf);
    void DrawPoint(const b2Vec2& p, float32 size, const b2Color& color);
    void DrawString(int x, int y, const char* string);
    void DrawAABB(b2AABB* aabb, const b2Color& color);
    sf::Color B2SFColor(const b2Color &color, int alpha);
    void DrawMouseJoint(b2Vec2& p1, b2Vec2& p2, const b2Color &boxColor, const b2Color &lineColor);
private:
    sf::RenderWindow *window;
};

#endif // DEBUGDRAW_HPP_INCLUDED
