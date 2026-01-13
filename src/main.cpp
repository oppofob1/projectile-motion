// MIT License
// Copyright (c) 2026 İbrahim Aslan
#include <SFML/Graphics.hpp>
#include <cmath>
#include <optional>
#include <imgui.h>
#include <imgui-SFML.h>

int main()
{   
    //setup the window
    auto window = sf::RenderWindow(sf::VideoMode({1920u, 1080u}), "CMake SFML Project");
    window.setFramerateLimit(144);

    if (!ImGui::SFML::Init(window))
        return -1;

    //setup clocks, clock for physics update, deltaClock for ImGui
    sf::Clock clock;
    sf::Clock deltaClock;

    //starting angle, velocity and elasticity
    float elasticity = 0.8f;
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
    circle.setOrigin({15.0f, 15.0f});

    //set trail
    sf::CircleShape trailPoint(5.0f);
    trailPoint.setFillColor(sf::Color(50, 50, 50));

    //trail position array
    std::vector<sf::Vector2f> trail;

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

        //update physics 
        velocityVector += GRAVITY * dt;
        circle.move(velocityVector * dt);

        //floor collision
        if (circle.getPosition().y >= 1055.0f)
        {
            circle.setPosition({circle.getPosition().x, 1055.0f});
            velocityVector.y = -velocityVector.y * elasticity;

            //stop very small velocities
            if (std::abs(velocityVector.y) < 1.f) velocityVector.y = 0.f;
        }

        //wall collisions
        if (circle.getPosition().x >= 1895.0f)
        {
            circle.setPosition({1895.0f, circle.getPosition().y});
            velocityVector.x = -velocityVector.x * elasticity;
        }
        if (circle.getPosition().x <= 15.0f)
        {
            circle.setPosition({15.0f, circle.getPosition().y});
            velocityVector.x = -velocityVector.x * elasticity;
        }

        //draw trail
        trail.push_back(circle.getPosition());

        //sfml render
        window.clear(sf::Color(200, 200, 200));
        window.draw(circle);
        for (size_t i = 0; i < trail.size(); ++i)
        {
            trailPoint.setPosition(trail[i]);
            window.draw(trailPoint);
        }

        //imgui objects
        ImGui::SetNextWindowPos(ImVec2(100, 100), ImGuiCond_Always);
        ImGui::SetNextWindowSize(ImVec2(300, 200), ImGuiCond_Always);

        ImGui::Begin("Settings");
        ImGui::SliderFloat("Angle", &angleInDegrees, 0.0f, 90.0f);
        ImGui::SliderFloat("Velocity", &velocity, 0.0f, 1200.0f);
        ImGui::SliderFloat("Elasticity", &elasticity, 0.0f, 1.0f);

        //projectile throw button
        if (ImGui::Button("Throw"))
        {
            circle.setPosition({240.0f, 720.0f});
            velocityVector.x = velocity * std::cos(angle.asRadians());
            velocityVector.y = velocity * std::sin(angle.asRadians());
            trail.erase(trail.begin(), trail.end());
        }
        ImGui::End();

        //display and render imgui, display sfml windows
        ImGui::SFML::Render(window);
        window.display();
    }
}
