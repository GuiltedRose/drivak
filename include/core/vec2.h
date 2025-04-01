#pragma once

struct vec2 {
    float x = 0.0f;
    float y = 0.0f;

    vec2() = default;
    vec2(float x, float y) : x(x), y(y) {}

    bool operator==(const vec2& other) const {
        return x == other.x && y == other.y;
    }

    bool operator!=(const vec2& other) const {
        return !(*this == other);
    }

    vec2 operator+(const vec2& other) const {
        return vec2(x + other.x, y + other.y);
    }

    vec2 operator-(const vec2& other) const {
        return vec2(x - other.x, y - other.y);
    }

    vec2& operator+=(const vec2& other) {
        x += other.x;
        y += other.y;
        return *this;
    }

    vec2& operator-=(const vec2& other) {
        x -= other.x;
        y -= other.y;
        return *this;
    }
};
