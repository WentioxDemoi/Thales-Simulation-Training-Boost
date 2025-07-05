#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include <functional>
#include <memory>

class UIManager {
private:
    sf::RenderWindow window;
    sf::Font font;
    sf::Text inputText;
    sf::Text placeholderText;
    std::string currentInput;
    std::function<void(const std::string&)> send_callback;
    std::function<void(const std::string&)> my_message_callback;
    bool font_loaded = false;
    
    int cached_window_width = 0;
    int cached_window_height = 0;
    bool dimensions_cache_valid = false;
    
    static constexpr float INPUT_FIELD_HEIGHT = 50.0f;
    static constexpr float MESSAGE_HEIGHT = 45.0f;
    static constexpr float MARGIN = 10.0f;
    static constexpr float PADDING = 20.0f;
    static constexpr int TEXT_SIZE = 18;
    static constexpr int MESSAGE_TEXT_SIZE = 16;
    
    sf::Clock cursor_clock;
    
    UIManager() {}
    
    void updateDimensionsCache();
    sf::Vector2f getWindowSize();
    float getMessageAreaHeight();
    float getInputFieldY();
    sf::Color getMessageBoxColor(const std::string& message) const;
    void handleTextInput(const sf::Event& event);
    void drawMessages(const std::vector<std::string>& messages);
    void drawInputField();
    void invalidateDimensionsCache();
    
public:
    static UIManager& getInstance() {
        static UIManager instance;
        return instance;
    }
    
    UIManager(const UIManager&) = delete;
    UIManager& operator=(const UIManager&) = delete;
    
    bool initialize(const std::string& font_path);
    void setSendCallback(std::function<void(const std::string&)> callback);
    void setMyMessageCallback(std::function<void(const std::string&)> callback);
    void update(const std::vector<std::string>& messages);
    bool isOpen() const;
    void close();
    
    
};