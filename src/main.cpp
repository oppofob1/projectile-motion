#include <SFML/Graphics.hpp>
#include <cmath>
#include <optional>

int main()
{
    auto window = sf::RenderWindow(sf::VideoMode({1920u, 1080u}), "CMake SFML Project");
    window.setFramerateLimit(144);

    sf::Clock clock;

    float angleInDegrees = 45.0f;
    sf::Angle angle = sf::degrees(-angleInDegrees);

    float velocity = 1000.f;
    sf::Vector2f velocityVector = {0.0f, 0.0f};

    velocityVector.x = velocity * std::cos(angle.asRadians());
    velocityVector.y = velocity * std::sin(angle.asRadians());

    const sf::Vector2f GRAVITY = {0.0f, 1000.0f};
    
    sf::CircleShape circle(20.0f);
    circle.setFillColor(sf::Color::Red);
    circle.setPosition({240.0f, 720.0f});


    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }
        }
        float dt = clock.restart().asSeconds();

        if (circle.getPosition().y >= 740.0f)
        {
            circle.setPosition({240.0f, 720.0f});
            velocityVector.x = velocity * std::cos(angle.asRadians());
            velocityVector.y = velocity * std::sin(angle.asRadians());
            sf::sleep(sf::milliseconds(100));
        }

        velocityVector += GRAVITY * dt;
        circle.move(velocityVector * dt);


        window.clear(sf::Color(200, 200, 200));

        window.draw(circle);
        window.display();
    }
}
