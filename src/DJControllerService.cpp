#include "DJControllerService.h"
#include "MP3Track.h"
#include "WAVTrack.h"
#include <iostream>
#include <memory>

DJControllerService::DJControllerService(size_t cache_size)
    : cache(cache_size) {}
/**
 * TODO: Implement loadTrackToCache method
 */
int DJControllerService::loadTrackToCache(AudioTrack& track) {
    bool eviction_occurred = false;
    const std::string& title = track.get_title();
    if (cache.contains(title)) {
        //HIT case
        cache.get(title);
        return 1;
    }
    
    PointerWrapper<AudioTrack> cloned_wrapper = track.clone();
    if (cloned_wrapper.get() == nullptr) {
        std::cerr << "[ERROR] Track: \"" << title << "\" failed to clone\n";
        return 0; 
    }

    cloned_wrapper->load();
    cloned_wrapper->analyze_beatgrid();
    eviction_occurred = cache.put(std::move(cloned_wrapper));
    
    if (eviction_occurred) {
        return -1; // MISS with Eviction
    } else {
        return 0; // MISS without Eviction
    }
}

void DJControllerService::set_cache_size(size_t new_size) {
    cache.set_capacity(new_size);
}
//implemented
void DJControllerService::displayCacheStatus() const {
    std::cout << "\n=== Cache Status ===\n";
    cache.displayStatus();
    std::cout << "====================\n";
}

/**
 * TODO: Implement getTrackFromCache method
 */
AudioTrack* DJControllerService::getTrackFromCache(const std::string& track_title) {
    AudioTrack* found_track = cache.get(track_title);

    if (found_track == nullptr) {
        std::cerr << "[ERROR] Track: \"" << track_title << "\" not found in cache.\n";
        return nullptr;
    }

    return found_track;
}
