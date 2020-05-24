#ifndef MUSICLIST_TRACK_HPP
#define MUSICLIST_TRACK_HPP

#include <filesystem>
#include <string>
#include <map>
#include <cinttypes>
#include <memory>

#include <json/value.h>

namespace fs = std::filesystem;

using std::string;
using std::map;
using std::unique_ptr;
using std::shared_ptr;

namespace MusicList
{
    struct unsupported_format_error : public std::exception
    {
        const char* what() const throw()
        {
            return "Unsupported audio format.";
        }
    };

    /**
     * Possbile track formats.
     */
    enum class AudioFormat 
    {
        flac,
        ogg_flac,
        opus,
        aac,
        vorbis,
        mp3,
        unknown
    }; 

    class Track 
    {
    private:
        // Data info
        fs::path path;
        AudioFormat format = AudioFormat();

        // ==================
        // Metadata Retrieval
        // ==================

        /**
         * Determines which audio format is held within an Ogg container.
         * 
         * @param path fs path to the track to check
         * 
         * @returns AudioFormat corresponding to the detected file type.
         */
        static AudioFormat determineOggAudioFormat(const fs::path& path);

        /**
         * @brief Adds the metadata pair to the object.
         * 
         * Special metadata such as artist name are also assigned to the appropriate instance variables.
         * 
         * @param key metadata entry key
         * @param value metadata entry value
         */
        void addMetadataPair(const string& key, const string& value);

        /**
         * @brief Handles parsing FLAC metadata into memory.
         */
        void readFlacMetadata();

        /**
         * @brief Handles parsing Opus metadata into memory.
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
        map<string,string> tags;
    public:
        /**
         * @brief Creates a new Track instance where the path is set to "./" by default.
         * 
         * The appropriate path must be set with Track::setPath().
         */
        Track();

        /**
         * @brief Creates a new Track instance using the provided path.
         * 
         * Only the path variable is populated using this constructor. Other variables must be set or calculated as usual.
         * 
         * @param path filesystem path to the track to use.
         */
        explicit Track(const fs::path& path);

        /**
         * @brief Sets the track search path for this Track instance.
         * 
         * @param path filesystem path to the track to use.
         */
        inline void setPath(const fs::path& path);

        // ==================
        // Metadata Retrieval
        // ==================

        /**
         * @brief Reads the track's metadata into memory.
         */
        void readMetadata();

        /**
         * @brief Determines the AudioFormat of the file at the provided path.
         * 
         * In the event that the file cannot be opened or the format isn't supported, AudioFormat::unknown is returned.
         * 
         * @param path fs path to the track to check
         * 
         * @returns AudioFormat corresponding to the detected file type.
         */
        static AudioFormat determineFormat(const fs::path& path);

        // ==========
        // Operations
        // ==========

        /**
         * @brief Creates a JSON value containing the object's data.
         */
        const Json::Value toJSON() const;

        /**
         * @brief Converts the first 4 bytes in the input array into an unsigned 32-bit int.
         * 
         * @param bytes character array containing at least 4 bytes of data.
         * 
         * @returns unsigned 32-bit int from the first 4 bytes in the input array.
         */
        static inline uint_fast32_t toUInt32(const char* bytes);

        // =======
        // Getters
        // =======

        /**
         * @returns a const reference to the complete tag map for this Track instance.
         */
        const map<string,string>& getTags() const;

        /**
         * @returns Track title.
         */
        const string& getTitle() const;

        /**
         * @returns Track artist.
         */
        const string& getArtist() const;

        /**
         * @returns Track album.
         */
        const string& getAlbum() const;

        /**
         * @returns Track index in album.
         */
        const uint_fast8_t& getTrackNum() const;

        /**
         * @returns total number of Tracks in album or collection.
         */
        const uint_fast8_t& getTotalTracks() const;

        /**
         * @returns index of associated disc in collection or album.
         */
        const uint_fast8_t& getDiscNum() const;

        /**
         * @returns total number of discs in collection or album.
         */
        const uint_fast8_t& getTotalDiscs() const;

        /**
         * @returns AudioFormat associated with the Track;
         */
        const AudioFormat& getAudioFormat() const;

        // ==================
        // Operator Overloads
        // ==================

        /**
         * @brief Returns true if the left hand side is less than the right hand side.
         * 
         * @param lhs left-hand Track
         * @param rhs right-hand Track
         * 
         * @returns true if the left hand side is less than the right hand side.
         */
        friend inline bool operator< (const Track& lhs, const Track& rhs)
        {
            return std::tie(lhs.trackNum, lhs.discNum) < std::tie(rhs.trackNum, rhs.discNum);
        }

        friend inline bool operator> (const Track& lhs, const Track& rhs) { return rhs < lhs;}

        friend inline bool operator<= (const Track& lhs, const Track& rhs) { return !(lhs > rhs); }

        friend inline bool operator>= (const Track& lhs, const Track& rhs) { return !(lhs < rhs); }
    };
} // namespace MusicList

#endif // MUSICLIST_TRACK_HPP