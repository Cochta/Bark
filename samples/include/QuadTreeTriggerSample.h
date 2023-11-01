#pragma once

#include <iostream>
#include "Sample.h"
#include "Random.h"


class QuadTreeTriggerSample : public Sample, public ContactListener
{
private:
    std::vector<GraphicsData> _quadTreeGraphicsData;
    std::vector<int> _triggerNbrPerCollider;
    
    static constexpr float SPEED = 100;
    static constexpr float RADIUS = Metrics::MetersToPixels(0.2f);
    static constexpr std::size_t CIRCLE_NBR = 200;

public:
    void BeginContact(ColliderRef &col1, ColliderRef &col2) noexcept override;
    
    void EndContact(ColliderRef &col1, ColliderRef &col2) noexcept override;

protected:
    void SampleSetUp() noexcept override;
    
    void SampleUpdate() noexcept override;
    
    void SampleTearDown() noexcept override;

private:
    void DrawQuadtree(const QuadNode *node);
};