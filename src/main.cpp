#include <iostream>

#include <SFML/Graphics.hpp>

#include <bigboy/Emulator.h>

constexpr unsigned long SCREEN_WIDTH = 160;
constexpr unsigned long SCREEN_HEIGHT = 144;
constexpr unsigned long SCREEN_SCALE = 4;

void handleInput(Emulator& emulator,
        const sf::Keyboard::Key key,
        const InputEvent pressEvent,
        const InputEvent releaseEvent) {
    if (sf::Keyboard::isKeyPressed(key)) {
        emulator.handleInput(pressEvent);
    } else {
        emulator.handleInput(releaseEvent);
    }
}

void handleInputs(Emulator& emulator) {
    handleInput(emulator, sf::Keyboard::Down, InputEvent::DOWN_PRESSED, InputEvent::DOWN_RELEASED);
    handleInput(emulator, sf::Keyboard::Up, InputEvent::UP_PRESSED, InputEvent::UP_RELEASED);
    handleInput(emulator, sf::Keyboard::Left, InputEvent::LEFT_PRESSED, InputEvent::LEFT_RELEASED);
    handleInput(emulator, sf::Keyboard::Right, InputEvent::RIGHT_PRESSED, InputEvent::RIGHT_RELEASED);

    handleInput(emulator, sf::Keyboard::Z, InputEvent::START_PRESSED, InputEvent::START_RELEASED);
    handleInput(emulator, sf::Keyboard::X, InputEvent::SELECT_PRESSED, InputEvent::SELECT_RELEASED);
    handleInput(emulator, sf::Keyboard::S, InputEvent::B_PRESSED, InputEvent::B_RELEASED);
    handleInput(emulator, sf::Keyboard::A, InputEvent::A_PRESSED, InputEvent::A_RELEASED);
}

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "fatal: invalid command line arguments\n- usage: bigboy [rom_path]\n";
        return -1;
    }

    Emulator emulator{};
    if (!emulator.loadRomFile(argv[1])) {
        std::exit(-1);
    }

    const std::string savePath = "./saves/" + emulator.getGameTitle() + ".sav";
    emulator.loadRamFileIfSupported(savePath);

    // Create the main window
    sf::RenderWindow window{
        sf::VideoMode{SCREEN_WIDTH * SCREEN_SCALE, SCREEN_HEIGHT * SCREEN_SCALE},
        "Bigboy - " + emulator.getGameTitle()};
    window.setFramerateLimit(59);

    sf::Texture frameTexture{};
    frameTexture.create(160,144);

    sf::Sprite frameSprite{frameTexture};
    frameSprite.setScale(SCREEN_SCALE, SCREEN_SCALE);

    sf::Clock deltaClock;
    while (window.isOpen()) {
        // Handle all events triggered since the last iteration
        sf::Event event{};
        while (window.pollEvent(event)) {
            switch (event.type) {
                case sf::Event::Closed:
                    emulator.saveRamFileIfSupported(savePath);
                    window.close();
                    break;
                default:
                    //std::cerr << "[debug] unhandled sfml event\n";
                    break;
            }
        }

        handleInputs(emulator);

        frameTexture.update(reinterpret_cast<const sf::Uint8*>(emulator.update().data()));

        window.clear(sf::Color::Black);
        window.draw(frameSprite);

        window.display();
    }

    return 0;
}
