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

#include "Track.hpp"
#include "Album.hpp"

using std::map;
using std::vector;
using std::unique_ptr;
using std::shared_ptr;

namespace fs = std::filesystem;

namespace MusicList
{
    class Importer
    {
    private:
        map<string,shared_ptr<Album>> albums;
        vector<shared_ptr<Track>> tracks;
    public:
        Importer();

        void runTrackSearch(const fs::path& path);

        Json::Value toJSON();

        const vector<shared_ptr<Track>>& getTracks();
    };
} // namespace MusicList

#endif // MUSICLIST_IMPORTER_HPP
