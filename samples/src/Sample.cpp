#include "Sample.h"

void Sample::SetUp() noexcept
{
    _timer.SetUp();
    _world.SetUp();
    SampleSetUp();
}

void Sample::Update() noexcept
{
    SampleUpdate();
    _timer.Tick();
    _world.Update(_timer.DeltaTime);
    _wasMouseReleased = false;
}

void Sample::TearDown() noexcept
{
    SampleTearDown();
    _bodyRefs.clear();
    AllGraphicsData.clear();
    _colRefs.clear();
    _world.TearDown();
}

void Sample::GetMousePos(const Math::Vec2F mousePos) noexcept
{
    _mousePos = mousePos;
}

void Sample::OnClick() noexcept
{
    _wasMouseReleased = true;
}
