#include "scene.h"
#include <iostream>

namespace MoLin {

SceneNode::SceneNode(const std::string& name) : m_Name(name) {}

SceneNode::~SceneNode() {
    RemoveAllChildren();
}

void SceneNode::AddChild(SceneNode* child) {
    if (!child) return;
    if (child->m_Parent) child->m_Parent->RemoveChild(child);
    child->m_Parent = this;
    m_Children.push_back(child);
    SortChildren();
}

void SceneNode::RemoveChild(SceneNode* child) {
    auto it = std::find(m_Children.begin(), m_Children.end(), child);
    if (it != m_Children.end()) {
        (*it)->m_Parent = nullptr;
        m_Children.erase(it);
    }
}

void SceneNode::RemoveFromParent() {
    if (m_Parent) m_Parent->RemoveChild(this);
}

void SceneNode::RemoveAllChildren() {
    for (auto* child : m_Children) {
        child->m_Parent = nullptr;
        delete child;
    }
    m_Children.clear();
}

void SceneNode::SortChildren() {
    std::sort(m_Children.begin(), m_Children.end(),
        [](SceneNode* a, SceneNode* b) { return a->m_Layer < b->m_Layer; });
    if (m_Parent) m_Parent->SortChildren();
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
    for (int i = m_Children.size() - 1; i >= 0; --i) {
        auto* child = m_Children[i];
        if (child->GetDestroyFlag()) {
            RemoveChild(child);
            delete child;
            continue;
        }
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

Scene::Scene() : m_Root("SceneRoot") {}
Scene::~Scene() {}

void Scene::Update(float delta) {
    if (!m_Active) return;
    m_Root.Update(delta);
    CleanupDestroyed();
}

void Scene::Render(SDL_Renderer* renderer) {
    if (!m_Active) return;
    m_Root.Render(renderer);
}

void Scene::CleanupDestroyed() {
    auto& children = m_Root.GetChildren();
    std::vector<SceneNode*> toDelete;
    for (auto* child : children) {
        if (child->GetDestroyFlag()) toDelete.push_back(child);
    }
    for (auto* node : toDelete) {
        m_Root.RemoveChild(node);
        delete node;
    }
}

void Scene::AddNode(SceneNode* node) { m_Root.AddChild(node); }

SceneNode* Scene::FindNodeByTag(const std::string& tag) {
    return FindRecursive(&m_Root, tag, true);
}
SceneNode* Scene::FindNodeByName(const std::string& name) {
    return FindRecursive(&m_Root, name, false);
}
std::vector<SceneNode*> Scene::FindNodesByTag(const std::string& tag) {
    std::vector<SceneNode*> results;
    FindRecursiveByTag(&m_Root, tag, results);
    return results;
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
void Scene::FindRecursiveByTag(SceneNode* node, const std::string& tag, std::vector<SceneNode*>& results) {
    if (node->GetTag() == tag) results.push_back(node);
    for (auto* child : node->GetChildren()) {
        FindRecursiveByTag(child, tag, results);
    }
}

void Scene::OnEnter() {
    if (onEnterCallback) onEnterCallback();
}
void Scene::OnExit() {
    if (onExitCallback) onExitCallback();
}

} // namespace MoLin
