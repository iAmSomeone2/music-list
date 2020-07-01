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

#include <fstream>
#include <memory>
#include <vector>
#include <cstring>
#include <iostream>

#include <FLAC/format.h>
#include <FLAC/metadata.h>

#include <opus/opusfile.h>

#include "Track.hpp"

using namespace MusicList;

using std::unique_ptr;

// ===============
// Instance Set Up
// ===============

Track::Track()
{
    this->path = "./";
}

Track::Track(const fs::path &path)
{
    try
    {
        this->setPath(path);
        this->readMetadata();
    }
    catch (const std::runtime_error &e)
    {
        std::cerr << e.what() << '\n';
    }
}

void Track::setPath(const fs::path &newPath)
{
    this->path = newPath;
    try
    {
        this->format = Track::determineFormat(this->path);
    }
    catch (const std::runtime_error &err)
    {
        throw err;
    }

    switch (this->format)
    {
        case AudioFormat::flac:
            this->isLossless = true;
            break;
        case AudioFormat::ogg_flac:
            this->isLossless = true;
            break;
        default:
            this->isLossless = false;
    }
}

AudioFormat Track::determineOggAudioFormat(const fs::path &path)
{
    // Don't check the extension since this is a private method, and we should already know that it's .ogg

    std::ifstream trackFile = std::ifstream(path, std::ios::binary);
    if (!trackFile.is_open())
    {
        throw std::runtime_error("Failed to open Ogg file to determine audio format.");
    }

    trackFile.seekg(std::ios::beg + 26);
    char segTableSize = 0;
    trackFile.read(&segTableSize, 1);
    std::streampos readPos = trackFile.tellg() + static_cast<int64_t>(segTableSize);
    trackFile.seekg(std::ios::beg + readPos);

    char buff[9];
    buff[8] = 0;

    trackFile.read(buff, 8);

    trackFile.close();

    if (strncmp("OpusHead", buff, 8) == 0)
    {
        return AudioFormat::opus;
    }
    else if (strncmp("vorbis", buff + 1, 6) == 0)
    {
        return AudioFormat::vorbis;
    }
    else if (strncmp("FLAC", buff + 1, 4) == 0)
    {
        return AudioFormat::ogg_flac;
    }

    return AudioFormat::unknown;
}

AudioFormat Track::determineFormat(const fs::path &path)
{
    std::ostringstream errStr;

    string fileExt = path.extension();

    AudioFormat format = AudioFormat::unknown;

    char* buff = new char[5];

    if (fileExt == ".flac")
    {
        std::ifstream trackFile = std::ifstream(path, std::ios::binary);

        if (!trackFile.is_open())
        {
            errStr << "Failed to open file to determine audio format: " << path.string() << ".";
            delete [] buff;
            throw std::runtime_error(errStr.str());
        }

        // Check for flac audio data.
        buff[4] = 0;

        trackFile.readsome(buff, 4);
        if (strncmp(buff, "fLaC", 4) == 0)
        {
            format = AudioFormat::flac;
        }

        trackFile.close();
    }
    else if (fileExt == ".ogg" || fileExt == ".oga" || fileExt == ".opus")
    {
        format = Track::determineOggAudioFormat(path);
    }

    delete [] buff;
    return format;
}

// ==================
// Metadata Retrieval
// ==================

void Track::readMetadata()
{
    std::ostringstream  errStr;

    switch (this->format)
    {
    case AudioFormat::flac:
        this->readFlacMetadata();
        break;
    case AudioFormat::opus:
        try
        {
            this->readOpusMetadata();
        }
        catch(const std::exception& e)
        {
            std::cerr << "\n" << e.what() << '\n';
            std::cerr << "File: " << this->path.string() << "\n";
        }
        break;
    default:
        errStr << "Unknown audio format type: " << this->path;
        throw std::runtime_error(errStr.str());
    }
}

void Track::addMetadataPair(const string &key, const string &value)
{
    string tmpKey = key;
    if (tmpKey == "ARTIST")
    {
        tmpKey = tmpKey.append(std::to_string(this->artistCount));
        this->artistCount++;
    }
    else if (tmpKey == "TRACKNUMBER")
    {
        this->trackNum = strtoul(value.c_str(), nullptr, 10);
    }
    else if (tmpKey == "TOTALTRACKS")
    {
        this->totalTracks = strtoul(value.c_str(), nullptr, 10);
    }
    else if (tmpKey == "DISCNUMBER")
    {
        this->discNum = strtoul(value.c_str(), nullptr, 10);
    }
    else if (tmpKey == "TOTALDISCS")
    {
        this->totalDiscs = strtoul(value.c_str(), nullptr, 10);
    }
    else if (tmpKey == "MUSICBRAINZ_TRACKID")
    {
        this->mbid = value;
    }
    else if (tmpKey != "METADATA_BLOCK_PICTURE")
    {
        // Skip the picture. There's no need to store it in memory
        this->tags[tmpKey] = value;
    }
}

void Track::readFlacMetadata()
{
    FLAC__StreamMetadata *streamMetadata = FLAC__metadata_object_new(FLAC__METADATA_TYPE_VORBIS_COMMENT);

    bool success = static_cast<bool>(FLAC__metadata_get_tags(this->path.c_str(), &streamMetadata));

    if (!success)
    {
        FLAC__metadata_object_delete(streamMetadata);
        throw std::runtime_error("Failed to read metadata from FLAC file.");
    }

    FLAC__StreamMetadata_VorbisComment vorbisComment = streamMetadata->data.vorbis_comment;
    for (uint32_t i = 0; i < vorbisComment.num_comments; i++)
    {
        string fullEntry = reinterpret_cast<char *>(vorbisComment.comments[i].entry);
        auto splitLoc = fullEntry.find('=');

        string key = fullEntry.substr(0, splitLoc);
        string value = fullEntry.substr(splitLoc + 1);

        this->addMetadataPair(key, value);
    }

    /*
     * The FLAC lib has about a 12k/object memory leak that occurs when running FLAC__metadata_object_delete. Nothing
     * can be done about it on this end.
     */
    FLAC__metadata_object_delete(streamMetadata);

    this->artist = this->tags["ALBUMARTIST"];
    this->album = this->tags["ALBUM"];
    this->title = this->tags["TITLE"];
}

void Track::readOpusMetadata()
{
    int errCode;
    OggOpusFile* opusFile = op_open_file(this->path.c_str(), &errCode);

    if (errCode == OP_EFAULT)
    {
        throw std::runtime_error("Failed to open Opus File.");
    }

    const OpusTags* opTags = op_tags(opusFile, -1);

    for (uint32_t i = 0; i < static_cast<uint32_t>(opTags->comments); i++)
    {
        string fullEntry = opTags->user_comments[i];
        auto splitLoc = fullEntry.find('=');

        string key = fullEntry.substr(0, splitLoc);
        string value = fullEntry.substr(splitLoc + 1);

        this->addMetadataPair(key, value);
    }

    //opus_tags_clear(opTags);
    op_free(opusFile);

    this->artist = this->tags["ALBUMARTIST"];
    this->album = this->tags["ALBUM"];
    this->title = this->tags["TITLE"];
}

// ==========
// Operations
// ==========

Json::Value Track::toJSON() const
{
    Json::Value root;

    root["track_num"] = this->trackNum;
    root["total_tracks"] = this->totalTracks;
    root["disc_num"] = this->discNum;
    root["total_discs"] = this->totalDiscs;
    root["title"] = this->title;
    root["artist"] = this->artist;
    root["album"] = this->album;
    root["is_lossless"] = this->isLossless;
    root["musicbrainz_id"] = this->mbid;

    string formatStr;
    switch (this->format)
    {
    case AudioFormat::aac:
        formatStr = "AAC";
        break;
    case AudioFormat::flac:
        formatStr = "FLAC";
        break;
    case AudioFormat::mp3:
        formatStr = "MP3";
        break;
    case AudioFormat::ogg_flac:
        formatStr = "FLAC";
        break;
    case AudioFormat::opus:
        formatStr = "Opus";
        break;
    case AudioFormat::vorbis:
        formatStr = "Vorbis";
        break;
    default:
        formatStr = "unkown";
    }

    root["format"] = formatStr;

    return root;
}

void Track::saveToDB(sqlite3 *dbConnection)
{
    std::ostringstream errStr;

    int sqlExecResult = SQLITE_OK;
    sqlite3_stmt* sqlStmt;
    // Check to see if track already exists.
    std::string findExisting = "SELECT * FROM track WHERE path == ?;";
    sqlite3_prepare_v2(dbConnection, findExisting.c_str(), findExisting.size(), &sqlStmt, nullptr);
    sqlite3_bind_text(sqlStmt, 1, this->path.c_str(), this->path.string().size(), SQLITE_TRANSIENT);
    sqlExecResult = sqlite3_step(sqlStmt);
    // Run statement and get data if it returns. In the event that it does, compare it with the current Track.
    // Don't forget to finalize the statement when done.
}

inline uint32_t Track::toUInt32(const char *bytes)
{
    return (bytes[3] << 24) | (bytes[2] << 16) | (bytes[1] << 8) | bytes[0];
}

// =======
// Getters
// =======

const map<string, string> &Track::getTags() const
{
    return this->tags;
}

const string &Track::getTitle() const
{
    return this->title;
}

const string &Track::getArtist() const
{
    return this->artist;
}

const string &Track::getAlbum() const
{
    return this->album;
}

const uint_fast8_t &Track::getTrackNum() const
{
    return this->trackNum;
}

const uint_fast8_t &Track::getTotalTracks() const
{
    return this->totalTracks;
}

const uint_fast8_t &Track::getDiscNum() const
{
    return this->discNum;
}

const uint_fast8_t &Track::getTotalDiscs() const
{
    return this->totalDiscs;
}

const AudioFormat &Track::getAudioFormat() const
{
    return this->format;
}

const fs::path &Track::getPath() const
{
    return this->path;
}

const string &Track::getMBID() const
{
    return this->mbid;
}
