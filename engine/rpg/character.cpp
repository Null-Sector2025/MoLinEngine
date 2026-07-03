#include "character.h"

namespace MoLin::RPG {

void Character::AddAttribute(const std::string& key, int base, int max) {
    Attribute attr{key, base, base, max};
    m_Attributes[key] = attr;
}

Attribute* Character::GetAttribute(const std::string& key) {
    auto it = m_Attributes.find(key);
    return (it != m_Attributes.end()) ? &it->second : nullptr;
}

} // namespace MoLin::RPG
