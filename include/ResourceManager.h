#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include "pch.hpp"
#include "Util/Color.hpp"
#include "Util/Image.hpp"
#include "Util/Text.hpp"

class ResourceManager {
public:
    static ResourceManager& GetInstance();

    void Initialize();
    void Clear();

    std::shared_ptr<Util::Image> GetImage(const std::string& key);
    std::shared_ptr<Util::Text> CreateText(
        const std::string& fontKey,
        int fontSize,
        const std::string& text,
        const Util::Color& color
    ) const;

    std::string GetFontPath(const std::string& key) const;

private:
    ResourceManager() = default;
    ResourceManager(const ResourceManager&) = delete;
    ResourceManager& operator=(const ResourceManager&) = delete;

private:
    std::unordered_map<std::string, std::string> m_ImagePaths;
    std::unordered_map<std::string, std::string> m_FontPaths;
    std::unordered_map<std::string, std::shared_ptr<Util::Image>> m_ImageCache;
};

#endif