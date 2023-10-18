
#include "Metrics.h"
#include "SDLApp.h"
#include "Vec2.h"


constexpr int width = Metrics::MetersToPixels(8), height = Metrics::MetersToPixels(6);

SDLApp app("Star System", Metrics::Width, Metrics::Height);

int main(int argc, char *argv[]) {
    app.SetUp();

    app.Run();

    app.TearDown();

    return EXIT_SUCCESS;
}