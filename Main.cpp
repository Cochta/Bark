
#include "World.h"

#include "Random.h"

#include "SDLApp.h"

#include "UniquePtr.h"

#include <array>

int main(int argc, char *argv[])
{
    World world;
    world.Init();

    SDLApp app("SDL Application", 800, 600, world);


    for (std::size_t i = 0; i < 50; ++i)
    {
        Body b({Random::Range(100.f, app.Width - 100.f),
                Random::Range(100.f, app.Height - 100.f)},
               Vec2F::Right());
        world.AddBody(b);
    }

    app.Init();

    app.Run();

    world.UnInit();
    app.UnInit();

    return 0;
}