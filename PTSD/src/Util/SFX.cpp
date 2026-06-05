#include "Util/SFX.hpp"
#include "Util/Logger.hpp"

std::shared_ptr<Mix_Chunk> LoadChunk(const std::string &filepath) {
    auto chunk = std::shared_ptr<Mix_Chunk>(Mix_LoadWAV(filepath.c_str()),
                                            Mix_FreeChunk);

    if (chunk == nullptr) {
        LOG_DEBUG("Failed to load SFX: '{}'", filepath);
        LOG_DEBUG("{}", Mix_GetError());
    }

    return chunk;
}

namespace Util {

SFX::SFX(const std::string &path)
    : m_Chunk(s_Store.Get(path)) {}

int SFX::GetVolume() const {
    return Mix_VolumeChunk(m_Chunk.get(), -1);
}

int SFX::GetDurationMs() const {
    if (m_Chunk == nullptr) {
        return 0;
    }

    int frequency = 0;
    Uint16 format = 0;
    int channels = 0;
    if (Mix_QuerySpec(&frequency, &format, &channels) == 0 || frequency <= 0 || channels <= 0) {
        return 0;
    }

    const int bytesPerSample = (SDL_AUDIO_BITSIZE(format) / 8) * channels;
    if (bytesPerSample <= 0) {
        return 0;
    }

    const double sampleCount = static_cast<double>(m_Chunk->alen) / static_cast<double>(bytesPerSample);
    return static_cast<int>((sampleCount * 1000.0) / static_cast<double>(frequency));
}

void SFX::SetVolume(const int volume) {
    Mix_VolumeChunk(m_Chunk.get(), volume);
}

void SFX::LoadMedia(const std::string &path) {
    m_Chunk = s_Store.Get(path);
}

void SFX::VolumeUp(const int step) {
    int volume = GetVolume();
    SetVolume(volume + step);
}

void SFX::VolumeDown(const int step) {
    int volume = GetVolume();
    SetVolume(volume - step);
}
void SFX::Play(const int loop, const int duration) {
    Mix_PlayChannelTimed(-1, m_Chunk.get(), loop, duration);
}
void SFX::FadeIn(const unsigned int tick, const int loop,
                 const unsigned int duration) {
    Mix_FadeInChannelTimed(-1, m_Chunk.get(), loop, static_cast<int>(tick),
                           static_cast<int>(duration));
}

Util::AssetStore<std::shared_ptr<Mix_Chunk>> SFX::s_Store(LoadChunk);

} // namespace Util
