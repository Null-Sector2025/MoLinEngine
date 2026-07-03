#include "tilemap.h"

namespace MoLin {

TileMap::TileMap(const std::string& name, int mapWidth, int mapHeight, int tileSize, SDL_Texture* tileset)
    : SceneNode(name), m_MapWidth(mapWidth), m_MapHeight(mapHeight), m_TileSize(tileSize), m_Tileset(tileset) {
    m_Tiles.resize(mapHeight, std::vector<Tile>(mapWidth));
}

void TileMap::SetTile(int x, int y, const Tile& tile) {
    if (x >= 0 && x < m_MapWidth && y >= 0 && y < m_MapHeight) {
        m_Tiles[y][x] = tile;
    }
}

Tile TileMap::GetTile(int x, int y) const {
    if (x >= 0 && x < m_MapWidth && y >= 0 && y < m_MapHeight) {
        return m_Tiles[y][x];
    }
    return Tile();
}

bool TileMap::IsSolid(int x, int y) const {
    return GetTile(x, y).solid;
}

void TileMap::OnRender(SDL_Renderer* renderer) {
    if (!m_Tileset) return;
    Transform world = GetWorldTransform();
    int tileCols = 0;
    SDL_QueryTexture(m_Tileset, nullptr, nullptr, &tileCols, nullptr);
    tileCols /= m_TileSize;

    for (int y = 0; y < m_MapHeight; y++) {
        for (int x = 0; x < m_MapWidth; x++) {
            Tile tile = m_Tiles[y][x];
            if (tile.id <= 0) continue;

            SDL_Rect src = { (tile.id % tileCols) * m_TileSize, (tile.id / tileCols) * m_TileSize, m_TileSize, m_TileSize };
            SDL_Rect dst = { static_cast<int>(world.x) + x * m_TileSize, static_cast<int>(world.y) + y * m_TileSize, m_TileSize, m_TileSize };
            SDL_RenderCopy(renderer, m_Tileset, &src, &dst);
        }
    }
}

} // namespace MoLin
