#pragma once
#include "scene/scene.h"
#include <vector>
#include <cmath>
#include <cstdlib>

namespace MoLin {

struct Particle {
    float x, y;
    float vx, vy;
    float life = 1.0f;
    float decay = 0.01f;
    int r = 255, g = 255, b = 255, a = 255;
};

class ParticleSystem : public SceneNode {
public:
    ParticleSystem(const std::string& name, int maxParticles = 500);

    void Emit(float x, float y, int count = 1);
    void SetGravity(float gx, float gy) { m_GravityX = gx; m_GravityY = gy; }
    void SetColorRange(int rMin, int rMax, int gMin, int gMax, int bMin, int bMax);

protected:
    void OnUpdate(float delta) override;
    void OnRender(SDL_Renderer* renderer) override;

    std::vector<Particle> m_Particles;
    int m_MaxParticles;
    float m_GravityX = 0.0f, m_GravityY = 9.8f;
    int m_RMin=255,m_RMax=255,m_GMin=255,m_GMax=255,m_BMin=255,m_BMax=255;
};

} // namespace MoLin
