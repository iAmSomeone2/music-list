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

    std::string RADIOACTIVE_PIC_BLOCK = "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";
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

TEST_F(BlockPictureTest, DISABLED_FullPictureBlock)
{
    BlockPicture blkPic = BlockPicture();

    blkPic.readPictureBlock(RADIOACTIVE_PIC_BLOCK);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}