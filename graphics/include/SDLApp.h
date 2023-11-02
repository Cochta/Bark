#pragma once


#include "SampleManager.h"

#include <SDL2/SDL.h>
#include <imgui.h>
#include "imgui_impl_sdl2.h"
#include "imgui_impl_sdlrenderer2.h"


class SDLApp
{
private:
    SDL_Window *_window{};
    SDL_Renderer *_renderer{};

    std::vector<SDL_Vertex> _vertices;
    std::vector<int> _indices;

    SampleManager _sampleManager;

public:
    std::string Title;
    int Width, Height;
    Math::Vec2I MousePos;

    SDLApp(std::string_view title, int width, int height) noexcept: _window(nullptr), _renderer(nullptr)
    {
        Width = width;
        Height = height;
        Title = title;
    }

    void SetUp();

    void TearDown() const noexcept;

    void Run() noexcept;

private:
    void DrawCircle(Math::Vec2F center, float radius, int segments, const SDL_Color &col) noexcept;

    void DrawRectangle(Math::Vec2F minBound, Math::Vec2F maxBound, const SDL_Color &col) noexcept;
    void DrawRectangleBorder(Math::Vec2F minBound, Math::Vec2F maxBound, const SDL_Color &col) noexcept;

    void DrawPolygon(const std::vector<Math::Vec2F> &vertices, const SDL_Color &col);

    void DrawAllGraphicsData() noexcept;
};