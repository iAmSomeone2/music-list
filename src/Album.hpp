#ifndef MUSICLIST_ALBUM_HPP
#define MUSICLIST_ALBUM_HPP

#include <set>
#include <string>
#include <cinttypes>
#include <json/value.h>

#include "Track.hpp"

using std::string;
using std::set;

namespace MusicList
{
    class Album
    {
    private:
        string name;
        string artist;
        string mbid;
        set<Track> tracks;
        uint_fast8_t totalTracks;
    public:
        Album();

        explicit Album(const Track& track);

        /**
         * @brief Adds a new track to the Album.
         * 
         * @param track Track object to add to the Album.
         */
        bool addTrack(const Track& track);

        /**
         * @brief Creates a JSON value containing the object's data.
         */
        const Json::Value toJSON() const;

        // =======
        // Getters
        // =======

        const string& getName() const;

        const string& getArtist() const;

        const set<Track>& getTrackSet() const;

        const uint_fast8_t& getTotalTracks() const;

        const string& getMBID() const;

        // ==================
        // Operator Overloads
        // ==================

        friend inline bool operator< (const Album& lhs, const Album& rhs)
        {
            return std::tie(lhs.mbid, lhs.artist, lhs.name) < std::tie(rhs.mbid, rhs.artist, rhs.name);
        }

        friend inline bool operator> (const Album& lhs, const Album& rhs) { return rhs < lhs; }

        friend inline bool operator== (const Album& lhs, const Album& rhs)
        {
            return std::tie(lhs.artist, lhs.name) == std::tie(rhs.artist, rhs.name);
        }

        friend inline bool operator!= (const Album& lhs, const Album& rhs) { return !(lhs == rhs); }
    };
}

#endif // MUSICLIST_ALBUM_HPP
