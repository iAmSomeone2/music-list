#include <unistd.h>
#include <filesystem>
#include <iostream>

#include <Importer.hpp>

#include <json/value.h>
#include <json/writer.h>

namespace fs = std::filesystem;

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

}

int main(int argc, char* argv[])
{
    // User input handling.
    char* searchPath = nullptr;
    char* outPath = nullptr;

    char opt;

    opterr = 0;

    while((opt = getopt(argc, argv, "i:o:")) != -1)
    {
        switch (opt)
        {
            case 'i':
                searchPath = optarg;
                break;
            case 'o':
                outPath = optarg;
                break;
            case '?':
                if (optopt == 'i' || optopt == 'o')
                {
                    std::cerr << "Option -" << std::to_string(optopt) << " requires an argument\n";
                }
                else
                {
                    std::cerr << "Unkown option `-" << std::to_string(optopt) << "`.\n";
                }

                return EXIT_FAILURE;
            default:
                abort();
        }
    }

    // Assign search and output values.
    fs::path inDir = searchPath ? fs::path(searchPath) : fs::path("./");
    fs::path outFile = outPath ? fs::path(outPath) : fs::path(DEFAULT_OUT_PATH);

    return EXIT_SUCCESS;
}
