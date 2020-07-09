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

#include <stdexcept>

#include "Album.hpp"

using namespace MusicList;

Album::Album() = default;

Album::Album(const shared_ptr<Track>& track)
{
    this->addTrack(track);
}

Json::Value Album::toJSON() const
{
    Json::Value root;

    root["name"] = this->name;
    root["artist"] = this->artist;
    root["total_tracks"] = this->totalTracks;
    root["musicbrainz_id"] = this->mbid;
    root["tracks"] = Json::Value();
    for (const auto& track : this->tracks)
    {
        root["tracks"].append(track.second->toJSON());
    }

    return root;
}

void Album::addTrack(const shared_ptr<Track>& track)
{
    std::ostringstream errStr;

    if (track->getAudioFormat() == AudioFormat::unknown)
    {
        errStr << "Unsupported audio format: " << track->getPath();
        throw std::runtime_error(errStr.str());
    }

    // Consider grabbing the album artwork when reading in the first track.
    if (this->tracks.empty())
    {
        // Initialize basic data since this is the first track to be added.
        this->totalTracks = track->getTotalTracks();
        this->totalDiscs = track->getTotalDiscs();
        this->name = track->getAlbum();
        this->artist = track->getArtist();
        map<string,string> trackTags = track->getTags();
        this->mbid = trackTags["MUSICBRAINZ_ALBUMID"];
    }

    if (this->tracks.count(track->getMBID()) == 0)
    {
        this->tracks[track->getMBID()] = track;
    }
}

void Album::addToDB(sqlite3 *dbConnection)
{
    std::ostringstream errStr;

    sqlite3_stmt* sqlStmt;
    int res = SQLITE_OK;

    // Add album to DB if it doesn't already exist.
    const char insertAlbum[] = "INSERT INTO album ("
                               "album_name, album_mbid, num_discs, num_tracks"
                               ") VALUES (?, ?, ?, ?)";

    sqlite3_prepare_v2(dbConnection, insertAlbum, 159, &sqlStmt, nullptr);
    sqlite3_bind_text(sqlStmt, 1, this->name.c_str(), this->name.size(), SQLITE_TRANSIENT);
    sqlite3_bind_text(sqlStmt, 2, this->mbid.c_str(), this->mbid.size(), SQLITE_TRANSIENT);
    sqlite3_bind_int(sqlStmt, 3, this->totalDiscs);
    sqlite3_bind_int(sqlStmt, 4, this->totalTracks);

    res = sqlite3_step(sqlStmt);
    sqlite3_finalize(sqlStmt);
    if (res != SQLITE_DONE)
    {
        errStr << "Failed to insert album '" << this->name
               << "' into database. SQLite error: " << std::to_string(res);
        throw std::runtime_error(errStr.str());
    }

    
}

// =======
// Getters
// =======

const string& Album::getName() const
{
    return this->name;
}

const string& Album::getArtist() const
{
    return this->artist;
}

const map<string,shared_ptr<Track>>& Album::getTrackSet() const
{
    return this->tracks;
}

const uint_fast8_t& Album::getTotalTracks() const
{
    return this->totalTracks;
}

const string& Album::getMBID() const
{
    return this->mbid;
}
