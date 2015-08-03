#ifndef STATISTIQUES_HPP_INCLUDED
#define STATISTIQUES_HPP_INCLUDED
#include<SFML/Graphics.hpp>
#include<vector>


class Statistiques
{
public:
    sf::RenderWindow& mWindow;
    sf::Texture* texLifeBar;
    sf::Sprite lifeBar;
    sf::Texture* texManaBar;
    sf::Sprite manaBar;

    sf::Texture* texBar;
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

};


#endif // STATISTIQUES_HPP_INCLUDED
