#include "scene.h"
#include <iostream>

namespace MoLin {

SceneNode::SceneNode(const std::string& name) : m_Name(name) {}

void SceneNode::AddChild(SceneNode* child) {
    if (!child) return;
    if (child->m_Parent) child->m_Parent->RemoveChild(child);
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
        Transform pw = m_Parent->GetWorldTransform();
        Transform w = m_Transform;
        w.x += pw.x;
        w.y += pw.y;
        w.rotation += pw.rotation;
        w.scaleX *= pw.scaleX;
        w.scaleY *= pw.scaleY;
        return w;
    }
    return m_Transform;
}

void SceneNode::Update(float delta) {
    if (!m_Active || !m_Visible) return;
    OnUpdate(delta);
    for (auto* child : m_Children) child->Update(delta);
}

void SceneNode::Render(SDL_Renderer* renderer) {
    if (!m_Visible) return;
    OnRender(renderer);
    for (auto* child : m_Children) child->Render(renderer);
}

Scene::Scene() : m_Root("SceneRoot") {}

void Scene::Update(float delta) { m_Root.Update(delta); }
void Scene::Render(SDL_Renderer* renderer) { m_Root.Render(renderer); }
void Scene::AddNode(SceneNode* node) { m_Root.AddChild(node); }

SceneNode* Scene::FindNodeByTag(const std::string& tag) {
    return FindRecursive(&m_Root, tag, true);
}
SceneNode* Scene::FindNodeByName(const std::string& name) {
    return FindRecursive(&m_Root, name, false);
}
SceneNode* Scene::FindRecursive(SceneNode* node, const std::string& str, bool byTag) {
    if (byTag && node->GetTag() == str) return node;
    if (!byTag && node->GetName() == str) return node;
    for (auto* child : node->GetChildren()) {
        auto* found = FindRecursive(child, str, byTag);
        if (found) return found;
    }
    return nullptr;
}

} // namespace MoLin
