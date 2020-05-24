#include "Album.hpp"

using namespace MusicList;

Album::Album() = default;

Album::Album(const Track& track)
{
    this->addTrack(track);
}

const Json::Value Album::toJSON() const
{
    Json::Value root;

    root["name"] = this->name;
    root["artist"] = this->artist;
    root["total_tracks"] = this->totalTracks;
    root["tracks"] = Json::Value();
    for (const auto& track : this->tracks)
    {
        root["tracks"].append(track.toJSON());
    }

    return root;
}

bool Album::addTrack(const Track& track)
{
    if (this->tracks.empty())
    {
        // Initialize basic data since this is the first track to be added.
        this->totalTracks = track.getTotalTracks();
        this->name = track.getAlbum();
        this->artist = track.getArtist();
    }

    auto result = this->tracks.insert(track);

    return result.second;
}

const string& Album::getName() const
{
    return this->name;
}

const string& Album::getArtist() const
{
    return this->artist;
}

const set<Track>& Album::getTrackSet() const
{
    return this->tracks;
}

const uint_fast8_t& Album::getTotalTracks() const
{
    return this->totalTracks;
}