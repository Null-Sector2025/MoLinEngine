#include "sprite.h"

namespace MoLin {

Sprite::Sprite(const std::string& name, SDL_Texture* texture)
    : SceneNode(name), m_Texture(texture) {}

void Sprite::SetTexture(SDL_Texture* texture) {
    m_Texture = texture;
}

void Sprite::OnRender(SDL_Renderer* renderer) {
    if (!m_Texture) return;
    Transform world = GetWorldTransform();
    SDL_Rect destRect;
    SDL_QueryTexture(m_Texture, nullptr, nullptr, &destRect.w, &destRect.h);
    destRect.w *= world.scaleX;
    destRect.h *= world.scaleY;
    destRect.x = static_cast<int>(world.x);
    destRect.y = static_cast<int>(world.y);

    SDL_RenderCopyEx(renderer, m_Texture,
                     (m_SrcRect.w == 0 ? nullptr : &m_SrcRect),
                     &destRect,
                     world.rotation,
                     nullptr,
                     m_Flip);
}

} // namespace MoLin
