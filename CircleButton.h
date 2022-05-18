#include <SFML/Graphics.hpp>
#include <string>

class CircleButton {
    Text title;
    int backlight_thickness = 0;

public:
    CircleShape field;
    CircleShape in;

    void setTitle(std::string name, int size, Color color = { 0, 0, 0, 255 }) {
        title.setString(name);
        title.setCharacterSize(size);
        title.setFillColor(color);
    }

    void setTitlePosition(Vector2f pos) {
        title.setPosition(pos);
    }

    void setButtonPosition(Vector2f pos) {
        field.setPosition(pos);
    }

    void setButtonSize(float size, int size_thickness = 2) {
        field.setRadius(size);
        in.setRadius(size / 2);
        backlight_thickness = size_thickness;
    }

    void setButtonColor(Color fill_color1, Color fill_color2, Color outline_color) {
        field.setFillColor(fill_color1);
        in.setFillColor(fill_color2);
        field.setOutlineColor(outline_color);
    }

    bool contains(const Vector2i& mouse) {
        float dx = (mouse.x - field.getPosition().x - field.getRadius()), dy = (mouse.y - field.getPosition().y - field.getRadius());
        bool check = dx * dx + dy * dy <= field.getRadius() * field.getRadius();
        return check;
    }

    bool was_pressed = 0;

    bool pressed(Vector2i& mouse, Event now) {
        bool check = (contains(mouse) && now.type == Event::MouseButtonPressed && now.key.code == Mouse::Left);
        was_pressed ^= check;
        return check;
    }

    void draw(RenderWindow& window) {
        field.setOutlineThickness(backlight_thickness);
        window.draw(field);
        if (was_pressed) {
            float Radius = field.getRadius();
            in.setPosition(field.getPosition().x + Radius / 2, field.getPosition().y + Radius / 2);
            in.setRadius(Radius / 2);
            window.draw(in);
        }
        title.setFont(font);
        window.draw(title);
    }
};

