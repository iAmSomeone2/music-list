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

#include <unistd.h>
#include <filesystem>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include <Importer.hpp>

#include <json/value.h>
#include <json/writer.h>

namespace fs = std::filesystem;

using std::string;

static const char DEFAULT_OUT_PATH[] = "./musiclist.json";

/**
 * @brief Confirms that the supplied path points to a directory.
 * 
 * If the supplied path is not a directory than the parent of the supplied path will
 * be used.
 * 
 * @param searchDir path to verify.
 */
void verifySearchDir(fs::path& searchDir)
{
    if(!fs::is_directory(searchDir))
    {
        searchDir = searchDir.parent_path();
    }
}

/**
 * @brief Confirms that the supplied path is not a directory and has the appropriate extension.
 * 
 * If the supplied path is a directory, then the default out file name will be appended to it.
 * If the supplied path does not use the proper (.json) extension, then it will be fixed.
 * 
 * @param outFile path to verify
 */
void verifyOutFile(fs::path& outFile)
{
    if (fs::is_directory(outFile))
    {
        outFile = outFile.append("/musiclist.json");
    }
    else
    {
        // Check extension
        const string ext = outFile.extension().string();
        if (ext != ".json")
        {
            outFile = outFile.replace_extension(".json");
        }
    }
}

void printHelp()
{
    std::cout << "MusicList CLI\n" << std::endl;;

    std::cout << "Option: -i (Input directory)\n  Sets directory to search for audio files.\n  Usage: 'musiclist -i ~/Music'\n";
    std::cout << std::endl;

    std::cout << "Option: -o (Output file)\n  Sets the file to output the search results to.\n  Usage: 'musiclist -o ~/Documents/musiclist.json'\n";
    std::cout << std::endl;

    std::cout << "Option: -l (Limit)\n  Sets the maximum number of files to process.\n  Usage: 'musiclist -l 100'\n";
    std::cout << std::endl;

    std::cout << "Option -h (Help)\n  Prints this message and exits." << std::endl;
}

int main(int argc, char* argv[])
{
    // User input handling.
    char* searchPath = nullptr;
    char* outPath = nullptr;
    uint32_t limit = 0;

    char opt;

    opterr = 0;

    while((opt = getopt(argc, argv, "i:o:l:h")) != -1)
    {
        switch (opt)
        {
            case 'i':
                searchPath = optarg;
                break;
            case 'o':
                outPath = optarg;
                break;
            case 'l':
                limit = strtoul(optarg, nullptr, 10);
                break;
            case 'h':
                printHelp();
                return EXIT_SUCCESS;
            case '?':
                if (optopt == 'i' || optopt == 'o')
                {
                    std::cerr << "Option -" << char(optopt) << " requires an argument\n";
                }
                else
                {
                    std::cerr << "Unknown option `-" << char(optopt) << "`.\n";
                }

                return EXIT_FAILURE;
            default:
                abort();
        }
    }

    // Assign search and output values.
    fs::path inDir = searchPath ? fs::path(searchPath) : fs::path("./");
    fs::path outFile = outPath ? fs::path(outPath) : fs::path(DEFAULT_OUT_PATH);

    verifySearchDir(inDir);
    verifyOutFile(outFile);

    // Run import process
    MusicList::Importer importer = MusicList::Importer();

    importer.runTrackSearch(inDir, limit);
    importer.generateAlbumsFromTracks();

    // Export to JSON file
    std::cout << "Exporting data to '" << outFile.string() << "'.\n";

    const auto& outJson = importer.toJSON();

    Json::StreamWriterBuilder builder;
    builder["commentStyle"] = "None";
    builder["indentation"] = "  ";

    std::unique_ptr<Json::StreamWriter> writer(builder.newStreamWriter());

    std::ostringstream outStr;

    writer->write(outJson, &outStr);
    outStr << std::endl;

    std::ofstream outStream = std::ofstream(outFile, std::ios::out | std::ios::trunc);
    if (outStream.is_open())
    {
        outStream.write(outStr.str().c_str(), outStr.str().length());
        outStream.close();
    }

    std::cout << "done\n";

    return EXIT_SUCCESS;
}
