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
    fs::path FLAC_PATH = fs::path("../res/turn_away.flac");
    fs::path OPUS_PATH = fs::path("../res/turn_away.opus");
    fs::path VORBIS_PATH = fs::path("../res/turn_away.ogg");
    fs::path OGG_FLAC_PATH = fs::path("../res/turn_away.oga");
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

    flacTrack.readMetadata();

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

    opusTrack.readMetadata();

    ASSERT_EQ("Beck", opusTrack.getArtist());
    ASSERT_EQ("Morning Phase", opusTrack.getAlbum());
    ASSERT_EQ("Turn Away", opusTrack.getTitle());
    ASSERT_EQ(11, opusTrack.getTrackNum());
    ASSERT_EQ(13, opusTrack.getTotalTracks());
    ASSERT_EQ(1, opusTrack.getDiscNum());
    ASSERT_EQ(1, opusTrack.getTotalDiscs());
}

TEST_F(TrackTest, GenerateJSON)
{
    Track opusTrack = Track(this->OPUS_PATH);

    opusTrack.readMetadata();

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