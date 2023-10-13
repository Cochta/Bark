//
// Created by const on 13/10/2023.
//
#include "World.h"

void World::AddBody(const Body &body)
{
    Bodies.push_back(body);
}

void World::Init()
{
    WorldTimer.Init();
}

void World::UnInit()
{

}

void World::Update()
{
    WorldTimer.Tick();
    for (auto &body: Bodies)
    {
        body.Update(WorldTimer.DeltaTime);
    }
}
