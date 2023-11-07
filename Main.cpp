#include "SDLApp.h"

int main([[maybe_unused]] int argc, [[maybe_unused]] char *argv[])
{
    /*int* SanitzeTest = new int(4);
    SanitzeTest[1] = 4;*/

    SDLApp app("Samples", Metrics::Width, Metrics::Height);

    app.SetUp();

    app.Run();

    app.TearDown();

    return EXIT_SUCCESS;
}