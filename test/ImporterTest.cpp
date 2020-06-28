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
#include <filesystem>
#include <fstream>
#include <sstream>
#include <cinttypes>
#include <memory>
#include <vector>

#include <Importer.hpp>

#include <gtest/gtest.h>
#include <json/value.h>
#include <json/writer.h>

namespace fs = std::filesystem;

using namespace MusicList;

using std::vector;
using std::shared_ptr;

class ImporterTest : public ::testing::Test
{
protected:
    const fs::path RES_DIR = fs::path("./res");
    const uint32_t RES_TRACK_NUM = 14U;

    const fs::path MUSIC_DIR = fs::path("/home/bdavidson/Music");
    const uint32_t MUSIC_TRACK_NUM = 1480U;
};

TEST_F(ImporterTest, ResDirTrackCount)
{
    Importer imptr = Importer();
    imptr.runTrackSearch(RES_DIR);
    auto tracks = imptr.getTracks();

    ASSERT_EQ(RES_TRACK_NUM, tracks.size());
}

TEST_F(ImporterTest, MusicDirTrackCount)
{
    Importer imptr = Importer();
    imptr.runTrackSearch(MUSIC_DIR);
    auto tracks = imptr.getTracks();

    ASSERT_EQ(MUSIC_TRACK_NUM, tracks.size());
}

TEST_F(ImporterTest, ResDirAlbumCount)
{
    Importer imptr = Importer();
    imptr.runTrackSearch(RES_DIR);

    imptr.generateAlbumsFromTracks();

    const auto& albumMap = imptr.getAlbums();

    ASSERT_EQ(2, albumMap.size());
}

TEST_F(ImporterTest, MusicDirAlbumCount)
{
    Importer imptr = Importer();
    imptr.runTrackSearch(MUSIC_DIR);

    imptr.generateAlbumsFromTracks();

    const auto& albumMap = imptr.getAlbums();

    ASSERT_EQ(128, albumMap.size());
}

TEST_F(ImporterTest, ResDirJSON)
{
    Importer imptr = Importer();
    imptr.runTrackSearch(RES_DIR);

    imptr.generateAlbumsFromTracks();

    const auto& imptrJSON = imptr.toJSON();

    Json::StreamWriterBuilder builder;
    builder["commentStyle"] = "None";
    builder["indentation"] = "  ";

    std::unique_ptr<Json::StreamWriter> writer(builder.newStreamWriter());

    std::ostringstream outStr;

    writer->write(imptrJSON, &outStr);
    outStr << std::endl;

    std::ofstream outFile = std::ofstream("./reslist.json", std::ios::out | std::ios::trunc);
    if (outFile.is_open())
    {
        outFile.write(outStr.str().c_str(), outStr.str().length());
        outFile.close();
    }
}

TEST_F(ImporterTest, MusicDirJSON)
{
    Importer imptr = Importer();
    imptr.runTrackSearch(MUSIC_DIR);

    imptr.generateAlbumsFromTracks();

    const auto& imptrJSON = imptr.toJSON();

    Json::StreamWriterBuilder builder;
    builder["commentStyle"] = "None";
    builder["indentation"] = "  ";

    std::unique_ptr<Json::StreamWriter> writer(builder.newStreamWriter());

    std::ostringstream outStr;

    writer->write(imptrJSON, &outStr);
    outStr << std::endl;

    std::ofstream outFile = std::ofstream("./musiclist.json", std::ios::out | std::ios::trunc);
    if (outFile.is_open())
    {
        outFile.write(outStr.str().c_str(), outStr.str().length());
        outFile.close();
    }
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
