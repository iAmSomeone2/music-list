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
#ifndef MUSICLIST_IMPORTER_HPP
#define MUSICLIST_IMPORTER_HPP

#include <filesystem>
#include <iostream>
#include <cinttypes>
#include <memory>
#include <vector>
#include <map>

#include <json/value.h>
#include <json/writer.h>
#include <sqlite3.h>

#include "Track.hpp"
#include "Album.hpp"

using std::map;
using std::vector;
using std::unique_ptr;
using std::shared_ptr;

namespace fs = std::filesystem;

namespace MusicList
{
    static const char DB_NAME[] = "mellophone_library.sqlite";

    class Importer
    {
    private:
        sqlite3* dbConnection;
        fs::path userDataDir;

        map<string,shared_ptr<Album>> albums;
        vector<shared_ptr<Track>> tracks;

    protected:
        /**
         * \brief Uses environment variables to get current user's app data directory
         * \return path to user's app data directory
         */
        static fs::path getUserDataDir();

        /**
         * \brief Sets up the database in the event that one or all of the tables are missing.
         */
        void initDB();
    public:
        /**
         * \brief Creates a new Importer instance
         *
         * \details
         * Upon creation, the Importer instance locates a preexisting SQLite DB in the program's user-specific data dir
         * or creates a new SQLite DB in that location if one is not found.
         */
        Importer();

        /**
         * \brief Releases connection to SQLite DB on destruction.
         */
        ~Importer();

        /**
         * \brief Performs a search and import for supported files in the specified directory.
         * 
         * \param path directory to import from
         */
        void runTrackSearch(const fs::path& path, const uint32_t& limit);

        /**
         * \brief Organizes tracks into their respective albums.
         * 
         * Running this method after generating a track list allows the user to export the
         * data using the toJSON() method.
         */
        void generateAlbumsFromTracks();

        /**
         * \brief Generates a Json Value containing an array with all imported albums.
         * 
         * \returns Json::Value containing albums array
         */
        [[nodiscard]] [[maybe_unused]] Json::Value toJSON() const;

        /**
         * \returns a reference to the tracks vector.
         */
        [[nodiscard]] [[maybe_unused]] const vector<shared_ptr<Track>>& getTracks() const;

        /**
         * \returns a reference to the albums map.
         */
        [[nodiscard]] [[maybe_unused]] const map<string,shared_ptr<Album>>& getAlbums() const;
    };
} // namespace MusicList

#endif // MUSICLIST_IMPORTER_HPP
