#include <iostream>

#include <SFML/Graphics.hpp>

#include "CPU.h"

constexpr unsigned long WINDOW_HEIGHT = 160;
constexpr unsigned long WINDOW_WIDTH = 144;

int main() {
    std::cout << std::bitset<8>{0x10};

    // Create the main window
    sf::RenderWindow window{sf::VideoMode{WINDOW_HEIGHT, WINDOW_WIDTH}, "Bigboy"};

    // Make some text to render on it
    sf::Font pixelated{};
    pixelated.loadFromFile("./resources/fonts/VT323-Regular.ttf");

    sf::Text helloText{};
    helloText.setPosition(15.f, 50.f);
    helloText.setFont(pixelated);
    helloText.setString("Hello, world!");
    helloText.setCharacterSize(24);
    helloText.setFillColor(sf::Color::Black);

    // Keep it open
    while (window.isOpen()) {
        // Handle all events triggered since the last iteration
        sf::Event event{};
        while (window.pollEvent(event)) {
            switch (event.type) {
                case sf::Event::Closed:
                    window.close();
                    break;
                default:
                    std::cerr << "[debug] unhandled sfml event\n";
                    break;
            }
        }

        // Clear the previous contents of the window
        window.clear(sf::Color::White);

        // Render the text
        window.draw(helloText);

        // Display the changes that we have made to the window since the last iteration
        window.display();
    }

    return 0;
}
