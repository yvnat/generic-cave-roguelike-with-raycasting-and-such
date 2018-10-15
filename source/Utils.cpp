#pragma once
#include <stdio.h>
#include <string>
#include <SDL2/SDL.h>

class Color
{ // SDL_Color but better
  public:
    float r; // By better
    float g; // we actually mean
    float b; // like, 4 times less memory efficient
    Color(float r, float g, float b)
    {
        normalize();
        this->r = r;
        this->g = g;
        this->b = b;
    }
    Color(float v)
    {
        normalize();
        this->r = v;
        this->g = v;
        this->b = v;
    }
    SDL_Color to_sdl()
    {
        return SDL_Color({to_byte(r), to_byte(g), to_byte(b)});
    }
    void normalize()
    {
        to_float(&r);
        to_float(&g);
        to_float(&b);
    }
    void to_float(float *n)
    {
        if (*n > 1)
        {
            *n = *n / 255;
        }
    }
    Uint8 to_byte(float n)
    {
        return (Uint8)(int)(n * 255);
    }

    Color operator*(float n)
    {
        return Color(r * n, g * n, b * n);
    }

    Color operator/(float n)
    {
        return Color(r / n, g / n, b / n);
    }
    Color() {} // This line does something important but I don't really get why
};

using namespace std;
namespace utils
{
float inf = std::numeric_limits<float>::infinity();
float ninf = -std::numeric_limits<float>::infinity();
float r_01()
{
    return ((float)rand() / (RAND_MAX));
}
float randint(int max, int min)
{
    return rand() % (max - min + 1) + min;
}
} // namespace utils

template <class T>
class V
{
  public:
    T x, y;
    V<T>(T x, T y)
    {
        this->x = x;
        this->y = y;
    }
    static V<int> izero()
    {
        return V<int>(0, 0);
    }
    static V<float> fzero()
    {
        return V<float>(0, 0);
    }
    V<T> operator*(V<T> other)
    {
        return V<T>(x * other.x, y * other.y);
    }
    V<T> operator+(V<T> other)
    {
        return V<T>(x + other.x, y + other.y);
    }
    V<T> operator-(V<T> other)
    {
        return V<T>(x - other.x, y - other.y);
    }
    V<T> operator*(int scale)
    {
        return V<T>(x * scale, y * scale);
    }
    float magnitude()
    {
        return (float)sqrt(x * x + y * y);
    }
    V<T> normalized()
    {
        float m = magnitude();
        return V<T>(x / m, y / m);
    }
    // V<T> operator * (float scale) {
    //     return V<T>(x * scale, y * scale);
    // }
};
