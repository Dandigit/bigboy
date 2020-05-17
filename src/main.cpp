#include <iostream>

#include <SFML/Graphics.hpp>

#include <bigboy/CPU/CPU.h>

constexpr unsigned long WINDOW_HEIGHT = 160;
constexpr unsigned long WINDOW_WIDTH = 144;

void updateVertexArray(sf::VertexArray& vertexArray, const std::array<Pixel, 160*144>& frame) {
    for (int y = 0; y < 144; ++y) {
        for (int x = 0; x < 160; ++x) {
            sf::Vertex vertex;
            vertex.position = sf::Vector2f(x, y);
            switch (frame[y * 160 + x]) {
                case Pixel::OFF:
                    vertex.color = sf::Color::White;
                    break;
                case Pixel::LIGHT_GREY:
                    vertex.color = {192, 192, 192};
                    break;
                case Pixel::DARK_GREY:
                    vertex.color = {96, 96, 96};
                    break;
                case Pixel::BLACK:
                    vertex.color = sf::Color::Black;
                    break;
            }
        }
    }
}

int main() {
    CPU cpu{};
    cpu.reset();
    cpu.load(Cartridge::fromFile("./resources/games/testGame.gb"));

    // Create the main window
    sf::RenderWindow window{sf::VideoMode{WINDOW_HEIGHT, WINDOW_WIDTH}, "Bigboy"};

    sf::VertexArray frame{sf::Points, 160*144};

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

        bool frameIsReady = false;
        try {
            frameIsReady = cpu.cycle();
        } catch (const std::runtime_error& e) {
            std::cerr << e.what();
        }

        if (frameIsReady) {
            updateVertexArray(frame, cpu.getCurrentFrame());
        }

        // Clear the previous contents of the window
        window.clear(sf::Color::White);

        // Render the text
        window.draw(frame);

        // Display the changes that we have made to the window since the last iteration
        window.display();
    }

    return 0;
}
