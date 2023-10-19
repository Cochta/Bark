#include "Timer.h"

#include "gtest/gtest.h"

TEST(Timer, DeltaTime)
{
    Timer timer;
    timer.SetUp();

    std::chrono::time_point<std::chrono::steady_clock> endOfFrame, startOfFrame;
    std::chrono::duration<float> frameTime{0.f};

    startOfFrame = std::chrono::high_resolution_clock::now();

    float epsilon = 0.00001f;

    int loopCounter = 0;

    while(loopCounter < 100000)
    {
        timer.Tick();

        endOfFrame = std::chrono::high_resolution_clock::now();

        frameTime = std::chrono::duration_cast<std::chrono::duration<float>>(
                endOfFrame - startOfFrame);

        startOfFrame = endOfFrame;

        loopCounter++;
    }

    EXPECT_NEAR(timer.DeltaTime, frameTime.count(), epsilon);
}