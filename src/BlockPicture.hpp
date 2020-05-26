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

    static map<char,uint8_t> BASE64_TABLE = {
        {'A',0},  {'B',1},  {'C',2},  {'D',3},  {'E',4},  {'F',5},  {'G',6},  {'H',7},
        {'I',8},  {'J',9},  {'K',10}, {'L',11}, {'M',12}, {'N',13}, {'O',14}, {'P',15},
        {'Q',16}, {'R',17}, {'S',18}, {'T',19}, {'U',20}, {'V',21}, {'W',22}, {'X',23},
        {'Y',24}, {'Z',25}, {'a',26}, {'b',27}, {'c',28}, {'d',29}, {'e',30}, {'f',31},
        {'g',32}, {'h',33}, {'i',34}, {'j',35}, {'k',36}, {'l',37}, {'m',38}, {'n',39},
        {'o',40}, {'p',41}, {'q',42}, {'r',43}, {'s',44}, {'t',45}, {'u',46}, {'v',47},
        {'w',48}, {'x',49}, {'y',50}, {'z',51}, {'0',52}, {'1',53}, {'2',54}, {'3',55},
        {'4',56}, {'5',57}, {'6',58}, {'7',59}, {'8',60}, {'9',61}, {'+',62}, {'/',63},
        {'=',00}
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
