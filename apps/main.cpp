#include <iostream>
#include <stdexcept>
#include <unordered_map>

#include <SDL.h>

#include <bigboy/Emulator.h>

class App {
public:
    App(const std::string& romPath) {
        // Initialise Bigboy
        if (!m_emulator.loadRomFile(romPath)) {
            throw std::runtime_error{"Bigboy could not load a ROM from the path " + romPath};
        }

        const std::string savePath = "./saves/" + m_emulator.getGameTitle() + ".sav";
        m_emulator.loadRamFileIfSupported(savePath);

        // Initialise SDL
        if (SDL_Init(SDL_INIT_VIDEO) != 0) {
            throw std::runtime_error{"SDL could not initialise: " + std::string{SDL_GetError()}};
        }

        m_window = SDL_CreateWindow(
                ("Bigboy (SDL) - " + m_emulator.getGameTitle()).c_str(),
                SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                screenWidth, screenHeight,
                SDL_WINDOW_SHOWN);
        if (!m_window) {
            throw std::runtime_error{"SDL could not create window: " + std::string{SDL_GetError()}};
        }

        m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED);
        if (!m_renderer) {
            throw std::runtime_error{"SDL could not create renderer: " + std::string{SDL_GetError()}};
        }

        // Bigboy gives us an array of 160*144 pixel values, which are each a 32-bit struct
        // containing the red, green, blue and alpha 8-bit channels in that order
        m_screen = SDL_CreateTexture(
        m_renderer,
        SDL_PIXELFORMAT_ABGR8888,
        SDL_TEXTUREACCESS_STREAMING,
            160, 144);
        if (!m_screen) {
            throw std::runtime_error{"SDL could not create texture: " + std::string{SDL_GetError()}};
        }
    }

    ~App() {
        // Destroy screen texture
        SDL_DestroyTexture(m_screen);

        // Destroy renderer
        SDL_DestroyRenderer(m_renderer);

        // Destroy window
        SDL_DestroyWindow(m_window);

        // Quit SDL subsystems
        SDL_Quit();
    }

    void run() {
        m_running = true;

        while (m_running) {
            handleEvents();

            const std::array<Colour, 160*144>& frame = m_emulator.update();
            auto framePixels = reinterpret_cast<const uint8_t*>(frame.data());
            uint8_t* screenPixels;
            int pitch = 0;

            // Copy the frame data returned by Bigboy into the texture
            SDL_LockTexture(m_screen, nullptr, reinterpret_cast<void**>(&screenPixels), &pitch);
            std::memcpy(screenPixels, framePixels, 160*144*4);
            SDL_UnlockTexture(m_screen);

            // Clear out our renderer
            SDL_RenderClear(m_renderer);

            // Render the screen texture
            SDL_RenderCopy(m_renderer, m_screen, nullptr, nullptr);

            // Present our changes to the window
            SDL_RenderPresent(m_renderer);
        }
    }

private:
    void handleEvents() {
        SDL_Event event;
        while (SDL_PollEvent(&event) != 0) {
            switch (event.type) {
                case SDL_KEYDOWN: {
                    SDL_Keycode key = event.key.keysym.sym;
                    if (pressEvents.count(key) > 0) {
                        m_emulator.handleInput(pressEvents.at(key));
                    }
                    break;
                }
                case SDL_KEYUP: {
                    SDL_Keycode key = event.key.keysym.sym;
                    if (releaseEvents.count(key) > 0) {
                        m_emulator.handleInput(releaseEvents.at(key));
                    }
                    break;
                }
                case SDL_QUIT:
                    m_running = false;
                    break;
            }
        }
    }

    Emulator m_emulator; // Our Bigboy instance

    SDL_Window* m_window; // The window we'll be rendering to
    SDL_Renderer* m_renderer; // The renderer that updates the window
    SDL_Texture* m_screen; // The Gameboy "screen" we will be rendering

    bool m_running = false;

    static constexpr unsigned int screenScale = 4;
    static constexpr unsigned int screenWidth = 160 * screenScale;
    static constexpr unsigned int screenHeight = 144 * screenScale;

    static inline const std::unordered_map<SDL_Keycode, InputEvent> pressEvents{
            {SDLK_UP, InputEvent::UP_PRESSED},
            {SDLK_DOWN, InputEvent::DOWN_PRESSED},
            {SDLK_LEFT, InputEvent::LEFT_PRESSED},
            {SDLK_RIGHT, InputEvent::RIGHT_PRESSED},
            {SDLK_a, InputEvent::A_PRESSED},
            {SDLK_s, InputEvent::B_PRESSED},
            {SDLK_z, InputEvent::START_PRESSED},
            {SDLK_x, InputEvent::SELECT_PRESSED}};
    static inline const std::unordered_map<SDL_Keycode, InputEvent> releaseEvents{
            {SDLK_UP, InputEvent::UP_RELEASED},
            {SDLK_DOWN, InputEvent::DOWN_RELEASED},
            {SDLK_LEFT, InputEvent::LEFT_RELEASED},
            {SDLK_RIGHT, InputEvent::RIGHT_RELEASED},
            {SDLK_a, InputEvent::A_RELEASED},
            {SDLK_s, InputEvent::B_RELEASED},
            {SDLK_z, InputEvent::START_RELEASED},
            {SDLK_x, InputEvent::SELECT_RELEASED}};
};

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "fatal: invalid command line arguments\n- usage: bigboy [rom_path]\n";
        return -1;
    }

    App app{argv[1]};
    app.run();

    return 0;
}

/*constexpr unsigned long SCREEN_WIDTH = 160;
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
}*/
