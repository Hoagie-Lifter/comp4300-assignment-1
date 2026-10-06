#include <iostream>
#include <memory>
#include <fstream>

#include <SFML/Graphics.hpp>

void initialize(const std::string & configFile)
{
	std::ifstream fin(configFile);
	std::string token;
	while (fin >> token);
}

int main(int argc, char* argv[])
{
	initialize("config/config.txt");
	
    // create a new window of size w*h pixels
    // top-left of the window is (0,0) and bottom-right is (w,h)
    // you will have to read these from the config file
    const int windowWidth = 1280;
    const int windowHeight = 720;
    sf::RenderWindow window(sf::VideoMode({ windowWidth, windowHeight }), "SFML works!");
    window.setFramerateLimit(60); // limit frame rate to 60 fps

    // let's make a shape that we will draw to the screen
    float circleRadius = 50;    // radius to draw the circle
    int circleSegments = 32;    // number of segments to draw the circle with
    float circleSpeedX = 2.0f;  // we will use this to move the circle later
    float circleSpeedY = 2.0f;  // you will read these values from the file
	bool drawCircle    = true;

    // create the sfml circle shape based on our parameters
    sf::CircleShape circle(circleRadius, circleSegments);           // create a circle shape with radius 50
    circle.setPosition({ windowWidth / 2.f, windowHeight / 2.f});   // set the top-left position of the circle
	circle.setOrigin({circle.getRadius(), circle.getRadius()});		// set origin t ocenter of circle

    // let's load a font so we can display some text
    sf::Font myFont;

    // attempt to load the font from a file
    if (!myFont.openFromFile("bin/fonts/PPFraktionMonoBold.otf"))
    {
        // if we can't load the font, print an error to the error console and exit
        std::cout << "Could not load font!\n";
        std::exit(-1);
    }

    // set up the text object that will be drawn to the screen
    sf::Text text(myFont, "Sample Text", 24);

    // position the top-left corner of the text so that the text aligns on the bottom
    // text character size is in pixels, so move the text up  from the bottom by its height
    text.setPosition({ 0, windowHeight - (float)text.getCharacterSize() });

    // set up a character array to set the text
    std::string displayString = "Sample Text";

    // main loop - continues for each frame while window is open
    while (window.isOpen())
    {
        // event handling 
        while (auto event = window.pollEvent())
        {
            // this event triggers when the window is closed
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }

            // this event is triggered when a key is pressed
            if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
            {
                // print the key that was pressed to the console
                std::cout << "Key pressed with code = " << int(keyPressed->scancode) << "\n";

                if (keyPressed->scancode == sf::Keyboard::Scancode::X)
                {
                    circleSpeedX *= -1.0f;
                }
            }
        }

        // prevents further processing of the frame if the window is closed
        if (!window.isOpen()) { break;}

        // set the circle properties, because they may have been updated with the ui
        circle.setPointCount(circleSegments);
        circle.setRadius(circleRadius);

        // imgui uses 0-1 float for colors, sfml uses 0-255 for colors
        // we must convert from the ui floats to sfml Uint8
        circle.setFillColor(sf::Color(255, 255, 255));

        // basic rendering function calls
        window.clear();         // clear the window of anything previously drawn
        if (drawCircle)         // draw the circle if the boolean is true
        {
			// basic animation - move the circle each frame if it's still in frame
			circle.setPosition({ circle.getPosition().x + circleSpeedX, circle.getPosition().y + circleSpeedY });
			
			if (circle.getPosition().x - circle.getRadius() <= 0 || circle.getPosition().x + circle.getRadius() >= windowWidth)
			{
				circleSpeedX *= -1;
			}
			
			if (circle.getPosition().y - circle.getRadius() <= 0 || circle.getPosition().y + circle.getRadius() >= windowHeight)
			{
				circleSpeedY *= -1;
			}
			
            window.draw(circle); 
        }   
        
        window.draw(text); 
            
        window.display();
    }

    return 0;
}
