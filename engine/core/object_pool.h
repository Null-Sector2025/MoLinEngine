#pragma once
#include <vector>
#include <stack>
#include <functional>

namespace MoLin {

template<typename T>
class ObjectPool {
public:
    ObjectPool(int initialSize = 50, std::function<void(T*)> resetFunc = nullptr)
        : m_ResetFunc(resetFunc) {
        for (int i = 0; i < initialSize; ++i) {
            m_Pool.push(new T());
        }
    }

    ~ObjectPool() {
        while (!m_Pool.empty()) {
            delete m_Pool.top();
            m_Pool.pop();
        }
    }

    T* Acquire() {
        if (m_Pool.empty()) {
            return new T();
        }
        T* obj = m_Pool.top();
        m_Pool.pop();
        if (m_ResetFunc) m_ResetFunc(obj);
        return obj;
    }

    void Release(T* obj) {
        if (m_ResetFunc) m_ResetFunc(obj);
        m_Pool.push(obj);
    }

private:
    std::stack<T*> m_Pool;
    std::function<void(T*)> m_ResetFunc;
};

} // namespace MoLin
