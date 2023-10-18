//
// Created by Coch on 13.10.2023.
//

#pragma once

#include <chrono>

class Timer // todo: test timer
{
private:
    std::chrono::time_point<std::chrono::high_resolution_clock> _startTime;

public:
    float DeltaTime{0.f};

    void SetUp() noexcept;
    void Tick() noexcept;
};