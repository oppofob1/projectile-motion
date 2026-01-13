#include <SFML/Graphics.hpp>
#include <cmath>
#include <optional>
#include <imgui.h>
#include <imgui-SFML.h>

int main()
{   
    //setup windows
    auto window = sf::RenderWindow(sf::VideoMode({1920u, 1080u}), "CMake SFML Project");
    window.setFramerateLimit(144);

    if (!ImGui::SFML::Init(window))
        return -1;

    //setup clocks, clock for physics update, deltaClock for ImGui
    sf::Clock clock;
    sf::Clock deltaClock;

    //starting angle and velocity
    float angleInDegrees = 45.0f;
    float velocity = 1000.f;
    
    //converting values to use in formula
    sf::Angle angle = sf::degrees(-angleInDegrees);
    sf::Vector2f velocityVector;
    velocityVector.x = velocity * std::cos(angle.asRadians());
    velocityVector.y = velocity * std::sin(angle.asRadians());

    //gravity constant
    const sf::Vector2f GRAVITY = {0.0f, 1000.0f};
    
    //set projectile
    sf::CircleShape circle(20.0f);
    circle.setFillColor(sf::Color::Red);
    circle.setPosition({240.0f, 720.0f});


    while (window.isOpen())
    {   
        //window event polling
        while (const std::optional event = window.pollEvent())
        {
            ImGui::SFML::ProcessEvent(window, *event);

            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }
        }
        
        //setup dt and imgui frame
        float dt = clock.restart().asSeconds();
        ImGui::SFML::Update(window, deltaClock.restart()); // update ImGui frame

        //recalculate angle
        angle = sf::degrees(-angleInDegrees);

        /* 
        reset animation if projectile hits the limit
        if (circle.getPosition().y >= 1100.0f)
        {
            circle.setPosition({240.0f, 720.0f});
            velocityVector.x = velocity * std::cos(angle.asRadians());
            velocityVector.y = velocity * std::sin(angle.asRadians());
            sf::sleep(sf::milliseconds(100));
        } 
        */

        //update physics
        velocityVector += GRAVITY * dt;
        circle.move(velocityVector * dt);

        //sfml render
        window.clear(sf::Color(200, 200, 200));
        window.draw(circle);

        //imgui objects
        ImGui::Begin("Settings");
        ImGui::SliderFloat("Angle", &angleInDegrees, 0.0f, 90.0f);
        ImGui::SliderFloat("Velocity", &velocity, 0.0f, 1200.0f);
        //projectile throw button
        if (ImGui::Button("Throw"))
        {
            circle.setPosition({240.0f, 720.0f});
            velocityVector.x = velocity * std::cos(angle.asRadians());
            velocityVector.y = velocity * std::sin(angle.asRadians());
        }
        ImGui::End();

        //display and render imgui, display sfml windows
        ImGui::SFML::Render(window);
        window.display();
    }

}
