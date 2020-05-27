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
    catch (const unsupported_format_error &e)
    {
        std::cerr << e.what() << '\n';
    }
}

void Track::setPath(const fs::path &path)
{
    this->path = path;
    try
    {
        this->format = this->determineFormat(this->path);
    }
    catch (const unsupported_format_error &err)
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
    string fileExt = path.extension();

    AudioFormat format = AudioFormat::unknown;

    unique_ptr<char> buff = std::make_unique<char>();

    if (fileExt == ".flac")
    {
        std::ifstream trackFile = std::ifstream(path, std::ios::binary);

        if (!trackFile.is_open())
        {
            throw std::runtime_error("Failed to open file to determine audio format.");
        }

        // Check for flac audio data.
        buff.reset(new char[5]);
        buff.get()[4] = 0;

        trackFile.readsome(buff.get(), 4);
        if (strncmp(buff.get(), "fLaC", 4) == 0)
        {
            format = AudioFormat::flac;
        }

        trackFile.close();
    }
    else if (fileExt == ".ogg" || fileExt == ".oga" || fileExt == ".opus")
    {
        format = Track::determineOggAudioFormat(path);
    }

    return format;
}

// ==================
// Metadata Retrieval
// ==================

void Track::readMetadata()
{
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
        throw unsupported_format_error(this->path);
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

    this->tags[tmpKey] = value;
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

    FLAC__metadata_object_delete(streamMetadata);

    this->artist = this->tags["ARTIST0"];
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

    const OpusTags* tags = op_tags(opusFile, -1);

    for (uint32_t i = 0; i < tags->comments; i++)
    {
        string fullEntry = tags->user_comments[i];
        auto splitLoc = fullEntry.find('=');

        string key = fullEntry.substr(0, splitLoc);
        string value = fullEntry.substr(splitLoc + 1);

        this->addMetadataPair(key, value);
    }

    //opus_tags_clear(tags);
    op_free(opusFile);

    this->artist = this->tags["ARTIST0"];
    this->album = this->tags["ALBUM"];
    this->title = this->tags["TITLE"];
}

// ==========
// Operations
// ==========

const Json::Value Track::toJSON() const
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
