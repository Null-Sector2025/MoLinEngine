#include "map_loader.h"
#include <fstream>
#include <sstream>
#include <iostream>

namespace MoLin::RPG {

// 简易 TMX 解析（支持 csv 格式的瓦片数据）
bool MapLoader::LoadTMX(const std::string& tmxPath, MapData& outMap) {
    std::ifstream file(tmxPath);
    if (!file) {
        std::cerr << "[MapLoader] Cannot open " << tmxPath << std::endl;
        return false;
    }
    std::string line;
    bool inData = false;
    std::string csvData;
    while (std::getline(file, line)) {
        if (line.find("<map ") != std::string::npos) {
            auto extract = [&](const std::string& attr) -> int {
                size_t p = line.find(attr + "=\"");
                if (p == std::string::npos) return 0;
                p += attr.size() + 2;
                size_t end = line.find("\"", p);
                return std::stoi(line.substr(p, end - p));
            };
            outMap.width = extract("width");
            outMap.height = extract("height");
            outMap.tileSize = extract("tilewidth");
        }
        if (line.find("<tileset ") != std::string::npos) {
            size_t p = line.find("source=\"");
            if (p != std::string::npos) {
                p += 8;
                size_t end = line.find("\"", p);
                outMap.tilesetImage = line.substr(p, end - p);
            }
        }
        if (line.find("<data") != std::string::npos) {
            inData = true;
            continue;
        }
        if (inData) {
            if (line.find("</data>") != std::string::npos) break;
            csvData += line;
        }
    }
    // 解析 CSV 瓦片数据
    outMap.tiles.resize(outMap.height, std::vector<int>(outMap.width, 0));
    std::replace(csvData.begin(), csvData.end(), ',', ' ');
    std::stringstream ss(csvData);
    for (int y = 0; y < outMap.height; ++y)
        for (int x = 0; x < outMap.width; ++x) {
            if (!(ss >> outMap.tiles[y][x])) outMap.tiles[y][x] = 0;
        }
    std::cout << "[MapLoader] Loaded TMX: " << outMap.width << "x" << outMap.height << std::endl;
    return true;
}

TileMap* MapLoader::BuildTileMap(const MapData& mapData, SDL_Texture* tileset) {
    auto* tilemap = new TileMap("map", mapData.width, mapData.height, mapData.tileSize, tileset);
    for (int y = 0; y < mapData.height; ++y)
        for (int x = 0; x < mapData.width; ++x)
            tilemap->SetTile(x, y, {mapData.tiles[y][x]});
    return tilemap;
}

} // namespace MoLin::RPG
