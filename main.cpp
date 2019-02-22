#include <SFML/Graphics.hpp>
#include <unistd.h>
#include <vector>
#include "player.h"
#include "weapon.h"
#include "enemy.h"
#include <stdio.h>
#include <iostream>
#include "Box2D/Box2D.h"
#include "object.h"

int main() {


    // Define the gravity vector.
    b2Vec2 gravity(0.0f, -9.81f);

    // Construct a world object, which will hold and simulate the rigid bodies.
    b2World world(gravity);

#if 1
    int counter;
    int counter1;
    float32 scaleX = 40;
    float32 scaleY = -40;
    sf::RenderWindow window(sf::VideoMode(800, 600), "My window");
    std::string Path_to_res = "/home/leonid/CLionProjects/Game/Resourses/";
    std::string Path_to_duck = "/home/leonid/CLionProjects/Game/Resourses/Duck Game Sprites/";
    sf::Texture textureCastle;
  if (!textureCastle.loadFromFile(Path_to_duck + "castle.png")) {
        return EXIT_FAILURE;
    }
    window.setFramerateLimit(60);
    //View

    object floor;

    floor.sprite.setTexture(textureCastle);
    floor.bound.setSize(sf::Vector2f(96,79-65));
    floor.bound.setPosition(400,500);
    floor.sprite.setTextureRect(sf::IntRect(0,65,96,79-65));
    floor.sprite.setScale(3,3);
    floor.bound.setScale(3,3);


    floor.bodyInit(world);


    sf::View view1;
    view1.setSize(sf::Vector2f(window.getSize().x,window.getSize().y));
    view1.setCenter(sf::Vector2f(view1.getSize().x/2,view1.getSize().y/2));
    window.setView(view1);


    sf::Texture textureBox;
    if(!textureBox.loadFromFile(Path_to_res+"box1.png")){
        return EXIT_FAILURE;
    }

    sf::Texture textureBarrel;
    if(!textureBarrel.loadFromFile(Path_to_res+"barrel.png")){
        return EXIT_FAILURE;
    }
    class object barrel;
    barrel.sprite.setTexture(textureBox);
    barrel.sprite.setScale(0.25,0.25);
    barrel.bound.setSize(sf::Vector2f(textureBox.getSize().x,textureBox.getSize().y));
    barrel.bound.setScale(0.25,0.25);
    barrel.bound.setPosition(500,100);


    // Define the dynamic body. We set its position and call the body factory.
    barrel.realBodyDef.type = b2_dynamicBody;
    barrel.realBodyDef.position.Set(barrel.bound.getPosition().x/scaleX, barrel.bound.getPosition().y/scaleY);
    barrel.realBody = world.CreateBody(&barrel.realBodyDef);

    // Define another box shape for our dynamic body.
    //b2PolygonShape dynamicBox;

    barrel.shape.SetAsBox((textureBox.getSize().x/(float32)(8*scaleX)), (textureBox.getSize().x/(float32)(8*scaleX)));

    // Define the dynamic body fixture.

    barrel.Fixture.shape = &barrel.shape;

    // Set the box density to be non-zero, so it will be dynamic.
    barrel.Fixture.density = 1.0f;

    // Override the default friction.
    barrel.Fixture.friction = 0.3f;

    // Add the shape to the body.
    barrel.realBody->CreateFixture(&barrel.Fixture);

/*

    class object box;
    box.sprite.setTexture(textureBox);
    box.sprite.setScale(0.25,0.25);
    box.bound.setSize(sf::Vector2f(textureBox.getSize().x,textureBox.getSize().y));
    box.bound.setScale(0.25,0.25);
    box.bound.setPosition(0,400);
*/

    sf::Texture textureSans;
    if (!textureSans.loadFromFile(Path_to_res + "sans.png")) {
        return EXIT_FAILURE;
    }


    class player Player1(world);

    Player1.sprite.setTexture(textureSans);





    std::vector<weapon>::const_iterator iter;
    std::vector<weapon> weaponArray;
    weapon weapon1;


    // Prepare for simulation. Typically we use a time step of 1/60 of a
    // second (60Hz) and 10 iterations. This provides a high quality simulation
    // in most game scenarios.

    float32 timeStep = 1.0f / 60.0f;
    int32 velocityIterations = 6;
    int32 positionIterations = 2;


    while (window.isOpen()) {

        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;
        while (window.pollEvent(event)) {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear(sf::Color::Black);

        //столкновения


        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {

            weapon1.bound.setPosition(Player1.bound.getPosition());
            //Теперь объект оружия рядом с человеком
            weapon1.direction = Player1.direction;
            weaponArray.push_back(weapon1);

        }
        counter1 = 0;
        for (iter = weaponArray.begin(); iter != weaponArray.end(); iter++) {
            if (weaponArray[counter1].bound.getPosition().y>600||weaponArray[counter1].bound.getPosition().y<0
            ||weaponArray[counter1].bound.getPosition().x<0||weaponArray[counter1].bound.getPosition().x>800) {
                weaponArray.erase(iter);
                break;//Why?
            }
            counter1++;
        }

        //draw projectile


        counter = 0;
        for (iter = weaponArray.begin(); iter != weaponArray.end(); iter++) {
            weaponArray[counter].update();
            window.draw(weaponArray[counter].bound);
            counter++;
        }
        Player1.update();//Теперь спрайт привязан к прямоугольнику


        Player1.move();

        //обновление мира Box2D
        world.Step(timeStep, velocityIterations, positionIterations);


       // barrel.sprite.setRotation(-angle);
        barrel.update();


        floor.update();
        //window.draw(floor.sprite);
        window.draw(floor.bound);

        window.draw(barrel.sprite);
        window.setView(view1);
        view1.setCenter(Player1.bound.getPosition());
        window.draw(Player1.bound);
        window.draw(Player1.sprite);

        window.display();
    }
#endif
    return 0;
}