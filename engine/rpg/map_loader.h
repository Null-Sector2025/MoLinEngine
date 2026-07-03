#pragma once
#include <string>
#include <vector>
#include "../graphics/tilemap.h"

namespace MoLin::RPG {

struct MapData {
    int width, height, tileSize;
    std::vector<std::vector<int>> tiles; // tile ID 矩阵
    std::string tilesetImage;
};

class MapLoader {
public:
    static bool LoadTMX(const std::string& tmxPath, MapData& outMap);
    static TileMap* BuildTileMap(const MapData& mapData, SDL_Texture* tileset);
};

} // namespace MoLin::RPG
