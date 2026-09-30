#include <iostream>
#include <memory>
#include <fstream>

#include <SFML/Graphics.hpp>
#include "imgui.h"
#include "imgui-SFML.h"
#include "imgui_stdlib.h"

int main(int argc, char* argv[])
{
    // create a new window of size w*h pixels
    // top-left of the window is (0,0) and bottom-right is (w,h)
    // you will have to read these from the config file
    const int windowWidth = 1280;
    const int windowHeight = 720;
    sf::RenderWindow window(sf::VideoMode({ windowWidth, windowHeight }), "SFML works!");
    window.setFramerateLimit(60); // limit frame rate to 60 fps

    // initialize imgui and create a clock used for its internal timing
    if (!ImGui::SFML::Init(window))
    {
        std::cout << "Could not initialize window\n";
        std::exit(-1);
    }

    sf::Clock deltaClock;

    // scale the imgui ui and text size by 2
    ImGui::GetStyle().ScaleAllSizes(2.0f);
    ImGui::GetIO().FontGlobalScale = 2.0f;

    // the imgui color {r, g, b} wheel requires floats from 0-1 
    // sfml will require instead of uint8_t from 0-255
    // this is the only really annoying conversion between sfml and imgui
    float c[3] = { 0.0f, 1.0f, 1.0f };

    // let's make a shape that we will draw to the screen
    float circleRadius = 50;    // radius to draw the circle
    int circleSegments = 32;    // number of segments to draw the circle with
    float circleSpeedX = 1.0f;  // we will use this to move the circle later
    float circleSpeedY = 0.5f;  // you will read these values from the file
    bool drawCircle = true;     // whether or not to draw the circle
    bool drawText = true;     // whether or not to draw the text

    // create the sfml circle shape based on our parameters
    sf::CircleShape circle(circleRadius, circleSegments);   // create a circle shape with radius 50
    circle.setPosition({ 10.0f, 10.0f });                     // set the top-left position of the circle

    // let's load a font so we can display some text
    sf::Font myFont;

    // attempt to load the font from a file
    if (!myFont.openFromFile("fonts/PPFraktionMonoBold.otf"))
    {
        // if we can't load the font, print an error to the error console and exit
        std::cout << "Could not load font!\n";
        std::exit(-1);
    }

    // set up the text object that will be drawn to the screen
    sf::Text text(myFont, "Sample Text", 24);

    // position the top-left corner of the text so that the text aligns on the bottom
    // text character size is in pixels, so move the text up  from the bottom by its height
    text.setPosition({ 0, wHeight - (float)text.getCharacterSize() });

    // set up a character array to set the text
    std::string displayString = "Sample Text";

    // main loop - continues for each frame while window is open
    while (window.isOpen())
    {
        // event handling 
        while (auto event = window.pollEvent())
        {
            // pass the event to imgui to be parsed
            ImGui::SFML::ProcessEvent(window, *event);

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

        // update imgui for this frame with the time that the last frame took
        ImGui::SFML::Update(window, deltaClock.restart());

        // draw the UI
        ImGui::Begin("Window title");
        ImGui::Text("Window text!");
        ImGui::Checkbox("Draw Circle", &drawCircle);
        ImGui::SameLine();
        ImGui::Checkbox("Draw Text", &drawText);
        ImGui::SliderFloat("Radius", &circleRadius, 0.0f, 300.0f);
        ImGui::SliderInt("Sides", &circleSegments, 3, 64);
        ImGui::ColorEdit3("Color Circle", c);
        ImGui::InputText("Text", &displayString);
        if (ImGui::Button("Set Text"))
        {
            text.setString(displayString);
        }
        ImGui::SameLine();
        if (ImGui::Button("Reset Circle"))
        {
            circle.setPosition({ 0, 0 });
        }
        ImGui::End();

        // set the circle properties, because they may have been updated with the ui
        circle.setPointCount(circleSegments);
        circle.setRadius(circleRadius);

        // imgui uses 0-1 float for colors, sfml uses 0-255 for colors
        // we must convert from the ui floats to sfml Uint8
        circle.setFillColor(sf::Color(uint8_t(c[0]*255), uint8_t(c[1]*255), uint8_t(c[2]*255)));

        // basic animation - move the each frame if it's still in frame
        circle.setPosition({ circle.getPosition().x + circleSpeedX, circle.getPosition().y + circleSpeedY });

        // basic rendering function calls
        window.clear();         // clear the window of anything previously drawn
        if (drawCircle)         // draw the circle if the boolean is true
        { 
            window.draw(circle); 
        }   
        if (drawText)           // draw the text if the boolean is true
        { 
            window.draw(text); 
        }     
        ImGui::SFML::Render(window); // draw the ui last so it's on top
        window.display();       // call the window display function
    }

    ImGui::SFML::Shutdown();

    return 0;
}
