#ifndef STATISTIQUES_HPP_INCLUDED
#define STATISTIQUES_HPP_INCLUDED
#include<SFML/Graphics.hpp>
#include<vector>
#include "TextureHolder.h"


class Statistiques
{
public:
    TextureHolder Textures;

    sf::RenderWindow& mWindow;
    sf::Sprite lifeBar;
    sf::Sprite manaBar;

    sf::Sprite spriteBar;
    int     playerLife = 0;
    int     playerMana = 0;
    sf::Clock lifeClock;
    std::vector<sf::Sprite> spriteList;

public:
            Statistiques(sf::RenderWindow& window);
    void    render(sf::Time frameTime, sf::Shader* shader);
    void    adaptPosition(sf::Vector2f place);
    void    updateLife(int life);
    void    updateMana(int mana);
    void    resetView();

};


#endif // STATISTIQUES_HPP_INCLUDED
