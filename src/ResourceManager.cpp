#include "ResourceManager.h"

#include "Util/Logger.hpp"
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
    m_ImagePaths["difficulty_phrame"] = root + "/phrame.png";

    // Start scene animation frames
    m_ImagePaths["banana_cat_01"] = root + "/banancat01.png";
    m_ImagePaths["banana_cat_02"] = root + "/banancat02.png";
    m_ImagePaths["banana_cat_03"] = root + "/banancat03.png";
    m_ImagePaths["banana_cat_04"] = root + "/banancat04.png";
    m_ImagePaths["banana_cat_05"] = root + "/banancat05.png";
    m_ImagePaths["banana_cat_06"] = root + "/banancat06.png";
    m_ImagePaths["banana_cat_07"] = root + "/banancat07.png";

    // Buttons
    m_ImagePaths["btn_tower_1"] = root + "/button-0.png";
    m_ImagePaths["btn_tower_2"] = root + "/button-1.png";
    m_ImagePaths["btn_tower_3"] = root + "/button-2.png";
    m_ImagePaths["btn_tower_4"] = root + "/button-3.png";
    m_ImagePaths["btn_tower_5"] = root + "/button-4.png";
    m_ImagePaths["btn_tower_6"] = root + "/button-5.png";
    m_ImagePaths["btn_tower_7"] = root + "/button-6.png";
    m_ImagePaths["btn_tower_8"] = root + "/button-7.png";

    m_ImagePaths["btn_start"]  = root + "/btn_start.png";
    m_ImagePaths["btn_sell"] = root + "/btn_sell.png";
    m_ImagePaths["btn_upgrade_1"] = root + "/btn_upgrade_1.png";
    m_ImagePaths["btn_upgrade_2"] = root + "/btn_upgrade_2.png";

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
    m_ImagePaths["projectile_3"] = root + "/projectile-3.png";
    m_ImagePaths["projectile_4"] = root + "/projectile-4.png";
    m_ImagePaths["projectile_5"] = root + "/projectile-5.png";


    m_ImagePaths["hit"] = root + "/BTD1_dart_hitbit.png";
    m_ImagePaths["pop"] = root + "/pop.png";
    m_ImagePaths["tower3_explosion"] = root + "/tower3_explosion.png";

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
    m_ImagePaths["whiteground"] = root + "/whiteground.png";
    // Placement preview
    m_ImagePaths["range_circle_valid"]   = root + "/range-circle-valid.png";
    m_ImagePaths["range_circle_invalid"] = root + "/range-circle-invalid.png";

    // Fonts
    m_FontPaths["default"] = root + "/font.ttf";

    // Sound effects
    m_SoundPaths["pop_01"] = root + "/Pop01.wav";
    m_SoundPaths["pop_02"] = root + "/Pop02.wav";
    m_SoundPaths["pop_03"] = root + "/Pop03.wav";
    m_SoundPaths["pop_04"] = root + "/Pop04.wav";
    m_SoundPaths["bombsnd"] = root + "/BombSnd.wav";
    m_SoundPaths["splat"] = root + "/splat.wav";

    // Json
    RegisterJson("map_paths", "map_paths.json");
    RegisterJson("waves", "waves.json");

    LOG_TRACE("ResourceManager initialized.");
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
    m_SoundPaths.clear();
    m_JsonPaths.clear();
    m_ImageCache.clear();
    m_JsonCache.clear();
}

std::string ResourceManager::GetImagePath(const std::string& key) const {
    const auto found = m_ImagePaths.find(key);
    if (found == m_ImagePaths.end()) {
        throw std::runtime_error("Image key not found: " + key);
    }

    return found->second;
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

std::string ResourceManager::GetSoundPath(const std::string& key) const {
    const auto found = m_SoundPaths.find(key);
    if (found == m_SoundPaths.end()) {
        throw std::runtime_error("Sound key not found: " + key);
    }
    return found->second;
}
