#include "sprite.h"

namespace MoLin {

Sprite::Sprite(const std::string& name, SDL_Texture* texture)
    : SceneNode(name), m_Texture(texture) {}

void Sprite::SetTexture(SDL_Texture* texture) {
    m_Texture = texture;
}

int Sprite::GetWidth() const {
    if (!m_Texture) return 0;
    int w;
    SDL_QueryTexture(m_Texture, nullptr, nullptr, &w, nullptr);
    return static_cast<int>(w * m_Transform.scaleX);
}

int Sprite::GetHeight() const {
    if (!m_Texture) return 0;
    int h;
    SDL_QueryTexture(m_Texture, nullptr, nullptr, nullptr, &h);
    return static_cast<int>(h * m_Transform.scaleY);
}

void Sprite::ApplyAnchor(SDL_Rect& dst) const {
    switch (m_Anchor) {
        case AnchorPoint::TopCenter:    dst.x -= dst.w / 2; break;
        case AnchorPoint::TopRight:     dst.x -= dst.w; break;
        case AnchorPoint::CenterLeft:   dst.y -= dst.h / 2; break;
        case AnchorPoint::Center:       dst.x -= dst.w / 2; dst.y -= dst.h / 2; break;
        case AnchorPoint::CenterRight:  dst.x -= dst.w; dst.y -= dst.h / 2; break;
        case AnchorPoint::BottomLeft:   dst.y -= dst.h; break;
        case AnchorPoint::BottomCenter: dst.x -= dst.w / 2; dst.y -= dst.h; break;
        case AnchorPoint::BottomRight:  dst.x -= dst.w; dst.y -= dst.h; break;
        default: break;
    }
}

void Sprite::OnRender(SDL_Renderer* renderer) {
    if (!m_Texture) return;

    Transform world = GetWorldTransform();
    SDL_Rect dst;
    if (m_SrcRect.w != 0 && m_SrcRect.h != 0) {
        dst.w = static_cast<int>(m_SrcRect.w * world.scaleX);
        dst.h = static_cast<int>(m_SrcRect.h * world.scaleY);
    } else {
        SDL_QueryTexture(m_Texture, nullptr, nullptr, &dst.w, &dst.h);
        dst.w = static_cast<int>(dst.w * world.scaleX);
        dst.h = static_cast<int>(dst.h * world.scaleY);
    }
    dst.x = static_cast<int>(world.x);
    dst.y = static_cast<int>(world.y);
    ApplyAnchor(dst);

    SDL_SetTextureAlphaMod(m_Texture, m_Alpha);
    if (m_UseColorMod) {
        SDL_SetTextureColorMod(m_Texture, m_ColorMod.r, m_ColorMod.g, m_ColorMod.b);
    }

    SDL_RenderCopyEx(renderer, m_Texture,
                     (m_SrcRect.w == 0 ? nullptr : &m_SrcRect),
                     &dst, world.rotation, nullptr, m_Flip);

    SDL_SetTextureAlphaMod(m_Texture, 255);
    if (m_UseColorMod) {
        SDL_SetTextureColorMod(m_Texture, 255, 255, 255);
    }
}

} // namespace MoLin
