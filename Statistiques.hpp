#ifndef STATISTIQUES_HPP_INCLUDED
#define STATISTIQUES_HPP_INCLUDED
#include<SFML/Graphics.hpp>


class Statistiques
{
public:
    sf::RenderWindow& mWindow;
    sf::Texture* texLifeBar;
    sf::Sprite lifeBar;
public:
            Statistiques(sf::RenderWindow& window);
    void    render(sf::Time frameTime, sf::Shader* shader);
    void    adaptPosition(sf::Vector2f place);

};


#endif // STATISTIQUES_HPP_INCLUDED
