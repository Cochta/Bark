#include "SDLApp.h"

int main([[maybe_unused]] int argc, [[maybe_unused]] char *argv[])
{
    SDLApp app("Samples", Metrics::Width, Metrics::Height);

    app.SetUp();

    app.Run();

    app.TearDown();

    return EXIT_SUCCESS;
}