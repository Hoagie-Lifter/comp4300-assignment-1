#include <iostream>
#include <memory>
#include <fstream>

#include <SFML/Graphics.hpp>

struct WindowConfig    {unsigned int width, height; int frameLimit; std::string name;};
struct RectangleConfig {bool draw; float width, height, speedX, speedY; int R, G, B; std::string name;};
struct CircleConfig    {bool draw; float radius, speedX, speedY; int R, G, B, sides; std::string name;};
struct TriangleConfig  {bool draw; float radius, speedX, speedY; int R, G, B, sides; std::string name;};

WindowConfig    windowConfig;
RectangleConfig rectangleConfig;
CircleConfig    circleConfig;
TriangleConfig  triangleConfig;

void moveRectangle(sf::RenderWindow & window, sf::RectangleShape & rectangle, sf::Text & rectangleText)
{
	rectangle.move({rectangleConfig.speedX, rectangleConfig.speedY});
	
	sf::Vector2f rectanglePosition = rectangle.getPosition();
	
	if (rectangle.getPosition().x - rectangleConfig.width / 2 <= 0)
	{
		rectanglePosition.x = rectangleConfig.width / 2;
		rectangleConfig.speedX *= -1.f;
	}
	
	else if (rectangle.getPosition().x + rectangleConfig.width / 2 >= windowConfig.width)
	{
		rectanglePosition.x = windowConfig.width - rectangleConfig.width / 2;
		rectangleConfig.speedX *= -1.f;
	}
	
	if (rectangle.getPosition().y - rectangleConfig.height / 2 <= 0)
	{
		rectanglePosition.y = rectangleConfig.height / 2;
		rectangleConfig.speedY *= -1.f;
	}
	
	else if (rectangle.getPosition().y + rectangleConfig.height / 2 >= windowConfig.height)
	{
		rectanglePosition.y = windowConfig.height - rectangleConfig.height / 2;
		rectangleConfig.speedY *= -1.f;
	}
	
	rectangle.setPosition(rectanglePosition);
	
	rectangleText.setPosition(rectanglePosition);
	
	window.draw(rectangle);
	window.draw(rectangleText);
}

void moveCircle(sf::RenderWindow & window, sf::CircleShape & circle, sf::Text & circleText)
{
	circle.move({circleConfig.speedX, circleConfig.speedY});
	
	sf::Vector2f circlePosition = circle.getPosition();
	float circleRadius = circle.getRadius();
	
	if (circlePosition.x - circleRadius <= 0)
	{
		circlePosition.x = circleRadius;
		circleConfig.speedX *= -1.f;
	}
	
	else if (circlePosition.x + circleRadius >= windowConfig.width)
	{
		circlePosition.x = windowConfig.width - circleRadius;
		circleConfig.speedX *= -1.f;
	}
	
	if (circlePosition.y - circleRadius <= 0)
	{
		circlePosition.y = circleRadius;
		circleConfig.speedY *= -1.f;
	}
	
	else if (circlePosition.y + circleRadius >= windowConfig.height)
	{
		circlePosition.y = windowConfig.height - circleRadius;
		circleConfig.speedY *= -1.f;
	}
	
	circle.setPosition(circlePosition);
	circleText.setPosition(circlePosition);
	
	window.draw(circle);
	window.draw(circleText);
}

void run(sf::Font myFont, sf::RectangleShape rectangle, sf::CircleShape circle, sf::CircleShape triangle)
{
	sf::RenderWindow window(sf::VideoMode({ windowConfig.width, windowConfig.height }), windowConfig.name);
    window.setFramerateLimit(windowConfig.frameLimit);
	
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
		
		sf::Text rectangleText(myFont, "Rectangle", 16);
		rectangleText.setOrigin(rectangleText.getLocalBounds().size / 2.f);
		
		sf::Text circleText(myFont, "Circle", 16);
		circleText.setOrigin(circleText.getLocalBounds().size / 2.f);

        // basic rendering function calls
        window.clear();         // clear the window of anything previously drawn

		// basic animation - move the shape each frame if they're still in frame
		if (rectangleConfig.draw = true)
		{
			moveRectangle(window, rectangle, rectangleText);
		}
		
		if (circleConfig.draw = true)
		{
			moveCircle(window, circle, circleText);
		}
		
		if (triangleConfig.draw = true)
		{
			triangle.move({triangleConfig.speedX, triangleConfig.speedY});
			
			sf::Vector2f trianglePosition = triangle.getPosition();
				
			if (triangle.getPosition().x - triangle.getRadius() <= 0 || triangle.getPosition().x + triangle.getRadius() >= windowConfig.width)
			{
				trianglePosition.x = 0;
				triangleConfig.speedX *= -1.f;
			}
				
			else if (triangle.getPosition().y - triangle.getRadius() <= 0 || triangle.getPosition().y + triangle.getRadius() >= windowConfig.height)
			{
				trianglePosition.y = 0;
				triangleConfig.speedY *= -1.f;
			}
			
			window.draw(triangle); 
		}

        window.display();
    }
}

sf::Font setFont()
{
	sf::Font myFont;

    if (!myFont.openFromFile("bin/fonts/PPFraktionMonoBold.otf"))
    {
        std::cout << "Could not load font!\n";
        std::exit(-1);
    }
	
	return myFont;
}

void initialize(const std::string & configFile)
{
	std::ifstream fin(configFile);
	std::string token;
	while (fin >> token)
	{
		if (token == "Window:")
		{
			fin >> windowConfig.width;
			fin >> windowConfig.height;
			fin >> windowConfig.frameLimit;
			fin >> windowConfig.name;
		}
		
		else if (token == "Rectangle:")
		{
			fin >> rectangleConfig.draw;
			fin >> rectangleConfig.width;
			fin >> rectangleConfig.height;
			fin >> rectangleConfig.speedX;
			fin >> rectangleConfig.speedY;
			fin >> rectangleConfig.R;
			fin >> rectangleConfig.G;
			fin >> rectangleConfig.B;
			fin >> rectangleConfig.name;
		}
		
		else if (token == "Circle:")
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
		
		else if (token == "Triangle:")
		{
			fin >> triangleConfig.draw;
			fin >> triangleConfig.radius;
			fin >> triangleConfig.speedX;
			fin >> triangleConfig.speedY;
			fin >> triangleConfig.R;
			fin >> triangleConfig.G;
			fin >> triangleConfig.B;
			fin >> triangleConfig.sides;
			fin >> triangleConfig.name;
		}
		
		//else if (token == "Pentagon")
		//{
		//	fin >> pentagonConfig.draw;
		//	fin >> pentagonConfig.radius;
		//	fin >> pentagonConfig.speedX;
		//	fin >> pentagonConfig.speedY;
		//	fin >> pentagonConfig.R;
		//	fin >> pentagonConfig.G;
		//	fin >> pentagonConfig.B;
		//	fin >> pentagonConfig.sides;
		//	fin >> pentagonConfig.name;
		//}
		
	}
}

sf::RectangleShape createRectangle()
{
	sf::RectangleShape rectangle({rectangleConfig.width, rectangleConfig.height});
	rectangle.setFillColor(sf::Color(rectangleConfig.R, rectangleConfig.G, rectangleConfig.B));
	rectangle.setOrigin({rectangleConfig.width / 2, rectangleConfig.height / 2});
	rectangle.setPosition({windowConfig.width / 2.f, windowConfig.height / 2.f});
	
	return rectangle;
}

sf::CircleShape createCircle()
{
	sf::CircleShape circle(circleConfig.radius, circleConfig.sides);
	circle.setFillColor(sf::Color(circleConfig.R, circleConfig.G, circleConfig.B));
	circle.setOrigin({circle.getRadius(), circle.getRadius()});
    circle.setPosition({windowConfig.width / 2.f, windowConfig.height / 2.f});
	
	return circle;
}

sf::CircleShape createTriangle()
{
	sf::CircleShape triangle(triangleConfig.radius, triangleConfig.sides);
	triangle.setFillColor(sf::Color(triangleConfig.R, triangleConfig.G, triangleConfig.B));
	triangle.setOrigin({triangle.getRadius(), triangle.getRadius()});
    triangle.setPosition({windowConfig.width / 2.f, windowConfig.height / 2.f});
	
	return triangle;
}

int main(int argc, char* argv[])
{
	initialize("bin/config.txt");
	sf::Font myFont = {setFont()};
	sf::RectangleShape rectangle = {createRectangle()};
	sf::CircleShape circle = {createCircle()};
	sf::CircleShape triangle = {createTriangle()};

    // set up the text object that will be drawn to the screen
    sf::Text text(myFont, "Sample Text", 24);

    // position the top-left corner of the text so that the text aligns on the bottom
    // text character size is in pixels, so move the text up  from the bottom by its height
    text.setPosition({ 0, windowConfig.height - (float)text.getCharacterSize() });
	
	run(myFont, rectangle, circle, triangle);

    return 0;
}
