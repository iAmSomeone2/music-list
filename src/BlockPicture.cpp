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
    int8_t workSet[4];
    for (uint_fast32_t i = 0; i < numChars; i+=4)
    {
        // Read the original 4 bytes into the working set.
        workSet[0] = BASE64_TABLE[data[i]];
        workSet[1] = BASE64_TABLE[data[i+1]];
        workSet[2] = BASE64_TABLE[data[i+2]];
        workSet[3] = BASE64_TABLE[data[i+3]];

        // Shift the first 2 bits off the end.
        workSet[0] = workSet[0] << 2;
        workSet[1] = workSet[1] << 2;
        workSet[2] = workSet[2] << 2;
        workSet[3] = workSet[3] << 2;

        // Create temporary 32-bit value.
        int32_t temp = (workSet[0] << 24);
        temp = temp | (workSet[1] << 18);
        temp = temp | (workSet[2] << 12);
        temp = temp | workSet[3] << 6;

        workSet[0] = (temp & 0xFF000000) >> 24;
        workSet[1] = (temp & 0x00FF0000) >> 16;
        workSet[2] = (temp & 0x0000FF00) >> 8;
        workSet[3] = temp & 0x000000FF;

        outVec.push_back(workSet[0]);
        outVec.push_back(workSet[1]);
        outVec.push_back(workSet[2]);
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
