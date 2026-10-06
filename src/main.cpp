#include <iostream>
#include <memory>
#include <fstream>

#include <SFML/Graphics.hpp>

struct CircleConfig {bool draw; float radius, speedX, speedY; int R, G, B, sides; std::string name;};
CircleConfig circleConfig;

void initialize(const std::string & configFile)
{
	std::ifstream fin(configFile);
	std::string token;
	while (fin >> token)
	{
		if (token == "Circle:")
		{
			fin >> circleConfig.draw;
			fin >> circleConfig.radius;
			fin >> circleConfig.speedX;
			fin >> circleConfig.speedY;
			fin >> circleConfig.R;
			fin >> circleConfig.G;
			fin >> circleConfig.B;
			fin >> circleConfig.sides;
			fin >> circleConfig.name;
		}
	}
	
	std::cout << circleConfig.draw   << "\n";
	std::cout << circleConfig.radius << "\n";
	std::cout << circleConfig.speedX << "\n";
	std::cout << circleConfig.speedY << "\n";
	std::cout << circleConfig.R      << "\n";
	std::cout << circleConfig.G      << "\n";
	std::cout << circleConfig.B      << "\n";
	std::cout << circleConfig.sides  << "\n";
	std::cout << circleConfig.name   << "\n";
}

int main(int argc, char* argv[])
{
	initialize("bin/config.txt");
	
    const int windowWidth = 1280;
    const int windowHeight = 720;
    sf::RenderWindow window(sf::VideoMode({ windowWidth, windowHeight }), "SFML works!");
    window.setFramerateLimit(60);

    sf::CircleShape circle(circleConfig.radius, circleConfig.sides);
    circle.setPosition({ windowWidth / 2.f, windowHeight / 2.f});
	circle.setOrigin({circle.getRadius(), circle.getRadius()});

    sf::Font myFont;

    if (!myFont.openFromFile("bin/fonts/PPFraktionMonoBold.otf"))
    {
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
                    circleConfig.speedX *= -1.f;
                }
            }
        }

        // prevents further processing of the frame if the window is closed
        if (!window.isOpen()) { break;}

        circle.setFillColor(sf::Color(circleConfig.R, circleConfig.G, circleConfig.B));

        // basic rendering function calls
        window.clear();         // clear the window of anything previously drawn
        if (circleConfig.draw == true)         // draw the circle if the boolean is true
        {
			// basic animation - move the circle each frame if it's still in frame
			circle.setPosition({ circle.getPosition().x + circleConfig.speedX, circle.getPosition().y + circleConfig.speedY });
			
			if (circle.getPosition().x - circle.getRadius() <= 0 || circle.getPosition().x + circle.getRadius() >= windowWidth)
			{
				circleConfig.speedX *= -1.f;
			}
			
			if (circle.getPosition().y - circle.getRadius() <= 0 || circle.getPosition().y + circle.getRadius() >= windowHeight)
			{
				circleConfig.speedY *= -1.f;
			}
			
            window.draw(circle); 
        }   
        
        window.draw(text); 
            
        window.display();
    }

    return 0;
}
