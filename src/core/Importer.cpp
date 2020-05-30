#include "Importer.hpp"

using namespace MusicList;

Importer::Importer() = default;

void Importer::runTrackSearch(const fs::path& path)
{
    vector<fs::path> trackPaths;
    for (const auto& item : fs::recursive_directory_iterator(path))
    {
        if (fs::is_regular_file(item))
        {
            const string fileExt = item.path().extension().string();
            for (uint_fast8_t i = 0; i < NUM_EXTS; i++)
            {
                if (fileExt == SUPPORTED_EXTS[i])
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

    uint32_t processed = 0;
    for (; processed < totalTracks; processed++)
    {
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
        
        this->tracks.push_back(trackPtr);
        std::cout << "\33[2K\rImported " << std::to_string(this->tracks.size()) << " of " << std::to_string(totalTracks) << std::flush;
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

const Json::Value Importer::toJSON() const
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
