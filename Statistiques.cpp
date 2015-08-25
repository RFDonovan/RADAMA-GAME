#include "Statistiques.hpp"

Statistiques::Statistiques(sf::RenderWindow& window)
: mWindow(window)
{
    ///BAR CONTAINER
    Textures.loadFromFile("barContainer", "Resources/Images/lifebar_.png");
    spriteBar.setTexture(*Textures.getTexture("barContainer"));
    spriteBar.setPosition(0,0);
    spriteList.push_back(spriteBar);

    ///LIFEBAR
    Textures.loadFromFile("pLifeBar", "Resources/Images/lifebar2.png");
    lifeBar.setTexture(*Textures.getTexture("pLifeBar"));
    lifeBar.setPosition(60,35);
    spriteList.push_back(lifeBar);

    ///MANABAR
    Textures.loadFromFile("pManaBar", "Resources/Images/manabar.png");
    manaBar.setTexture(*Textures.getTexture("pManaBar"));
    manaBar.setPosition(65,10);
    spriteList.push_back(manaBar);
    //lifeBar.scale(0.5f,0.5f);

    ///NB PROJECTILE
    if (!MyFont.loadFromFile("Resources/CHIZZ___.ttf"))
    {
        // Error...
    }

    nbProjectile.setFont(MyFont);
    nbProjectile.setCharacterSize(40);
    nbProjectile.setString("Andrana");
    nbProjectile.setColor(sf::Color::Cyan);
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

    manaBar.setTextureRect(sf::IntRect(0,0,playerMana*Textures.getTexture("pManaBar")->getSize().x/100,Textures.getTexture("pManaBar")->getSize().y));
    manaBar.setColor(sf::Color::Black);
    mWindow.draw(manaBar);
    manaBar.setPosition(manaBar.getPosition().x+2,manaBar.getPosition().y+2);
    manaBar.setColor(sf::Color::White);
    if(playerMana>0)
        mWindow.draw(manaBar);

    manaBar.setPosition(manaBar.getPosition().x-2,manaBar.getPosition().y-2);

    /// DRAW LIFEBAR
    lifeBar.setTextureRect(sf::IntRect(0,0,playerLife*Textures.getTexture("pLifeBar")->getSize().x/100,Textures.getTexture("pLifeBar")->getSize().y));
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

    ///nb projectile based on lifebar position
    std::stringstream ss;
    ss<<"x"<<projectileCount;
    nbProjectile.setString(ss.str());
    nbProjectile.setPosition(lifeBar.getPosition().x+250,lifeBar.getPosition().y);
    mWindow.draw(nbProjectile);
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
void Statistiques::updateProjectile(int pCount)
{
    projectileCount = pCount;
}
void Statistiques::resetView()
{
    spriteBar.setPosition(0,0);
    lifeBar.setPosition(60,35);
    manaBar.setPosition(65,10);
}
