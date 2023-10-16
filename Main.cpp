#include "World.h"

#include "Random.h"

#include "SDLApp.h"

constexpr int width = 800, height = 600;
constexpr float G = 6.67f;

const Body sun({width / 2, height / 2}, Vec2F::Zero(), 1000000.f);

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

Body CreatePlanet()
{
    Body b({Random::Range(100.f, width - 100.f),
            Random::Range(100.f, height - 100.f)},
           Vec2F::Zero(),
           10.f);
    auto r = sun.Position - b.Position;
    auto v = sqrt(G * (sun.Mass / r.Length()));
    b.Velocity = Vec2F(-r.Y, r.X).Normalized() * v;
    return b;
}

void CreateStarSystem()
{
    for (std::size_t i = 0; i < 50; ++i)
    {
        app.GameWorld.AddBody(CreatePlanet());
    }
}

void SDLApp::SDLUpdate()
{
    for (auto &body: GameWorld.Bodies)
    {
        auto m1m2 = sun.Mass * body.Mass;
        auto r = (sun.Position - body.Position).Length();
        auto r2 = r * r;
        auto F = G * (m1m2 / r2);

        Vec2F forceDirection = (sun.Position - body.Position).Normalized();
        Vec2F force = forceDirection * F;
        body.ApplyForce(force);
    }
}