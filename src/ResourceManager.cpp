#include "ResourceManager.h"

#include "Util/Logger.hpp"
#include <filesystem>
#include <fstream>
#include <sstream>
#include <stdexcept>

ResourceManager& ResourceManager::GetInstance() {
    static ResourceManager instance;
    return instance;
}

void ResourceManager::Initialize() {
    if (!m_ImagePaths.empty()) {
        return;
    }

    const std::string root = std::string(RESOURCE_DIR);

    // Scene backgrounds
    m_ImagePaths["bg_start"]  = root + "/background-0.png";
    m_ImagePaths["bg_easy"]   = root + "/background-1.png";
    m_ImagePaths["bg_normal"] = root + "/background-2.png";
    m_ImagePaths["bg_hard"]   = root + "/background-3.png";
    m_ImagePaths["bg_result"] = root + "/background-0.png";
    m_ImagePaths["bg_game"]   = root + "/background.png";

    // Buttons
    m_ImagePaths["btn_tower_1"] = root + "/button-0.png";
    m_ImagePaths["btn_tower_2"] = root + "/button-1.png";
    m_ImagePaths["btn_tower_3"] = root + "/button-2.png";
    m_ImagePaths["btn_tower_4"] = root + "/button-3.png";
    m_ImagePaths["btn_tower_5"] = root + "/button-4.png";
    m_ImagePaths["btn_tower_6"] = root + "/button-5.png";
    m_ImagePaths["btn_tower_7"] = root + "/button-6.png";
    m_ImagePaths["btn_tower_8"] = root + "/button-7.png";

    m_ImagePaths["btn_start"]  = root + "/button-3.png";
    m_ImagePaths["btn_action"] = root + "/button-4.png";

    // Enemies
    m_ImagePaths["bloon_0"] = root + "/enemy-0.png";
    m_ImagePaths["bloon_1"] = root + "/enemy-1.png";
    m_ImagePaths["bloon_2"] = root + "/enemy-2.png";
    m_ImagePaths["bloon_3"] = root + "/enemy-3.png";
    m_ImagePaths["bloon_4"] = root + "/enemy-4.png";
    m_ImagePaths["bloon_5"] = root + "/enemy-5.png";
    m_ImagePaths["bloon_6"] = root + "/enemy-6.png";
    m_ImagePaths["bloon_7"] = root + "/enemy-7.png";
    m_ImagePaths["bloon_default"] = root + "/enemy.png";

    // Projectiles
    m_ImagePaths["projectile_0"] = root + "/projectile-0.png";
    m_ImagePaths["projectile_1"] = root + "/projectile-1.png";
    m_ImagePaths["projectile_2"] = root + "/projectile-2.png";
    m_ImagePaths["projectile_2_test"] = root + "/projectile-2-test.png";
    m_ImagePaths["projectile_3"] = root + "/projectile-3.png";
    m_ImagePaths["projectile_4"] = root + "/projectile-4.png";
    m_ImagePaths["projectile_5"] = root + "/projectile-5.png";


    m_ImagePaths["hit"] = root + "/BTD1_dart_hitbit.png";

    // Towers / slots
    m_ImagePaths["tower_basic"]  = root + "/tower-0.png";
    m_ImagePaths["tower_dart"]   = root + "/tower-0.png";
    m_ImagePaths["tower_track"]  = root + "/tower-1.png";
    m_ImagePaths["tower_ice"]    = root + "/tower-2.png";
    m_ImagePaths["tower_cannon"] = root + "/tower-3.png";
    m_ImagePaths["tower_boom"]   = root + "/tower-4.png";
    m_ImagePaths["tower_super"]  = root + "/tower-5.png";
    m_ImagePaths["tower_spikes"] = root + "/tower-6.png";
    m_ImagePaths["tower_glues"]  = root + "/tower-7.png";

    m_ImagePaths["tower_slot"] = root + "/slot.png";
    //UI
    m_ImagePaths["hud"] = root + "/hud.png";

    // Animations
    m_ImagePaths["bananacat_sheet"] = root + "/bananacatframes.png";
    RegisterSpriteSheetAnimation("bananacat", "bananacatframes.png", 5, 10, 80);
    // Placement preview
    m_ImagePaths["range_circle_valid"]   = root + "/range-circle-valid.png";
    m_ImagePaths["range_circle_invalid"] = root + "/range-circle-invalid.png";

    // Fonts
    m_FontPaths["default"] = root + "/font.ttf";

    // Json
    RegisterJson("map_paths", "map_paths.json");

    LOG_TRACE("ResourceManager initialized.");
}

void ResourceManager::RegisterSpriteSheetAnimation(
    const std::string& key,
    const std::string& relativePath,
    int columns,
    int rows,
    std::size_t intervalMs,
    bool looping,
    std::size_t cooldownMs
) {
    if (columns <= 0 || rows <= 0) {
        throw std::runtime_error("Invalid sprite sheet grid for animation: " + key);
    }

    const std::string root = std::string(RESOURCE_DIR);
    m_SpriteSheetAnimations[key] = SpriteSheetAnimationSpec{
        root + "/" + relativePath,
        columns,
        rows,
        intervalMs,
        looping,
        cooldownMs
    };
}

std::vector<std::string> ResourceManager::GetSpriteSheetFramePaths(const std::string& key) const {
    const auto found = m_SpriteSheetAnimations.find(key);
    if (found == m_SpriteSheetAnimations.end()) {
        throw std::runtime_error("Animation key not found: " + key);
    }

    const SpriteSheetAnimationSpec& spec = found->second;
    const std::filesystem::path sheetPath(spec.path);
    const std::filesystem::path frameDirectory =
        sheetPath.parent_path() / ".generated" / (sheetPath.stem().string() + "_frames");

    std::filesystem::create_directories(frameDirectory);

    std::vector<std::string> framePaths;
    framePaths.reserve(static_cast<std::size_t>(spec.columns * spec.rows));

    for (int row = 0; row < spec.rows; ++row) {
        for (int column = 0; column < spec.columns; ++column) {
            const int index = row * spec.columns + column;
            std::ostringstream fileName;
            fileName << sheetPath.stem().string() << "_" << index << ".png";
            framePaths.push_back((frameDirectory / fileName.str()).string());
        }
    }

    const bool allFramesExist = std::all_of(
        framePaths.begin(),
        framePaths.end(),
        [](const std::string& path) { return std::filesystem::exists(path); }
    );
    if (allFramesExist) {
        return framePaths;
    }

    std::shared_ptr<SDL_Surface> sheet(IMG_Load(spec.path.c_str()), SDL_FreeSurface);
    if (sheet == nullptr) {
        throw std::runtime_error("Failed to load animation sprite sheet: " + spec.path);
    }

    const int frameWidth = sheet->w / spec.columns;
    const int frameHeight = sheet->h / spec.rows;
    if (frameWidth <= 0 || frameHeight <= 0) {
        throw std::runtime_error("Invalid frame size for animation: " + key);
    }

    for (int row = 0; row < spec.rows; ++row) {
        for (int column = 0; column < spec.columns; ++column) {
            const int index = row * spec.columns + column;
            const std::string& framePath = framePaths.at(static_cast<std::size_t>(index));

            SDL_Rect sourceRect{column * frameWidth, row * frameHeight, frameWidth, frameHeight};
            std::shared_ptr<SDL_Surface> frame(
                SDL_CreateRGBSurfaceWithFormat(0, frameWidth, frameHeight, 32, SDL_PIXELFORMAT_RGBA32),
                SDL_FreeSurface
            );
            if (frame == nullptr) {
                throw std::runtime_error("Failed to create animation frame surface: " + key);
            }

            SDL_SetSurfaceBlendMode(sheet.get(), SDL_BLENDMODE_NONE);
            if (SDL_BlitSurface(sheet.get(), &sourceRect, frame.get(), nullptr) != 0) {
                throw std::runtime_error("Failed to slice animation frame: " + key);
            }
            if (IMG_SavePNG(frame.get(), framePath.c_str()) != 0) {
                throw std::runtime_error("Failed to save animation frame: " + framePath);
            }
        }
    }

    return framePaths;
}

std::shared_ptr<Util::Animation> ResourceManager::CreateAnimation(const std::string& key) {
    const auto found = m_SpriteSheetAnimations.find(key);
    if (found == m_SpriteSheetAnimations.end()) {
        throw std::runtime_error("Animation key not found: " + key);
    }

    const SpriteSheetAnimationSpec& spec = found->second;
    return std::make_shared<Util::Animation>(
        GetSpriteSheetFramePaths(key),
        true,
        spec.intervalMs,
        spec.looping,
        spec.cooldownMs
    );
}

void ResourceManager::RegisterJson(const std::string& key, const std::string& relativePath) {
    const std::string root = std::string(RESOURCE_DIR);
    m_JsonPaths[key] = root + "/" + relativePath;
}

nlohmann::json ResourceManager::LoadJsonFromFile(const std::string& fullPath) const {
    std::ifstream file(fullPath);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open json file: " + fullPath);
    }

    nlohmann::json data;
    file >> data;
    return data;
}

const nlohmann::json& ResourceManager::GetJson(const std::string& key) {
    const auto cached = m_JsonCache.find(key);
    if (cached != m_JsonCache.end()) {
        return cached->second;
    }

    const auto found = m_JsonPaths.find(key);
    if (found == m_JsonPaths.end()) {
        throw std::runtime_error("Json key not found: " + key);
    }

    m_JsonCache[key] = LoadJsonFromFile(found->second);
    return m_JsonCache[key];
}

void ResourceManager::Clear() {
    m_ImagePaths.clear();
    m_FontPaths.clear();
    m_JsonPaths.clear();
    m_SpriteSheetAnimations.clear();
    m_ImageCache.clear();
    m_JsonCache.clear();
}

std::shared_ptr<Util::Image> ResourceManager::GetImage(const std::string& key) {
    const auto cached = m_ImageCache.find(key);
    if (cached != m_ImageCache.end()) {
        return cached->second;
    }

    const auto found = m_ImagePaths.find(key);
    if (found == m_ImagePaths.end()) {
        throw std::runtime_error("Image key not found: " + key);
    }

    auto image = std::make_shared<Util::Image>(found->second);
    m_ImageCache[key] = image;
    return image;
}

std::shared_ptr<Util::Text> ResourceManager::CreateText(
    const std::string& fontKey,
    int fontSize,
    const std::string& text,
    const Util::Color& color
) const {
    std::string safeText = text.empty() ? " " : text;
    return std::make_shared<Util::Text>(GetFontPath(fontKey), fontSize, safeText, color);
}

std::string ResourceManager::GetFontPath(const std::string& key) const {
    const auto found = m_FontPaths.find(key);
    if (found == m_FontPaths.end()) {
        throw std::runtime_error("Font key not found: " + key);
    }
    return found->second;
}
