#include <iostream>

#include <SFML/Graphics.hpp>

#include <bigboy/CPU/CPU.h>

constexpr unsigned long SCREEN_WIDTH = 160;
constexpr unsigned long SCREEN_HEIGHT = 144;

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

    handleInput(cpu, sf::Keyboard::X, InputEvent::START_PRESSED, InputEvent::START_RELEASED);
    handleInput(cpu, sf::Keyboard::S, InputEvent::SELECT_PRESSED, InputEvent::SELECT_RELEASED);
    handleInput(cpu, sf::Keyboard::C, InputEvent::B_PRESSED, InputEvent::B_RELEASED);
    handleInput(cpu, sf::Keyboard::Z, InputEvent::A_PRESSED, InputEvent::A_RELEASED);
}

int main() {
    CPU cpu{};
    cpu.load(Cartridge::fromFile("./resources/tests/11-op a,(hl).gb"));

    // Create the main window
    sf::RenderWindow window{sf::VideoMode{SCREEN_WIDTH, SCREEN_HEIGHT}, "Bigboy"};

    sf::Texture frame{};
    frame.create(160,144);

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
        frame.update(reinterpret_cast<const sf::Uint8*>(cpu.stepFrame().data()));

        window.clear(sf::Color::Black);
        window.draw(sf::Sprite{frame});
        window.display();
    }

    return 0;
}
