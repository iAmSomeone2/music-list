#include <cstring>
#include <array>

#include "BlockPicture.hpp"

using namespace MusicList;

BlockPicture::BlockPicture() = default;

BlockPicture::~BlockPicture()
{
    if (this->mimeType != nullptr)
    {
        delete[] this->mimeType;
    }

    if (this->description != nullptr)
    {
        delete[] this->description;
    }

    if (this->picData != nullptr)
    {
        delete[] this->picData;
    }
}

vector<int8_t> BlockPicture::base64Decode(const string& data)
{
    uint_fast32_t numChars = data.size();
    vector<int8_t> outVec;
    outVec.reserve(numChars);

    std::array<string,4> workSet;
    for (uint_fast32_t i = 0; i < numChars; i+=4)
    {
        std::string tmp = "";
        for (uint8_t j = 0; j < 4; j++)
        {
            workSet[j] = BASE64_TABLE[data[i+j]];

            if (workSet[j] != "")
            {
                workSet[j] = workSet[j].substr(2);
                tmp.append(workSet[j]);
            }
        }

        uint8_t tmpLen = tmp.length();
        
        for (uint8_t j = 0; j < 4; j++)
        {
            int8_t val;
            if (j*8 < tmpLen)
            {
                std::string byte = tmp.substr(8*j, 8);
                val = static_cast<int8_t>(strtol(byte.c_str(), nullptr, 2));
                outVec.push_back(val);
            }
        }
    }

    return outVec;
}

void BlockPicture::readPictureBlock(const string& data, bool b64Encoded)
{
    if (b64Encoded)
    {
        // Decode the base64 data.

    }
}
