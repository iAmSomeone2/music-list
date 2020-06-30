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

#ifndef MUSICLIST_TRACK_HPP
#define MUSICLIST_TRACK_HPP

#include <filesystem>
#include <string>
#include <map>
#include <cinttypes>

#include <json/value.h>

extern "C"
{
#include <sqlite3.h>
};

namespace fs = std::filesystem;

using std::string;
using std::map;

namespace MusicList
{
    /**
     * \brief All file extensions that a Track may have.
     *
     * \details
     * An extension in this array allows the file to be recognised by the Track object, but it does not imply that the
     * Track is capable of processing it. The Track object will throw an exception when it can't read a specific file
     * structure.
     */
    static const string SUPPORTED_EXTS[] = {
        ".flac", ".opus", ".ogg", ".oga", ".mp3", ".m4a"
    };
    /**
     * \brief Length of SUPPORTED_EXTS array.
     */
    static const uint_fast8_t NUM_EXTS = 6;

//    struct unsupported_format_error : public std::exception
//    {
//        fs::path errPath;
//        explicit unsupported_format_error(const fs::path& filePath)
//        {
//            this->errPath = filePath;
//        }
//        [[nodiscard]] const char* what() const noexcept override
//        {
//            string outStr = "Unsupported audio format. File: ";
//            outStr.append(errPath.string());
//            const char* cStr = outStr.c_str();
//            return cStr;
//        }
//    };

    /**
     * \brief Possible track formats.
     */
    enum class AudioFormat : uint_fast8_t
    {
        unknown = 0,
        flac,
        ogg_flac,
        opus,
        aac,
        vorbis,
        mp3
    };

    class Track 
    {
    private:
        // Data info
        fs::path path; /**< Filesystem path to file associated with this Track */
        AudioFormat format = AudioFormat(); /**< Codec/container format of Track */

        // ==================
        // Metadata Retrieval
        // ==================

        /**
         * \brief Determines which audio format is held within an Ogg container.
         * 
         * \param path fs path to the track to check
         * 
         * \returns AudioFormat corresponding to the detected file type.
         */
        static AudioFormat determineOggAudioFormat(const fs::path& path);

        /**
         * \brief Adds the metadata pair to the object.
         *
         * \details
         * Special metadata such as artist name are also assigned to the appropriate instance variables.
         * 
         * \param key metadata entry key
         * \param value metadata entry value
         */
        void addMetadataPair(const string& key, const string& value);

        /**
         * \brief Handles parsing FLAC metadata into memory.
         */
        void readFlacMetadata();

        /**
         * \brief Handles parsing Opus metadata into memory.
         */
        void readOpusMetadata();
    protected:
        // Data info
        bool isLossless = false;

        // Music info
        uint_fast8_t trackNum = 0;
        uint_fast8_t totalTracks = 0;
        uint_fast8_t discNum = 0;
        uint_fast8_t totalDiscs = 0;
        uint_fast8_t artistCount = 0;
        string title = "";
        string artist = "";
        string album = "";
        string mbid = "";
        map<string,string> tags;
        
    public:
        /**
         * \brief Creates a new Track instance where the path is set to "./" by default.
         *
         * \details
         * The appropriate path must be set with Track::setPath().
         */
        Track();

        /**
         * \brief Creates a new Track instance using the provided path.
         *
         * \details
         * Track metadata is parsed at instance creation and can be immediately accessed.
         * 
         * \param path filesystem path to the track to use.
         */
        explicit Track(const fs::path& path);

        /**
         * \brief Sets the track search newPath for this Track instance.
         * 
         * \param newPath filesystem newPath to the track to use.
         */
        void setPath(const fs::path& newPath);

        // ==================
        // Metadata Retrieval
        // ==================

        /**
         * \brief Reads the track's metadata into memory.
         */
        void readMetadata();

        /**
         * \brief Saves this Track's data to the supplied SQLite database.
         *
         * \param dbConnection pointer to sqlite3 struct.
         */
        void saveToDB(const sqlite3* dbConnection);

        /**
         * \brief Determines the AudioFormat of the file at the provided path.
         *
         * \details
         * In the event that the file cannot be opened or the format isn't supported, AudioFormat::unknown is returned.
         * 
         * \param path fs path to the track to check
         * 
         * \returns AudioFormat corresponding to the detected file type.
         */
        static AudioFormat determineFormat(const fs::path& path);

        // ==========
        // Operations
        // ==========

        /**
         * \brief Creates a JSON value containing the object's data.
         */
        [[nodiscard]] Json::Value toJSON() const;

        /**
         * \brief Converts the first 4 bytes in the input array into an unsigned 32-bit int.
         * 
         * \param bytes character array containing at least 4 bytes of data.
         * 
         * \returns unsigned 32-bit int from the first 4 bytes in the input array.
         */
        static inline uint32_t toUInt32(const char* bytes);

        // =======
        // Getters
        // =======

        /**
         * \returns a const reference to the complete tag map for this Track instance.
         */
        [[nodiscard]] const map<string,string>& getTags() const;

        /**
         * \returns Track title.
         */
        [[nodiscard]] [[maybe_unused]] const string& getTitle() const;

        /**
         * \returns Track artist.
         */
        [[nodiscard]] [[maybe_unused]] const string& getArtist() const;

        /**
         * \returns Track album.
         */
        [[nodiscard]] [[maybe_unused]] const string& getAlbum() const;

        /**
         * \returns Track index in album.
         */
        [[nodiscard]] [[maybe_unused]] const uint_fast8_t& getTrackNum() const;

        /**
         * \returns total number of Tracks in album or collection.
         */
        [[nodiscard]] [[maybe_unused]] const uint_fast8_t& getTotalTracks() const;

        /**
         * \returns index of associated disc in collection or album.
         */
        [[nodiscard]] [[maybe_unused]] const uint_fast8_t& getDiscNum() const;

        /**
         * \returns total number of discs in collection or album.
         */
        [[nodiscard]] [[maybe_unused]] const uint_fast8_t& getTotalDiscs() const;

        /**
         * \returns AudioFormat associated with the Track;
         */
        [[nodiscard]] [[maybe_unused]] const AudioFormat& getAudioFormat() const;

        /**
         * \returns Filesystem path associated with the Track
         */
        [[nodiscard]] [[maybe_unused]] const fs::path& getPath() const;

        [[nodiscard]] [[maybe_unused]] const string& getMBID() const;

        // ==================
        // Operator Overloads
        // ==================

        /**
         * \brief Returns true if the left hand side is less than the right hand side.
         * 
         * \param lhs left-hand Track
         * \param rhs right-hand Track
         * 
         * \returns true if the left hand side is less than the right hand side.
         */
        friend inline bool operator< (const Track& lhs, const Track& rhs)
        {
            return std::tie(lhs.mbid) < std::tie(rhs.mbid);
        }

        friend inline bool operator> (const Track& lhs, const Track& rhs) { return rhs < lhs;}

        friend inline bool operator<= (const Track& lhs, const Track& rhs) { return !(lhs > rhs); }

        friend inline bool operator>= (const Track& lhs, const Track& rhs) { return !(lhs < rhs); }
    };
} // namespace MusicList

#endif // MUSICLIST_TRACK_HPP