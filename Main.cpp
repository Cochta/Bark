
#ifdef SDL_ENABLE
#include "SDLApp.h"
using App = SDLApp;
#else
#include "SFMLApp.h"
using App = SFMLApp;
#endif

int main([[maybe_unused]] int argc, [[maybe_unused]] char *argv[]) {
  App app("Samples", Metrics::Width, Metrics::Height);

  app.SetUp();

  app.Run();

  app.TearDown();

  return EXIT_SUCCESS;
}
