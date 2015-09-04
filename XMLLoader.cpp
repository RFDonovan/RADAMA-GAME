#include "XMLLoader.hpp"
/**
    BOUNDARY =          0x0001,
    FRIENDLY_SHIP =     0x0002,
    ENEMY_SHIP =        0x0004,
    FRIENDLY_AIRCRAFT = 0x0008,
    ENEMY_AIRCRAFT =    0x0010,
*/
XMLLoader::XMLLoader(b2World* world, TextureHolder* th)
    : p_world(world)
    , textureHolder(th)
{
    ///BODY TYPES
    attributeMap["dynamic"] = b2_dynamicBody;
    attributeMap["static"] = b2_staticBody;
    attributeMap["kinematic"] = b2_kinematicBody;
    ///BODY SHAPES
    //attributeMap[]
}


std::vector<Item*> XMLLoader::loadItems(std::string dir)
{
    std::vector<Item*> itemList;
    pugi::xml_node itemsNode = XMLDocument.child("box2d").child("metaData");
    for (pugi::xml_node node = itemsNode.first_child(); node ; node = node.next_sibling())
        ///ITEMS ITERATION
    {
        std::stringstream ss;
        ss <<dir<<node.attribute("image").as_string();
        std::string filename = ss.str();

        Item* item = new Item(p_world,
                              filename,
                              (float32)node.attribute("x").as_float(),
                              -(float32)node.attribute("y").as_float(),
                              node.attribute("mana").as_float(),
                              node.attribute("life").as_float()
                              );
        itemList.push_back(item);
    std::cout << "XMLLoader::loadItems item : "<<node.attribute("name").as_string()<< "\n";
    }
    return itemList;
}

std::vector<bodyData> XMLLoader::loadXML(std::string XMLFile, std::string dir)
{
    std::vector<bodyData> bodyDataList;
    std::cout << "directory: "<<dir<< "\n";
    std::cout << "file: "<<XMLFile<< "\n";

    if (!XMLDocument.load_file(XMLFile.c_str()))
    {
        std::cout << "error on loading "<<XMLFile<< "\n";
        //return NULL;
    }
    std::cout << "on cherche les bodiesnode....\n";
    pugi::xml_node bodiesNode = XMLDocument.child("box2d").child("bodies");
    std::cout << "file: "<<XMLFile<< "\n";
    for (pugi::xml_node node = bodiesNode.first_child(); node ; node = node.next_sibling())
        ///BODIES ITERATION
    {
        std::cout<<">>>creation d un "<<node.attribute("type").as_string()<<std::endl;
        b2Body* body;
        body = createBody(attributeMap[node.attribute("type").as_string()], node);

        std::stringstream ss;
        ss<<node.attribute("name").as_string();
        //ss<<"bieber";
        bodyData bData;
        bData.body = body;///Structures.hpp
        bData.name = ss.str();///Structures.hpp
        std::cout << "error on loading "<<bData.name<< "\n";
        ///CREATE FIXTURES:
        if(body)
        {
            nameToBody[ss.str()] = body;
            std::map<std::string, b2Fixture*>   mapFixture = addFixtures(body, node);
            bData.mapFixture = mapFixture;
            std::stringstream ss2;
            std::size_t found = XMLFile.find(dir.c_str());
//            if (found!=std::string::npos)
//                ss2<<dir<<node.attribute("image").as_string();
//            else
            ss2<<node.attribute("image").as_string();

            std::cout << "image : "<<ss2.str()<< "\n";
            std::string attrImage(ss2.str());


            if(!(attrImage.compare("null") == 0))
            {
                bodyList.push_back(body);
                bData.sprite = loadImage(attrImage, dir);///Structures.hpp
            }
            else
            {
                bData.sprite = nullptr;
            }
            bodyDataList.push_back(bData);
        }

        //std::cout<<"XMLLoader::loadXML----fin for\n";

    }
    //if(bodyList.size()>0)
    //loadSprites();
    //return bData;

    pugi::xml_node jointsNode = XMLDocument.child("box2d").child("joints");
    jointMap.clear();
    for (pugi::xml_node node = jointsNode.first_child(); node ; node = node.next_sibling())
        ///JOINTS ITERATION
    {
        std::string jointType(node.attribute("type").as_string());
        std::string jointName(node.attribute("name").as_string());
        b2Joint* j;
        if(jointType.compare("RevoluteJoint") == 0)
            j = addRevJoint(node);
        else if(jointType.compare("RevoluteJoint") == 0)
            j = addWeldJoint(node);
        jointMap[jointName] = j;
    }

    return bodyDataList;


}

std::map<std::string, b2Joint*> XMLLoader::GetCurrentJointMap()
{
    return jointMap;
}
b2Joint* XMLLoader::addWeldJoint(pugi::xml_node jointNode)
{
    return nullptr;
}

b2Joint* XMLLoader::addRevJoint(pugi::xml_node jointNode)
{
    b2RevoluteJointDef rJointDef;
    rJointDef.bodyA = nameToBody[jointNode.attribute("bodyA").as_string()];
    rJointDef.bodyB = nameToBody[jointNode.attribute("bodyB").as_string()];

    rJointDef.collideConnected = jointNode.attribute("collideConnected").as_bool();

//    rJointDef.localAnchorA = rJointDef.bodyA->GetLocalPoint(
//                                                            m_body->GetWorldPoint(
//                                                                                  b2Vec2(jointNode.attribute("x").as_float()/RATIO,
//                                                                                         jointNode.attribute("y").as_float()/RATIO
//                                                                                         )
//                                                                                  )
//                                                            );
    float Ax = jointNode.attribute("Ax").as_float();
    float Ay = jointNode.attribute("Ay").as_float();
    float Bx = jointNode.attribute("Bx").as_float();
    float By = jointNode.attribute("By").as_float();
    rJointDef.localAnchorA = rJointDef.bodyA->GetLocalPoint( rJointDef.bodyA->GetWorldPoint(b2Vec2(Ax/RATIO, Ay/RATIO)) );
    rJointDef.localAnchorB = rJointDef.bodyB->GetLocalPoint( rJointDef.bodyB->GetWorldPoint(b2Vec2(Bx/RATIO, By/RATIO)) );

    rJointDef.referenceAngle = rJointDef.bodyB->GetAngle() - rJointDef.bodyA->GetAngle();

    rJointDef.enableLimit = jointNode.attribute("enableLimit").as_bool();
    b2Joint * j = p_world->CreateJoint( &rJointDef );
    return j;
}

b2Body* XMLLoader::createBody(int bodyType, pugi::xml_node bodyNode)
///CREER UN BODY
{
    b2Body * mBody;
    b2BodyDef myBodyDef;
    myBodyDef.type = (b2BodyType)bodyType;
    myBodyDef.bullet = bodyNode.attribute("bullet").as_bool();
    myBodyDef.fixedRotation = bodyNode.attribute("fixedRotation").as_bool();
    if(strcmp(bodyNode.attribute("isActive").value(), "") != 0)
        myBodyDef.active = bodyNode.attribute("isActive").as_bool();
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

        float angleRatio = std::abs((mBody->GetAngle()/RADTODEG) -  imageCorrespondant.attribute("rotation").as_float());
        angleRatioList.push_back(angleRatio);
    }


    std::cout<<"body crEEEEEEEEEEEEEEEEE"<<std::endl;
    return mBody;
}

//std::vector<b2Fixture*> XMLLoader::addFixtures(b2Body* body, pugi::xml_node bodyNode)
std::map<std::string, b2Fixture*> XMLLoader::addFixtures(b2Body* body, pugi::xml_node bodyNode)
///ADD APPROPRIATE FIXTURE ACCOURDING TO THE XML CHILD NODES
{
    std::vector<b2Fixture*> fixtureList;
    std::map<std::string, b2Fixture*>   mapFixture;
    pugi::xml_node fixturesNode = bodyNode.first_child();
    if(fixturesNode)///si le noeud fixtures existe
    {
        for (pugi::xml_node fixture1 = fixturesNode.first_child(); fixture1 ; fixture1 = fixture1.next_sibling() )
            ///FIXTURES LEVEL ITERATION
        {
            std::string shapeType(fixture1.attribute("shapeType").as_string());
            std::string fixtureName(fixture1.attribute("name").as_string());
            b2Fixture* fixture = nullptr;
            std::cout<<fixture<<" ceci est un nullptr fixture ****"<<std::endl;
            if(shapeType.compare("edgeShape") == 0)
                fixtureList = createEdgeShape(body, fixture1);///ON A UN PROBLEME CAR ICI ON ECRASE LA FIXTURELIST, ON N'AJOUTE PAS

            else if(shapeType.compare("circleShape") == 0)
            {
                fixture = createCircleShape(body, fixture1);
                std::cout<<fixture<<" Circle fixture ****"<<fixtureName<<"***** crEEEEEEEEEEEEEEEEE"<<std::endl;
            }


            else if(shapeType.compare("polygonShape") == 0)
            {
                fixture = createPolygonShape(body, fixture1);
                std::cout<<fixture<<" Polygon fixture ****"<<fixtureName<<"***** crEEEEEEEEEEEEEEEEE"<<std::endl;
            }

            else if(shapeType.compare("rectangleShape") == 0)
                {
                    fixture = createRectangleShape(body, fixture1);
                    std::cout<<fixture<<" rectangle fixture ****"<<fixtureName<<"***** crEEEEEEEEEEEEEEEEE"<<std::endl;
                }
            if(fixture)
            {
                fixtureList.push_back(fixture);
                mapFixture[fixtureName] = fixture;
                std::cout<<fixture<<"fixtures ****"<<fixtureName<<"***** crEEEEEEEEEEEEEEEEE"<<std::endl;
            }

            std::cout<<"fixtures "<<fixtureName<<" crEEEEEEEEEEEEEEEEE"<<std::endl;

        }
    }
    //return fixtureList;
    return mapFixture;

}

std::vector<b2Fixture*>   XMLLoader::createEdgeShape(b2Body* body, pugi::xml_node fixtureNode)
///CREATE EDGES
{

    std::vector<b2Fixture*> listFixtures;

    b2EdgeShape Shape;
    b2FixtureDef FixtureDef;
    ///COLLISION FILTERING
    if(strcmp(fixtureNode.attribute("categoryBits").value(), "") != 0)
        FixtureDef.filter.categoryBits = (uint16)fixtureNode.attribute("categoryBits").as_uint();
    //exit(-1);
    if(strcmp(fixtureNode.attribute("maskBits").value(), "") != 0)
        FixtureDef.filter.maskBits = (uint16)fixtureNode.attribute("maskBits").as_uint();
    ///------------------

    FixtureDef.density = fixtureNode.attribute("density").as_float();
    FixtureDef.friction = fixtureNode.attribute("friction").as_float();
    FixtureDef.restitution = fixtureNode.attribute("restitution").as_float();
    FixtureDef.isSensor = fixtureNode.attribute("isSensor").as_bool();
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

        ///fixture->SetUserData(this);
        listFixtures.push_back(fixture);
    }

    return listFixtures;

}

b2Fixture*   XMLLoader::createPolygonShape(b2Body* body, pugi::xml_node fixtureNode)
///CREATE POLYGONS
{

    b2FixtureDef FixtureDef;
    ///COLLISION FILTERING
    if(strcmp(fixtureNode.attribute("categoryBits").value(), "") != 0)
        FixtureDef.filter.categoryBits = (uint16)fixtureNode.attribute("categoryBits").as_uint();
    //exit(-1);
    if(strcmp(fixtureNode.attribute("maskBits").value(), "") != 0)
        FixtureDef.filter.maskBits = (uint16)fixtureNode.attribute("maskBits").as_uint();
    ///------------------

    FixtureDef.density = fixtureNode.attribute("density").as_float();
    FixtureDef.friction = fixtureNode.attribute("friction").as_float();
    FixtureDef.restitution = fixtureNode.attribute("restitution").as_float();
    FixtureDef.isSensor = fixtureNode.attribute("isSensor").as_bool();
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

    return fixture;
}

b2Fixture*   XMLLoader::createRectangleShape(b2Body* body, pugi::xml_node fixtureNode)
///CREATE POLYGONS
{

    b2FixtureDef FixtureDef;
    ///COLLISION FILTERING
    if(strcmp(fixtureNode.attribute("categoryBits").value(), "") != 0)
        FixtureDef.filter.categoryBits = (uint16)fixtureNode.attribute("categoryBits").as_uint();
    //exit(-1);
    if(strcmp(fixtureNode.attribute("maskBits").value(), "") != 0)
        FixtureDef.filter.maskBits = (uint16)fixtureNode.attribute("maskBits").as_uint();
    ///------------------
    std::cout<<"categoryBits: "<<(uint16)fixtureNode.attribute("categoryBits").as_uint()<<std::endl;
    std::cout<<"maskbit: "<<(uint16)fixtureNode.attribute("maskBits").as_uint()<<std::endl;

    FixtureDef.density = fixtureNode.attribute("density").as_float();
    FixtureDef.friction = fixtureNode.attribute("friction").as_float();
    FixtureDef.restitution = fixtureNode.attribute("restitution").as_float();
    FixtureDef.isSensor = fixtureNode.attribute("isSensor").as_bool();
    //FixtureDef.shape = &Shape;


    b2PolygonShape Shape;
    Shape.SetAsBox(fixtureNode.attribute("width").as_float()/RATIO,
                   fixtureNode.attribute("height").as_float()/RATIO);
    FixtureDef.shape = &Shape;
    b2Fixture* fixture = body->CreateFixture(&FixtureDef);
    //fixture->SetUserData(this);

    return fixture;
}

b2Fixture*   XMLLoader::createCircleShape(b2Body* body, pugi::xml_node fixtureNode)
///CREATE CIRCLE
{

    b2CircleShape Shape;
    b2FixtureDef FixtureDef;

    ///COLLISION FILTERING
    if(strcmp(fixtureNode.attribute("categoryBits").value(), "") != 0)
        FixtureDef.filter.categoryBits = (uint16)fixtureNode.attribute("categoryBits").as_uint();
    //exit(-1);
    if(strcmp(fixtureNode.attribute("maskBits").value(), "") != 0)
        FixtureDef.filter.maskBits = (uint16)fixtureNode.attribute("maskBits").as_uint();
    ///------------------

    FixtureDef.density = fixtureNode.attribute("density").as_float();
    FixtureDef.friction = fixtureNode.attribute("friction").as_float();
    FixtureDef.restitution = fixtureNode.attribute("restitution").as_float();
    FixtureDef.isSensor = fixtureNode.attribute("isSensor").as_bool();
    Shape.m_p.Set(0,0);
    Shape.m_radius = ( fixtureNode.attribute("circleRadius").as_float() /RATIO);
    FixtureDef.shape = &Shape;
    b2Fixture* fixture = body->CreateFixture(&FixtureDef);
    ///fixture->SetUserData(this);
    return fixture;
}
/// ///////////////////////////////////////////////
/// //////////////////SPRITES STUFFS////////////////

sf::Sprite* XMLLoader::loadImage(std::string imageName, std::string dir)
{
    std::cout << "Loading " <<imageName<< std::endl;
    pugi::xml_node imagesNode = XMLDocument.child("box2d").child("images");
    pugi::xml_node image = imagesNode.find_child_by_attribute("image","name",imageName.c_str());
    std::stringstream ss;
//    ss <<dir<<imageName; ///PEUT ETRE CHANGER?
    ss <<dir<<image.attribute("path").as_string(); ///PEUT ETRE CHANGER?
    std::string filename = ss.str();

    sf::Sprite imgSprite;

    textureHolder->loadFromFile(filename, filename);

////    sf::Texture  tex;
////    if ( !tex.loadFromFile(filename) )
////    {
////        std::cout << "Failed to load  spritesheet!" << std::endl;
////    }




////    tex.setSmooth(true);
    imgSprite.setTexture(*textureHolder->getTexture(filename));
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
        (textureHolder->getTexture(filename)->getSize().x)/2,
        (textureHolder->getTexture(filename)->getSize().y)/2
    );
    spriteList.push_back(imgSprite);
///TROP STATIQUE !!!!!!
    spriteRectList.push_back(sf::IntRect(imgSprite.getPosition().x, imgSprite.getPosition().y,
                                         imgSprite.getTexture()->getSize().x*imgSprite.getScale().x,
                                         imgSprite.getTexture()->getSize().y*imgSprite.getScale().y
                                         )
                             );
////    texList.push_back(*textureHolder->getTexture(imageName));
    texNames.push_back(filename);
    std::cout<<filename<<" loadE........\n";

    nameToSprite[imageName] = &spriteList[spriteList.size()-1];
    return &spriteList[spriteList.size()-1];
}

void XMLLoader::render(sf::RenderWindow& mWindow, sf::Shader* shader)
{
    int i = 0;
    //std::cout << "TEXLIST"<<texList.size()<< "ratioLIST"<<ratioList.size()<< "spriteLIST"<<spriteList.size()<< "bodyLIST"<< bodyList.size() << std::endl;
//    for (sf::Sprite s : spriteList )
//    {
//        s.setTexture(texList[i]);
//        if(bodyList[i]->GetType() == b2_dynamicBody)
//        {
//            s.setPosition((bodyList[i]->GetPosition().x*RATIO)-ratioList[i].x,  (bodyList[i]->GetPosition().y*RATIO)+ratioList[i].y);
//            s.setRotation(bodyList[i]->GetAngle()/ RADTODEG - angleRatioList[i]);
//        }
//
//        mWindow.draw(s);
//
//        i++;
//    }
    for (int it = 0 ; it < spriteList.size(); it++)
    {
////        spriteList[it].setTexture(texList[i]);
        spriteList[it].setTexture(*textureHolder->getTexture(texNames[i]));
        if(bodyList[i]->GetType() == b2_dynamicBody)
        {
            spriteList[it].setPosition((bodyList[i]->GetPosition().x*RATIO)-ratioList[i].x,  (bodyList[i]->GetPosition().y*RATIO)+ratioList[i].y);
            spriteList[it].setRotation(bodyList[i]->GetAngle()/ RADTODEG - angleRatioList[i]);
        }

        mWindow.draw(spriteList[it]);

        i++;
    }

}


void XMLLoader::move(float x, float y)
{
    for (b2Body* s : bodyList )
    {
        s->SetTransform(b2Vec2(s->GetPosition().x + x/RATIO, s->GetPosition().y),s->GetAngle());
        ///! LES BODIES QUI N ONT PAS D IMAGES NE SONT PAS AJOUTE DANS LE BODYLIST
        //s->ApplyLinearImpulse(b2Vec2(std::pow(-1,(int)randomClock.getElapsedTime().asSeconds()), 5*std::pow(-1,(int)randomClock.getElapsedTime().asMilliseconds())), s->GetWorldCenter(), true);
//        srand (time(NULL));
        //s->ApplyForce( s->GetMass() * -p_world->GetGravity()+b2Vec2(rand()%3,rand()%3), s->GetWorldCenter(), true );

    }
    if(randomClock.getElapsedTime().asSeconds()>6)
        randomClock.restart();
}

