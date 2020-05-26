#ifndef MUSICLIST_BLOCKPICTURE_HPP
#define MUSICLIST_BLOCKPICTURE_HPP

#include <cinttypes>
#include <memory>
#include <vector>
#include <map>
#include <string>

using std::shared_ptr;
using std::unique_ptr;
using std::map;
using std::vector;
using std::string;

namespace MusicList
{
    /**
     * Possible metadata picture types.
     */
    enum class PictureType : uint_fast8_t
    {
        other = 0,
        file_icon,
        other_file_icon,
        front_cover,
        back_cover,
        leaflet,
        media,
        lead_artist,
        artist,
        conductor,
        band,
        composer,
        lyricist,
        recording_location,
        during_recording,
        during_performance,
        screen_capture,
        fish,
        illustration,
        band_logotype,
        publisher_logotype
    };

    static map<const char,const char*> BASE64_TABLE = {
        {'A',"00000000"}, {'B',"00000001"}, {'C',"00000010"}, {'D',"00000011"}, {'E',"00000100"}, {'F',"00000101"}, {'G',"00000110"}, {'H',"00000111"},
        {'I',"00001000"}, {'J',"00001001"}, {'K',"00001010"}, {'L',"00001011"}, {'M',"00001100"}, {'N',"00001101"}, {'O',"00001110"}, {'P',"00001111"},
        {'Q',"00010000"}, {'R',"00010001"}, {'S',"00010010"}, {'T',"00010011"}, {'U',"00010100"}, {'V',"00010101"}, {'W',"00010110"}, {'X',"00010111"},
        {'Y',"00011000"}, {'Z',"00011001"}, {'a',"00011010"}, {'b',"00011011"}, {'c',"00011100"}, {'d',"00011101"}, {'e',"00011110"}, {'f',"00011111"},
        {'g',"00100000"}, {'h',"00100001"}, {'i',"00100010"}, {'j',"00100011"}, {'k',"00100100"}, {'l',"00100101"}, {'m',"00100110"}, {'n',"00100111"},
        {'o',"00101000"}, {'p',"00101001"}, {'q',"00101010"}, {'r',"00101011"}, {'s',"00101100"}, {'t',"00101101"}, {'u',"00101110"}, {'v',"00101111"},
        {'w',"00110000"}, {'x',"00110001"}, {'y',"00110010"}, {'z',"00110011"}, {'0',"00110100"}, {'1',"00110101"}, {'2',"00110110"}, {'3',"00110111"},
        {'4',"00111000"}, {'5',"00111001"}, {'6',"00111010"}, {'7',"00111011"}, {'8',"00111100"}, {'9',"00111101"}, {'+',"00111110"}, {'/',"00111111"},
        {'=',""}
    };

    class BlockPicture
    {
    private:
        PictureType     type        = PictureType::fish;
        char*           mimeType    = nullptr;
        char*           description = nullptr;
        uint_fast32_t   width       = 0U;
        uint_fast32_t   height      = 0U;
        uint_fast32_t   colorDepth  = 0U;
        uint_fast32_t   numColors   = 0U;
        uint_fast32_t   numBytes    = 0U;
        int_fast8_t*    picData     = nullptr;

    protected:
    public:
        BlockPicture();

        ~BlockPicture();

        void readPictureBlock(const string& data, bool b64Encoded);

        static vector<int8_t> base64Decode(const string& data);
    };
};

#endif
