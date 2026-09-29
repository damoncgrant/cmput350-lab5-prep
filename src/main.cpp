#include <algorithm>
#include <cstdint>
#include <iostream>
#include <memory>
#include <random>
#include <vector>
#include <cmath>
#include <numbers>

#include <SFML/Graphics.hpp>

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 800;
const int FPS_LIMIT = 30;
const float ANIMATION_TIME = 2.f; // 2 seconds
const float CIRCLE_RADIUS = 15.0f;

// global tween function
std::function<float(float, float, float)> tween = [](float a, float b, float t) {
    return (1 - t) * a + t * b;
};

void handleInput(sf::Window& window, bool& shouldQuit) {
    while (const std::optional<sf::Event> event = window.pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            window.close();
            shouldQuit = true;
        }

        // ====== ====== ======
        // TODO: (Q2)
        //  implement key presses (1-9) that replace the tween function
        //  with different alternate tween functions.
        //  Functions can be from lecture or from https://easings.net/#
        // ====== ====== ======
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num1)) {  // Default linear
            tween = [](float a, float b, float t) {
                return (1 - t) * a + t * b;
            };
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num2)) {  // Ease in quad
            tween = [](float a, float b, float t) {
                float tNew = t * t;
                return (1 - tNew) * a + tNew * b;
            };
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num3)) {  // Ease in cube
            tween = [](float a, float b, float t) {
                float tNew = t * t * t;
                return (1 - tNew) * a + tNew * b;
            };
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num4)) {  // Sin ease in out
            tween = [](float a, float b, float t) {
                float tNew = (std::sin((t - 0.5f) * std::numbers::pi) + 1) / 2;
                return (1 - tNew) * a + tNew * b;
            };
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num5)) {  // Ease in out expo
            tween = [](float a, float b, float t) {
                float tNew = t == 0 ? 0 : t == 1 ? 1 : t < 0.5f ? std::pow(2, 20 * t - 10) / 2 : (2 - std::pow(2, -20 * t + 10)) / 2;
                return (1 - tNew) * a + tNew * b;
            };
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num6)) {  // Ease in back
            tween = [](float a, float b, float t) {
                float c1 = 1.70158f;
                float c2 = c1 + 1;  
                float tNew = c2 * t * t * t - c1 * t * t;
                return (1 - tNew) * a + tNew * b;
            };
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num7)) {  // Ease out elastic 
            tween = [](float a, float b, float t) {
                float c1 = (2 * std::numbers::pi) / 3;
                float tNew = t == 0 ? 0 : t == 1 ? 1 : std::pow(2, -10 * t) * std::sin((t * 10 - 0.75f) * c1) + 1;
                return (1 - tNew) * a + tNew * b;
            };
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num8)) {  // Ease out circle
            tween = [](float a, float b, float t) {
                float tNew = std::sqrt(1 - std::pow(t - 1, 2));
                return (1 - tNew) * a + tNew * b;
            };
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num9)) {  // Ease out bounce
            tween = [](float a, float b, float t) {
                float c1 = (2 * std::numbers::pi) / 4.5f;
                float tNew = t == 0 ? 0 : t == 1 ? 1 : t < 0.5 ? -1 * std::pow(2, 20 * t - 10) * std::sin((20 * t - 11.125) * c1) / 2 :
                    std::pow(2, -20 * t + 10) * std::sin((20 * t - 11.125) * c1) / 2 + 1;
                return (1 - tNew) * a + tNew * b;
            };
        }
    }
}

void render(sf::RenderWindow& window) {
    // Clear with blue background (sky)
    window.clear(sf::Color::Black);
    // ====== ====== ======
    // TODO: (Q1) Draw circle that moves between
    // the left/right half of the screen.
    // Movement should be governed by the tween function.
    // ====== ====== ======
    static float t = 0.f;
    static int direction = 1;
    sf::CircleShape circle(CIRCLE_RADIUS);
    
    circle.setFillColor(sf::Color::Blue);
    const float a = 0.f + 2.f * CIRCLE_RADIUS;
    const float b = WINDOW_WIDTH - 2.f * CIRCLE_RADIUS;

    t += direction / (ANIMATION_TIME * FPS_LIMIT);

    if (t > 1) {
        t = 1.f;
        direction = -1;
    } else if (t <= 0) {
        t = 0.f;
        direction = 1;
    }

    circle.setPosition({tween(a, b, t), WINDOW_HEIGHT * 0.33f});
    window.draw(circle);
    

    // ====== ====== ======
    // TODO: (Q3) Draw tween function graph with a dot
    // on the current portion of the curve
    // ====== ====== ======
    window.display();
}

int main() {
    sf::RenderWindow window;

    try {
        // Initialize window
        window.create(sf::VideoMode({WINDOW_WIDTH, WINDOW_HEIGHT}), "Tween");
        window.setFramerateLimit(FPS_LIMIT);
        // Prevent key repeats.
        window.setKeyRepeatEnabled(false);

        bool shouldQuit = false;
        // Main game loop
        while (window.isOpen()) {
            handleInput(window, shouldQuit);
            if (shouldQuit) {
                break;
            }
            render(window);
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return -1;
    }
    return 0;
}
