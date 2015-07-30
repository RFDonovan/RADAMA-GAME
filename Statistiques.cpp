#include "Statistiques.hpp"

Statistiques::Statistiques(sf::RenderWindow& window)
: mWindow(window)
{
    texLifeBar = new sf::Texture();
    texLifeBar->loadFromFile("Resources/lifebar.png");
    texLifeBar->setSmooth(true);
    lifeBar.setTexture(*texLifeBar);
    lifeBar.setPosition(0,0);
    lifeBar.scale(0.5f,0.5f);
}

void Statistiques::render(sf::Time frameTime, sf::Shader* shader)
{
    lifeBar.setColor(sf::Color::Black);
    mWindow.draw(lifeBar);
    lifeBar.setPosition(lifeBar.getPosition().x+2,lifeBar.getPosition().y+2);
    lifeBar.setColor(sf::Color::White);
    mWindow.draw(lifeBar);
    lifeBar.setPosition(lifeBar.getPosition().x-2,lifeBar.getPosition().y-2);
}

void Statistiques::adaptPosition(sf::Vector2f place)
{
    lifeBar.move(place);
}
