#ifndef MUSICLIST_ALBUM_HPP
#define MUSICLIST_ALBUM_HPP

#include <map>
#include <string>
#include <memory>
#include <cinttypes>
#include <json/value.h>

#include "Track.hpp"

using std::string;
using std::map;
using std::shared_ptr;

namespace MusicList
{
    class Album
    {
    private:
        string name;
        string artist;
        string mbid;
        map<string,shared_ptr<Track>> tracks;
        uint_fast8_t totalTracks;
    public:
        Album();

        explicit Album(const shared_ptr<Track>& track);

        /**
         * @brief Adds a new track to the Album.
         * 
         * @param track Track object to add to the Album.
         */
        void addTrack(const shared_ptr<Track>& track);

        /**
         * @brief Creates a JSON value containing the object's data.
         */
        const Json::Value toJSON() const;

        // =======
        // Getters
        // =======

        const string& getName() const;

        const string& getArtist() const;

        const map<string,shared_ptr<Track>>& getTrackSet() const;

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
