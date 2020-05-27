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

uint32_t BlockPicture::toUint32(const int8_t* data)
{
    return static_cast<uint32_t>((data[3] << 24) | (data[2] << 16) | (data[1] << 8) | data[0]);
}

void BlockPicture::readPictureBlock(const string& data)
{
    vector<int8_t> blockData = BlockPicture::base64Decode(data);

    this->type = PictureType(BlockPicture::toUint32(blockData.data()));

    uint32_t mimeLen = BlockPicture::toUint32(blockData.data() + 4);

    this->mimeType = new char[mimeLen+1];
    strncpy(this->mimeType, reinterpret_cast<char*>(blockData.data() + 8), mimeLen);


}