#pragma once
#include "scene/scene.h"
#include <functional>

namespace MoLin {

class UIButton : public SceneNode {
public:
    UIButton(const std::string& name, SDL_Texture* normal, SDL_Texture* hover = nullptr);
    virtual ~UIButton() {}

    void SetOnClick(std::function<void()> callback) { m_OnClick = callback; }
    bool CheckClick(int mouseX, int mouseY);

protected:
    void OnUpdate(float delta) override {}
    void OnRender(SDL_Renderer* renderer) override;

    SDL_Texture* m_NormalTex = nullptr;
    SDL_Texture* m_HoverTex = nullptr;
    bool m_Hovered = false;
    std::function<void()> m_OnClick;
};

} // namespace MoLin
