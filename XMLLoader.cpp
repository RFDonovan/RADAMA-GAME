#include "XMLLoader.hpp"

XMLLoader::XMLLoader(b2World* world)
    : p_world(world)
{
    ///BODY TYPES
    attributeMap["dynamic"] = b2_dynamicBody;
    attributeMap["static"] = b2_staticBody;
    attributeMap["kinematic"] = b2_kinematicBody;
    ///BODY SHAPES
    //attributeMap[]
}

void XMLLoader::loadEntity(std::string XMLFile)
{
    if (!XMLDocument.load_file(XMLFile.c_str()))
    {
        std::cout << "error on loading "<<XMLFile<< "\n";
        return;
    }
    pugi::xml_node bodiesNode = XMLDocument.child("box2d").child("bodies");

    for (pugi::xml_node node = bodiesNode.first_child(); node ; node = node.next_sibling())
        ///BODIES ITERATION
    {
        std::cout<<">>>creation d un "<<node.attribute("type").as_string()<<std::endl;
        b2Body* body;
        body = createBody(attributeMap[node.attribute("type").as_string()], node);
        ///CREATE FIXTURES:
        if(body)
        {
            addFixtures(body, node);
            std::string attrImage(node.attribute("image").as_string());
            if(!(attrImage.compare("null") == 0))
            {
                bodyList.push_back(body);
                loadImage(attrImage);
            }

        }

    }
    //if(bodyList.size()>0)
        //loadSprites();



}
b2Body* XMLLoader::createBody(int bodyType, pugi::xml_node bodyNode)
///CREER UN BODY
{
    b2Body * mBody;
    b2BodyDef myBodyDef;
    myBodyDef.type = (b2BodyType)bodyType;
    //std::cout<<"x:"<<(float32)bodyNode.attribute("x").as_float()/RATIO<<"y:"<<(float32) bodyNode.attribute("y").as_float()/RATIO;
    myBodyDef.position.Set((float32)bodyNode.attribute("x").as_float()/RATIO,-(float32) bodyNode.attribute("y").as_float()/RATIO);
    mBody = p_world->CreateBody(&myBodyDef);
    ///RATIO BODYPOS & IMGPOS
    std::string attrImage(bodyNode.attribute("image").as_string());
    if(!(attrImage.compare("null") == 0))
    {
       pugi::xml_node imagesNode = XMLDocument.child("box2d").child("images");
        pugi::xml_node imageCorrespondant = imagesNode.find_child_by_attribute("image","name",bodyNode.attribute("image").as_string());
        int xRatio = std::abs((float32)bodyNode.attribute("x").as_float() -  imageCorrespondant.attribute("x").as_float());
        int yRatio = std::abs((float32)bodyNode.attribute("y").as_float() -  imageCorrespondant.attribute("y").as_float());
        ratioList.push_back(b2Vec2(xRatio,yRatio));
    }


    std::cout<<"body crEEEEEEEEEEEEEEEEE"<<std::endl;
    return mBody;
}

std::vector<b2Fixture*> XMLLoader::addFixtures(b2Body* body, pugi::xml_node bodyNode)
///ADD APPROPRIATE FIXTURE ACCOURDING TO THE XML CHILD NODES
{
    std::vector<b2Fixture*> fixtureList;
    pugi::xml_node fixturesNode = bodyNode.first_child();
    if(fixturesNode)///si le noeud fixtures existe
    {
        for (pugi::xml_node fixture1 = fixturesNode.first_child(); fixture1 ; fixture1 = fixture1.next_sibling() )
            ///FIXTURES LEVEL ITERATION
        {
            std::string shapeType(fixture1.attribute("shapeType").as_string());
            b2Fixture* fixture = nullptr;

            if(shapeType.compare("edgeShape") == 0)
                fixtureList = createEdgeShape(body, fixture1);

            else if(shapeType.compare("circleShape") == 0)
                fixture = createCircleShape(body, fixture1);

                else if(shapeType.compare("polygonShape") == 0)
                    fixture = createPolygonShape(body, fixture1);
            if(fixture)
                fixtureList.push_back(fixture);
        std::cout<<"fixtures crEEEEEEEEEEEEEEEEE"<<std::endl;
        }
    }
    return fixtureList;

}

std::vector<b2Fixture*>   XMLLoader::createEdgeShape(b2Body* body, pugi::xml_node fixtureNode)
///CREATE EDGES
{

    std::vector<b2Fixture*> listFixtures;

    b2EdgeShape Shape;
    b2FixtureDef FixtureDef;

    FixtureDef.density = fixtureNode.attribute("density").as_float();
    FixtureDef.friction = fixtureNode.attribute("friction").as_float();
    FixtureDef.restitution = fixtureNode.attribute("restitution").as_float();
    FixtureDef.shape = &Shape;

    for(pugi::xml_node vertice = fixtureNode.first_child(); vertice; vertice = vertice.next_sibling())
        ///PARCOURS DES VERTEX
    {
        Shape.Set(
            b2Vec2((float32)vertice.attribute("x").as_int()/RATIO,
                   -(float32)vertice.attribute("y").as_int()/RATIO)
            ,b2Vec2((float32)vertice.next_sibling().attribute("x").as_int()/RATIO,
                    -(float32)vertice.next_sibling().attribute("y").as_int()/RATIO));

        std::cout<<"coord"<<(float32)vertice.attribute("x").as_float()/RATIO<< ","<<vertice.attribute("y").as_int()/RATIO<<" ; "<<vertice.next_sibling().attribute("x").as_int()/RATIO<<","<<vertice.next_sibling().attribute("y").as_int()/RATIO<<"\n";
        FixtureDef.shape = &Shape;
        b2Fixture* fixture = body->CreateFixture(&FixtureDef);

        fixture->SetUserData(this);
        listFixtures.push_back(fixture);
    }

    return listFixtures;

}

b2Fixture*   XMLLoader::createPolygonShape(b2Body* body, pugi::xml_node fixtureNode)
///CREATE POLYGONS
{



    b2FixtureDef FixtureDef;

    FixtureDef.density = fixtureNode.attribute("density").as_float();
    FixtureDef.friction = fixtureNode.attribute("friction").as_float();
    FixtureDef.restitution = fixtureNode.attribute("restitution").as_float();
    //FixtureDef.shape = &Shape;

    int n = std::distance(fixtureNode.children("vertex").begin(), fixtureNode.children("vertex").end());
    b2Vec2 vertices[n+1];
    int i = 0;
    for(pugi::xml_node vertice = fixtureNode.first_child(); vertice; vertice = vertice.next_sibling())
        ///PARCOURS DES VERTEX
    {
        vertices[i].Set(
                                  vertice.attribute("x").as_float()/RATIO,
                                  vertice.attribute("y").as_float()/RATIO

                           );

        std::cout<<"n"<<n<<"x:"<<(float32)vertice.attribute("x").as_int()/RATIO<<"y : "<<-(float32)vertice.attribute("y").as_int()/RATIO<<std::endl;
        i++;
    }
    b2PolygonShape Shape;
    Shape.Set(vertices, n);
    FixtureDef.shape = &Shape;
    b2Fixture* fixture = body->CreateFixture(&FixtureDef);
    //fixture->SetUserData(this);

    //return fixture;
}

b2Fixture*   XMLLoader::createCircleShape(b2Body* body, pugi::xml_node fixtureNode)
///CREATE CIRCLE
{

            b2CircleShape Shape;
            b2FixtureDef FixtureDef;



            FixtureDef.density = fixtureNode.attribute("density").as_float();
            FixtureDef.friction = fixtureNode.attribute("friction").as_float();
            FixtureDef.restitution = fixtureNode.attribute("restitution").as_float();
            Shape.m_p.Set(0,0);
            Shape.m_radius = ( fixtureNode.attribute("circleRadius").as_float() /RATIO);
            FixtureDef.shape = &Shape;
            b2Fixture* fixture = body->CreateFixture(&FixtureDef);
            fixture->SetUserData(this);
            return fixture;
}
/// ///////////////////////////////////////////////
/// //////////////////SPRITES STUFFS////////////////

void XMLLoader::loadImage(std::string imageName)
{
        pugi::xml_node imagesNode = XMLDocument.child("box2d").child("images");
        pugi::xml_node image = imagesNode.find_child_by_attribute("image","name",imageName.c_str());
        std::stringstream ss;
        ss << "./Resources/" <<imageName; ///PEUT ETRE CHANGER?
        std::string filename = ss.str();

        sf::Sprite imgSprite;
        sf::Texture  tex;
        if ( !tex.loadFromFile(filename) )
        {
            std::cout << "Failed to load  spritesheet!" << std::endl;
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
}

void XMLLoader::loadSprites()
{
    pugi::xml_node imagesNode = XMLDocument.child("box2d").child("images");
    std::cout<<"Images loading........\n";
    for(pugi::xml_node image = imagesNode.first_child(); image; image = image.next_sibling())
        ///IMAGE BODIES ITERATIONS
    {

        std::stringstream ss;
        ss << "./Resources/" <<image.attribute("path").as_string(); ///PEUT ETRE CHANGER?
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

void XMLLoader::render(sf::RenderWindow& mWindow, sf::Shader* shader)
{
    int i = 0;
    std::cout << "TEXLIST"<<texList.size()<< "ratioLIST"<<ratioList.size()<< "spriteLIST"<<spriteList.size()<< "bodyLIST"<< bodyList.size() << std::endl;
    for (sf::Sprite s : spriteList )
    {
        std::cout << "FOR DANS RENDER" << std::endl;
        s.setTexture(texList[i]);
        if(bodyList[i]->GetType() == b2_dynamicBody)
        {
            s.setPosition((bodyList[i]->GetPosition().x*RATIO)-ratioList[i].x,  (bodyList[i]->GetPosition().y*RATIO)+ratioList[i].y);
            s.setRotation(bodyList[i]->GetAngle()/ RADTODEG);
        }

        mWindow.draw(s);

        i++;
        std::cout << "I++" << std::endl;
    }

}


