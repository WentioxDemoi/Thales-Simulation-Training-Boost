#include "UI.hpp"
#include <iostream>
#include <algorithm>

bool UIManager::initialize(const std::string& font_path) {
    window.create(sf::VideoMode(800, 600), "Thales Chat App");
    
    if (!font.loadFromFile(font_path)) {
        std::cerr << "Impossible to load font : " << font_path << std::endl;
        return false;
    }
    font_loaded = true;

    inputText.setFont(font);
    inputText.setCharacterSize(TEXT_SIZE);
    inputText.setFillColor(sf::Color::White);

    placeholderText.setFont(font);
    placeholderText.setCharacterSize(TEXT_SIZE);
    placeholderText.setFillColor(sf::Color(128, 128, 128));
    placeholderText.setString("Type message...");

    return true;
}

void UIManager::setSendCallback(std::function<void(const std::string&)> callback) {
    send_callback = std::move(callback);
}

void UIManager::setMyMessageCallback(std::function<void(const std::string&)> callback) {
    my_message_callback = std::move(callback);
}

void UIManager::update(const std::vector<std::string>& messages) {
    if (!font_loaded || !window.isOpen()) {
        return;
    }

    sf::Event event;
    while (window.pollEvent(event)) {
        switch (event.type) {
            case sf::Event::Closed:
                window.close();
                break;
            case sf::Event::TextEntered:
                handleTextInput(event);
                break;
            default:
                break;
        }
    }

    window.clear(sf::Color(30, 30, 30));
    drawMessages(messages);
    drawInputField();
    window.display();
}

bool UIManager::isOpen() const {
    return window.isOpen();
}

void UIManager::close() {
    if (window.isOpen()) {
        window.close();
    }
}

sf::Color UIManager::getMessageBoxColor(const std::string& message) const {
    if (message.find("me:") == 0) {
        return sf::Color(70, 130, 180);
    } else if (message.find("client:") == 0) {
        return sf::Color(60, 179, 113);
    } else {
        return sf::Color(105, 105, 105);
    }
}

void UIManager::handleTextInput(const sf::Event& event) {
    if (event.text.unicode == '\b') {
        if (!currentInput.empty()) {
            currentInput.pop_back();
        }
    } else if (event.text.unicode == '\r' || event.text.unicode == '\n') {
        if (!currentInput.empty() && send_callback) {
            my_message_callback("me: " + currentInput);
            send_callback("client: " + currentInput);
            currentInput.clear();
        }
    } else if (event.text.unicode < 128 && event.text.unicode >= 32) {
        std::string testString = currentInput + static_cast<char>(event.text.unicode);
        sf::Text testText(testString, font, TEXT_SIZE);
        
        float maxWidth = 800 - (PADDING * 3);
        
        if (testText.getLocalBounds().width <= maxWidth) {
            currentInput += static_cast<char>(event.text.unicode);
        }
    }
}

void UIManager::drawMessages(const std::vector<std::string>& messages) {
    if (messages.empty()) return;
    
    float messageAreaHeight = 600 - INPUT_FIELD_HEIGHT - PADDING;
    int maxVisibleMessages = static_cast<int>(messageAreaHeight / MESSAGE_HEIGHT);
    int startIndex = std::max(0, static_cast<int>(messages.size()) - maxVisibleMessages);
    int messagesToDraw = static_cast<int>(messages.size()) - startIndex;
    float startY = messageAreaHeight - messagesToDraw * MESSAGE_HEIGHT + MARGIN;
    float y = startY;
    
    sf::RectangleShape messageBox;
    sf::Text messageText;
    messageText.setFont(font);
    messageText.setCharacterSize(MESSAGE_TEXT_SIZE);
    messageText.setFillColor(sf::Color::White);
    
    for (int i = startIndex; i < static_cast<int>(messages.size()); i++) {
        const auto& msg = messages[i];
        sf::Color boxColor = getMessageBoxColor(msg);
        messageText.setString(msg);
        sf::FloatRect textBounds = messageText.getLocalBounds();
        float boxWidth = std::min(textBounds.width + PADDING, 800.0f - (MARGIN * 4));
        float boxHeight = 35;

        messageBox.setSize(sf::Vector2f(boxWidth, boxHeight));
        messageBox.setPosition(15, y);
        messageBox.setFillColor(boxColor);
        messageBox.setOutlineColor(sf::Color(200, 200, 200));
        messageBox.setOutlineThickness(1);
        window.draw(messageBox);

        std::string displayText = msg;
        if (textBounds.width > boxWidth - PADDING) {
            sf::Text tempText(msg, font, MESSAGE_TEXT_SIZE);
            while (tempText.getLocalBounds().width > boxWidth - (PADDING * 2) && displayText.length() > 3) {
                displayText.pop_back();
                tempText.setString(displayText + "...");
            }
            displayText += "...";
        }

        messageText.setString(displayText);
        messageText.setPosition(25, y + 8);
        window.draw(messageText);

        y += MESSAGE_HEIGHT;
    }
}

void UIManager::drawInputField() {
    float inputFieldY = 600 - INPUT_FIELD_HEIGHT - MARGIN;

    sf::RectangleShape inputBox(sf::Vector2f(800 - PADDING, INPUT_FIELD_HEIGHT));
    inputBox.setPosition(MARGIN, inputFieldY);
    inputBox.setFillColor(sf::Color(50, 50, 50));
    
    sf::Color outlineColor = currentInput.empty() ? sf::Color(100, 100, 100) : sf::Color(150, 150, 255);
    inputBox.setOutlineColor(outlineColor);
    inputBox.setOutlineThickness(2);
    window.draw(inputBox);

    if (currentInput.empty()) {
        placeholderText.setPosition(PADDING, inputFieldY + 15);
        window.draw(placeholderText);
    } else {
        inputText.setString(currentInput);
        inputText.setPosition(PADDING, inputFieldY + 15);
        window.draw(inputText);
    }

    if (cursor_clock.getElapsedTime().asSeconds() < 0.5) {
        sf::Text cursor("|", font, TEXT_SIZE);
        cursor.setFillColor(sf::Color::White);

        inputText.setString(currentInput);
        float cursorX = PADDING + inputText.getLocalBounds().width;
        cursor.setPosition(cursorX, inputFieldY + 15);
        window.draw(cursor);
    }

    if (cursor_clock.getElapsedTime().asSeconds() >= 1.0) {
        cursor_clock.restart();
    }
}