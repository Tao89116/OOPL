//
// Created by polyunicorn on 2026/3/12.
//
#include "ResourceManager.h"

#include "Util/Logger.hpp"

ResourceManager& ResourceManager::GetInstance() {
    static ResourceManager instance;
    return instance;
}

void ResourceManager::Initialize() {
    if (!m_ImagePaths.empty()) {
        return;
    }

    const std::string root = std::string(RESOURCE_DIR);

    m_ImagePaths["background"] = root + "/background.png";
    m_ImagePaths["enemy"] = root + "/enemy.png";
    m_ImagePaths["tower"] = root + "/tower.png";
    m_ImagePaths["slot"] = root + "/slot.png";

    m_FontPaths["default"] = root + "/font.ttf";

    LOG_TRACE("ResourceManager initialized.");
}

void ResourceManager::Clear() {
    m_Images.clear();
    m_ImagePaths.clear();
    m_FontPaths.clear();
}

std::shared_ptr<Util::Image> ResourceManager::GetImage(const std::string& key) {
    auto found = m_Images.find(key);
    if (found != m_Images.end()) {
        return found->second;
    }

    auto it = m_ImagePaths.find(key);
    if (it == m_ImagePaths.end()) {
        throw std::runtime_error("Image key not found: " + key);
    }

    auto image = std::make_shared<Util::Image>(it->second);
    m_Images[key] = image;
    return image;
}

std::shared_ptr<Util::Text> ResourceManager::CreateText(
    const std::string& fontKey,
    int fontSize,
    const std::string& text,
    const Util::Color& color
) const {
    return std::make_shared<Util::Text>(
        GetFontPath(fontKey),
        fontSize,
        text,
        color
    );
}

std::string ResourceManager::GetImagePath(const std::string& key) const {
    auto it = m_ImagePaths.find(key);
    if (it == m_ImagePaths.end()) {
        throw std::runtime_error("Image path key not found: " + key);
    }
    return it->second;
}

std::string ResourceManager::GetFontPath(const std::string& key) const {
    auto it = m_FontPaths.find(key);
    if (it == m_FontPaths.end()) {
        throw std::runtime_error("Font path key not found: " + key);
    }
    return it->second;
}
