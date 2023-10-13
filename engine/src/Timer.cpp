//
// Created by Coch on 13.10.2023.
//
#include "Timer.h"

void Timer::Init() noexcept
{
    auto nowTime = std::chrono::high_resolution_clock::now();
    _startOfProgramTime = nowTime;
    _startOfFrameTime = nowTime;
}

void Timer::Tick() noexcept
{
    _endOfFrameTime = std::chrono::high_resolution_clock::now();

    _frameTime = std::chrono::duration_cast<std::chrono::duration<float>>(
            _endOfFrameTime - _startOfFrameTime);

    DeltaTime = _frameTime.count();

    _startOfFrameTime = _endOfFrameTime;
}

float Timer::TotalTime()
{
    return std::chrono::duration_cast<std::chrono::duration<float>>(
            std::chrono::high_resolution_clock::now() - _startOfProgramTime).count();
}