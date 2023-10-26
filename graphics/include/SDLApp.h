#pragma once
//
// Created by Coch on 09.10.2023.
//

#include <SDL2/SDL.h>

#include "Sample.h"
#include "UniquePtr.h"


class SDLApp
{
private:
    SDL_Window *_window{};
    SDL_Renderer *_renderer{};

    std::vector<SDL_Vertex> _vertices;
    std::vector<int> _indices;

    std::size_t _sampleIdx = 0;

public:
    std::string Title;
    int Width, Height;
    Math::Vec2I MousePos;

    std::vector<UniquePtr<Sample>> samples;

    SDLApp(std::string_view title, int width, int height) noexcept: _window(nullptr), _renderer(nullptr)
    {
        Width = width;
        Height = height;
        Title = title;
    }

    void SetUp();

    void TearDown() const noexcept;

    void Run() noexcept;

    void DrawCircle(Math::Vec2F center, float radius, int segments, const SDL_Color &col) noexcept;

    void DrawRectangle(Math::Vec2F minBound, Math::Vec2F maxBound, const SDL_Color &col) noexcept;

    void DrawPolygon(const std::vector<Math::Vec2F> &vertices, const SDL_Color &col) noexcept;

    void DrawAllBodiesData() noexcept;
};