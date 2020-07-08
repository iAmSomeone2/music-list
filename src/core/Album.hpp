/*
  BSD 3-Clause License
  
  Copyright (c) 2020, Brenden Davidson
  All rights reserved.
  
  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions are met:
  
  1. Redistributions of source code must retain the above copyright notice, this
     list of conditions and the following disclaimer.
  
  2. Redistributions in binary form must reproduce the above copyright notice,
     this list of conditions and the following disclaimer in the documentation
     and/or other materials provided with the distribution.
  
  3. Neither the name of the copyright holder nor the names of its
     contributors may be used to endorse or promote products derived from
     this software without specific prior written permission.
  
  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  
*/
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
        string name = "";
        string artist = "";
        string mbid = "";
        map<string,shared_ptr<Track>> tracks = map<string,shared_ptr<Track>>();
        uint_fast8_t totalTracks = 0;
    public:
        Album();

        explicit Album(const shared_ptr<Track>& track);

        /**
         * @brief Adds a new track to the Album.
         * 
         * @param track Track object to add to the Album.
         */
        void addTrack(const shared_ptr<Track>& track);


        void addToDB(sqlite3* dbConnection);

        /**
         * @brief Creates a JSON value containing the object's data.
         */
        [[nodiscard]] Json::Value toJSON() const;

        // =======
        // Getters
        // =======

        [[nodiscard]] [[maybe_unused]] const string& getName() const;

        [[nodiscard]] [[maybe_unused]] const string& getArtist() const;

        [[nodiscard]] [[maybe_unused]] const map<string,shared_ptr<Track>>& getTrackSet() const;

        [[nodiscard]] [[maybe_unused]] const uint_fast8_t& getTotalTracks() const;

        [[nodiscard]] [[maybe_unused]] const string& getMBID() const;

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
