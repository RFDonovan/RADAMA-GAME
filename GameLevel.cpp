#include "GameLevel.hpp"

GameLevel::GameLevel(b2World* world)
    : p_world(world)
{

}
///*
int GameLevel::loadLevel(std::string filename)
{

    if (!XMLDocument.load_file(filename.c_str()))
    {
        std::cout << "error on loading "<<filename<< "\n";
        return -1;
    }
    loadSprites();
    pugi::xml_node bodiesNode = XMLDocument.child("box2d").child("bodies");

    for (pugi::xml_node node = bodiesNode.first_child(); node ; node = node.next_sibling())
        ///BODIES LEVEL ITERATION
    {
        std::cout<< node.name()<<"-----------\n";
        createBody(node, node.child("fixtures"));

    }

    return 1;
}

void GameLevel::createBody(pugi::xml_node body, pugi::xml_node fixtures)
{
    std::map<std::string , int> shapeMap;
    std::map<std::string, b2BodyType> bodyType;

    bodyType["dynamic"] = b2_dynamicBody;
    bodyType["static"] = b2_staticBody;
    bodyType["kinematic"] = b2_kinematicBody;


    /// BODY DEFINITION
    b2Body * mBody;
    b2BodyDef myBodyDef;
    myBodyDef.type = bodyType[body.attribute("type").as_string()];
    std::cout<<"x:"<<(float32)body.attribute("x").as_float()/RATIO<<"y:"<<(float32) body.attribute("y").as_float()/RATIO;
    myBodyDef.position.Set((float32)body.attribute("x").as_float()/RATIO,-(float32) body.attribute("y").as_float()/RATIO);
    mBody = p_world->CreateBody(&myBodyDef);

    //mBody->SetUserData(this);
    /// SHAPE AND FIXTURE DEFINITION

    pugi::xml_node imagesNode = XMLDocument.child("box2d").child("images");
    pugi::xml_node imageCorrespondant = imagesNode.find_child_by_attribute("image","name",body.attribute("image").as_string());
    int xRatio = std::abs((float32)body.attribute("x").as_float() -  imageCorrespondant.attribute("x").as_float());
    int yRatio = std::abs((float32)body.attribute("y").as_float() -  imageCorrespondant.attribute("y").as_float());
    ratioList.push_back(b2Vec2(xRatio,yRatio));


    for (pugi::xml_node nodeSon = fixtures.first_child(); nodeSon ; nodeSon = nodeSon.next_sibling() )
        ///FIXTURES LEVEL ITERATION
    {

        std::cout<< "--------"<<nodeSon.name()<<"-----------\n";
        //b2EdgeShape Shape;
        //b2Shape Shape;
        std::string shapeType(nodeSon.attribute("shapeType").as_string());

        if(shapeType.compare("edgeShape") == 0)
            /// -------EDGESHAPE
        {


            b2EdgeShape Shape;
            b2FixtureDef FixtureDef;

            FixtureDef.density = nodeSon.attribute("density").as_float();
            //FixtureDef.friction = 1.0f;
            FixtureDef.friction = nodeSon.attribute("friction").as_float();
            //FixtureDef.restitution = 0.f;
            FixtureDef.shape = &Shape;
///*
            for(pugi::xml_node vertice = nodeSon.first_child(); vertice; vertice = vertice.next_sibling())
                ///PARCOURS DES VERTEX
            {
                Shape.Set(
                    b2Vec2((float32)vertice.attribute("x").as_int()/RATIO,
                           -(float32)vertice.attribute("y").as_int()/RATIO)
                    ,b2Vec2((float32)vertice.next_sibling().attribute("x").as_int()/RATIO,
                            -(float32)vertice.next_sibling().attribute("y").as_int()/RATIO));
                /*Shape.m_vertex0.Set((vertice.attribute("x").as_int()/RATIO)-1, -(float32)vertice.attribute("y").as_int()/RATIO);
                Shape.m_vertex0.Set((vertice.next_sibling().attribute("x").as_int()/RATIO ) -1, -(float32)vertice.next_sibling().attribute("y").as_int()/RATIO);
                Shape.m_hasVertex0 = true;
                Shape.m_hasVertex3 = true;*/

                std::cout<<"coord"<<(float32)vertice.attribute("x").as_float()/RATIO<< ","<<vertice.attribute("y").as_int()/RATIO<<" ; "<<vertice.next_sibling().attribute("x").as_int()/RATIO<<","<<vertice.next_sibling().attribute("y").as_int()/RATIO<<"\n";
                FixtureDef.shape = &Shape;
                b2Fixture* fixture = mBody->CreateFixture(&FixtureDef);

                fixture->SetUserData(this);
            }

        }
        /// TAPITRA -------EDGESHAPE

        if(shapeType.compare("circleShape") == 0)
            /// -------CIRCLESHAPE
        {
            b2CircleShape Shape;
            b2FixtureDef FixtureDef;

            std::cout<< "--------"<<nodeSon.name()<<"-----------\n";

            FixtureDef.density = nodeSon.attribute("density").as_float();
            //FixtureDef.friction = 1.0f;
            FixtureDef.friction = nodeSon.attribute("friction").as_float();
            FixtureDef.restitution = nodeSon.attribute("restitution").as_float();
            //FixtureDef.restitution = 0.f;
            Shape.m_p.Set(0,0);
            Shape.m_radius = ( nodeSon.attribute("circleRadius").as_float() /RATIO);
            FixtureDef.shape = &Shape;
            b2Fixture* fixture = mBody->CreateFixture(&FixtureDef);
            fixture->SetUserData(this);

        }
        /// TAPITRA-------CIRCLESHAPE
        //mBody->SetUserData(this);

    }///


    mBody->SetUserData(this);
    bodyList.push_back(mBody);
}
//*/
void GameLevel::loadSprites()
{
    pugi::xml_node imagesNode = XMLDocument.child("box2d").child("images");
    std::cout<<"Images loading........\n";
    for(pugi::xml_node image = imagesNode.first_child(); image; image = image.next_sibling())
        ///IMAGE BODIES ITERATIONS
    {

        std::stringstream ss;
        ss << "./Resources/" <<image.attribute("path").as_string();
        std::string filename = ss.str();

        sf::Sprite imgSprite;
        sf::Texture  tex;
        if ( !tex.loadFromFile(filename) )
        {
            std::cout << "Failed to load  spritesheet!" << std::endl;
            continue;
        }



        //std::cout<<filename<<" Images 1........\n";
        tex.setSmooth(true);
        imgSprite.setTexture(tex);
        imgSprite.setScale(
            image.attribute("scaleX").as_float(),
            image.attribute("scaleY").as_float()
        );
        imgSprite.setPosition(
            image.attribute("x").as_float(),
            -(image.attribute("y").as_float())
        );
        imgSprite.setRotation(image.attribute("rotation").as_float());
        imgSprite.setOrigin(
            (tex.getSize().x)/2,
            (tex.getSize().y)/2
        );
        spriteList.push_back(imgSprite);
        texList.push_back(tex);
        //std::cout<<"TAILLE TEX:"<<(imgSprite.getScale().x*tex.getSize().x)/2<<std::endl;

    }

}
void GameLevel::clearAll()
{

}

void    GameLevel::render(sf::RenderWindow& mWindow)
{

}
void    GameLevel::render(sf::RenderWindow& mWindow, sf::Shader* shader)
{
    int i = 0;
    for (sf::Sprite s : spriteList )
    {
        //s.setTexture(tex)
        s.setTexture(texList[i]);
        //s.setPosition(bodyList[i]->GetPosition().x*RATIO + ( s.getPosition().x - bodyList[i]->GetPosition().x*RATIO), - bodyList[i]->GetPosition().y*RATIO + ( s.getPosition().y + bodyList[i]->GetPosition().x*RATIO));
        if(bodyList[i]->GetType() == b2_dynamicBody)
        {
            s.setPosition((bodyList[i]->GetPosition().x*RATIO)-ratioList[i].x,  (bodyList[i]->GetPosition().y*RATIO)+ratioList[i].y);
            s.setRotation(bodyList[i]->GetAngle()/ RADTODEG);
        }

        //std::cout<<"RATIO: "<<ratioList[i].x<<","<<ratioList[i].y<<std::endl;
        mWindow.draw(s);

        i++;
    }
    //std::cout<<spriteList.size();

}

void    GameLevel::startContact(b2Fixture   *fixture)
{

}
void    GameLevel::endContact(b2Fixture   *fixture)
{

}

GameLevel::~GameLevel()
{

}
