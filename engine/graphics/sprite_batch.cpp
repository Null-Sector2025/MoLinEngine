#include "sprite_batch.h"
#include <algorithm>

namespace MoLin {

SpriteBatch::SpriteBatch(SDL_Renderer* renderer) : m_Renderer(renderer) {}

void SpriteBatch::AddItem(const SpriteBatchItem& item) {
    m_Items.push_back(item);
}

void SpriteBatch::SortByLayer() {
    std::sort(m_Items.begin(), m_Items.end(),
        [](const SpriteBatchItem& a, const SpriteBatchItem& b) {
            return a.layer < b.layer;
        });
}

void SpriteBatch::Render() {
    SortByLayer();
    for (auto& item : m_Items) {
        if (!item.texture) continue;
        SDL_Rect dst = item.dst;
        dst.x -= static_cast<int>(m_CamX);
        dst.y -= static_cast<int>(m_CamY);
        SDL_RenderCopyEx(m_Renderer, item.texture,
            (item.src.w == 0 ? nullptr : &item.src),
            &dst, item.rotation, nullptr, item.flip);
    }
}

void SpriteBatch::Clear() {
    m_Items.clear();
}

} // namespace MoLin
