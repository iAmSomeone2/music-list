#include <filesystem>
#include <iostream>
#include <string>
#include <map>
#include <cinttypes>
#include <cctype>

#include <json/value.h>
#include <json/writer.h>

#include "Track.hpp"
#include "Album.hpp"

using namespace MusicList;

namespace fs = std::filesystem;

using std::string;
using std::vector;
using std::map;
using std::set;

map<string,Album*> runAlbumSearch(const fs::path& path)
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
                    break;
                }
            }
        }
    }

    uint32_t totalTracks = trackPaths.size();
    vector<Track> allTracks;
    allTracks.reserve(totalTracks);

    std::cout << "Discovered " << std::to_string(totalTracks) << " audio files in " << path.string() << ".\n";
    std::cout << "Processing files...\n";

    uint32_t totalProcessed = 0;
    for (totalProcessed = 0; totalProcessed < totalTracks; totalProcessed++)
    {
        std::cout << "\33[2K\rProcessing file " << std::to_string(totalProcessed) << " of " << std::to_string(totalTracks) << "..." << std::flush;
        fs::path trackPath = trackPaths[totalProcessed];

        //std::cout << "\33[2K\rImporting " << trackPath.string() << std::flush; 
        
        Track track = Track();

        try
        {
            track.setPath(trackPath);
        } 
        catch(const unsupported_format_error& err)
        {
            std::cerr << err.what() << std::endl;
            continue;
        }

        try
        {
            track.readMetadata();
        }
        catch(const unsupported_format_error& err)
        {
            std::cerr << err.what() << std::endl;
            continue;
        }

        allTracks.push_back(track);
    }

    std::cout << "\33[2K\rProcessing file " << std::to_string(totalProcessed) << " of " << std::to_string(totalTracks) << "..." << std::flush;
    std::cout << std::endl;
    std::cout << "Generating albums...\n";

    map<string,Album*> albums;
    for (const auto& track : allTracks)
    {
        auto trackTags = track.getTags();
        const auto& albumID = trackTags["MUSICBRAINZ_ALBUMID"];
        Album* workAlbum = albums.count(albumID) ? albums[albumID] : new Album();

        workAlbum->addTrack(track);
        albums[albumID] = workAlbum;

        std::cout << "\33[2K\rTrack, " << track.getTitle() << ", added to " << workAlbum->getName() << "." << std::flush; 
    }

    std::cout << std::endl;
    std::cout << "Generated " << std::to_string(albums.size()) << " albums.\n";

    return albums;
}

int main(int argc, char* argv[])
{
    fs::path jsonOut = fs::path("./musiclist.json");

    if (argc < 2)
    {
        std::cerr << "Search directory must be provided." << std::endl;
        return EXIT_FAILURE;
    }
    else if (argc >= 3)
    {
        jsonOut = fs::path(argv[2]);

        if (fs::is_directory(jsonOut))
        {
            jsonOut.append("musiclist.json");
        }
    }

    const fs::path searchPath = fs::path(argv[1]);

    std::cout << "Searching through: " << searchPath.string() << "\n";

    // Search through the directory.
    auto albums = runAlbumSearch(searchPath);

    for (const auto& albumPair : albums)
    {
        delete albumPair.second;
    }

    return EXIT_SUCCESS;
}
