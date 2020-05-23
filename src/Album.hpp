#ifndef MUSICLIST_ALBUM_HPP
#define MUSICLIST_ALBUM_HPP

#include <vector>

#include "Track.hpp"

using std::vector;

namespace MusicList
{
    class Album
    {
    private:
        vector<Track> tracks;
    public:
        Album() = default;

        /**
         * @brief Adds a new track to the Album.
         * 
         * @param track Track object to add to the Album.
         */
        void addTrack(const Track& track);
    };
}

#endif // MUSICLIST_ALBUM_HPP
