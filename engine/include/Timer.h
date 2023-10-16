//
// Created by Coch on 13.10.2023.
//

#pragma once

#include <chrono>

class Timer
{
private:
    std::chrono::time_point<std::chrono::steady_clock> _startTime;

public:
    float DeltaTime{0.f};

    void Init() noexcept;
    void Tick() noexcept;
};