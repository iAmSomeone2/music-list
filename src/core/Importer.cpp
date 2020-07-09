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

extern "C"
{
#include <unistd.h>
#include <pwd.h>
}

#include <string>
#include <fstream>

#include "Importer.hpp"

#include <config.hpp>

using namespace MusicList;

fs::path Importer::getUserDataDir()
{
    bool xdgFound = false;
    char *homeDataDir = getenv("XDG_DATA_HOME");
    if (homeDataDir == nullptr)
    {
        homeDataDir = getenv("HOME");
        if (homeDataDir == nullptr)
        {
            homeDataDir = getpwuid(getuid())->pw_dir;
        }
    }
    else
    {
        xdgFound = true;
    }

    fs::path dataPath = fs::path(homeDataDir);

    if (!xdgFound)
    {
        // Append the user data dir to the user's HOME path
        dataPath = dataPath.append(".local/share/");
    }

    dataPath = dataPath.append(PROJECT_NAME);

    return dataPath;
}

Importer::Importer()
{
    this->userDataDir = Importer::getUserDataDir();
    // Create app data dir if it doesn't already exist
    try
    {
        fs::create_directories(this->userDataDir);
    }
    catch (const fs::filesystem_error& fsErr)
    {
        std::cerr << "\n" << fsErr.what() << std::endl;
    }

    const fs::path dbPath = this->userDataDir.append(DB_NAME);
    sqlite3_open_v2(
            dbPath.c_str(), &this->dbConnection,
            SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE,
            nullptr
    );

    // Check that the tables have been defined. Run DDL if not.
    const std::string tableCheck = "SELECT name FROM sqlite_master WHERE type='table' AND name='artist';";
    sqlite3_stmt* stmt;
    int res = sqlite3_prepare_v2(this->dbConnection, tableCheck.c_str(), tableCheck.size(), &stmt, nullptr);
    res = sqlite3_step(stmt);
    // SQLITE_DONE should return if the 'artist' table doesn't exist.
    switch (res)
    {
        case SQLITE_DONE:
            sqlite3_finalize(stmt);
            // Run DDL since the tables don't exist.
            this->initDB();
            break;
        case SQLITE_ROW:
            sqlite3_finalize(stmt);
            break;
        default:
            std::cerr << "Unexpected SQLite result code: " << std::to_string(res) << "\n";
    }
}

Importer::~Importer()
{
    sqlite3_close_v2(this->dbConnection);
}

void Importer::initDB()
{
    std::clog << "Setting up database for first use...\n";

    fs::path dataDir = fs::path(DATA_DIR);
    const fs::path ddlScript = dataDir.append("db/db_init.ddl");
    // Load DDL into memory and execute it.
    std::ifstream ddlStream = std::ifstream(ddlScript, std::ios::ate | std::ios::in);
    if (!ddlStream.is_open())
    {
        throw std::runtime_error("Failed to open DDL script to init database.");
    }

    const uint64_t ddlSize = static_cast<uint64_t>(ddlStream.tellg());
    ddlStream.seekg(std::ios::beg);
    char* buff = new char[ddlSize+1];
    buff[ddlSize] = '\0';
    ddlStream.readsome(buff, ddlSize);
    ddlStream.close();

    char* errMsg;
    int res = sqlite3_exec(this->dbConnection, buff, nullptr, nullptr, &errMsg);
    delete [] buff;

    if (res == SQLITE_ABORT)
    {
        std::cerr << errMsg << "\n";
    }
    sqlite3_free(errMsg);
}

void Importer::runTrackSearch(const fs::path& path, const uint32_t& limit)
{
    std::cout << "Checking '" << path.string() << "' for audio files...\n";
    vector<fs::path> trackPaths;
    for (const auto& item : fs::recursive_directory_iterator(path))
    {
        if (fs::is_regular_file(item))
        {
            const string fileExt = item.path().extension().string();
            for (const auto & i : SUPPORTED_EXTS)
            {
                if (fileExt == i)
                {
                    trackPaths.push_back(item.path());
                }
            }
        }
    }

    uint32_t totalTracks = trackPaths.size();
    this->tracks.reserve(totalTracks);

    std::cout << "Discovered " << std::to_string(totalTracks) << " audio files in " << path.string() << ".\n";
    std::cout << "Processing files...\n";

    bool useLimit = false;

    if (limit > 0)
    {
        std::cout << "Limiting import to " << std::to_string(limit) << " files.";
        useLimit = true;
    }

    uint32_t processed = 0;
    for (; processed < totalTracks; processed++)
    {
        if (useLimit && processed >= limit)
        {
            break;
        }

        const fs::path& trackPath = trackPaths[processed];

        shared_ptr<Track> trackPtr = std::make_shared<Track>(Track());

        try
        {
            trackPtr->setPath(trackPath);
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
            continue;
        }

        try
        {
            trackPtr->readMetadata();
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
            continue;
        }

        // Track should be added to sqlite DB at this point.

        this->tracks.push_back(trackPtr);
        std::cout << "\33[2K\rImported " << std::to_string(this->tracks.size()) << " of "
            << std::to_string(totalTracks) << std::flush;
    }
    std::cout << std::endl;
}

void Importer::generateAlbumsFromTracks()
{
    while (!this->tracks.empty())
    {
        const auto& track = this->tracks.back();

        auto trackTags = track->getTags();
        const auto& albumID = trackTags["MUSICBRAINZ_ALBUMID"];
        shared_ptr<Album> albumPtr = this->albums[albumID];

        if (albumPtr == nullptr)
        {
            albumPtr = std::make_shared<Album>(Album());
        }
        
        albumPtr->addTrack(track);
        this->albums[albumID] = albumPtr;

        this->tracks.pop_back();
        std::cout << "\33[2K\rGenerated " << std::to_string(this->albums.size()) << " albums." << std::flush;
    }
    std::cout << std::endl;
}

void Importer::updateDB()
{
    for (const auto& albumPair : this->albums)
    {
        albumPair.second->addToDB(this->dbConnection);
    }
}

Json::Value Importer::toJSON() const
{
    Json::Value root;
    uint32_t count = 0;

    for (const auto& albumPair : this->albums)
    {
        root[count] = albumPair.second->toJSON();
        count++;
    }

    return root;
}

const vector<shared_ptr<Track>>& Importer::getTracks() const
{
    return this->tracks;
}

const map<string,shared_ptr<Album>>& Importer::getAlbums() const
{
    return this->albums;
}
