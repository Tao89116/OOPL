#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include "pch.hpp"
#include "Util/Color.hpp"
#include "Util/Image.hpp"
#include "Util/Animation.hpp"
#include "Util/Text.hpp"
#include <nlohmann/json.hpp>

class ResourceManager {
public:
    static ResourceManager& GetInstance();

    void Initialize();
    void Clear();

    std::shared_ptr<Util::Image> GetImage(const std::string& key);
    std::shared_ptr<Util::Animation> CreateAnimation(const std::string& key);
    std::shared_ptr<Util::Text> CreateText(
        const std::string& fontKey,
        int fontSize,
        const std::string& text,
        const Util::Color& color
    ) const;

    std::string GetFontPath(const std::string& key) const;

    void RegisterJson(const std::string& key, const std::string& relativePath);
    const nlohmann::json& GetJson(const std::string& key);

private:
    ResourceManager() = default;
    ResourceManager(const ResourceManager&) = delete;
    ResourceManager& operator=(const ResourceManager&) = delete;

    struct SpriteSheetAnimationSpec {
        std::string path;
        int columns = 1;
        int rows = 1;
        std::size_t intervalMs = 100;
        bool looping = true;
        std::size_t cooldownMs = 0;
    };

    nlohmann::json LoadJsonFromFile(const std::string& fullPath) const;
    void RegisterSpriteSheetAnimation(
        const std::string& key,
        const std::string& relativePath,
        int columns,
        int rows,
        std::size_t intervalMs,
        bool looping = true,
        std::size_t cooldownMs = 0
    );
    std::vector<std::string> GetSpriteSheetFramePaths(const std::string& key) const;

private:
    std::unordered_map<std::string, std::string> m_ImagePaths;
    std::unordered_map<std::string, std::string> m_FontPaths;
    std::unordered_map<std::string, std::string> m_JsonPaths;
    std::unordered_map<std::string, SpriteSheetAnimationSpec> m_SpriteSheetAnimations;

    std::unordered_map<std::string, std::shared_ptr<Util::Image>> m_ImageCache;
    std::unordered_map<std::string, nlohmann::json> m_JsonCache;
};

#endif
