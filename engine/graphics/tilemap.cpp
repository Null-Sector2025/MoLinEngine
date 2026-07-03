#include "tilemap.h"

namespace MoLin {

TileMap::TileMap(const std::string& name, int w, int h, int ts, SDL_Texture* tex)
    : SceneNode(name), m_MapWidth(w), m_MapHeight(h), m_TileSize(ts), m_Tileset(tex) {
    m_Tiles.resize(h, std::vector<Tile>(w));
}
void TileMap::SetTile(int x, int y, const Tile& tile) {
    if (x >= 0 && x < m_MapWidth && y >= 0 && y < m_MapHeight) m_Tiles[y][x] = tile;
}
Tile TileMap::GetTile(int x, int y) const {
    if (x >= 0 && x < m_MapWidth && y >= 0 && y < m_MapHeight) return m_Tiles[y][x];
    return Tile();
}
bool TileMap::IsSolid(int x, int y) const { return GetTile(x, y).solid; }
void TileMap::OnRender(SDL_Renderer* renderer) {
    if (!m_Tileset) return;
    Transform world = GetWorldTransform();
    int cols; SDL_QueryTexture(m_Tileset, nullptr, nullptr, &cols, nullptr); cols /= m_TileSize;
    for (int y = 0; y < m_MapHeight; y++)
        for (int x = 0; x < m_MapWidth; x++) {
            Tile t = m_Tiles[y][x];
            if (t.id <= 0) continue;
            SDL_Rect src = { (t.id % cols) * m_TileSize, (t.id / cols) * m_TileSize, m_TileSize, m_TileSize };
            SDL_Rect dst = { static_cast<int>(world.x) + x * m_TileSize, static_cast<int>(world.y) + y * m_TileSize, m_TileSize, m_TileSize };
            SDL_RenderCopy(renderer, m_Tileset, &src, &dst);
        }
}

} // namespace MoLin
