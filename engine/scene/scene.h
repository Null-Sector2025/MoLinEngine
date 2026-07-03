#pragma once
#include <SDL2/SDL.h>
#include <vector>
#include <string>
#include <algorithm>
#include <cmath>

namespace MoLin {

struct Transform {
    float x = 0.0f, y = 0.0f;
    float rotation = 0.0f;  // 角度制
    float scaleX = 1.0f, scaleY = 1.0f;
};

class SceneNode {
public:
    explicit SceneNode(const std::string& name);
    virtual ~SceneNode() = default;

    // 每帧调用，自动递归子节点（子类一般重写 OnUpdate）
    void Update(float delta);
    // 渲染（递归子节点，子类重写 OnRender）
    void Render(SDL_Renderer* renderer);

    // 添加/移除子节点（引擎管理生命周期，也可外部传入裸指针，由调用者保证有效性）
    void AddChild(SceneNode* child);
    void RemoveChild(SceneNode* child);
    SceneNode* GetParent() const { return m_Parent; }
    const std::vector<SceneNode*>& GetChildren() const { return m_Children; }

    // 变换与可见性
    void SetPosition(float x, float y) { m_Transform.x = x; m_Transform.y = y; }
    void SetRotation(float degrees) { m_Transform.rotation = degrees; }
    void SetScale(float sx, float sy) { m_Transform.scaleX = sx; m_Transform.scaleY = sy; }
    const Transform& GetTransform() const { return m_Transform; }
    Transform GetWorldTransform() const; // 结合父节点计算世界变换（简单实现可忽略层级）

    void SetVisible(bool visible) { m_Visible = visible; }
    bool IsVisible() const { return m_Visible; }

    const std::string& GetName() const { return m_Name; }

protected:
    // 子类重写这两个方法实现具体逻辑
    virtual void OnUpdate(float delta) {}
    virtual void OnRender(SDL_Renderer* renderer) {}

    std::string m_Name;
    Transform m_Transform;
    bool m_Visible = true;
    SceneNode* m_Parent = nullptr;
    std::vector<SceneNode*> m_Children;

    // 递归更新/渲染（内部调用）
    void UpdateRecursive(float delta);
    void RenderRecursive(SDL_Renderer* renderer);
};

class Scene {
public:
    Scene();
    ~Scene() = default;

    void Update(float delta);
    void Render(SDL_Renderer* renderer);

    SceneNode& GetRoot() { return m_Root; }
    void AddNode(SceneNode* node);  // 添加到根节点下

private:
    SceneNode m_Root;
};

} // namespace MoLin
