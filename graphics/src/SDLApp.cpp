//
// Created by Coch on 09.10.2023.
//
#include "SDLApp.h"

#include "StarSystem.h"

void SDLApp::SetUp() {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        // Handle SDL initialization error
        SDL_Log("SDL_Init Error: %s", SDL_GetError());
        return;
    }
    SDLApp::_window = SDL_CreateWindow(Title.data(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, Width, Height,
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

    samples.push_back(UniquePtr<StarSystem>(new StarSystem()));
    samples[0]->SetUp();
}

void SDLApp::TearDown() const
{
    if (_renderer != nullptr) {
        SDL_DestroyRenderer(_renderer);
    }
    if (_window != nullptr) {
        SDL_DestroyWindow(_window);
    }
    SDL_Quit();
}

void SDLApp::Run() {
    bool quit = false;
    SDL_Event e;

    while (!quit) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
        }
        SDL_GetMouseState(&MousePos.X, &MousePos.Y);

        // Clear the renderer
        SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 255);
        SDL_RenderClear(_renderer);


        samples[0]->Update(); // the function pointer for the Sample Update

        DrawAllBodies();

        // Present the renderer
        SDL_RenderPresent(_renderer);
    }
}

void SDLApp::DrawCircle(const Body &b, float radius, int segments, SDL_Color col) {
    std::vector<SDL_Vertex> vertices;
    std::vector<int> indices;

    // Calculate vertices for the circle
    for (int i = 0; i < segments; ++i) {
        auto angle = Radian(2.f * MathUtility::Pi * static_cast<float>(i) / static_cast<float>(segments));
        float x = b.Position.X + radius * MathUtility::Cos(angle);
        float y = b.Position.Y + radius * MathUtility::Sin(angle);
        vertices.push_back({{x, y}, col, {1.0f, 1.0f}});
    }

    // Calculate indices to create triangles for filling the circle
    for (int i = 0; i < segments - 1; ++i) {
        indices.push_back(0); // Center point
        indices.push_back(i);
        indices.push_back(i + 1);
    }
    indices.push_back(0); // Center point
    indices.push_back(segments - 1);
    indices.push_back(0);  // Connect the last vertex to the center

    SDL_RenderGeometry(_renderer, nullptr, vertices.data(), vertices.size(), indices.data(), indices.size());
}

void SDLApp::DrawAllBodies() {
    for (auto &bodyRef: samples[0]->BodyRefs) {
        auto &body = samples[0]->World.GetBody(bodyRef);
        if (body.IsEnabled()) {
            BodyData bd = samples[0]->AllBodyData[bodyRef.Index];

            DrawCircle(body, bd.Radius, 50, {
                    static_cast<Uint8>(bd.Color.r),
                    static_cast<Uint8>(bd.Color.g),
                    static_cast<Uint8>(bd.Color.b),
                    static_cast<Uint8>(bd.Color.a)});
        }
    }
}
