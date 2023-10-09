#include "SDLApplication.h"


int main(int argc, char *argv[])
{
    SDLApplication app("SDL Application", 800, 600);

    Circle c1(100,100,10, Red);
    Circle c2(300,200,30, Green);
    Circle c3(400,300,50, Blue);
    Circle c4(600,400,100, White);
    app.AddCircle(c1);
    app.AddCircle(c2);
    app.AddCircle(c3);
    app.AddCircle(c4);

    app.Run();

    return 0;
}