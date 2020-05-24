#include <filesystem>
#include <iostream>
#include <string>
#include <map>
#include <vector>
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

void runAlbumSearch(const fs::path& path, map<string,Album>& albums)
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

    uint_fast32_t totalTracks = trackPaths.size();

    std::cout << "Discovered " << std::to_string(totalTracks) << " audio files in " << path.string() << ".\n";
    std::cout << "Processing files...\n";

    uint_fast32_t totalProcessed = 0;
    for (totalProcessed = 0; totalProcessed < totalTracks; totalProcessed++)
    {
        fs::path trackPath = trackPaths[totalProcessed];
        
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
        

        string trackMBID = track.getMBID();
        map<string,string> trackTags = track.getTags();
        string albumMBID = trackTags["MUSICBRAINZ_ALBUMID"];

        if (albumMBID == "")
        {
            continue;
        }

        // Determine whether an Album for this track is already present.
        // Create a new one if not.
        Album album = albums.count(albumMBID) == 0 ? Album() : albums[albumMBID];
        
        album.addTrack(track);
        albums[albumMBID] = album;

        std::cout << "\33[2K\rProcessed " << std::to_string(totalProcessed) << " of " << std::to_string(totalTracks) << " files." << std::flush;

        if (totalProcessed >= 2239)
        {
            std::cerr << "\nCurrent file: " << trackPath.string() << std::endl;
        }
    }

    std::cout << "\nGenerated " << std::to_string(albums.size()) << " albums.\n";
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
    map<string,Album> albums;

    runAlbumSearch(searchPath, albums);

    return EXIT_SUCCESS;
}
