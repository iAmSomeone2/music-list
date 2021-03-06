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
#include <memory>

#include <Track.hpp>

#include <gtest/gtest.h>
#include <json/json.h>

using namespace MusicList;

namespace fs = std::filesystem;

class TrackTest : public ::testing::Test
{
protected:
    fs::path FLAC_PATH = fs::path("./res/turn_away.flac");
    fs::path OPUS_PATH = fs::path("./res/turn_away.opus");
    fs::path VORBIS_PATH = fs::path("./res/turn_away.ogg");
    fs::path OGG_FLAC_PATH = fs::path("./res/turn_away.oga");

    TrackTest()
    {
        std::cout << "Working dir: " << fs::current_path().string() << std::endl;
    }
};

// FORMAT CHECKS

TEST_F(TrackTest, CheckFlacFormat)
{
    AudioFormat actualFormat = Track::determineFormat(FLAC_PATH);

    ASSERT_EQ(AudioFormat::flac, actualFormat);
}

TEST_F(TrackTest, CheckOpusFormat)
{
    AudioFormat actualFormat = Track::determineFormat(OPUS_PATH);

    ASSERT_EQ(AudioFormat::opus, actualFormat);
}

TEST_F(TrackTest, CheckVorbisFormat)
{
    AudioFormat actualFormat = Track::determineFormat(VORBIS_PATH);

    ASSERT_EQ(AudioFormat::vorbis, actualFormat);
}

TEST_F(TrackTest, CheckOggFlacFormat)
{
    AudioFormat actualFormat = Track::determineFormat(OGG_FLAC_PATH);

    ASSERT_EQ(AudioFormat::ogg_flac, actualFormat);
}

// METADATA IMPORT

TEST_F(TrackTest, ImportFlacMetadata)
{
    Track flacTrack = Track(this->FLAC_PATH);

    ASSERT_EQ("Beck", flacTrack.getArtist());
    ASSERT_EQ("Morning Phase", flacTrack.getAlbum());
    ASSERT_EQ("Turn Away", flacTrack.getTitle());
    ASSERT_EQ(11, flacTrack.getTrackNum());
    ASSERT_EQ(13, flacTrack.getTotalTracks());
    ASSERT_EQ(1, flacTrack.getDiscNum());
    ASSERT_EQ(1, flacTrack.getTotalDiscs());
}

TEST_F(TrackTest, ImportOpusMetadata)
{
    Track opusTrack = Track(this->OPUS_PATH);

    ASSERT_EQ("Beck", opusTrack.getArtist());
    ASSERT_EQ("Morning Phase", opusTrack.getAlbum());
    ASSERT_EQ("Turn Away", opusTrack.getTitle());
    ASSERT_EQ(11, opusTrack.getTrackNum());
    ASSERT_EQ(13, opusTrack.getTotalTracks());
    ASSERT_EQ(1, opusTrack.getDiscNum());
    ASSERT_EQ(1, opusTrack.getTotalDiscs());

    auto trackTags = opusTrack.getTags();
    ASSERT_EQ(trackTags.size(), 27);
}

TEST_F(TrackTest, GenerateJSON)
{
    Track opusTrack = Track(this->OPUS_PATH);

    Json::Value trackJson = opusTrack.toJSON();

    Json::StreamWriterBuilder builder;
    builder["commentStyle"] = "None";
    builder["indentation"] = "    ";
    std::unique_ptr<Json::StreamWriter> writer(builder.newStreamWriter());

    std::ostringstream outStr;

    writer->write(trackJson, &outStr);
    outStr << std::endl;

    std::ofstream outFile = std::ofstream("./track.json", std::ios::out | std::ios::trunc);
    if(outFile.is_open())
    {
        outFile.write(outStr.str().c_str(), outStr.str().length());
        outFile.close();
    }
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}