#include "ProgramOptions.h"

#include <stdexcept>

#include "World.h"

ProgramOptions::ProgramOptions(int argc, char* argv[])
{
    error = parseArgs(argc, argv);
}

const std::string ProgramOptions::parseArgs(int argc, char* argv[])
{
    for (int i = 1; i < argc; ++i)
    {
        std::string arg = argv[i];

        // novsync
        if (arg == "-novsync")
        {
            vsyncEnabled = false;
        }

        // fullscreen
        else if (arg == "-fullscreen")
        {
            fullscreenEnabled = true;
        }

        // numPlayers
        else if (arg == "-numPlayers" || arg == "-n")
        {
            try
            {
                numPlayers = parseInt(argc, argv, i + 1, World::minPlayers, World::maxPlayers);
                ++i;  // Skip next argument
            }
            catch (const std::runtime_error& e)
            {
                return std::string(e.what()) + "\nExpected: -numPlayers [n]";
            }
        }

        // host
        else if (arg == "-host")
        {
            host = true;
        }

        // connect
        else if (arg == "-connect")
        {
            try
            {
                hostAddress = parseString(argc, argv, i + 1);
                ++i;  // Skip next argument
            }
            catch (const std::runtime_error& e)
            {
                return std::string(e.what()) + "\nExpected: -connect [hostAddress]";
            }
        }

        else
        {
            return "Invalid argument: " + arg;
        }
    }

    // Final validation
    if (host && !hostAddress.empty())
    {
        return "connect argument not valid when hosting";
    }

    return {};
}

bool ProgramOptions::hasError() const
{
    return !error.empty();
}

int ProgramOptions::parseInt(int argc, char* argv[], int index, int min, int max) const
{
    if (index >= argc)
    {
        throw std::runtime_error("No value supplied for argument");
    }
    try
    {
        int val = std::stoi(argv[index]);
        if (val < min || val > max)
        {
            throw std::runtime_error("Argument out of range");
        }
        return val;
    }
    catch (const std::invalid_argument&)
    {
        throw std::runtime_error("Invalid value supplied for argument");
    }
    catch (const std::out_of_range&)
    {
        throw std::runtime_error(
                "Value must be between "                       //
                + std::to_string(World::minPlayers) + " and "  //
                + std::to_string(World::maxPlayers));
    }
}

std::string ProgramOptions::parseString(int argc, char* argv[], int index) const
{
    if (index >= argc)
    {
        throw std::runtime_error("No value supplied for argument");
    }
    return argv[index];
}
