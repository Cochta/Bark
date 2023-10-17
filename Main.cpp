#include "World.h"

#include "Random.h"

#include "SDLApp.h"

constexpr int width = 800, height = 600;
constexpr float G = 6.67f;

std::vector<BodyRef> celestialBodies;

enum BodyType
{
    Sun = 0,
    Planet = 1
};

struct BodyData
{
    BodyType Type = Planet;
    float Radius = 0; //if radius is 0 then the object is not a circle
    SDL_Color Color = {0, 255, 0, 255};

};

std::vector<BodyData> celestialBodyData;

SDLApp app("Star System", width, height);

void CreateStarSystem();

int main(int argc, char *argv[])
{
    app.Init();

    CreateStarSystem();

    app.Run();

    app.UnInit();

    return 0;
}

void CreateStarSystem()
{
    auto sunRef = app.GameWorld.CreateBody();
    auto &sun = app.GameWorld.GetBody(sunRef);
    sun.Position = {static_cast<float>(app.Width) / 2, static_cast<float>(app.Height) / 2};
    sun.Mass = 1000000;

    celestialBodies.push_back(sunRef);
    BodyData sbd{BodyType::Sun, 30, {255, 255, 0, 255}};
    celestialBodyData.push_back(sbd);

    const auto copiedSun = sun;

    for (std::size_t i = 0; i < 50; ++i)
    {
        auto bodyRef = app.GameWorld.CreateBody();
        auto &body = app.GameWorld.GetBody(bodyRef);
        body.Position = {Random::Range(100.f, width - 100.f), Random::Range(100.f, height - 100.f)};
        auto r = copiedSun.Position - body.Position;
        auto v = sqrt(G * (copiedSun.Mass / r.Length()));
        body.Velocity = Vec2F(-r.Y, r.X).Normalized() * v;
        body.Mass = 10.f;

        celestialBodies.push_back(bodyRef);
        BodyData pbd{BodyType::Planet, Random::Range(5.f, 15.f),
                     {static_cast<Uint8>(Random::Range(0, 255)),
                      static_cast<Uint8>(Random::Range(0, 255)),
                      static_cast<Uint8>(Random::Range(0, 255)), 255}};
        celestialBodyData.push_back(pbd);
    }

}

void CalculateGravitationalForce(const Body &sun, Body &body)
{
    auto m1m2 = sun.Mass * body.Mass;
    auto r = (sun.Position - body.Position).Length();
    auto r2 = r * r;
    auto F = G * (m1m2 / r2);

    Vec2F forceDirection = (sun.Position - body.Position).Normalized();
    Vec2F force = forceDirection * F;
    body.ApplyForce(force);
}

BodyRef FindSunRef()
{
    auto itSun = std::find_if(celestialBodyData.begin(), celestialBodyData.end(), [](const BodyData &bd)
    { return bd.Type == BodyType::Sun; });

    auto idx = std::distance(celestialBodyData.begin(), itSun);
    return celestialBodies[idx];
}

void SDLApp::SDLUpdate()
{
    // Find the Sun reference
    auto sunRef = FindSunRef();

    auto &sun = GameWorld.GetBody(sunRef);

    for (auto &bodyRef: celestialBodies)
    {
        if (bodyRef == sunRef)
        {
            continue; // Skip the Sun
        }

        auto &body = GameWorld.GetBody(bodyRef);
        CalculateGravitationalForce(sun, body);
    }
}

void SDLApp::DrawAllBodies()
{
    for (auto &bodyRef: celestialBodies)
    {
        auto &body = GameWorld.GetBody(bodyRef);
        if (body.IsEnabled())
        {
            BodyData bd = celestialBodyData[bodyRef.Index];

            DrawCircle(body, bd.Radius, 30, bd.Color);  // Draw the circle with the determined segment value
        }
    }
}