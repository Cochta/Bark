//
// Created by Coch on 09.10.2023.
//
#include "SDLApp.h"

#include "StarSystemSample.h"
#include "TriggerSample.h"
#include "FormsSample.h"

void SDLApp::SetUp() {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        // Handle SDL initialization error
        SDL_Log("SDL_Init Error: %s", SDL_GetError());
        return;
    }
    _window = SDL_CreateWindow(
            Title.data(),
            SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED,
            Width,
            Height,
            SDL_WINDOW_SHOWN);

    if (_window == nullptr) {
        // Handle window creation error
        SDL_Log("SDL_CreateWindow Error: %s", SDL_GetError());
        return;
    }


    _renderer = SDL_CreateRenderer(_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (_renderer == nullptr) {
        // Handle renderer creation error
        SDL_Log("SDL_CreateRenderer Error: %s", SDL_GetError());
        return;
    }

    samples.push_back(UniquePtr<StarSystemSample>(new StarSystemSample()));
    samples.push_back(UniquePtr<TriggerSample>(new TriggerSample()));
    samples.push_back(UniquePtr<FormsSample>(new FormsSample()));

    samples[_sampleIdx]->SetUp();
}

void SDLApp::TearDown() const noexcept {
    if (_renderer != nullptr) {
        SDL_DestroyRenderer(_renderer);
    }
    if (_window != nullptr) {
        SDL_DestroyWindow(_window);
    }
    SDL_Quit();
}

void SDLApp::Run() noexcept {
    bool quit = false;
    SDL_Event e;

    while (!quit) {
        while (SDL_PollEvent(&e)) {
            switch (e.type) {
                case SDL_QUIT:
                    quit = true;
                    break;
                case SDL_KEYUP:
                    samples[_sampleIdx]->TearDown(); // todo: sample manager to load samples
                    switch (e.key.keysym.sym) {
                        case SDLK_LEFT:
                            if (_sampleIdx <= 0)
                                _sampleIdx = samples.size() - 1;
                            else
                                _sampleIdx--;
                            break;
                        case SDLK_RIGHT:
                            if (_sampleIdx >= samples.size() - 1)
                                _sampleIdx = 0;
                            else
                                _sampleIdx++;
                            break;
                    }
                    samples[_sampleIdx]->SetUp();
                    break;
            }
        }
        SDL_GetMouseState(&MousePos.X, &MousePos.Y);

        // Clear the renderer
        SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 255);
        SDL_RenderClear(_renderer);

        samples[_sampleIdx]->GetMousePos(static_cast<Math::Vec2F>(MousePos));
        samples[_sampleIdx]->Update(); // the function pointer for the Sample Update

        DrawAllBodiesData();

        // Present the renderer
        SDL_RenderPresent(_renderer);
    }
}

void SDLApp::DrawCircle(const Math::Vec2F center, const float radius, const int segments, const SDL_Color &col) noexcept {
    auto offset = _vertices.size();

    // Calculate vertices for the Circle
    for (int i = 0; i < segments; ++i) {
        auto angle = Math::Radian(2.f * Math::Pi * static_cast<float>(i) / static_cast<float>(segments));
        float x = center.X + radius * Math::Cos(angle);
        float y = center.Y + radius * Math::Sin(angle);
        _vertices.push_back({{x, y}, col, {1.0f, 1.0f}});
    }

    // Calculate indices to create triangles for filling the Circle
    for (int i = 0; i < segments - 1; ++i) {
        _indices.push_back(offset); // Center point
        _indices.push_back(offset + i);
        _indices.push_back(offset + i + 1);
    }
    _indices.push_back(offset); // Center point
    _indices.push_back(offset + segments - 1);
    _indices.push_back(offset);  // Connect the last vertex to the center
}

void SDLApp::DrawRectangle(const Math::Vec2F minBound, const Math::Vec2F maxBound, const SDL_Color &col) noexcept {
    auto offset = _vertices.size();

    _vertices.push_back({{minBound.X, minBound.Y}, col, {1.0f, 1.0f}});
    _vertices.push_back({{maxBound.X, minBound.Y}, col, {1.0f, 1.0f}});
    _vertices.push_back({{minBound.X, maxBound.Y}, col, {1.0f, 1.0f}});
    _vertices.push_back({{maxBound.X, maxBound.Y}, col, {1.0f, 1.0f}});

    _indices.push_back(offset);                 // Top left vertex
    _indices.push_back(offset + 1);             // Top right vertex
    _indices.push_back(offset + 2);             // Bottom left vertex
    _indices.push_back(offset + 2);             // Bottom left vertex
    _indices.push_back(offset + 1);             // Top right vertex
    _indices.push_back(offset + 3);             // Bottom right vertex
}

void SDLApp::DrawPolygon(const std::vector<Math::Vec2F> &vertices, const SDL_Color &col) {
    if (vertices.size() < 3) return; // todo exeption

    auto offset = _vertices.size();

    for (const Math::Vec2F &v: vertices) {
        _vertices.push_back({{v.X, v.Y}, col, {1.0f, 1.0f}});
    }

    for (int i = 1; i < vertices.size() - 1; ++i) {
        _indices.push_back(offset);
        _indices.push_back(offset + i);
        _indices.push_back(offset + i + 1);
    }

    // Connect the last vertex to the first vertex to close the polygon.
    _indices.push_back(offset);
    _indices.push_back(offset + vertices.size() - 1);
    _indices.push_back(offset + 1);
}

void SDLApp::DrawAllBodiesData() {
    _vertices.clear();
    _indices.clear();
    for (auto &bd : samples[_sampleIdx]->AllBodyData) {

            if (bd.Shape.index() == (int)Math::ShapeType::Circle) {
                Math::CircleF circle = std::get<Math::CircleF>(bd.Shape);
                DrawCircle(circle.Center(), circle.Radius(), 30, {
                        static_cast<Uint8>(bd.Color.r),
                        static_cast<Uint8>(bd.Color.g),
                        static_cast<Uint8>(bd.Color.b),
                        static_cast<Uint8>(bd.Color.a)
                });
            } else if (bd.Shape.index() == (int)Math::ShapeType::Rectangle) {
                auto rect = std::get<Math::RectangleF>(bd.Shape);
                DrawRectangle(rect.MinBound(), rect.MaxBound(), {
                        static_cast<Uint8>(bd.Color.r),
                        static_cast<Uint8>(bd.Color.g),
                        static_cast<Uint8>(bd.Color.b),
                        static_cast<Uint8>(bd.Color.a)
                });
            } else if (bd.Shape.index() == (int)Math::ShapeType::Polygon) {
                auto polygon = std::get<Math::PolygonF>(bd.Shape);
                DrawPolygon(polygon.Vertices(), {
                        static_cast<Uint8>(bd.Color.r),
                        static_cast<Uint8>(bd.Color.g),
                        static_cast<Uint8>(bd.Color.b),
                        static_cast<Uint8>(bd.Color.a)
                });
            }
    }
    SDL_RenderGeometry(_renderer, nullptr, _vertices.data(), _vertices.size(), _indices.data(), _indices.size());
}
