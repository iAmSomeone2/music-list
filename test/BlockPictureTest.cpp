#include <vector>
#include <string>

#include <BlockPicture.hpp>

#include <gtest/gtest.h>

using namespace MusicList;

class BlockPictureTest : public ::testing::Test
{
protected:
    std::string SHORT_TEST_DATA = {'Q','U','J','D'};
    std::string SHORT_EXPECTED = "ABC";
    std::string LONG_TEST_DATA = "SGVsbG8uIERvIHlvdSBrbm93IG15IG5hbWU/IQ==";
    std::string LONG_EXPECTED = "Hello. Do you know my name?!";
};

TEST_F(BlockPictureTest, ShortMessage)
{
    auto returnData = BlockPicture::base64Decode(SHORT_TEST_DATA);
    std::string text = std::string(reinterpret_cast<char*>(returnData.data()));

    ASSERT_EQ(text, SHORT_EXPECTED);
}

TEST_F(BlockPictureTest, LongMessage)
{
    auto returnData = BlockPicture::base64Decode(LONG_TEST_DATA);
    std::string text = std::string(reinterpret_cast<char*>(returnData.data()));

    ASSERT_EQ(text, LONG_EXPECTED);
}


int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}