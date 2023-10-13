//
// Created by Coch on 13.10.2023.
//

#pragma once

#include <chrono>

class Timer
{
private:
    std::chrono::time_point<std::chrono::steady_clock> _startOfProgramTime;
    std::chrono::time_point<std::chrono::steady_clock> _startOfFrameTime;
    std::chrono::time_point<std::chrono::steady_clock> _endOfFrameTime;
    std::chrono::duration<float> _frameTime{0.f};

public:
    // Delta time in seconds.
    float DeltaTime{0.f};

    void Init() noexcept;
    void Tick() noexcept;
    [[nodiscard]] float TotalTime();
};