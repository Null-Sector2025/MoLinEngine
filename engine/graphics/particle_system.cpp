#include "particle_system.h"
#include <cstdlib>

namespace MoLin {

ParticleSystem::ParticleSystem(const std::string& name, int max) : SceneNode(name), m_MaxParticles(max) {
    m_Particles.reserve(max);
}
void ParticleSystem::Emit(float x, float y, int count) {
    for (int i = 0; i < count && m_Particles.size() < m_MaxParticles; i++) {
        Particle p;
        p.x = x + (rand() % 20 - 10); p.y = y + (rand() % 20 - 10);
        p.vx = (rand() % 200 - 100) / 10.0f; p.vy = (rand() % 200 - 300) / 10.0f;
        p.life = 1.0f; p.decay = 0.01f + (rand() % 10) / 500.0f;
        p.r = m_RMin + rand() % (m_RMax - m_RMin + 1);
        p.g = m_GMin + rand() % (m_GMax - m_GMin + 1);
        p.b = m_BMin + rand() % (m_BMax - m_BMin + 1);
        m_Particles.push_back(p);
    }
}
void ParticleSystem::OnUpdate(float delta) {
    for (auto& p : m_Particles) {
        p.vx += m_GravityX * delta; p.vy += m_GravityY * delta;
        p.x += p.vx * delta; p.y += p.vy * delta; p.life -= p.decay;
    }
    m_Particles.erase(std::remove_if(m_Particles.begin(), m_Particles.end(),
        [](const Particle& p) { return p.life <= 0.0f; }), m_Particles.end());
}
void ParticleSystem::OnRender(SDL_Renderer* renderer) {
    Transform world = GetWorldTransform();
    for (auto& p : m_Particles) {
        SDL_SetRenderDrawColor(renderer, p.r, p.g, p.b, static_cast<int>(255 * p.life));
        SDL_Rect rect = { static_cast<int>(world.x + p.x), static_cast<int>(world.y + p.y), 4, 4 };
        SDL_RenderFillRect(renderer, &rect);
    }
}

} // namespace MoLin
