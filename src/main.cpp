#include <iostream>

#include <SFML/Graphics.hpp>

#include <bigboy/CPU/CPU.h>

constexpr unsigned long WINDOW_WIDTH = 160;
constexpr unsigned long WINDOW_HEIGHT = 144;

int main() {
    auto cpu = std::make_unique<CPU>();
    cpu->load(Cartridge::fromFile("./resources/tests/06-ld r,r.gb"));

    // Create the main window
    sf::RenderWindow window{sf::VideoMode{WINDOW_WIDTH, WINDOW_HEIGHT}, "Bigboy"};

    sf::Texture frame{};
    frame.create(160,144);
    //sf::VertexArray frame{sf::Points, 160*144};

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

        try {
            frame.update(reinterpret_cast<const sf::Uint8*>(cpu->stepFrame().data()));
        } catch (const std::runtime_error& e) {
            std::cerr << e.what() << '\n';
        }

        // Clear the previous contents of the window
        window.clear(sf::Color::Black);

        // Render the text
        window.draw(sf::Sprite{frame});

        // Display the changes that we have made to the window since the last iteration
        window.display();
    }

    return 0;
}
