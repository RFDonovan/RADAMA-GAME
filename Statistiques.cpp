#include "Statistiques.hpp"

Statistiques::Statistiques(sf::RenderWindow& window)
: mWindow(window)
{
    ///BAR CONTAINER
    texBar = new sf::Texture();
    texBar->loadFromFile("Resources/lifebar_.png");
    texBar->setSmooth(true);
    spriteBar.setTexture(*texBar);
    spriteBar.setPosition(0,0);
    spriteList.push_back(spriteBar);

    ///LIFEBAR
    texLifeBar = new sf::Texture();
    texLifeBar->loadFromFile("Resources/lifebar2.png");
    texLifeBar->setSmooth(true);
    lifeBar.setTexture(*texLifeBar);
    lifeBar.setPosition(60,35);
    spriteList.push_back(lifeBar);

    ///MANABAR
    texManaBar = new sf::Texture();
    texManaBar->loadFromFile("Resources/manabar.png");
    texManaBar->setSmooth(true);
    manaBar.setTexture(*texManaBar);
    manaBar.setPosition(65,10);
    spriteList.push_back(manaBar);
    //lifeBar.scale(0.5f,0.5f);
}

void Statistiques::render(sf::Time frameTime, sf::Shader* shader)
{
    /// DRAW BAR CONTAINER
    spriteBar.setColor(sf::Color::Black);
    mWindow.draw(spriteBar);
    spriteBar.setPosition(spriteBar.getPosition().x+2,spriteBar.getPosition().y+2);
    spriteBar.setColor(sf::Color::White);
    mWindow.draw(spriteBar);

    spriteBar.setPosition(spriteBar.getPosition().x-2,spriteBar.getPosition().y-2);
    /// DRAW MANABAR

    manaBar.setTextureRect(sf::IntRect(0,0,playerMana*texManaBar->getSize().x/100,texManaBar->getSize().y));
    manaBar.setColor(sf::Color::Black);
    mWindow.draw(manaBar);
    manaBar.setPosition(manaBar.getPosition().x+2,manaBar.getPosition().y+2);
    manaBar.setColor(sf::Color::White);
    if(playerMana>0)
        mWindow.draw(manaBar);

    manaBar.setPosition(manaBar.getPosition().x-2,manaBar.getPosition().y-2);

    /// DRAW LIFEBAR
    lifeBar.setTextureRect(sf::IntRect(0,0,playerLife*texLifeBar->getSize().x/100,texLifeBar->getSize().y));
    lifeBar.setColor(sf::Color::Black);
    mWindow.draw(lifeBar);
    lifeBar.setPosition(lifeBar.getPosition().x+2,lifeBar.getPosition().y+2);
    lifeBar.setColor(sf::Color::White);


    if(playerLife == 100)
    {
        if(lifeClock.getElapsedTime().asMilliseconds()<700)
        {
            lifeBar.setColor(sf::Color(255,255,255,220 ));

        }
        else
            lifeBar.setColor(sf::Color(255,255,255,255));
        if(lifeClock.getElapsedTime().asMilliseconds()>1600)
        {
            lifeClock.restart();
        }
    }
    else if(playerLife <= 30)
    {
        if(lifeClock.getElapsedTime().asMilliseconds()<100)
        {
            lifeBar.setColor(sf::Color(200,50,50,220 ));

        }
        else
            lifeBar.setColor(sf::Color(250,60,150,255));
        if(lifeClock.getElapsedTime().asMilliseconds()>200)
        {
            lifeClock.restart();
        }
    }
    else if(playerLife>30 && playerLife<80)
        lifeBar.setColor(sf::Color::Yellow);
    if(playerLife>0)
        mWindow.draw(lifeBar);

    lifeBar.setPosition(lifeBar.getPosition().x-2,lifeBar.getPosition().y-2);


}

void Statistiques::adaptPosition(sf::Vector2f place)
{
//    for(int i=0 ; i < spriteList.size(); i++)
//    {
//        (spriteList[i]).move(place);
//    }
    spriteBar.move(place);
    manaBar.move(place);
    lifeBar.move(place);
}

void Statistiques::updateLife(int life)
{
    playerLife = life;
}
void Statistiques::updateMana(int mana)
{
    playerMana = mana;
}
