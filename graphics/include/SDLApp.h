#pragma once
//
// Created by Coch on 09.10.2023.
//

#include <SDL2/SDL.h>

#include "Sample.h"
#include "UniquePtr.h"


class SDLApp {
private:
    SDL_Window *_window{};
    SDL_Renderer *_renderer{};

    std::vector<SDL_Vertex> _vertices;
    std::vector<int> _indices;

public:
    std::string_view Title;
    int Width, Height;
    Math::Vec2I MousePos;

    std::vector<UniquePtr<Sample>> samples;

    SDLApp(std::string_view title, int width, int height) : _window(nullptr), _renderer(nullptr) {
        Width = width;
        Height = height;
        Title = title;
    }

    void SetUp();

    void TearDown() const;

    void Run();

    void DrawCircle(const Body &c, float radius, int segments, SDL_Color col);

    void DrawAllBodies();


};