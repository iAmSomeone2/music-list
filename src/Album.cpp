#include "Album.hpp"

using namespace MusicList;

Album::Album() = default;

Album::Album(const shared_ptr<Track>& track)
{
    this->addTrack(track);
}

const Json::Value Album::toJSON() const
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
    if (track->getAudioFormat() == AudioFormat::unknown)
    {
        throw unsupported_format_error(track->getPath());
    }

    if (this->tracks.empty())
    {
        // Initialize basic data since this is the first track to be added.
        this->totalTracks = track->getTotalTracks();
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