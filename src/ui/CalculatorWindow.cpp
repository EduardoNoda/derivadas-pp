#include "../../include/ui/CalculatorWindow.h"

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include <array>
#include <filesystem>
#include <iostream>
#include <optional>
#include <sstream>
#include <vector>

namespace calc::ui {

namespace {

// ---- Constantes de layout e cores (so apresentacao) ----------------------
constexpr unsigned kWindowWidth = 820;
constexpr unsigned kWindowHeight = 400;
constexpr unsigned kMaxInputLength = 200;

const sf::Color kBackground{30, 32, 40};
const sf::Color kPanel{45, 48, 60};
const sf::Color kText{235, 235, 240};
const sf::Color kMuted{150, 155, 170};
const sf::Color kAccent{70, 130, 220};
const sf::Color kAccentHover{95, 155, 245};
const sf::Color kOk{130, 220, 150};
const sf::Color kError{240, 105, 105};

bool loadFont(sf::Font& font) {
    const std::array<const char*, 9> candidates = {
        "assets/font.ttf",
        "C:/Windows/Fonts/consola.ttf",
        "C:/Windows/Fonts/lucon.ttf",
        "C:/Windows/Fonts/cour.ttf",
        "C:/Windows/Fonts/arial.ttf",
        "/usr/share/fonts/TTF/DejaVuSansMono.ttf",
        "/usr/share/fonts/dejavu/DejaVuSansMono.ttf",
        "/usr/share/fonts/truetype/dejavu/DejaVuSansMono.ttf",
        "/usr/share/fonts/liberation/LiberationMono-Regular.ttf",
    };
    for (const char* path : candidates) {
        if (std::filesystem::exists(path) && font.openFromFile(path)) {
            return true;
        }
    }
    return false;
}

std::vector<std::string> wrapText(const std::string& text, const sf::Font& font,
                                  unsigned size, float maxWidth) {
    std::vector<std::string> lines;
    std::istringstream words(text);
    std::string word;
    std::string line;
    sf::Text probe(font, "", size);

    while (words >> word) {
        const std::string candidate = line.empty() ? word : line + " " + word;
        probe.setString(candidate);
        if (!line.empty() && probe.getLocalBounds().size.x > maxWidth) {
            lines.push_back(line);
            line = word;
        } else {
            line = candidate;
        }
    }
    if (!line.empty()) {
        lines.push_back(line);
    }
    return lines;
}

std::string fitTail(const std::string& text, const sf::Font& font,
                    unsigned size, float maxWidth) {
    sf::Text probe(font, text, size);
    std::string shown = text;
    while (!shown.empty() && probe.getLocalBounds().size.x > maxWidth) {
        shown.erase(0, 1);
        probe.setString(shown);
    }
    return shown;
}

bool isPrintableAscii(char32_t c) {
    return c >= 32 && c < 127;
}

}  // namespace

CalculatorWindow::CalculatorWindow(const DerivativeService& service)
    : service_(service) {}

void CalculatorWindow::calculate() {
    // Unica ligacao com o nucleo: uma chamada ao servico.
    const CalculationResult result = service_.calculate(input_);
    output_ = result.message;
    outputIsError_ = !result.ok;
    hasOutput_ = true;
}

int CalculatorWindow::run() {
    sf::Font font;
    if (!loadFont(font)) {
        std::cerr << "Nao foi possivel carregar nenhuma fonte. "
                     "Coloque um arquivo .ttf em assets/font.ttf\n";
        return 1;
    }

    sf::RenderWindow window(sf::VideoMode({kWindowWidth, kWindowHeight}),
                            "Calculadora de Derivadas",
                            sf::Style::Titlebar | sf::Style::Close);
    window.setFramerateLimit(60);

    sf::Text title(font, "Calculadora de Derivadas", 28);
    title.setStyle(sf::Text::Bold);
    title.setFillColor(kText);
    title.setPosition({30.f, 20.f});

    sf::Text hint(font, "Exemplos:  3x^2 + 5ln(x)   |   4x^-2   |   2x^1.5 - 7", 16);
    hint.setFillColor(kMuted);
    hint.setPosition({ 30.f, 54.f });

    // Lembrete da notacao rigida (evita erros comuns de digitacao).
    sf::Text syntaxHint(font, "Sintaxe:  coeficiente antes do x   |   ln(x) com parenteses   |   1/x = x^-1", 16);
    syntaxHint.setFillColor(kMuted);
    syntaxHint.setPosition({ 30.f, 76.f });

    sf::Text prefix(font, "f(x) =", 24);
    prefix.setFillColor(kText);
    prefix.setPosition({30.f, 112.f});

    sf::RectangleShape inputBox({660.f, 46.f});
    inputBox.setPosition({130.f, 104.f});
    inputBox.setFillColor(kPanel);
    inputBox.setOutlineThickness(2.f);
    inputBox.setOutlineColor(kAccent);

    sf::Text inputText(font, "", 24);
    inputText.setFillColor(kText);
    inputText.setPosition({142.f, 112.f});

    sf::RectangleShape button({160.f, 44.f});
    button.setPosition({130.f, 170.f});

    sf::Text buttonLabel(font, "Derivar", 22);
    buttonLabel.setFillColor(kText);
    {
        const sf::FloatRect b = buttonLabel.getLocalBounds();
        buttonLabel.setOrigin({b.position.x + b.size.x / 2.f, b.position.y + b.size.y / 2.f});
        buttonLabel.setPosition({130.f + 80.f, 170.f + 22.f});
    }

    sf::RectangleShape resultPanel({760.f, 140.f});
    resultPanel.setPosition({30.f, 240.f});
    resultPanel.setFillColor(kPanel);

    sf::Clock cursorClock;

    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            } else if (const auto* typed = event->getIf<sf::Event::TextEntered>()) {
                if (isPrintableAscii(typed->unicode) && input_.size() < kMaxInputLength) {
                    input_ += static_cast<char>(typed->unicode);
                }
            } else if (const auto* key = event->getIf<sf::Event::KeyPressed>()) {
                if (key->code == sf::Keyboard::Key::Enter) {
                    calculate();
                } else if (key->code == sf::Keyboard::Key::Backspace) {
                    if (!input_.empty()) {
                        input_.pop_back();
                    }
                } else if (key->control && key->code == sf::Keyboard::Key::V) {
                    for (char c : sf::Clipboard::getString().toAnsiString()) {
                        if (isPrintableAscii(static_cast<char32_t>(static_cast<unsigned char>(c))) &&
                            input_.size() < kMaxInputLength) {
                            input_ += c;
                        }
                    }
                }
            } else if (const auto* click = event->getIf<sf::Event::MouseButtonPressed>()) {
                if (click->button == sf::Mouse::Button::Left &&
                    button.getGlobalBounds().contains(sf::Vector2f(click->position))) {
                    calculate();
                }
            }
        }

        const sf::Vector2f mouse(sf::Mouse::getPosition(window));
        button.setFillColor(button.getGlobalBounds().contains(mouse) ? kAccentHover : kAccent);

        const bool cursorVisible =
            static_cast<int>(cursorClock.getElapsedTime().asSeconds() * 2.f) % 2 == 0;
        inputText.setString(fitTail(input_, font, 24, 636.f) + (cursorVisible ? "|" : ""));

        window.clear(kBackground);
        window.draw(title);
        window.draw(hint);
        window.draw(prefix);
        window.draw(inputBox);
        window.draw(inputText);
        window.draw(button);
        window.draw(buttonLabel);
        window.draw(resultPanel);

        if (hasOutput_) {
            const std::string display = outputIsError_ ? output_ : "f'(x) = " + output_;
            const std::vector<std::string> lines = wrapText(display, font, 22, 730.f);
            float y = 254.f;
            for (const std::string& line : lines) {
                sf::Text lineText(font, line, 22);
                lineText.setFillColor(outputIsError_ ? kError : kOk);
                lineText.setPosition({46.f, y});
                window.draw(lineText);
                y += 30.f;
            }
        }

        window.display();
    }
    return 0;
}

}  // namespace calc::ui
