#include "SDLApplication.h"
#include "Random.h"

int main(int argc, char *argv[])
{
    SDLApplication app("SDL Application", 800, 600);

    std::vector<Color> colors{Red, Green, Blue, White};
    for (std::size_t i = 0; i < 50; ++i)
    {
        Circle c({static_cast<float>(Random::Range(100, app.Width - 100)),
                  static_cast<float>(Random::Range(100, app.Height - 100))},
                 50.f,
                 Random::Range(1.f, 30.f),
                 colors[Random::Range(0, 3)]);
        app.AddCircle(c);
    }

    app.Run();

    return 0;
}