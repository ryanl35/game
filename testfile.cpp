#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Window.hpp>

#include <iostream>
#include <stdio.h> 
#include <stdlib.h> 
#include <deque>


class Platform {
private:
    int x;
    int y;

public:
    Platform(int x, int y) : x(x), y(y) {};

    int getX() const { return this->x; }
    int getY() const { return this->y; }
    void setX(int x) { this->x = x; }
    void setY(int y) { this->y = y; }

};


int main()
{

    // define constants
    const int height = 850;
    const int width = 500;
    const double doodleScaleFactor = 0.5;
    const double platformScaleFactor = 0.275;
    const int positionFactor = 6;

    const int loseWidth = 250;
    const int loseHeight = 162;


    // constants handling gravity
    const float gravity = 0.2;


    // CREATING TEXTURES

    // creates the background texture to be used for the Sprite
    sf::Texture backgroundTxt;
    backgroundTxt.loadFromFile("media/images/doodleJumpBackground.png");

    // creates the Doodle Jump character texture to be used for the Sprite
    sf::Texture doodleTxt;
    doodleTxt.loadFromFile("media/images/doodleJumpCharacter.png");

    // the following three textures to be used for the Sprites for the platforms
    sf::Texture brownTxt;
    brownTxt.loadFromFile("media/images/brown.png");

    sf::Texture greenTxt;
    greenTxt.loadFromFile("media/images/green.png");

    sf::Texture blueTxt;
    blueTxt.loadFromFile("media/images/blue.png");

    // CREATING SPRITES

    // creates the Sprite for the Doodle Jump background
    sf::Sprite backgroundSpr;
    backgroundSpr.setTexture(backgroundTxt);

    // creates the Sprite for the Doodle Jump character
    sf::Sprite doodleSpr;
    doodleSpr.setTexture(doodleTxt);

    // the following three Sprites to be used for the platforms
    sf::Sprite brownSpr;
    brownSpr.setTexture(brownTxt);

    sf::Sprite greenSpr;
    greenSpr.setTexture(greenTxt);

    sf::Sprite blueSpr;
    blueSpr.setTexture(blueTxt);


    // INITIAL POSITIONING

    // sets the player's initial position
    doodleSpr.setPosition(width - 250, height - 100);

    // CREATING THE WINDOW

    // Renders a window with a specified size, titles it "Doodle Jump v2.0!"
    sf::RenderWindow window(sf::VideoMode(width, height), "Doodle Jump v2.0!");
    // controls movement so it's not too fast and erratic
    window.setFramerateLimit(60);

    // GAME MUSIC

    // Uses the Music feature of SFML by opening a specific file
    sf::Music music;
    if (!music.openFromFile("media/music/Electro music for Doodle Jump.wav"))
        return EXIT_FAILURE;
    // Play the music
    music.play();
    // sets a loop for the music so the music continues playing for the duration of the game
    music.setLoop(true);

    // initializes an empty deque to contain all the platforms
    std::deque<Platform *> greenPlatforms;

    // creates instances of Platforms to add onto the Deque to be rendered
    for (int i = 0; i < 15; i++) 
    {
        Platform * platform = new Platform(rand() % 250, rand() % 750);
        greenPlatforms.push_back(platform);

    }

    // variables to keep track of position of player
    int x = height - doodleSpr.getGlobalBounds().height;
    int y = width / 2;
    int height2 = 200;

    float dx = 0;
    float dy = 0;

    // MAIN FUNCTION:

    // a while loop while the window / game is in session
    while (window.isOpen())
    {
        
        // instantiates an instance of what's known as an "Event"
        // an Event can be a mouse pressed, moved, key pressed, etc. anything that makes the game interactive
        sf::Event event;

        // while loop .. while there are events left to do... essentially allows Events to be made
        while (window.pollEvent(event))
        {
            // what to do if the user closes the window ... the window closes
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }

            // what to do when the mouse is moved within the window
            else if (event.type == sf::Event::MouseMoved)
            {
                // moves the x position so it follows the mouse wherever it goes
                // does NOT follow it's y position (keeps the current y position by using getPosition())
                // keeps y position because in Doodle Jump the player CANNOT control the y position of the player
                // doodleSpr.setPosition(event.mouseMove.x, doodleSpr.getPosition().y);
                x = event.mouseMove.x;
            }

        }

        // HANDLING SCROLLING AND MOVEMENT

        // if the user wants to use the keys instead of the mouse
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) { x = x - 5; }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) { x = x + 5; }

        // handle gravity
        dy += gravity;
        y += dy;

        // initalize the lose texture and it's sprite for use when the character loses
        sf::Texture lostTxt;
        sf::Sprite loseSpr;

        // if the doodle jump character drops too low
        if (y > height - (doodleSpr.getGlobalBounds().height + 5)) 
        {
            lostTxt.loadFromFile("media/images/youLose.png");
            loseSpr.setTexture(lostTxt);
            loseSpr.setPosition(width / 4, height / 3);
            dy = -10;
        } 


        else if (y < height2) 
        {
            for (int i = 0; i < greenPlatforms.size(); i++) 
            {
                y = height2;
                greenPlatforms[i]->setY(greenPlatforms[i]->getY() - dy);

                if (greenPlatforms[i]->getY() > height) 
                {
                    greenPlatforms[i]->setX(rand() % width);
                    greenPlatforms[i]->setY(0);
                }
            }
        }

        for (int i = 0; i < greenPlatforms.size(); i++) 
        {
            if ((x + 40 > greenPlatforms[i]->getX()) 
                && (x + 40 < greenPlatforms[i]->getX() + 105)
                && (y + 64 > greenPlatforms[i]->getY()) 
                && (y + 64 < greenPlatforms[i]->getY() + 50) 
                && (dy > 0))
                {
                    dy = -10;
                }
        }

        doodleSpr.setPosition(x, y);

        window.clear();
        window.draw(backgroundSpr);
        for (int i = 0; i < greenPlatforms.size(); i++) 
        {
            greenSpr.setPosition(greenPlatforms[i]->getX(), greenPlatforms[i]->getY());
            window.draw(greenSpr);
        }
        window.draw(doodleSpr);
        window.draw(loseSpr);
        window.display();
    }

    return 0;
}