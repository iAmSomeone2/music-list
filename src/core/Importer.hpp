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

        /**
         * @brief Performs a search and import for supported files in the specified directory.
         * 
         * @param path directory to import from
         */
        void runTrackSearch(const fs::path& path);

        /**
         * @brief Organizes tracks into their respective albums.
         * 
         * Running this method after generating a track list allows the user to export the
         * data using the toJSON() method.
         */
        void generateAlbumsFromTracks();

        /**
         * @brief Generates a Json Value containing an array with all imported albums.
         * 
         * @returns Json::Value containing albums array
         */
        const Json::Value toJSON() const;

        /**
         * @returns a reference to the tracks vector.
         */
        const vector<shared_ptr<Track>>& getTracks() const;

        /**
         * @returns a reference to the albums map.
         */
        const map<string,shared_ptr<Album>>& getAlbums() const;
    };
} // namespace MusicList

#endif // MUSICLIST_IMPORTER_HPP
