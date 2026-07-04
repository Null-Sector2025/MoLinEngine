#pragma once
#include <SDL2/SDL.h>
#include <vector>
#include <string>
#include <algorithm>
#include <cmath>
#include <functional>
#include <map>

namespace MoLin {

struct Transform {
    float x = 0.0f, y = 0.0f;
    float rotation = 0.0f;
    float scaleX = 1.0f, scaleY = 1.0f;
};

class SceneNode {
public:
    explicit SceneNode(const std::string& name);
    virtual ~SceneNode();

    void Update(float delta);
    void Render(SDL_Renderer* renderer);

    void AddChild(SceneNode* child);
    void RemoveChild(SceneNode* child);
    void RemoveFromParent();
    void RemoveAllChildren();
    SceneNode* GetParent() const { return m_Parent; }
    const std::vector<SceneNode*>& GetChildren() const { return m_Children; }

    void SetPosition(float x, float y) { m_Transform.x = x; m_Transform.y = y; }
    void SetRotation(float degrees) { m_Transform.rotation = degrees; }
    void SetScale(float sx, float sy) { m_Transform.scaleX = sx; m_Transform.scaleY = sy; }
    const Transform& GetTransform() const { return m_Transform; }
    Transform GetWorldTransform() const;

    void SetVisible(bool visible) { m_Visible = visible; }
    bool IsVisible() const { return m_Visible; }
    void SetActive(bool active) { m_Active = active; }
    bool IsActive() const { return m_Active; }
    void SetTag(const std::string& tag) { m_Tag = tag; }
    const std::string& GetTag() const { return m_Tag; }
    void SetLayer(int layer) { m_Layer = layer; SortChildren(); }
    int GetLayer() const { return m_Layer; }

    const std::string& GetName() const { return m_Name; }
    void SetUserData(void* data) { m_UserData = data; }
    void* GetUserData() const { return m_UserData; }

    void SetDestroyFlag(bool flag) { m_DestroyFlag = flag; }
    bool GetDestroyFlag() const { return m_DestroyFlag; }

protected:
    virtual void OnUpdate(float delta) {}
    virtual void OnRender(SDL_Renderer* renderer) {}

    std::string m_Name;
    std::string m_Tag;
    Transform m_Transform;
    bool m_Visible = true;
    bool m_Active = true;
    int m_Layer = 0;
    bool m_DestroyFlag = false;
    void* m_UserData = nullptr;
    SceneNode* m_Parent = nullptr;
    std::vector<SceneNode*> m_Children;

    void SortChildren();
};

class Scene {
public:
    Scene();
    ~Scene();

    void Update(float delta);
    void Render(SDL_Renderer* renderer);
    void CleanupDestroyed();

    SceneNode& GetRoot() { return m_Root; }
    void AddNode(SceneNode* node);
    SceneNode* FindNodeByTag(const std::string& tag);
    SceneNode* FindNodeByName(const std::string& name);
    std::vector<SceneNode*> FindNodesByTag(const std::string& tag);

    void OnEnter();
    void OnExit();
    bool IsActive() const { return m_Active; }
    void SetActive(bool active) { m_Active = active; }

    std::function<void()> onEnterCallback;
    std::function<void()> onExitCallback;

private:
    SceneNode m_Root;
    bool m_Active = true;
    SceneNode* FindRecursive(SceneNode* node, const std::string& str, bool byTag);
    void FindRecursiveByTag(SceneNode* node, const std::string& tag, std::vector<SceneNode*>& results);
};

} // namespace MoLin
