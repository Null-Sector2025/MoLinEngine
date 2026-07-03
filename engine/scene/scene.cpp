#include "scene.h"
#include <iostream>

namespace MoLin {

// ---------- SceneNode ----------
SceneNode::SceneNode(const std::string& name)
    : m_Name(name) {}

void SceneNode::AddChild(SceneNode* child) {
    if (!child) return;
    // 如果已有父节点，先从旧父节点移除
    if (child->m_Parent) {
        child->m_Parent->RemoveChild(child);
    }
    child->m_Parent = this;
    m_Children.push_back(child);
}

void SceneNode::RemoveChild(SceneNode* child) {
    auto it = std::find(m_Children.begin(), m_Children.end(), child);
    if (it != m_Children.end()) {
        (*it)->m_Parent = nullptr;
        m_Children.erase(it);
    }
}

Transform SceneNode::GetWorldTransform() const {
    if (m_Parent) {
        Transform parentWorld = m_Parent->GetWorldTransform();
        Transform world = m_Transform;
        // 简易的世界变换：位置叠加，旋转叠加，缩放相乘
        world.x += parentWorld.x;
        world.y += parentWorld.y;
        world.rotation += parentWorld.rotation;
        world.scaleX *= parentWorld.scaleX;
        world.scaleY *= parentWorld.scaleY;
        return world;
    }
    return m_Transform;
}

void SceneNode::Update(float delta) {
    if (!m_Visible) return;
    OnUpdate(delta);
    for (auto* child : m_Children) {
        child->Update(delta);
    }
}

void SceneNode::Render(SDL_Renderer* renderer) {
    if (!m_Visible) return;
    OnRender(renderer);
    for (auto* child : m_Children) {
        child->Render(renderer);
    }
}

void SceneNode::UpdateRecursive(float delta) {
    Update(delta);  // 已经包含子节点遍历，因此直接调用
}

void SceneNode::RenderRecursive(SDL_Renderer* renderer) {
    Render(renderer);
}

// ---------- Scene ----------
Scene::Scene() : m_Root("SceneRoot") {}

void Scene::Update(float delta) {
    m_Root.Update(delta);
}

void Scene::Render(SDL_Renderer* renderer) {
    m_Root.Render(renderer);
}

void Scene::AddNode(SceneNode* node) {
    m_Root.AddChild(node);
}

} // namespace MoLin
