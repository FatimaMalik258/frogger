#include <SFML/Graphics.hpp>
#include <optional>
#include <vector>
#include <algorithm>
#include <string>
#include <cmath>
//struct is used to create structure
struct Car
{
    sf::RectangleShape shape;
    float speed;
};

struct WaterWave
{
    //vector is used to store multiple elements
    std::vector<sf::CircleShape> dots;
    float speed;
};

struct Leaf
{
    sf::CircleShape shape;
    float speed;
    //controls horizontal movement
    float drift;
    float age;
};

int main()
{
    //height is incresed from 600 to 650 to adjust river into the window
    sf::RenderWindow window(
        sf::VideoMode({ 800, 650 }),
        "Frogger Game"
    );

    window.setFramerateLimit(60);

    sf::RectangleShape background({ 800, 650 });
    background.setFillColor(sf::Color(50, 150, 60));

    sf::RectangleShape goal({ 800, 75 });
    goal.setPosition({ 0, 0 });
    goal.setFillColor(sf::Color(30, 180, 70));

    sf::RectangleShape house({ 100, 45 });
    house.setPosition({ 350, 25 });
    house.setFillColor(sf::Color(139, 69, 19));

    sf::ConvexShape roof;
    // 3 tells us that roof polygon will have 3 points
    roof.setPointCount(3);
    roof.setPoint(0, { 340, 25 });
    roof.setPoint(1, { 400, -10 });
    roof.setPoint(2, { 460, 25 });
    roof.setFillColor(sf::Color::Red);

    sf::RectangleShape door({ 25, 35 });
    door.setPosition({ 387, 35 });
    door.setFillColor(sf::Color(80, 40, 20));

    sf::RectangleShape houseWindow({ 20, 18 });
    houseWindow.setPosition({ 360, 38 });
    houseWindow.setFillColor(sf::Color::Cyan);  

    sf::RectangleShape trunk1({ 25, 65 });
    trunk1.setPosition({ 95, 25 });
    trunk1.setFillColor(sf::Color(100, 50, 20));

    sf::RectangleShape branch1a({ 35, 9 });
    branch1a.setPosition({ 70, 35 });
    branch1a.setRotation(sf::degrees(-35));
    branch1a.setFillColor(sf::Color(100, 50, 20));

    sf::RectangleShape branch1b({ 35, 9 });
    branch1b.setPosition({ 105, 30 });
    branch1b.setRotation(sf::degrees(35));
    branch1b.setFillColor(sf::Color(100, 50, 20));

    sf::CircleShape canopy1a(35);
    canopy1a.setPosition({ 35, -5 });
    canopy1a.setFillColor(sf::Color(20, 110, 25));

    sf::CircleShape canopy1b(38);
    canopy1b.setPosition({ 75, -20 });
    canopy1b.setFillColor(sf::Color(25, 130, 30));

    sf::CircleShape canopy1c(32);
    canopy1c.setPosition({ 110, 5 });
    canopy1c.setFillColor(sf::Color(20, 110, 25));

    sf::RectangleShape trunk2({ 25, 65 });
    trunk2.setPosition({ 675, 25 });
    trunk2.setFillColor(sf::Color(100, 50, 20));

    sf::RectangleShape branch2a({ 35, 9 });
    branch2a.setPosition({ 650, 35 });
    branch2a.setRotation(sf::degrees(-35));
    branch2a.setFillColor(sf::Color(100, 50, 20));

    sf::RectangleShape branch2b({ 35, 9 });
    branch2b.setPosition({ 685, 30 });
    branch2b.setRotation(sf::degrees(35));
    branch2b.setFillColor(sf::Color(100, 50, 20));

    sf::CircleShape canopy2a(35);
    canopy2a.setPosition({ 615, -5 });
    canopy2a.setFillColor(sf::Color(20, 110, 25));

    sf::CircleShape canopy2b(38);
    canopy2b.setPosition({ 655, -20 });
    canopy2b.setFillColor(sf::Color(25, 130, 30));

    sf::CircleShape canopy2c(32);
    canopy2c.setPosition({ 690, 5 });
    canopy2c.setFillColor(sf::Color(20, 110, 25));

    //flowing river is added
    sf::RectangleShape river({ 800, 90 });
    river.setPosition({ 0, 115 });
    river.setFillColor(sf::Color(0, 100, 255));

    // River Borders
    sf::RectangleShape riverTop({ 800, 4 });
    riverTop.setPosition({ 0, 115 });
    riverTop.setFillColor(sf::Color::White);

    sf::RectangleShape riverBottom({ 800, 4 });
    riverBottom.setPosition({ 0, 201 });
    riverBottom.setFillColor(sf::Color::White);

    std::vector<WaterWave> waterWaves;
    //values of y are for waves at different heights inside river 
    for (int y : {135, 165, 190})
    {
        for (int x = 0; x < 800; x += 200)
        {
            WaterWave wave;
            wave.speed = (y == 165) ? -45.f : 45.f;
            for (int i = 0; i < 9; i++)
            {
                sf::CircleShape dot(2.5f);
                //wave vertical position
                float waveY =
                    static_cast<float>(y) +
                    std::sin(i * 0.8f) * 4.f;

                dot.setPosition({
                    static_cast<float>(x + i * 10),
                    waveY
                    });

                dot.setFillColor(
                    sf::Color(150, 220, 255)
                );

                wave.dots.push_back(dot);
            }

            waterWaves.push_back(wave);
        }
    }

    std::vector<Leaf> leaves;
    sf::Clock leafClock;
    int leafCount = 0;

    //Road moved 90 pixels down and height reduced by 40 pixels
    //road now has four traffic lanes instead of five
    sf::RectangleShape road({ 800, 260 });
    road.setPosition({ 0, 230 });
    road.setFillColor(sf::Color(65, 65, 65));

    sf::CircleShape frog(15);
    frog.setFillColor(sf::Color::Green);
    frog.setPosition({ 385, 610 });

    std::vector<Car> cars;
    //reduced width of car from 90 to 65
    //used lambda to call this function again to make cars
    auto addCar = [&](float x, float y,
        sf::Color color, float speed)
        {
            sf::RectangleShape car({ 65, 32 });
            car.setPosition({ x, y });
            car.setFillColor(color);

            cars.push_back({ car, speed });
        };

    addCar(50, 245, sf::Color::Red, 180);
    addCar(350, 245, sf::Color::Red, 180);
    addCar(650, 245, sf::Color::Red, 180);

    addCar(100, 305, sf::Color::Blue, -150);
    addCar(400, 305, sf::Color::Blue, -150);
    addCar(700, 305, sf::Color::Blue, -150);

    addCar(0, 365, sf::Color::Yellow, 200);
    addCar(300, 365, sf::Color::Yellow, 200);
    addCar(600, 365, sf::Color::Yellow, 200);

    addCar(150, 425, sf::Color::Magenta, -170);
    addCar(450, 425, sf::Color::Magenta, -170);
    addCar(750, 425, sf::Color::Magenta, -170);

    sf::Clock clock;

    int score = 0;
    while (window.isOpen())
    {
        float dt = clock.restart().asSeconds();
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();

            if (const auto* key =
                event->getIf<sf::Event::KeyPressed>())
            {
                if (key->code == sf::Keyboard::Key::Escape)
                    window.close();

                if (key->code == sf::Keyboard::Key::Up)
                    frog.move({ 0, -40 });

                if (key->code == sf::Keyboard::Key::Down)
                    frog.move({ 0, 40 });

                if (key->code == sf::Keyboard::Key::Left)
                    frog.move({ -40, 0 });

                if (key->code == sf::Keyboard::Key::Right)
                    frog.move({ 40, 0 });
            }
        }

        // Keep Frog Inside Screen
        sf::Vector2f frogPos = frog.getPosition();
        frogPos.x = std::clamp(frogPos.x, 0.f, 770.f);
        frogPos.y = std::clamp(frogPos.y, 0.f, 620.f);
        frog.setPosition(frogPos);

        for (auto& wave : waterWaves)
        {
            for (auto& dot : wave.dots)
                //for movement
                dot.move({ wave.speed * dt, 0 });
            float x = wave.dots[0].getPosition().x;
            if (wave.speed > 0 && x > 800)
            {
                for (auto& dot : wave.dots)
                    dot.move({ -880, 0 });
            }

            if (wave.speed < 0 && x < -90)
            {
                for (auto& dot : wave.dots)
                    dot.move({ 890, 0 });
            }
        }

        if (leafClock.getElapsedTime().asSeconds() > 1.5f)
        {
            Leaf leaf;
            leaf.shape = sf::CircleShape(5);
            leaf.shape.setScale({ 1.5f, 0.7f });
            leaf.shape.setFillColor(
                sf::Color(100, 200, 35)
            );

            // Alternate between both trees
            float startX;

            if (leafCount % 2 == 0)
                startX = 55.f + (leafCount % 4) * 15.f;
            else
                startX = 635.f + (leafCount % 4) * 15.f;

            leaf.shape.setPosition({ startX, 55.f });

            leaf.speed = 35.f + (leafCount % 3) * 10.f;
            leaf.drift = (leafCount % 2 == 0) ? 18.f : -18.f;
            leaf.age = 0.f;

            leaves.push_back(leaf);

            leafCount++;
            leafClock.restart();
        }

        for (auto& leaf : leaves)
        {
            leaf.age += dt;

            sf::Vector2f leafPos = leaf.shape.getPosition();

            if (leafPos.y < 195)
            {
                // Fall towards the river
                leaf.shape.move({
                    leaf.drift * dt,
                    leaf.speed * dt
                    });
            }
            else
            {
                // Float gently on river
                leaf.shape.move({
                    leaf.drift * 0.5f * dt,
                    0
                    });
            }

            // Wrap leaves horizontally
            if (leaf.shape.getPosition().x > 800)
                leaf.shape.setPosition({ -10, leafPos.y });

            if (leaf.shape.getPosition().x < -10)
                leaf.shape.setPosition({ 800, leafPos.y });
        }

        // Remove Old Leaves
        leaves.erase(
            std::remove_if(
                leaves.begin(),
                leaves.end(),
                [](const Leaf& leaf)
                {
                    return leaf.age > 7.f;
                }
            ),
            leaves.end()
        );
        for (auto& car : cars)
        {
            car.shape.move({ car.speed * dt, 0 });

            float x = car.shape.getPosition().x;

            if (car.speed > 0 && x > 810)
            {
                car.shape.setPosition({
                    -65,
                    car.shape.getPosition().y
                    });
            }

            if (car.speed < 0 && x < -65)
            {
                car.shape.setPosition({
                    810,
                    car.shape.getPosition().y
                    });
            }
        }
        bool collision = false;

        for (const auto& car : cars)
        {
            sf::Vector2f frogPosition = frog.getPosition();
            sf::Vector2f carPosition = car.shape.getPosition();

            bool overlap =
                frogPosition.x < carPosition.x + 65 &&
                frogPosition.x + 30 > carPosition.x &&
                frogPosition.y < carPosition.y + 32 &&
                frogPosition.y + 30 > carPosition.y;

            if (overlap)
            {
                collision = true;
                break;
            }
        }

        // Reset Frog After Collision
        if (collision)
            frog.setPosition({ 385, 610 });

        if (frog.getPosition().y <= 50)
        {
            score++;

            window.setTitle(
                "Frogger Game - Score: " +
                std::to_string(score)
            );

            frog.setPosition({ 385, 610 });
        }
        window.clear();
        window.draw(background);

        window.draw(goal);

        window.draw(trunk1);
        window.draw(branch1a);
        window.draw(branch1b);
        window.draw(canopy1a);
        window.draw(canopy1b);
        window.draw(canopy1c);

        window.draw(trunk2);
        window.draw(branch2a);
        window.draw(branch2b);
        window.draw(canopy2a);
        window.draw(canopy2b);
        window.draw(canopy2c);

        window.draw(house);
        window.draw(roof);
        window.draw(door);
        window.draw(houseWindow);

        window.draw(river);
        window.draw(riverTop);
        window.draw(riverBottom);

        for (const auto& wave : waterWaves)
        {
            for (const auto& dot : wave.dots)
                window.draw(dot);
        }

        for (const auto& leaf : leaves)
            window.draw(leaf.shape);

        window.draw(road);

        // Road Markings
        for (int y = 290; y <= 410; y += 60)
        {
            sf::RectangleShape line({ 800, 2 });

            line.setPosition({
                0,
                static_cast<float>(y)
                });

            line.setFillColor(sf::Color::White);
            window.draw(line);
        }

        //narrow dowed the size of cars
        for (const auto& car : cars)
        {
            sf::Vector2f pos = car.shape.getPosition();
            sf::Color carColor = car.shape.getFillColor();

            sf::RectangleShape body({ 65, 28 });
            body.setPosition({ pos.x, pos.y + 4 });
            body.setFillColor(carColor);

            sf::RectangleShape cabin({ 40, 18 });
            cabin.setPosition({ pos.x + 12, pos.y - 10 });
            cabin.setFillColor(carColor);

            sf::RectangleShape window1({ 16, 13 });
            window1.setPosition({ pos.x + 15, pos.y - 8 });
            window1.setFillColor(sf::Color(180, 220, 240));

            sf::RectangleShape window2({ 16, 13 });
            window2.setPosition({ pos.x + 34, pos.y - 8 });
            window2.setFillColor(sf::Color(180, 220, 240));

            sf::CircleShape wheel1(7);
            wheel1.setPosition({ pos.x + 8, pos.y + 23 });
            wheel1.setFillColor(sf::Color::Black);

            sf::CircleShape wheel2(7);
            wheel2.setPosition({ pos.x + 49, pos.y + 23 });
            wheel2.setFillColor(sf::Color::Black);

            window.draw(body);
            window.draw(cabin);
            window.draw(window1);
            window.draw(window2);
            window.draw(wheel1);
            window.draw(wheel2);
        }

        window.draw(frog);
        window.display();
    }

    return 0;
}
