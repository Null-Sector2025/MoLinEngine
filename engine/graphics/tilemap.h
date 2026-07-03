#pragma once
#include "../scene/scene.h"
#include <vector>

namespace MoLin {

struct Tile {
    int id = 0;
    bool solid = false;
};

class TileMap : public SceneNode {
public:
    TileMap(const std::string& name, int mapWidth, int mapHeight, int tileSize, SDL_Texture* tileset);

    void SetTile(int x, int y, const Tile& tile);
    Tile GetTile(int x, int y) const;
    bool IsSolid(int x, int y) const;

    int GetMapWidth() const { return m_MapWidth; }
    int GetMapHeight() const { return m_MapHeight; }
    int GetTileSize() const { return m_TileSize; }

protected:
    void OnRender(SDL_Renderer* renderer) override;

    int m_MapWidth, m_MapHeight, m_TileSize;
    SDL_Texture* m_Tileset;
    std::vector<std::vector<Tile>> m_Tiles;
};

} // namespace MoLin
