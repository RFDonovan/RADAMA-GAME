#include "XMLLoader.hpp"
/**
    BOUNDARY =          0x0001,
    FRIENDLY_SHIP =     0x0002,
    ENEMY_SHIP =        0x0004,
    FRIENDLY_AIRCRAFT = 0x0008,
    ENEMY_AIRCRAFT =    0x0010,
*/
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

std::vector<bodyData> XMLLoader::loadXML(std::string XMLFile, std::string dir)
{
    std::vector<bodyData> bodyDataList;
    std::cout << "directory: "<<dir<< "\n";

    if (!XMLDocument.load_file(XMLFile.c_str()))
    {
        std::cout << "error on loading "<<XMLFile<< "\n";
        //return NULL;
    }
    pugi::xml_node bodiesNode = XMLDocument.child("box2d").child("bodies");

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

            if(shapeType.compare("edgeShape") == 0)
                fixtureList = createEdgeShape(body, fixture1);///ON A UN PROBLEME CAR ICI ON ECRASE LA FIXTURELIST, ON N'AJOUTE PAS

            else if(shapeType.compare("circleShape") == 0)
                fixture = createCircleShape(body, fixture1);

            else if(shapeType.compare("polygonShape") == 0)
                fixture = createPolygonShape(body, fixture1);
            else if(shapeType.compare("rectangleShape") == 0)
                fixture = createRectangleShape(body, fixture1);
            if(fixture)
            {
                fixtureList.push_back(fixture);
                mapFixture[fixtureName] = fixture;
            }

            std::cout<<"fixtures crEEEEEEEEEEEEEEEEE"<<std::endl;

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
    //FixtureDef.shape = &Shape;


    b2PolygonShape Shape;
    Shape.SetAsBox(fixtureNode.attribute("width").as_float()/RATIO,
                   fixtureNode.attribute("height").as_float()/RATIO);
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
    ss <<dir<<imageName; ///PEUT ETRE CHANGER?
    std::string filename = ss.str();

    sf::Sprite imgSprite;
    sf::Texture  tex;
    if ( !tex.loadFromFile(filename) )
    {
        std::cout << "Failed to load  spritesheet!" << std::endl;
    }




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
    std::cout<<filename<<" loadE........\n";
    return &imgSprite;
}

void XMLLoader::render(sf::RenderWindow& mWindow, sf::Shader* shader)
{
    int i = 0;
    //std::cout << "TEXLIST"<<texList.size()<< "ratioLIST"<<ratioList.size()<< "spriteLIST"<<spriteList.size()<< "bodyLIST"<< bodyList.size() << std::endl;
    for (sf::Sprite s : spriteList )
    {
        s.setTexture(texList[i]);
        if(bodyList[i]->GetType() == b2_dynamicBody)
        {
            s.setPosition((bodyList[i]->GetPosition().x*RATIO)-ratioList[i].x,  (bodyList[i]->GetPosition().y*RATIO)+ratioList[i].y);
            s.setRotation(bodyList[i]->GetAngle()/ RADTODEG - angleRatioList[i]);
        }

        mWindow.draw(s);

        i++;
    }

}


