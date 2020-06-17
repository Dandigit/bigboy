#include <iostream>

#include <SFML/Graphics.hpp>

#include <bigboy/CPU/CPU.h>

constexpr unsigned long SCREEN_WIDTH = 160;
constexpr unsigned long SCREEN_HEIGHT = 144;
constexpr unsigned long SCREEN_SCALE = 3;

void handleInput(CPU& cpu,
        const sf::Keyboard::Key key,
        const InputEvent pressEvent,
        const InputEvent releaseEvent) {
    if (sf::Keyboard::isKeyPressed(key)) {
        cpu.handleInput(pressEvent);
    } else {
        cpu.handleInput(releaseEvent);
    }
}

void handleInputs(CPU& cpu) {
    handleInput(cpu, sf::Keyboard::Down, InputEvent::DOWN_PRESSED, InputEvent::DOWN_RELEASED);
    handleInput(cpu, sf::Keyboard::Up, InputEvent::UP_PRESSED, InputEvent::UP_RELEASED);
    handleInput(cpu, sf::Keyboard::Left, InputEvent::LEFT_PRESSED, InputEvent::LEFT_RELEASED);
    handleInput(cpu, sf::Keyboard::Right, InputEvent::RIGHT_PRESSED, InputEvent::RIGHT_RELEASED);

    handleInput(cpu, sf::Keyboard::Z, InputEvent::START_PRESSED, InputEvent::START_RELEASED);
    handleInput(cpu, sf::Keyboard::X, InputEvent::SELECT_PRESSED, InputEvent::SELECT_RELEASED);
    handleInput(cpu, sf::Keyboard::S, InputEvent::B_PRESSED, InputEvent::B_RELEASED);
    handleInput(cpu, sf::Keyboard::A, InputEvent::A_PRESSED, InputEvent::A_RELEASED);
}

int main() {
    CPU cpu{};
    cpu.load(Cartridge::fromFile("./resources/games/Tetris.gb"));

    // Create the main window
    sf::RenderWindow window{
        sf::VideoMode{SCREEN_WIDTH * SCREEN_SCALE, SCREEN_HEIGHT * SCREEN_SCALE},
        "Bigboy - " + cpu.getGameTitle()};
    window.setFramerateLimit(60);

    sf::Texture frameTexture{};
    frameTexture.create(160,144);

    sf::Sprite frameSprite{frameTexture};
    frameSprite.setScale(SCREEN_SCALE, SCREEN_SCALE);

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
                    //std::cerr << "[debug] unhandled sfml event\n";
                    break;
            }
        }

        handleInputs(cpu);
        frameTexture.update(reinterpret_cast<const sf::Uint8*>(cpu.stepFrame().data()));

        window.clear(sf::Color::Black);
        window.draw(frameSprite);
        window.display();
    }

    return 0;
}
